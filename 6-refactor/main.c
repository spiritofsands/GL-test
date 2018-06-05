enum {
  STATUS_OK,
  STATUS_NOK
};

struct retdata {
  int minimum;
  int maximum;
  int average;
};

int calculate_min_max_avg(int *array, unsigned int length, struct retdata *result)
{
  result->average = array[0];
  result->minimum = array[0];
  result->maximum = array[0];

  for (unsigned int index = 1; index < length; ++index) {
    if (array[index] < result->minimum) {
      result->minimum = array[index];
    }

    if (array[index] > result->maximum) {
      result->maximum = array[index];
    }

    result->average += array[index];
  }

  result->average /= length;

  return STATUS_OK;
}
