#ifndef CAMERA_H
#define CAMERA_H

#include "ray.hpp"
#include <vecmath.h>
#include <float.h>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Camera {
public:
    Camera(const Vector3f &center, const Vector3f &direction, const Vector3f &up, int imgW, int imgH) {
        this->center = center;
        this->direction = direction.normalized();
        this->horizontal = Vector3f::cross(this->direction, up);
        this->up = Vector3f::cross(this->horizontal, this->direction);
        this->width = imgW;
        this->height = imgH;
    }

    // Generate rays for each screen-space coordinate
    virtual Ray generateRay(const Vector2f &point) = 0;
    virtual ~Camera() = default;

    int getWidth() const { return width; }
    int getHeight() const { return height; }

protected:
    // Extrinsic parameters
    Vector3f center;
    Vector3f direction;
    Vector3f up;
    Vector3f horizontal;
    // Intrinsic parameters
    int width;
    int height;
};


class PerspectiveCamera : public Camera {

public:
    PerspectiveCamera(const Vector3f &center, const Vector3f &direction,
            const Vector3f &up, int imgW, int imgH, float angle, float f, float r) : Camera(center, direction, up, imgW, imgH) {
        // angle is in radian.
        c[0]=((float)width-1.f)/2;
        c[1]=((float)height-1.f)/2;
        distance=(height/2)/tan(angle/2);
        focus = f;
        radius = r;
    }

    Ray generateRay(const Vector2f &point) override {
        Vector3f Rc(point-c, distance);
        Rc.normalize();
        Matrix3f m(horizontal, up, direction, true);
        Vector3f dir=m*Rc;
        float t=focus/Rc.z();
        Vector3f fpoint=center+t*dir;           //计算出原光线与焦平面的交点
        float x=(rand()%1000)/1000.0;
        x=sqrt(x);
        x=x*radius;
        float y=(rand()%1000)/1000.0;
        y=y*2*M_PI;
        Vector3f p=center+x*cos(y)*horizontal+x*sin(y)*up;      //在代表透镜的圆上随机取点
        dir = fpoint-p;             //得到新的光线
        dir.normalize();
        return Ray(center, dir);
    }

protected:
    Vector2f c;
    float distance;
    float focus;
    float radius;
};

#endif //CAMERA_H
