#include<iostream>
#include<vector>
#include<glm/vec3.hpp>
#include <utility>
#include <algorithm>

using namespace std;
using namespace glm;

float phi = (1.0f + sqrt(5.0f)) / 2.0f;

vector<pair<int, int>> edges_id;

vector<vec3> vertices = {
    {-1,  phi, 0}, { 1,  phi, 0}, {-1, -phi, 0}, { 1, -phi, 0},
    { 0, -1,  phi}, { 0,  1,  phi}, { 0, -1, -phi}, { 0,  1, -phi},
    { phi, 0, -1}, { phi, 0,  1}, {-phi, 0, -1}, {-phi, 0,  1}
};

// 每個面的頂點索引
vector<array<int,3>> faces = {
    {0,11,5}, {0,5,1}, {0,1,7}, {0,7,10}, {0,10,11},
    {1,5,9}, {5,11,4}, {11,10,2}, {10,7,6}, {7,1,8},
    {3,9,4}, {3,4,2}, {3,2,6}, {3,6,8}, {3,8,9},
    {4,9,5}, {2,4,11}, {6,2,10}, {8,6,7}, {9,8,1}
};

void mesh_generator(){
    for (auto&face : faces){

        pair<int, int> e1 = {face[0], face[1]};
        pair<int, int> e2 = {face[1], face[2]};
        pair<int, int> e3 = {face[0], face[2]};

        if (e1.first > e1.second) swap(e1.first, e1.second);
        if(find(edges_id.begin(), edges_id.end(), e1) == edges_id.end()){
            edges_id.push_back(e1);
        };

        if (e2.first > e2.second) swap(e2.first, e2.second);
        if(find(edges_id.begin(), edges_id.end(), e2) == edges_id.end()){
            edges_id.push_back(e2);
        };
        if (e3.first > e3.second) swap(e3.first, e3.second);
        if(find(edges_id.begin(), edges_id.end(), e3) == edges_id.end()){
            edges_id.push_back(e3);
        };       
    }


    //cout << edges_list << endl;
}
