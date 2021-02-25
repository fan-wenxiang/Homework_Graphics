#ifndef SPHERE_H
#define SPHERE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>


class Sphere : public Object3D {
public:
    Sphere() {
        center=Vector3f::ZERO;
        radius=1;
    }

    Sphere(const Vector3f &center, float radius, Material *material) : Object3D(material) {
        this->center=center;
        this->radius=radius;
    }

    ~Sphere() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        Vector3f Hypo=center-r.getOrigin();
        float c=Hypo.length();
        float b=Vector3f::dot(Hypo, r.getDirection().normalized());
        float a=sqrt(c*c-b*b);
        if (a<=radius)
        {
            float t=0;
            if (c<=radius)
                t=b+sqrt(radius*radius-a*a);
            else
                t=b-sqrt(radius*radius-a*a);
            t=t/Vector3f::dot(r.getDirection(), r.getDirection().normalized());
            if (t<tmin) return false;
            if (t<h.getT())
            {
                Vector3f normal;
                if (c<=radius)
                    normal=center-r.pointAtParameter(t);
                else
                    normal=r.pointAtParameter(t)-center;
                normal.normalize();
                h.set(t, material, normal);
                return true;
            }
        }
        return false;
    }

    void setCenter(Vector3f c)
    {
        center = c;
    }

    void setRadius(float r)
    {
        radius = r;
    }

    bool inSphere(Vector3f p)
    {
        if ((p-center).length()<=radius)
            return true;
        else
            return false;
    }

    Vector3f getCenter()
    {
        return center;
    }

    float getRadius()
    {
        return radius;
    }

protected:
    Vector3f center;
    float radius;
};


#endif
