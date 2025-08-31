#include "Triangle.hpp"
#include "math_utils.hpp"
#include <cmath>

// Rotates this Triangle around the Z axis with the specified rotation matrix.
Triangle Triangle::rotate(const mat4x4 &matrix) {
    Triangle rotated;
    for (int i = 0; i < 3; i++) {
        rotated.p[i] = p[i] * matrix;  // MultiplyMatrixVector(p[i], rotated.p[i], matrix);
    }
    return rotated;
}

// Offsets the designated coordinate by the designated amount of units.
Triangle Triangle::offset(const Vec3D units) {
    Triangle offset;
    for (int i = 0; i < 3; i++) {
        offset.p[i] = p[i] + units;
    }
    return offset;
}

// Projects 3D Triangle into a 2D 
Triangle Triangle::project(const mat4x4 &matrix) {
    Triangle projected;
    for (int i = 0; i < 3; i++) {
        projected.p[i] = p[i] * matrix;  // MultiplyMatrixVector(p[i], projected.p[i], matrix);
    }
    return projected;
}

// Calculates this Triangle's normal.
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

// Determines if this normal is facing the camera.
bool Triangle::isFacingCamera(const Vec3D &normal, const Vec3D &cameraPos) {
    float xComp = normal.x * (p[0].x - cameraPos.x);
    float yComp = normal.y * (p[0].y - cameraPos.y);
    float zComp = normal.z * (p[0].z - cameraPos.z);
    bool isFacingCamera = xComp + yComp + zComp < 0.0f;
    return isFacingCamera;
}

int Triangle_ClipAgainstPlane(Vec3D plane_p, Vec3D plane_n, Triangle &in_tri, Triangle &out_tri1, Triangle &out_tri2) {
    // Make sure plane normal is indeed normal
    plane_n.normalize();

    // Return signed shortest distance from point to plane, plane normal must be normalised
    auto dist = [&](Vec3D &p)
    {
        Vec3D n = p.normalize();
        return (plane_n.x * p.x + plane_n.y * p.y + plane_n.z * p.z - plane_n.dotProduct(plane_p));
    };

    // Create two temporary storage arrays to classify points either side of plane
    // If distance sign is positive, point lies on "inside" of plane
    Vec3D* inside_points[3];  int nInsidePointCount = 0;
    Vec3D* outside_points[3]; int nOutsidePointCount = 0;

    // Get signed distance of each point in Triangle to plane
    float d0 = dist(in_tri.p[0]);
    float d1 = dist(in_tri.p[1]);
    float d2 = dist(in_tri.p[2]);

    if (d0 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p[0]; }
    else { outside_points[nOutsidePointCount++] = &in_tri.p[0]; }
    if (d1 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p[1]; }
    else { outside_points[nOutsidePointCount++] = &in_tri.p[1]; }
    if (d2 >= 0) { inside_points[nInsidePointCount++] = &in_tri.p[2]; }
    else { outside_points[nOutsidePointCount++] = &in_tri.p[2]; }

    // Now classify Triangle points, and break the input Triangle into 
    // smaller output Triangles if required. There are four possible
    // outcomes...

    if (nInsidePointCount == 0)
    {
        // All points lie on the outside of plane, so clip whole Triangle
        // It ceases to exist

        return 0; // No returned Triangles are valid
    }

    if (nInsidePointCount == 3)
    {
        // All points lie on the inside of plane, so do nothing
        // and allow the Triangle to simply pass through
        out_tri1 = in_tri;

        return 1; // Just the one returned original Triangle is valid
    }

    if (nInsidePointCount == 1 && nOutsidePointCount == 2)
    {
        // Triangle should be clipped. As two points lie outside
        // the plane, the Triangle simply becomes a smaller Triangle

        // Copy appearance info to new Triangle
        out_tri1.color =  in_tri.color;

        // The inside point is valid, so keep that...
        out_tri1.p[0] = *inside_points[0];

        // but the two new points are at the locations where the 
        // original sides of the Triangle (lines) intersect with the plane
        out_tri1.p[1] = intersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);
        out_tri1.p[2] = intersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[1]);

        return 1; // Return the newly formed single Triangle
    }

    if (nInsidePointCount == 2 && nOutsidePointCount == 1)
    {
        // Triangle should be clipped. As two points lie inside the plane,
        // the clipped Triangle becomes a "quad". Fortunately, we can
        // represent a quad with two new Triangles

        // Copy appearance info to new Triangles
        out_tri1.color =  in_tri.color;

        out_tri2.color =  in_tri.color;

        // The first Triangle consists of the two inside points and a new
        // point determined by the location where one side of the Triangle
        // intersects with the plane
        out_tri1.p[0] = *inside_points[0];
        out_tri1.p[1] = *inside_points[1];
        out_tri1.p[2] = intersectPlane(plane_p, plane_n, *inside_points[0], *outside_points[0]);

        // The second Triangle is composed of one of he inside points, a
        // new point determined by the intersection of the other side of the 
        // Triangle and the plane, and the newly created point above
        out_tri2.p[0] = *inside_points[1];
        out_tri2.p[1] = out_tri1.p[2];
        out_tri2.p[2] = intersectPlane(plane_p, plane_n, *inside_points[1], *outside_points[0]);

        return 2; // Return two newly formed Triangles which form a quad
    }
}
 