#include "Mesh.hpp"
#include <fstream>
#include <sstream>
#include <string>

bool Mesh::loadObjectFromFile(std::string fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        throw std::exception();
    }

    std::vector<Vec3D> verts;
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream s;
        s << line;

        char junk;
        if(line[0] == 'v') {
            Vec3D v;
			s >> junk >> v.x >> v.y >> v.z;
			verts.push_back(v);
        }
        if (line[0] == 'f') {
            int f[3];
            s >> junk >> f[0] >> f[1] >> f[2];
            tris.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1], sf::Color::Red});
        }
    }
    return true;
}