#include "structs.hpp"
#include "math_utils.hpp"

Vec3D& Vec3D::operator+=(const float addend) {
    x += addend;
    y += addend;
    z += addend;
    return *this;
}

// Rotates this triangle around the Z axis with the specified rotation matrix.
Triangle Triangle::rotate(const mat4x4 &matrix) {
    Triangle rotated;
    for (int i = 0; i < 3; i++) {
        MultiplyMatrixVector(p[i], rotated.p[i], matrix);
    }
    return rotated;
}

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

Triangle Triangle::project(const mat4x4 &matrix) {
    Triangle projected;
    for (int i = 0; i < 3; i++) {
        MultiplyMatrixVector(p[i], projected.p[i], matrix);
    }
    return projected;
}