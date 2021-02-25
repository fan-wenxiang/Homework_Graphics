#include "mesh.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <utility>
#include <sstream>

bool Mesh::intersect(const Ray &r, Hit &h, float tmin) {
    Hit hit;
    bool result = surround.intersect(r, hit, 0);
    if (!result) return false;
    result=false;
    for (int triId = 0; triId < (int) group.size(); ++triId)
    {
        result |= group[triId].intersect(r, h, tmin);
    }
    return result;
}

Mesh::Mesh(const char *filename, Material *material) : Object3D(material) {

    // Optional: Use tiny obj loader to replace this simple one.
    std::ifstream f;
    f.open(filename);
    if (!f.is_open()) {
        std::cout << "Cannot open " << filename << "\n";
        return;
    }
    std::string line;
    std::string vTok("v");
    std::string fTok("f");
    std::string texTok("vt");
    char bslash = '/', space = ' ';
    std::string tok;
    int texID;
    while (true) {
        std::getline(f, line);
        if (f.eof()) {
            break;
        }
        if (line.size() < 3) {
            continue;
        }
        if (line.at(0) == '#') {
            continue;
        }
        std::stringstream ss(line);
        ss >> tok;
        if (tok == vTok) {
            Vector3f vec;
            ss >> vec[0] >> vec[1] >> vec[2];
            v.push_back(vec);
        } else if (tok == fTok) {
            if (line.find(bslash) != std::string::npos) {
                std::replace(line.begin(), line.end(), bslash, space);
                std::stringstream facess(line);
                TriangleIndex trig;
                facess >> tok;
                for (int ii = 0; ii < 3; ii++) {
                    facess >> trig[ii] >> texID;
                    trig[ii]--;
                }
                t.push_back(trig);
            } else {
                TriangleIndex trig;
                for (int ii = 0; ii < 3; ii++) {
                    ss >> trig[ii];
                    trig[ii]--;
                }
                t.push_back(trig);
            }
        } else if (tok == texTok) {
            Vector2f texcoord;
            ss >> texcoord[0];
            ss >> texcoord[1];
        }
    }

    for (int triId = 0; triId < (int) t.size(); ++triId) {
        TriangleIndex& triIndex = t[triId];
        Triangle triangle(v[triIndex[0]],
                          v[triIndex[1]], v[triIndex[2]], material);
        group.push_back(triangle);
    }

    //求包围球，先取一个初始的包围球
    int maxx=0, minx=0, maxy=0, miny=0, maxz=0, minz=0;
    for (int id = 0; id < (int) v.size(); ++id)
    {
        if (v[id].x()>v[maxx].x()) maxx=id;
        if (v[id].x()<v[minx].x()) minx=id;
        if (v[id].y()>v[maxy].y()) maxy=id;
        if (v[id].y()<v[miny].y()) miny=id;
        if (v[id].z()>v[maxz].z()) maxz=id;
        if (v[id].z()<v[minz].z()) minz=id;
    }
    Vector3f point1=v[maxx], point2=v[minx];
    if ((v[maxy]-v[miny]).length()>(point1-point2).length())
    {
        point1=v[maxy];
        point1=v[miny];
    }
    if ((v[maxz]-v[minz]).length()>(point1-point2).length())
    {
        point1=v[maxz];
        point2=v[minz];
    }

    surround.setCenter((point1+point2)/2);
    surround.setRadius((point1-point2).length()/2);

    //进行迭代，对每个在现有包围球外的点，扩大包围球将其纳入其中，并更新包围球，最终可将所有点纳入其中
    for (int id = 0; id < (int) v.size(); ++id)
    {
        if (!surround.inSphere(v[id]))
        {
            point1=v[id];
            point2=surround.getCenter()+surround.getRadius()*((surround.getCenter()-point1).normalized());
            surround.setCenter((point1+point2)/2);
            surround.setRadius((point1-point2).length()/2);
        }
    }

    f.close();
}
