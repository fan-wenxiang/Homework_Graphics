#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

#include "scene_parser.hpp"
#include "image.hpp"
#include "camera.hpp"
#include "group.hpp"

#include <string>

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


Vector3f PathTracing(Ray ray, Group* group, int depth, bool isAir)
{
    Hit hit;
    bool isIntersect=group->intersect(ray, hit, 0.001);
    if (!isIntersect)
        return Vector3f::ZERO;
    Vector3f color=Vector3f::ZERO;
    if (depth < 0 || hit.getMaterial()->getColor()==Vector3f::ZERO) return hit.getMaterial()->getLight();

    float x=(rand()%1000)/1000.0;

    if (x<hit.getMaterial()->getRefraction())       //折射
    {
        float refraction_rate=hit.getMaterial()->getRefraction_rate();
        float relative=1;
        if (isAir)
            relative=1/refraction_rate;
        else
            relative=refraction_rate;
        
        float cos2r = (1.0-Vector3f::dot(ray.getDirection().normalized(), hit.getNormal().normalized())*Vector3f::dot(ray.getDirection().normalized(), hit.getNormal().normalized()))*(relative*relative);
        if (cos2r<1)            //若是全反射，则不需要处理，自动归到下一步的反射来处理
        {
            float cosr=sqrt(1.0-cos2r);
            Ray refraction_ray(ray.pointAtParameter(hit.getT()), (ray.getDirection().normalized()*relative+(-Vector3f::dot(ray.getDirection().normalized(), hit.getNormal().normalized())*relative-cosr)*hit.getNormal().normalized()).normalized());
            return hit.calc(PathTracing(refraction_ray, group, depth-1, !isAir));
        }
    }

    if (x<hit.getMaterial()->getReflection()+hit.getMaterial()->getRefraction())        //反射
    {
        Ray reflection_ray(ray.pointAtParameter(hit.getT()), (ray.getDirection().normalized()-2*Vector3f::dot(ray.getDirection().normalized(), hit.getNormal())*hit.getNormal()).normalized());
        return hit.calc(PathTracing(reflection_ray, group, depth-1, isAir));
    }
    //漫反射
    float alpha=(rand()%1000)/1000.0;
    alpha=alpha*2*M_PI;
    float r=(rand()%1000)/1000.0;
    Vector3f c=hit.getNormal().normalized();
    Vector3f a=ray.getDirection();
    if (Vector3f::cross(a, c).length()<0.000001) a=a+Vector3f::UP;
    if (Vector3f::cross(a, c).length()<0.000001) a=a+Vector3f::RIGHT;
    a=a-Vector3f::dot(a, c)*c;
    a.normalize();
    Vector3f b=Vector3f::cross(a, c).normalized();
    Vector3f v=sqrt(r)*cos(alpha)*a+sqrt(r)*sin(alpha)*b+sqrt(1-r)*c;
    Ray diffuse_ray(ray.pointAtParameter(hit.getT()), v);
    return hit.calc(PathTracing(diffuse_ray, group, depth-1, isAir));
}

int main(int argc, char *argv[]) {
    for (int argNum = 1; argNum < argc; ++argNum) {
        std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
    }

    bool newImage=true;
    Image* original = nullptr;
    int startsamples = 0;
    if (argc != 4) {
        newImage=false;
        original = Image::LoadTGA(argv[4]);
        startsamples = atoi(argv[5]);
    }
    string inputFile = argv[1];
    string outputFile = argv[2];
    int samples = atoi(argv[3]);

    SceneParser parser(argv[1]);
    Camera* camera=parser.getCamera();
    Group* group=parser.getGroup();
    Image image(camera->getWidth(), camera->getHeight());

    if (!newImage && (original->Width() != image.Width() || original->Height() != image.Height()))
    {
        newImage=true;
        startsamples=0;
    }

    #pragma omp parallel for
    for (int x=0; x<camera->getWidth(); x++)
        for (int y=0; y<camera->getHeight(); y++)
        {
            fprintf(stderr,"\rRendering (%d spp) %5.2f%%",samples*4,100.0*(x*camera->getHeight()+y)/(camera->getWidth()*camera->getHeight()));
            Vector3f color=Vector3f::ZERO;
            //若是在原图片基础上增加样例数，则对原始颜色进行处理
            if (!newImage)
            {
                color=original->GetPixel(x, y)*startsamples/samples;
            }
            for (int i=startsamples; i<samples; i++)
                for (int sx=0; sx<2; sx++)              //对每个像素分成四个小像素，实现抗锯齿功能
                    for (int sy=0; sy<2; sy++)
                    {
                        float r1=(rand()%1000)/1000.0;
                        float r2=(rand()%1000)/1000.0;
                        float dx=0;
                        float dy=0;
                        if (r1<0.5)
                            dx=sqrt(2*r1)-1;
                        else
                            dx=1-sqrt(2-2*r1);
                        if (r2<0.5)
                            dy=sqrt(2*r2)-1;
                        else
                            dy=1-sqrt(2-2*r2);
                        Ray ray=camera->generateRay(Vector2f((sx+dx+0.5)/2+x, (sy+dy+0.5)/2+y));
                        color+=PathTracing(ray, group, 7, true)/(4*samples);
                    }
            image.SetPixel(x, y, color);
        }

    image.SaveImage(argv[2]);
    cout << "Hello! Computer Graphics!" << endl;
    return 0;
}

