#include <math.h>

#include "measurement_process.h"

int main (const int argc, const char *argv[]) {

    Measurement x_axis = {};

    MeasurementCtor (&x_axis);

    MeasurementDtor (&x_axis);

    return 0;
}
