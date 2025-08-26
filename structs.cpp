#include "structs.hpp"
#include "math_utils.hpp"

Vec3D& Vec3D::operator+=(const float addend) {
    x += addend;
    y += addend;
    z += addend;
    return *this;
}

// Rotates this triangle around the Z axis with the specified rotation matrix.
Triangle Triangle::rotate(mat4x4 &matrix) {
    Triangle rotated;
    for (int i = 0; i < 3; i++) {
        MultiplyMatrixVector(p[i], rotated.p[i], matrix);
    }
    return rotated;
}