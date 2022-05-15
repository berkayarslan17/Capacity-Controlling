#ifndef FILTER_H
#define FILTER_H


#define AVERAGE_LEVEL 3

int kalman_update(double, double);
int median_filter(int arr[]);
int mov_average_filter(int arr[], int arr_size);
int weighted_mov_average_filter(int arr[], int arr_size);
#endif
