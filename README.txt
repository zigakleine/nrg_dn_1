
I compile the program with:
    g++ main.cpp -o interpolate

Since I couldn't figure out how to write binary output to standard out in c++, it always outputs to a file with name "output.raw" that gets created 
in the same directory.

Example commands for running the program:
    ./interpolate < input1k.txt --method basic --p 0.5 --min-x -1.5 --min-y -1.5 --min-z -1  --max-x 1.5 --max-y 1.5 --max-z 1  --res-x 128 --res-y 128 --res-z 64
    ./interpolate < input1k.txt --method modified --r 0.9 --min-x -1.5 --min-y -1.5 --min-z -1  --max-x 1.5 --max-y 1.5 --max-z 1  --res-x 128 --res-y 128 --res-z 64

Octree implementation from:
    https://github.com/jbehley/octree
    J. Behley, V. Steinhage, A.B. Cremers. Efficient Radius Neighbor Search in Three-dimensional Point Clouds, Proc. of the IEEE International Conference on Robotics and Automation (ICRA), 2015.

