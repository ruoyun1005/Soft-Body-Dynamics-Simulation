#pragma once
#include<iostream>
#include<vector>
#include<glm/vec3.hpp>
#include <utility>
#include <algorithm>
#include<Eigen/Dense>


#include "mesh_generator.h"
using namespace std;
using namespace glm;
using namespace Eigen;

struct Point {
    vec3 x, v;
    float m;
};

struct Edge {
    int p1, p2;
    float l0, k;

};

struct Force {
    vec3 f1, f2;
};

vector<Point> points_list;
vector<Edge> edges_list;

MatrixXf H = MatrixXf::Zero(12, 12);

void mesh_structure(){
    for(auto& point : vertices){
        Point p;
        p.x = point;
        p.v = vec3 (0.0f, 0.0f, 0.0f);
        p.m = 1.0f;

        points_list.push_back(p);
    }

    for(auto& edge : edges_id){
        Edge e;
        e.p1 = edge.first;
        e.p2 = edge.second;
        e.l0 = distance(points_list[e.p1].x, points_list[e.p2].x);
        e.k = 100.0f;
        cout << e.l0 << endl;
        edges_list.push_back(e);

    }
}

Force compute_force(vector<Point> points_list, Edge edge){
    vec3 p1 = points_list[edge.p1].x;
    vec3 p2 = points_list[edge.p2].x;

    vec3 r = p1 - p2;
    float d = distance(p1, p2);
    

    vec3 f1 = -edge.k*(d - edge.l0)*r/d;
    vec3 f2 = -f1;
    Force F;
    F.f1 = f1;
    F.f2 = f2;
    return F;

}


MatrixXf compute_H(vector<Edge> edges_list){
    for (auto& e : edges_list){
        //cout << "p1=" << e.p1 << " p2=" << e.p2 << " k=" << e.k << endl;
        H(e.p1, e.p2) += e.k;
        H(e.p2, e.p1) += e.k;
        H(e.p1, e.p1) -= e.k;
        H(e.p2, e.p2) -= e.k;
    }

    return H;
}

MatrixXf compute_W(float dt, MatrixXf H, float m){
    MatrixXf I = MatrixXf::Identity(12, 12);
    MatrixXf A = I - dt*dt/m*H;
    MatrixXf W = A.inverse();

    return W;
}