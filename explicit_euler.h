#pragma once
#include<iostream>

#include<vector>

using namespace std;

struct Point1 { double x, v; };

//vector<float> x_history = {};

Point1 explicit_euler(float x, float v, int k, int m, float dt){
    
    float f = k*x;
    float v1 = v - (f*dt/m);
    float x1 = x + v1*dt;

    return {x1, v1};
}
