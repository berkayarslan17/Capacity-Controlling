#include "filter.h"

double _old_estimate = 0;
double expect_estimated = 1;

int kalman_update(double filter_value, double _err_measure)
{
  // static double _old_estimate = 0;
  // static double expect_estimated = 1;
  double _last_estimate = _old_estimate;
  double _err_estimate = expect_estimated;
  double _kalman_gain = _err_estimate / (_err_estimate + _err_measure);
  double _current_estimate = _last_estimate + _kalman_gain * (filter_value - _last_estimate);
  _err_estimate = (1 - _kalman_gain) * expect_estimated;

  _old_estimate = _current_estimate;

  return (int)_current_estimate;
}

int mov_average_filter(int arr[], int arr_size)
{
  int sum = 0, result = 0;

  for (int i = 0; i < arr_size; i++)
  {
    sum += arr[i];
  }

  result = sum / arr_size;

  return result;
}

int weighted_mov_average_filter(int arr[], int arr_size)
{
  int sum = 0, result = 0;

  for (int i = 0; i < arr_size; i++)
  {
    sum += (i + 1) * (i + 1) * arr[i];
  }

  // Sum of squares formula
  result = sum / ((arr_size) * (arr_size + 1) * (2 * arr_size + 1) / 6);

  return result;
}
/* Function to sort an array using insertion sort*/
static void sort(int arr[], int n)
{
  int i, key, j;
  for (i = 1; i < n; i++)
  {
    key = arr[i];
    j = i - 1;

    /* Move elements of arr[0..i-1], that are
        greater than key, to one position ahead
        of their current position */
    while (j >= 0 && arr[j] > key)
    {
      arr[j + 1] = arr[j];
      j = j - 1;
    }
    arr[j + 1] = key;
  }
}

int median_filter(int arr[])
{
  sort(arr, 5);
  return arr[2];
}
