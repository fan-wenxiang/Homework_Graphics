#ifndef HIT_H
#define HIT_H

#include <vecmath.h>
#include "ray.hpp"
#include "material.hpp"


class Hit {
public:

    // constructors
    Hit() {
        material = nullptr;
        t = 1e38;
        texture = false;
    }

    Hit(float _t, Material *m, const Vector3f &n) {
        t = _t;
        material = m;
        normal = n;
        texture = false;
    }

    Hit(const Hit &h) {
        t = h.t;
        material = h.material;
        normal = h.normal;
        texture = false;
    }

    // destructor
    ~Hit() = default;

    float getT() const {
        return t;
    }

    Material *getMaterial() const {
        return material;
    }

    const Vector3f &getNormal() const {
        return normal;
    }

    void set(float _t, Material *m, const Vector3f &n, bool addcolor = false, Vector3f c = Vector3f::ZERO) {
        t = _t;
        material = m;
        normal = n;
        if (addcolor)
        {
            texture = true;
            color = c;
        }
        else
        {
            texture = false;
            color = Vector3f::ZERO;
        }
    }

    Vector3f calc(Vector3f c)
    {
        if (texture)
            return material->getLight()+color*c;
        else
            return material->getLight()+material->getColor()*c;
    }

private:
    float t;
    Material *material;
    Vector3f normal;
    Vector3f color;
    bool texture;
};

inline std::ostream &operator<<(std::ostream &os, const Hit &h) {
    os << "Hit <" << h.getT() << ", " << h.getNormal() << ">";
    return os;
}

#endif // HIT_H
