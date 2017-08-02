

#ifndef LASPOINTREADER_H
#define LASPOINTREADER_H

#include <string>
#include <iostream>
#include <vector>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>

#include "laszip_dll.h"

#include "Point.h"
#include "PointReader.h"

using std::string;

using std::ifstream;
using std::cout;
using std::endl;
using std::vector;

namespace Potree{

class LIBLASReader{
private:

    double tr[16];
    bool hasTransform = false;

    Point transform(double x, double y, double z) const {
        Point p;
        if (hasTransform) {
            p.position.x = tr[0] * x + tr[4] * y + tr[8] * z + tr[12];
            p.position.y = tr[1] * x + tr[5] * y + tr[9] * z + tr[13];
            p.position.z = tr[2] * x + tr[6] * y + tr[10] * z + tr[14];
        } else {
			p.position = Vector3<double>{x,y,z};
        }
        return p;
    }
public:

	laszip_POINTER laszip_reader;
	laszip_header* header;
	laszip_point* point;
	int colorScale;
	double coordinates[3];
	long long pointsRead = 0;
    bool hasIntensity = false;
    bool hasColor = false;

    LIBLASReader(string path) {

		laszip_create(&laszip_reader);

		laszip_BOOL request_reader = 1;
		laszip_request_compatibility_mode(laszip_reader, request_reader);

		{// read first 1000 points to find if color is 1 or 2 bytes
            using namespace boost::accumulators;
			laszip_BOOL is_compressed = boost::iends_with(path, ".laz") ? 1 : 0;
			laszip_open_reader(laszip_reader, path.c_str(), &is_compressed);

			laszip_get_header_pointer(laszip_reader, &header);

			long long npoints = (header->number_of_point_records ? header->number_of_point_records : header->extended_number_of_point_records);
			
			laszip_get_point_pointer(laszip_reader, &point);

            accumulator_set<double, stats<tag::mean> > accColor;
            accumulator_set<double, stats<tag::mean> > accIntensity;

			colorScale = 1;
			for(int i = 0; i < 10000 && i < npoints; i++){
				laszip_read_point(laszip_reader);
		
				auto r = point->rgb[0];
				auto g = point->rgb[1];
				auto b = point->rgb[2];
		
				if(colorScale == 1 && (r > 255 || g > 255 || b > 255)){
					colorScale = 256;
				}

                accColor(static_cast<double>(r + g + b) / static_cast<double>(colorScale));
                accIntensity(point->intensity);
		
				i++;
			}

            if(mean(accColor) > 1.0) {
                hasColor = true;
            }

            if(mean(accIntensity) > 1.0) {
                hasIntensity = true;
            }
		}

		laszip_seek_point(laszip_reader, 0);
    }

	~LIBLASReader(){
		laszip_close_reader(laszip_reader);
		laszip_destroy(laszip_reader);
	}

	bool readPoint(){
		if(pointsRead < header->number_of_point_records){
			laszip_read_point(laszip_reader);
			pointsRead++;

			return true;
		}else{
			return false;
		}
	}

    Point GetPoint() {
		laszip_get_coordinates(laszip_reader, coordinates);

        Point p = transform(coordinates[0], coordinates[1], coordinates[2]);
        p.classification = point->classification;

        p.color.x = point->rgb[0] / colorScale;
        p.color.y = point->rgb[1] / colorScale;
        p.color.z = point->rgb[2] / colorScale;

        if(hasColor && !hasIntensity) {
            p.intensity = p.color.x;
        } else {
            p.intensity = point->intensity;
        }

		p.returnNumber = point->return_number;
		p.numberOfReturns = point->number_of_returns;
		p.pointSourceID = point->point_source_ID;

        return p;
    }

	void close(){

	}

	AABB getAABB();
};


class LASPointReader : public PointReader{
private:
	AABB aabb;
	string path;
	LIBLASReader *reader;
	vector<string> files;
	vector<string>::iterator currentFile;
public:

	LASPointReader(string path);

	~LASPointReader();

	bool readNextPoint();

	Point getPoint();

	AABB getAABB();

	long numPoints();

	unsigned char pointDataFormat() const;

    bool hasColor() const;

    bool hasIntensity() const;

	void close();

	Vector3<double> getScale();
};

}

#endif
