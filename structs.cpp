#include "structs.hpp"
#include "math_utils.hpp"
#include <fstream>
#include <exception>
#include <sstream>
#include <cmath>

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

Triangle Triangle::offset(const Vec3D units) {
    Triangle offset;
    for (int i = 0; i < 3; i++) {
        offset.p[i].x = p[i].x + units.x;
        offset.p[i].y = p[i].x + units.y;
        offset.p[i].z = p[i].x + units.z;
    }
    return offset;
}

// Projects 3D triangle into a 2D 
Triangle Triangle::project(const mat4x4 &matrix) {
    Triangle projected;
    for (int i = 0; i < 3; i++) {
        MultiplyMatrixVector(p[i], projected.p[i], matrix);
    }
    return projected;
}

// Calculates this triangle's normal.
Vec3D Triangle::calculateNormal() {
        Vec3D normal, line1, line2;
        line1.x = p[1].x - p[0].x;
        line1.y = p[1].y - p[0].y;
        line1.z = p[1].z - p[0].z;

        line2.x = p[2].x - p[0].x;
        line2.y = p[2].y - p[0].y;
        line2.z = p[2].z - p[0].z;

        normal.x = line1.y * line2.z - line1.z * line2.y;
        normal.y = line1.z * line2.x - line1.x * line2.z;
        normal.z = line1.x * line2.y - line1.y * line2.x;

        float l = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
        normal.x /= l; normal.y /= l; normal.z /= l;
        return normal;
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

// Determines if this normal is facing the camera
bool Triangle::isFacingCamera(const Vec3D &normal) {
    return true;
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