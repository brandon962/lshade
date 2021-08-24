#include "environment.hpp"

using namespace std;

class LSHADE
{
public:
    Parameter par;
    d2d solution;
    d2d h_table;

    LSHADE()
    {
        par.run = 1;
        par.degree = 10;
        par.func = 1;
        par.max_evo = 100000;
    }
    LSHADE(Parameter p)
    {
        par = p;
    }
    int run()
    {
        cout << "hi" << endl;
        return 0;
    }

private:
};