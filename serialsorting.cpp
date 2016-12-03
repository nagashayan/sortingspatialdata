/**
 * Serial implementation of encoding and decoding of spatial data with latitude and longitude coordinates
 * then sorting the data using merge sort.
 * g++ filename.cpp -lm
 **/

#include <stdio.h>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <ctime>

#include "libmorton/libmorton/include/morton.h"

using namespace std;
int rows = 8;
int **mat1;
int *mat2;
unsigned int morton;
unsigned short lat,lng;

double lapse;


// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(int arr[], int l, int m, int r)
{
        int i, j, k;
        int n1 = m - l + 1;
        int n2 =  r - m;

        /* create temp arrays */
        int L[n1], R[n2];

        /* Copy data to temp arrays L[] and R[] */
        for (i = 0; i < n1; i++)
                L[i] = arr[l + i];
        for (j = 0; j < n2; j++)
                R[j] = arr[m + 1+ j];

        /* Merge the temp arrays back into arr[l..r]*/
        i = 0; // Initial index of first subarray
        j = 0; // Initial index of second subarray
        k = l; // Initial index of merged subarray
        while (i < n1 && j < n2)
        {
                if (L[i] <= R[j])
                {
                        arr[k] = L[i];
                        i++;
                }
                else
                {
                        arr[k] = R[j];
                        j++;
                }
                k++;
        }

        /* Copy the remaining elements of L[], if there
           are any */
        while (i < n1)
        {
                arr[k] = L[i];
                i++;
                k++;
        }

        /* Copy the remaining elements of R[], if there
           are any */
        while (j < n2)
        {
                arr[k] = R[j];
                j++;
                k++;
        }
}

/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
void mergeSort(int arr[], int l, int r)
{
        if (l < r)
        {
                // Same as (l+r)/2, but avoids overflow for
                // large l and h
                int m = l+(r-l)/2;

                // Sort first and second halves
                mergeSort(arr, l, m);
                mergeSort(arr, m+1, r);

                merge(arr, l, m, r);
        }
}

int main(){
        clock_t begin, end;
        /* start time after initialization*/
        begin = clock();
        int i = 0, j = 0, x=0, y = 0;

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

                mat2[i] = morton2D_32_encode(x,y);
                cout<<endl;
        }
        cout<<"Morton Encoding"<<endl;
        for(i = 0; i < rows; i++) {
                cout<<mat2[i]<<endl;

        }
        mergeSort(mat2, 0, rows - 1);
        cout<<endl<<"After sorting"<<endl;
        for (i=0; i < rows; i++)
                cout<<mat2[i]<<endl;


        cout<<"Morton Decoding"<<endl;
        for(i = 0; i < rows; i++) {
                morton = mat2[i];
                morton2D_32_decode(morton, lng, lat);
                cout<<lat<<"\t"<<lng<<endl;

        }

        cout<<endl;
        /*finish time after Work*/
        end = clock();
        /*print total time it took*/
        lapse = (double)(end - begin) / CLOCKS_PER_SEC;
        cout<<"This took: "<<lapse<<" milliseconds"<<endl;
        return 0;
}
