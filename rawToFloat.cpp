
#include <fstream>
#include <fstream>
#include <iostream>


int main() {
//    std::ofstream out;
//    out.open("bin.dat", std::ios::out | std::ios::binary);
//    float f = 0.5;
//    out.write( reinterpret_cast<const char*>( &f ), sizeof( float ));
//    out.close();

    float f2;
    std::ifstream fin("output.raw", std::ios::binary);
    while (fin.read(reinterpret_cast<char*>(&f2), sizeof(float)))
        std::cout << f2 << '\n';
    return 0;

}