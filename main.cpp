#include <cstdlib>
#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include <vector>
#include <sstream>
#include <numeric>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <chrono> 


#include "Octree.hpp"

using namespace std;


class Point3f
{
 public:
  Point3f(float x, float y, float z) : x(x), y(y), z(z)
  {
  }

  float x, y, z;
};

float euclideanDistance(Point3f x1, Point3f x2) {

    float squareSum = pow((x1.x - x2.x),2) + pow((x1.y - x2.y),2) + pow((x1.z - x2.z),2);

    return sqrt(squareSum);
}

int main(int argc, char** argv) {

   std::ofstream outstream;
   outstream.open("output.raw", std::ios::out | std::ios::binary);
  
  
    string methodType = "";

    float p = 2;
    float R = 0.9;

    float minX = -1.5;
    float minY = -1.5;
    float minZ = -1.0;

    float maxX = 1.5;
    float maxY = 1.5;
    float maxZ = 1.0;

    float resX = 128;
    float resY = 128;
    float resZ = 64;

    for (int i = 1; i < argc; i+=2) {

        //cout << argv[i] << endl;

        if(strcmp(argv[i], "--method") == 0){
            methodType = string(argv[i+1]);
            // cout << string(argv[i+1]) << endl;
        }
        else if(strcmp(argv[i], "--r") == 0) {
            R = strtof(argv[i+1], NULL);
        }
        else if(strcmp(argv[i], "--p") == 0) {
            p = strtof(argv[i+1], NULL);
        }
        else if(strcmp(argv[i], "--min-x") == 0) {
            minX = strtof(argv[i+1], NULL);
        }
        else if(strcmp(argv[i], "--min-y") == 0) {
            minY = strtof(argv[i+1], NULL);
        }
        else if(strcmp(argv[i], "--min-z") == 0) {
            minZ = strtof(argv[i+1], NULL);
        }        
        else if(strcmp(argv[i], "--max-x") == 0) {
            maxX = strtof(argv[i+1], NULL);
        }        
        else if(strcmp(argv[i], "--max-y") == 0) {
            maxY = strtof(argv[i+1], NULL);
        }  
        else if(strcmp(argv[i], "--max-z") == 0) {
            maxZ = strtof(argv[i+1], NULL);
        }
        else if(strcmp(argv[i], "--res-x") == 0) {
            resX = strtof(argv[i+1], NULL);
        }                 
        else if(strcmp(argv[i], "--res-y") == 0) {
            resY = strtof(argv[i+1], NULL);
        }                 
        else if(strcmp(argv[i], "--res-z") == 0) {
            resZ = strtof(argv[i+1], NULL);
        }                 

    }



    // cout << p << endl;
    // cout << R << endl;

    // cout << minX << endl;
    // cout << minY << endl;
    // cout << minZ << endl;

    // cout << maxX << endl;
    // cout << maxY << endl;
    // cout << maxZ << endl;

    // cout << resX << endl;
    // cout << resY << endl;
    // cout << resZ << endl;


    // auto start = chrono::high_resolution_clock::now(); 
    
    string lineText;
    vector<Point3f> inputPointsVec;
    vector<float> inputValuesVec;

    // read file
    while (cin) {

        getline (cin, lineText);
        vector<float> lineVec;

        //istringstream iss(lineText);
        //copy(istream_iterator<float>(iss), istream_iterator<float>(), back_inserter(lineVec));
        float floatLine [4];
        stringstream ss(lineText);
        for(int i=0;i<4;i++){
            ss >> floatLine[i];
            //cout << floatLine[i] << endl;
        }

        Point3f currentPoint(floatLine[0], floatLine[1], floatLine[2]);
        inputValuesVec.push_back(floatLine[3]);
        inputPointsVec.push_back(currentPoint);

    }


    float stepX = abs(maxX - minX)/(resX-1);
    float stepY = abs(maxY - minY)/(resY-1);
    float stepZ = abs(maxZ - minZ)/(resZ-1);

    int inputVecSize = inputPointsVec.size();

    int iter = 0;
    ofstream outfile;

    if(methodType == "basic") {


        for (int z = 0; z < resZ; z++) {
            for (int y = 0; y < resY; y++) {
                for (int x = 0; x < resX; x++) {
                    
                    float currentX = minX + x*stepX;
                    float currentY = minY + y*stepY;
                    float currentZ = minZ + z*stepZ;

                    Point3f currentPoint(currentX, currentY, currentZ);
    
                    bool areAllNonzero = true;
                    float zeroVal = -1;

                    float wkSum = 0;
                    float wkykSum = 0;

                    for(int i = 0; i < inputVecSize; i++) {

                        float currentDistance = euclideanDistance(currentPoint, inputPointsVec[i]);

                        if(currentDistance == 0.0) {
                            areAllNonzero = false;
                            zeroVal = inputValuesVec[i];
                            break;
                        }

                        float wk = 1 / pow(currentDistance, p);
                        
                        wkSum += wk;
                        wkykSum += wk*inputValuesVec[i];

                    }
                    
                    if(areAllNonzero){
                        float out = wkykSum/wkSum;
                        //cout  << currentX << " " << currentY << " " << currentZ << " " << out << endl;ž
                        //cout << reinterpret_cast<const char*>( &out );
                        outstream.write( reinterpret_cast<const char*>( &out ), sizeof( float ));
                        // cout << out << endl;
                    }
                    else {
                        float out = zeroVal;
                        //cout  << currentX << " " << currentY << " " << currentZ << " " << out << endl;
                        //cout << reinterpret_cast<const char*>( &out );
                        outstream.write( reinterpret_cast<const char*>( &out ), sizeof( float ));
                        // cout << out << endl;
                    }
    

                }
            }
        } 

    }
    else if(methodType == "modified") {

        unibn::Octree<Point3f> octree;
        unibn::OctreeParams params;
        octree.initialize(inputPointsVec);


        for (int z = 0; z < resZ; z++) {
            for (int y = 0; y < resY; y++) {
                for (int x = 0; x < resX; x++) {
                    
                    float currentX = minX + x*stepX;
                    float currentY = minY + y*stepY;
                    float currentZ = minZ + z*stepZ;

                    Point3f currentPoint(currentX, currentY, currentZ); 

                    vector<uint32_t> results;
                    //const Point3f& q = inputPointsVec[0];
                    octree.radiusNeighbors<unibn::L2Distance<Point3f> >(currentPoint, R, results);

                    bool areAllNonzero = true;
                    float zeroVal = -1;

                    float wkSum = 0;
                    float wkykSum = 0;

                    for (int i = 0; i < results.size(); i++) { 

                        Point3f p = inputPointsVec[results[i]];
                        float currentDistance = euclideanDistance(currentPoint, p);
                        float wk = 0;

                        if(currentDistance == 0.0) {
                            areAllNonzero = false;
                            zeroVal = inputValuesVec[results[i]];
                            break;
                        }
                      
                        wk = pow(((R - currentDistance)/R*currentDistance), 2);

                        wkSum += wk;
                        wkykSum += wk*inputValuesVec[results[i]];
                                         
                    }

                    if(areAllNonzero){
                        float out = wkykSum/wkSum;
                        //MyFile  << currentX << " " << currentY << " " << currentZ << " " << out << endl;
                        //cout << reinterpret_cast<const char*>( &out );
                        outstream.write( reinterpret_cast<const char*>( &out ), sizeof( float ));                    
                        // cout << out << endl;

                    }
                    else {
                        float out = zeroVal;
                        //MyFile  << currentX << " " << currentY << " " << currentZ << " " << out << endl;
                        //cout << reinterpret_cast<const char*>( &out);
                        outstream.write( reinterpret_cast<const char*>( &out ), sizeof( float ));                
                        // cout << out << endl;

                    }       


                }
            }
        }


    }


    // auto stop = chrono::high_resolution_clock::now(); 

    // auto duration = chrono::duration_cast<chrono::microseconds>(stop - start); 
  
    // cout << "Time taken by function: "
    //      << duration.count() << " microseconds" << endl; 

    outstream.close();

    return 0;
}