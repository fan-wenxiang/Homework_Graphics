#ifndef GROUP_H
#define GROUP_H


#include "object3d.hpp"
#include "ray.hpp"
#include "hit.hpp"
#include <iostream>
#include <vector>


class Group : public Object3D {

public:

    Group() {
        num_objects=0;
    }

    explicit Group (int num_objects) {
        this->num_objects=num_objects;
    }

    ~Group() override {
        for (int i=0; i<num_objects; i++)
            delete objects[i];
    }

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        bool result=false;
        for (int i=0; i<num_objects; i++)
            result|=objects[i]->intersect(r, h, tmin);
        return result;
    }

    void addObject(int index, Object3D *obj) {
        objects.push_back(obj);
    }

    int getGroupSize() {
        return num_objects;
    }

private:
    int num_objects;
    std::vector<Object3D*> objects;

};

#endif
	
