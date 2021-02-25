#ifndef PLANE_H
#define PLANE_H

#include "object3d.hpp"
#include "image.hpp"

// function: ax+by+cz=d

class Plane : public Object3D {
public:
    Plane() {

    }

    Plane(const Vector3f &normal, float d, Material *m, bool t, char* file, Vector3f p1, Vector3f p2, Vector3f p3, Vector3f p4) : Object3D(m) {
        this->normal=normal;
        this->d=d;
        if (t)      //若有纹理贴图，则加载图片
        {
            texture = true;
            image = Image::LoadTGA(file);
            original = p3;
            y = (p1-p3).normalized();
            x = (p4-p3).normalized();
            mx = Vector3f::dot(x, (p2-p3));
            my = Vector3f::dot(y, (p2-p3));
        }
        else
            texture = false;
    }

    ~Plane() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        if (abs(Vector3f::dot(normal, r.getDirection()))<0.00001) return false;
        float t=-(Vector3f::dot(normal, r.getOrigin())-d)/Vector3f::dot(normal, r.getDirection());
        if (t<tmin) return false;
        if (t>=h.getT()) return false;
        Vector3f color = Vector3f::ZERO;
        bool addcolor = false;
        if (texture)        //若有纹理贴图，则找到交点在平面中的位置对应图片上的位置，并返回其颜色
        {
            Vector3f point = r.pointAtParameter(t);
            float dx = Vector3f::dot(point-original, x);
            float dy = Vector3f::dot(point-original, y);
            if (dx > mx || dx < 0 || dy > my || dy < 0)         //若超出图片范围，则直接返回0
                color = Vector3f::ZERO;
            else
            {
                addcolor = true;
                color = image->GetPixel((int)((float)(dx/mx)*(image->Width()-1)), (int)((float)(dy/my)*(image->Height()-1)));
            }
            
            
        }
        if (Vector3f::dot(normal, r.getDirection())>0) h.set(t, material, -normal, addcolor, color);
        else h.set(t, material, normal, addcolor, color);
        return true;
    }

protected:
    Vector3f normal;
    float d;
    bool texture;
    Vector3f original;
    Vector3f x;
    Vector3f y;
    float mx;
    float my;
    Image* image;
};

#endif //PLANE_H
		

