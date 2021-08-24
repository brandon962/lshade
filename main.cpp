#include "lshade.hpp"

int main(int argc, char **argv)
{
    srand(time(NULL));
    Parameter lshade_parameter;
    lshade_parameter.run = 1;
    lshade_parameter.degree = 10;
    lshade_parameter.func = 1;
    lshade_parameter.MAX_NFE = 100000;

    LSHADE lshade = LSHADE(lshade_parameter);

    lshade.Run();

    return 0;
}