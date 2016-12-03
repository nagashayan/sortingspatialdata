/**
 * Serial implementation of encoding and decoding of spatial data with latitude and longitude coordinates
 * then sorting the data using merge sort.
 * g++ filename.cpp -lm -std=c++11
 **/

#include <stdio.h>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <ctime>
#include <pthread.h>
#include <thread>
#include <future>

#include "libmorton/libmorton/include/morton.h"

using namespace std;
int rows = 16;
int **mat1;
int *mat2;
unsigned int morton;
unsigned short lat,lng;
double lapse;

typedef struct {
        int lat;
        int lng;
        int i;

} parm;

void *encode(void *arg)
{
        parm *p=(parm *)arg;
        //printf("Using Thread: %d\n", p->i);

        mat2[p->i] = morton2D_32_encode(p->lat,p->lng);

        pthread_exit(arg);
}

void *decode(void *arg)
{
        parm *p=(parm *)arg;
        //printf("Using Thread: %d\t%d \n", p->i,mat2[p->i]);
        unsigned short lat,lng;
        unsigned int morton;
        morton = mat2[p->i];

        morton2D_32_decode(morton, lng, lat);
        mat1[p->i][0] = lat;
        mat1[p->i][1] = lng;

        pthread_exit(arg);
}

template<typename Iter>

void mergesort(Iter begin, Iter end,
               unsigned int N = thread::hardware_concurrency()/2)
{
        auto len = distance(begin, end);
        if (len <= 1024 || N < 2)
        {
                sort(begin,end);
                return;
        }

        Iter mid = next(begin, len/2);
        auto fn = async(mergesort_mt3<Iter>, begin, mid, N-2);
        mergesort(mid, end, N-2);
        fn.wait();
        inplace_merge(begin, mid, end);
}

int main(){
        clock_t begin, end;
        /* start time after initialization*/
        begin = clock();
        int i = 0, j = 0, x=0, y = 0;

        //create thread object
        pthread_t *threads;
        parm *p;

        threads = (pthread_t *) malloc(sizeof(threads));
        p=(parm *)malloc(sizeof(parm)*rows);

        cout<<"Initial Array";

        /*memory allocation for matrix*/
        mat1 = (int**)malloc(rows * sizeof(int *));
        mat2 = (int*)malloc(rows * sizeof(int *));
        if(mat1 == NULL)
        {
                fprintf(stderr, "out of memory\n");
                exit(1);
        }
        for(i = 0; i < rows; i++)
        {
                mat1[i] = (int*)malloc(rows * sizeof(int));
                if(mat1[i] == NULL)
                {
                        fprintf(stderr, "out of memory\n");
                        exit(1);
                }
        }

        cout<<endl;

        /*initializing*/
        for(i = 0; i < rows; i++) {
                for(j = 0; j < 2; j++) {
                        mat1[i][j] = rand() % 1000;
                        cout<<mat1[i][j]<<"\t";
                        if(j == 1)
                                x = mat1[i][j];
                        else
                                y = mat1[i][j];

                }


                //mat2[i] = morton2D_32_encode(x,y);

                //creating threads
                p[i].i=i;
                p[i].lat = x;
                p[i].lng = y;

                pthread_create(&threads[i], NULL, encode, (void *)(p+i));

                cout<<endl;
        }

        for (i = 0; i < rows; i++)
        {
                pthread_join(threads[i],NULL);

        }

        cout<<"Morton Encoding"<<endl;
        for(i = 0; i < rows; i++) {
                cout<<mat2[i]<<endl;

        }

        mergesort_mt3(mat2,mat2+rows);

        cout<<"Sorted Encoding"<<endl;
        for(i = 0; i < rows; i++) {
                cout<<mat2[i]<<endl;

        }

        cout<<"Morton Decoding"<<endl;
        for(i = 0; i < rows; i++) {

                p[i].i=i;
                pthread_create(&threads[i], NULL, decode, (void *)(p+i));

        }
        for (i = 0; i < rows; i++)
        {
                pthread_join(threads[i],NULL);

        }

        cout<<"Final Data:"<<endl;
        for(i = 0; i < rows; i++) {
                for(j = 0; j < 2; j++) {
                        cout<<mat1[i][j]<<"\t";
                }
                cout<<endl;
        }


        /*free the pointer memory*/
        //free(p);

        cout<<endl;
        /*finish time after Work*/
        end = clock();
        /*print total time it took*/
        lapse = (double)(end - begin) / CLOCKS_PER_SEC;
        cout<<"This took: "<<lapse<<" milliseconds"<<endl;
        return 0;
}
