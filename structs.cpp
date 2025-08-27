#include "structs.hpp"
#include "math_utils.hpp"
#include <fstream>
#include <exception>
#include <sstream>

// Rotates this triangle around the Z axis with the specified rotation matrix.
Triangle Triangle::rotate(const mat4x4 &matrix) {
    Triangle rotated;
    for (int i = 0; i < 3; i++) {
        MultiplyMatrixVector(p[i], rotated.p[i], matrix);
    }
    return rotated;
}

// Offsets the designated coordinate by the designated amount of units.
Triangle Triangle::offsetX(const float units) {
    Triangle offset;
    for (int i = 0; i < 3; i++) {
        offset.p[i].x = p[i].x + units;
    }
    return offset;
}

Triangle Triangle::offsetY(const float units) {
    Triangle offset;
    for (int i = 0; i < 3; i++) {
        offset.p[i].y = p[i].y + units;
    }
    return offset;
}

Triangle Triangle::offsetZ(const float units) {
    Triangle offset;
    for (int i = 0; i < 3; i++) {
        offset.p[i].z = p[i].z + units;
    }
    return offset;
}

// Projects 3D triangle into a 2D triangle.
Triangle Triangle::project(const mat4x4 &matrix) {
    Triangle projected;
    for (int i = 0; i < 3; i++) {
        MultiplyMatrixVector(p[i], projected.p[i], matrix);
    }
    return projected;
}

// Scales to the designated screenWidth and screenHeight (use literals for best results).
void Triangle::scale(float screenWidth, float screenHeight) {
    for (int i = 0; i < 3; i++) {
        p[i].x += 1;
        p[i].y += 1;

        p[i].x *= 0.5 * screenWidth;
        p[i].y *= 0.5 * screenHeight;
    }
}

bool Mesh::loadObjectFromFile(std::string fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        throw std::exception();
    }

    std::vector<Vec3D> verts;
    while (!file.eof()) {
        char line[128];
        file.getline(line, 128);

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
            tris.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });
        }
    }
    return true;
}