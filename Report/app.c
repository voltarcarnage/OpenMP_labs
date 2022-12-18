#include "app.h"

void CG()
{
    int i, j, v, size, Np, it;
    float tv1, tv2;
    float dt1;
    mf =0;
    it = 0; 

    /* Генерация исходной матрицы коэффициентов и
правых частей уравнений */
    for(i = 0; i < M; i++)
    { 
        for(j = 0; j < M; j++)
        {
            if(i == j)
                A[i][j] = 12.0;
            else
                A[i][j] = 21.0;
        }
        F[i] = M + 1;
        mf += F[i]*F[i];
    }
    
    /* Вычисление нормы ||f|| */
    mf = sqrt(mf); 

    /* Задание начального приближения решений */

    for(i = 0; i < M; i++)
    { 
        Xk[i] = 0.2;
        Sz[i]=0;
    } 

    /* Задание начальных значений векторов невязки
и сопряженного направления */
    for(i = 0; i < M; i++)
    {
        for(j = 0; j < M; j++)
            Sz[i] += A[i][j] * Xk[j];
        Rk[i] = F[i] - Sz[i];
        Zk[i] = Rk[i];
    }

    tv1 = omp_get_wtime();
    printf("%f\n", tv1); 
    /* Для решения задачи запрашивается 4-е
процесса */
    omp_set_num_threads(4);
    
    /* Основной цикл */
    do
    {
        Spz = 0;
        Spr = 0;
        Spr1 = 0;
        /* Начало параллельного блока праграммы */
        #pragma omp parallel num_threads(4) private(size,i)
        {
            size = omp_get_num_threads();
            Np = M/size;
            /* Распараллеливание цикла, вычисляющего
числитель и знаменатель коэф. αk */
            #pragma omp for schedule(static,Np) private(j) reduction(+:Spz,Spr)
            for(i = 0; i < M; i++)
            { 
                for(Sz[i]=0, j = 0; j < M; j++)
                    Sz[i] += A[i][j] * Zk[j];
                Spz += Sz[i] * Zk[i];
                Spr += Rk[i] * Rk[i];
            }
            /* Вычисление коэффициента αk */
            #pragma omp critical
            alf = Spr/Spz;
            /* Распараллеливание цикла, вычисляющего
вектора решений и невязки */
            #pragma omp for schedule(static,Np) reduction(+:Spr1)
            for(i = 0; i < M; i++)
            { 
                Xk[i] += alf*Zk[i];
                Rk[i] -= alf*Sz[i];
                Spr1 += Rk[i]*Rk[i];
            }
            /* Вычисление коэффициента βk */
            #pragma omp critical
            bet = Spr1/Spr;
            /* Распараллеливание цикла, вычисляющего вектор
сопр. направления */
            #pragma omp for schedule(static,Np)
            for(i = 0; i < M; i++)
                Zk[i] = Rk[i] + bet*Zk[i];
        } /* Конец параллельного блока программы
*/
        it++;
    }while(sqrt(Spr1)/mf > E); /* Проверка на
точность */
    tv2 = omp_get_wtime();
    printf("%f\n", tv2);
    dt1 = tv2 - tv1;

    printf("Time = %f \n it=%d\n", dt1,it);

    printf("First eights roots for control: %f %f %f %f %f %f %f %f\n",Xk[0],Xk[1],Xk[2],Xk[3],Xk[4],Xk[5],Xk[6],Xk[7]); 
}
