#pragma once

#include <vector>
#include <iostream>
#include <time.h>
#include <string>
#include <chrono>
#include <algorithm>
#include <random>
#include "cec17_test_func.hpp"

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
    int MAX_NFE;
    int func;
};

int itemp;
double dtemp;
string stemp;