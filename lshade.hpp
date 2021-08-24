#include "environment.hpp"
#include "mycompare.hpp"

using namespace std;

class LSHADE
{
public:
    Parameter info;
    double until_now_best;
    d1d run_result;

    LSHADE()
    {
        info.run = 1;
        info.degree = 10;
        info.func = 1;
        info.MAX_NFE = 100000;
    }
    LSHADE(Parameter p)
    {
        info = p;
    }
    int Run()
    {
        cout << "hi" << endl;
        Init();
        Evalution(solution, fitness);
        Sort_solution();
        while (NFE < info.MAX_NFE)
        {
            // cout << NFE << endl;
            Transition();
            cout << until_now_best << endl;
        }
        return 0;
    }

private:
    int NFE;
    d2d solution, archieve;
    d2d h_table;
    d1d fitness;
    d2d rank_index;
    int population, h_size;
    double uprange, lowrange;
    d1d V;
    int P, Pbest, r1, r2, archieve_size, archieve_max_size;
    int H_current;

    void Init()
    {
        population = info.degree * 18;
        uprange = 100.0;
        lowrange = -100.0;
        NFE = 0;
        h_size = 6;
        P = round(population * 0.11);
        archieve_max_size = 2.6 * info.degree * 18;
        until_now_best = __DBL_MAX__;
        H_current = 0;
        Init_vector();
        Random_start();
    }

    void Init_vector()
    {
        run_result.assign(info.MAX_NFE / 500 + 1, 0);
        solution.assign(population, d1d(info.degree, 0));
        // archieve.assign(population * 2.6, d1d(info.degree, 0));
        fitness.assign(population, 0);
        rank_index.assign(population, d1d(2, 0));
        h_table.assign(h_size, d1d(2, 0));
        V.assign(info.degree, 0);
    }

    void Random_start()
    {
        for (int p = 0; p < population; p++)
            for (int d = 0; d < info.degree; d++)
                solution[p][d] = ((double)rand() / (double)RAND_MAX) * (uprange - lowrange) + lowrange;
    }

    void Evalution(d2d sol, d1d fit)
    {
        for (int p = 0; p < population; p++)
        {
            cec17_test_func(&sol[p][0], &fit[p], info.degree, 1, info.func);
            NFE++;
            if (fit[p] < until_now_best)
                until_now_best = fit[p];
            if (NFE % 500 == 0)
                run_result[NFE / 500] += until_now_best;
        }
    }

    void Sort_solution()
    {
        for (int p = 0; p < population; p++)
        {
            rank_index[p][0] = fitness[p];
            rank_index[p][1] = p;
        }
        sort(rank_index.begin(), rank_index.end(), RANK_SORT);
    }

    void Transition()
    {
        d1d tempV;
        d2d solution_V(population, d1d(info.degree, 0));
        d1d fitness_V(population);
        d1d table_CR(population, 0), table_F(population, 0), S_CR, S_F, S_fitness;

        double CR, F;
        int d_rand, h_rand;
        for (int p = 0; p < population; p++)
        {
            d_rand = rand() % info.degree;
            h_rand = rand() % h_size;
            CR = Normal_Distribution(h_table[h_rand][0]);
            F = Cauchy_Distribution(h_table[h_rand][1]);
            tempV = Generate_solution(p, F);
            for (int d = 0; d < info.degree; d++)
            {
                if ((rand() / (RAND_MAX + 1.0)) < CR || d_rand == d)
                    solution_V[p][d] = tempV[d];
                else
                    solution_V[p][d] = solution[p][d];
            }
            table_CR[p] = CR;
            table_F[p] = F;
        }
        Evalution(solution_V, fitness_V);
        for (int p = 0; p < population; p++)
        {
            if (fitness_V[p] < fitness[p])
            {
                S_CR.push_back(table_CR[p]);
                S_F.push_back(table_F[p]);
                S_fitness.push_back(fitness[p] - fitness_V[p]);

                solution[p].assign(solution_V[p].begin(), solution_V[p].end());
                fitness[p] = fitness_V[p];

                archieve.push_back(solution[p]);
                if (archieve.size() > archieve_max_size)
                    archieve.erase(archieve.begin() + rand() % archieve.size());
            }
        }
        Update_H_table(S_CR, S_F, S_fitness);
        Sort_solution();
    }

    double Normal_Distribution(double mean)
    {
        if (mean < 0)
            return 0;

        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine g1(seed);
        std::normal_distribution<double> distribution(mean, 0.1);
        double dis = distribution(g1);
        if (dis > 1)
            return 1;
        else if (dis < 0)
            return 0;
        else
            return dis;
    }
    double Cauchy_Distribution(double mean)
    {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine g2(seed);
        std::cauchy_distribution<double> distribution(mean, 0.1);
        double dis = distribution(g2);
        while (dis <= 0)
        {
            std::cauchy_distribution<double> distribution(mean, 0.1);
            dis = distribution(g2);
        }
        if (distribution(g2) > 1)
            return 1;

        return dis;
    }

    d1d Generate_solution(int p, int F)
    {
        for (int d = 0; d < info.degree; d++)
            V[d] = 0;
        Pbest = Find_pbest();
        do
        {
            r1 = rand() % population;
        } while (r1 == p || r1 == Pbest);

        archieve_size = archieve.size();
        do
        {
            r2 = rand() % (population + archieve_size);
        } while (r2 == p || r2 == r1 || r2 == Pbest);

        for (int d = 0; d < info.degree; d++)
        {
            if (r2 < population)
                dtemp = solution[r2][d];
            else
                dtemp = archieve[r2 - population][d];
            V[d] = solution[p][d] + F * (solution[Pbest][d] - solution[p][d]) + F * (solution[r1][d] - dtemp);

            if (V[d] > uprange)
                V[d] = (solution[p][d] + uprange) / 2;
            if (V[d] < lowrange)
                V[d] = (solution[p][d] + lowrange) / 2;
        }

        return V;
    }

    int Find_pbest()
    {
        return (int)rank_index[rand() % (P + 1)][1];
    }

    void Update_H_table(d1d CR, d1d F, d1d fit_dif)
    {
        int size = CR.size();
        double sum = 0, CR_temp1, CR_temp2, F_temp1, F_temp2;
        if (size != 0)
        {
            for (int i = 0; i < size; i++)
                sum += fit_dif[i];
            CR_temp1 = CR_temp2 = F_temp1 = F_temp2 = 0.0;
            for (int i = 0; i < size; i++)
            {
                dtemp = (fit_dif[i] / sum);
                CR_temp1 += dtemp * CR[i] * CR[i];
                CR_temp2 += dtemp * CR[i];
                F_temp1 += dtemp * F[i];
                F_temp2 += dtemp * F[i];
            }

            
        }
    }
};