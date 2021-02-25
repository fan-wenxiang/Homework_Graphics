#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

#include "ray.hpp"
#include <iostream>


class Material {
public:

    explicit Material(const Vector3f c, Vector3f l = Vector3f::ZERO, float reflect = 0, float refract = 0, float refract_rate = 0) :
            color(c), light(l), reflection(reflect), refraction(refract), refraction_rate(refract_rate) {
                if (reflection+refraction > 1)
                {
                    float sum=reflection+refraction;
                    reflection = reflection/sum;
                    refraction = refraction/sum;
                }
                if (refraction >0 && refraction_rate < 0.000001)
                {
                    refraction_rate = 1;
                }
    }

    virtual ~Material() = default;

    Vector3f getColor() const {
        return color;
    }

    Vector3f getLight()
    {
        return light;
    }

    float getReflection()
    {
        return reflection;
    }

    float getRefraction()
    {
        return refraction;
    }

    float getRefraction_rate()
    {
        return refraction_rate;
    }

protected:
    Vector3f color;
    Vector3f light;
    float reflection;
    float refraction;
    float refraction_rate;
};


#endif // MATERIAL_H
