#pragma once
#include<iostream>

#include<vector>

using namespace std;


//vector<float> x_history = {};


Point1 inplicit_euler(float x, float v, int k, int m, float dt){
    
    float omega = k/m;
    float x1 = ((1 - omega*dt)*x + v*dt)/(1 + omega*dt);
    float v1 = (v - omega*x*dt)/(1 + omega*dt); 

    return {x1, v1};
}
