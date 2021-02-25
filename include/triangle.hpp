#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>
#include <iostream>
using namespace std;


class Triangle: public Object3D {

public:
	Triangle() = delete;

    // a b c are three vertex positions of the triangle
	Triangle( const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m) : Object3D(m) {
        vertices[0]=a;
        vertices[1]=b;
        vertices[2]=c;
        normal=Vector3f::cross(a-b, b-c).normalized();
	}

	bool intersect( const Ray& ray,  Hit& hit , float tmin) override {
        float t=-Vector3f::dot(normal, ray.getOrigin()-vertices[0])/Vector3f::dot(normal, ray.getDirection());
        if (Vector3f::dot(normal, ray.getDirection())>0) normal=-normal;
        Vector3f dot=ray.getOrigin()+t*ray.getDirection();
        Vector3f n1=Vector3f::cross(vertices[0]-dot, vertices[1]-dot);
        Vector3f n2=Vector3f::cross(vertices[1]-dot, vertices[2]-dot);
        Vector3f n3=Vector3f::cross(vertices[2]-dot, vertices[0]-dot);
        if (Vector3f::dot(n1, n2)<0 || Vector3f::dot(n2, n3)<0) return false;

        if (t<tmin) return false;
        if (t>=hit.getT()) return false;
        hit.set(t, material, normal);
        return true;
	}
    
	Vector3f normal;
	Vector3f vertices[3];

};

#endif //TRIANGLE_H
