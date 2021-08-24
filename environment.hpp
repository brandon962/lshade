#pragma once

#include <vector>
#include <iostream>

using namespace std;

typedef vector<int> i1d;
typedef vector<i1d> i2d;
typedef vector<i2d> i3d;
typedef vector<i3d> i4d;
typedef vector<double> d1d;
typedef vector<d1d> d2d;
typedef vector<d2d> d3d;
typedef vector<d3d> d4d;
typedef vector<d4d> d5d;
typedef vector<d5d> d6d;
typedef vector<bool> b1d;

struct Parameter
{
    int run;
    int degree;
    int max_evo;
    int func;
};