#include "lshade.hpp"

int main(int argc, char **argv)
{
    Parameter lshade_parameter;
    lshade_parameter.run = 1;
    lshade_parameter.degree = 10;
    lshade_parameter.func = 1;
    lshade_parameter.max_evo = 100000;

    LSHADE lshade = LSHADE(lshade_parameter);

    lshade.run();

    return 0;
}