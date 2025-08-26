#include "structs.hpp"

Vec3D& Vec3D::operator+=(const float addend) {
    x += addend;
    y += addend;
    z += addend;
    return *this;
}