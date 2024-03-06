#ifndef MATH_CALCULATION_FUNC_H
#define MATH_CALCULATION_FUNC_H

#include <stdio.h>
#include <math.h>

enum MathFuncStatus {

    MATH_FUNC_STATUS_OK,
    MATH_FUNC_STATUS_FAIL
};

struct MeasurementData {

    double value;
    double error;
};

struct Measurement {

    MeasurementData *data;
    double           systematic_error;
    size_t           measurement_number;
    size_t           data_capacity;
};

const int    INCREASE_NUM          = 2;
const int    DEFAULT_DATA_CAPACITY = 4;
const double MSR_POISON            = NAN;

MathFuncStatus MeasurementCtor (Measurement *measurement_struct);

MathFuncStatus MeasurementDtor (Measurement *measurement_struct);

MathFuncStatus MeasurementDataCtor (Measurement *measurement_struct);

MathFuncStatus MeasurementDataDtor (Measurement *measurement_struct);

MathFuncStatus MeasurementValueAdd (Measurement *msr_struct, FILE *file_to_read);

MathFuncStatus MeasurementDataRecalloc (Measurement *measurement_struct);

double MeasurementMeanEval (const Measurement *measurement_struct);

double MeasurementRandErrEval (const Measurement *measurement_struct);

double MeasurementTotalErrEval (const Measurement *measurement_struct);

bool DoublesAreBitsEqual (const double number_1, const double number_2);

#endif
