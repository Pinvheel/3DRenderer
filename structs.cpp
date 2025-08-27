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