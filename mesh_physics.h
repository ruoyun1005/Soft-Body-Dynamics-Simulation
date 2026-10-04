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
    vec3 x, v, f;
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

MatrixXf H_init = MatrixXf::Zero(12, 12);

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
        // cout << e.l0 << endl;
        edges_list.push_back(e);

    }
}


Force compute_force(vector<Point>& points_list, Edge edge){
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

void accumulate_force(vector<Point>& points_list, vector<Edge>& edges_list){
    for(auto& p: points_list){
        p.f = vec3(0.0f);
    }
    for(auto& e : edges_list){
        Force F = compute_force(points_list, e);
        points_list[e.p1].f += F.f1;
        points_list[e.p2].f += F.f2;
    }
}


MatrixXf compute_H(vector<Edge>& edges_list){
    for (auto& e : edges_list){
        //cout << "p1=" << e.p1 << " p2=" << e.p2 << " k=" << e.k << endl;
        H_init(e.p1, e.p2) += e.k;
        H_init(e.p2, e.p1) += e.k;
        H_init(e.p1, e.p1) -= e.k;
        H_init(e.p2, e.p2) -= e.k;
    }

    return H_init;
}

MatrixXf compute_W(float dt, MatrixXf H, float m){
    MatrixXf I = MatrixXf::Identity(12, 12);
    MatrixXf A = I - dt*dt/m*H;
    MatrixXf W = A.inverse();

    return W;
}

void apply_W(vector<Point>& points_list, const MatrixXf& W, float dt, const MatrixXf&H){
    int n = points_list.size();
    VectorXf fx(n), fy(n), fz(n);
    VectorXf vx(n), vy(n), vz(n);

    for (int i = 0; i < n; i++){
        fx(i) = points_list[i].f.x;
        fy(i) = points_list[i].f.y;
        fz(i) = points_list[i].f.z;
        vx(i) = points_list[i].v.x;
        vy(i) = points_list[i].v.y;
        vz(i) = points_list[i].v.z;
    }

    VectorXf fx_new = W * (fx + dt*H*vx);
    VectorXf fy_new = W * (fy + dt*H*vy);
    VectorXf fz_new = W * (fz + dt*H*vz);


    for (int i = 0; i < n; i++){
        points_list[i].f = vec3(fx_new(i), fy_new(i), fz_new(i));
    }
}

void integrate(vector<Point>& points_list, float dt, vec3 gravity){
    for(auto& p : points_list){
        vec3 f_ext = gravity * p.m;
        p.v += (p.f + f_ext) * (dt / p.m);
        p.x += p.v*dt; 
    }
}

void collide_floor(vector<Point>& points_list, float floor_y, float restitution){
    for(auto& p : points_list){
        if(p.x.y < floor_y){
            p.x.y = floor_y;                 // 推回地板上
            if(p.v.y < 0.0f)
                p.v.y = -restitution * p.v.y; // 反彈並損失能量
        }
    }
}