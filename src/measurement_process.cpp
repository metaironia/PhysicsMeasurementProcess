#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "measurement_process.h"

MathFuncStatus MeasurementCtor (Measurement *measurement_struct) {

    assert (measurement_struct);

    measurement_struct -> systematic_error   = MSR_POISON;
    measurement_struct -> measurement_number = 0;
    measurement_struct -> data_capacity      = DEFAULT_DATA_CAPACITY;

    MeasurementDataCtor (measurement_struct);

    return MATH_FUNC_STATUS_OK;
}

//------------------------------------------------------------------------

MathFuncStatus MeasurementDtor (Measurement *measurement_struct) {

    assert (measurement_struct);

    MeasurementDataDtor (measurement_struct);

    measurement_struct -> systematic_error   = MSR_POISON;
    measurement_struct -> data_capacity      = DEFAULT_DATA_CAPACITY;
    measurement_struct -> measurement_number = 0;

    return MATH_FUNC_STATUS_OK;
}

//------------------------------------------------------------------------

MathFuncStatus MeasurementDataCtor (Measurement *measurement_struct) {

    assert (measurement_struct);

    const size_t data_size = measurement_struct -> data_capacity;

    measurement_struct -> data = (MeasurementData *) calloc (data_size, sizeof (MeasurementData));

    if (!(measurement_struct -> data))
        return MATH_FUNC_STATUS_FAIL;

    return MATH_FUNC_STATUS_OK;
}

//------------------------------------------------------------------------

MathFuncStatus MeasurementDataDtor (Measurement *measurement_struct) {

    assert (measurement_struct);
    assert (measurement_struct -> data);

    for (size_t i = 0; i < measurement_struct -> data_capacity; i++) {

        (measurement_struct -> data)[i].value = MSR_POISON;
        (measurement_struct -> data)[i].error = MSR_POISON;
    }

    return MATH_FUNC_STATUS_OK;
}

//------------------------------------------------------------------------

MathFuncStatus MeasurementValueAdd (Measurement *msr_struct, FILE *file_to_read) {

    assert (msr_struct);
    assert (msr_struct -> data);

    if (msr_struct -> measurement_number == msr_struct -> data_capacity)
        MeasurementDataRecalloc (msr_struct);

    const int fscanf_status = fscanf (file_to_read, " %lg",
                                      &(msr_struct -> data)[(msr_struct -> measurement_number)++].value);

    if (fscanf_status == 0 || fscanf_status == EOF)
        return MATH_FUNC_STATUS_FAIL;

    return MATH_FUNC_STATUS_OK;
}

//------------------------------------------------------------------------

MathFuncStatus MeasurementDataRecalloc (Measurement *measurement_struct) {

    assert (measurement_struct);
    assert (measurement_struct -> data);

    MeasurementData *msr_data  = measurement_struct -> data;

    size_t prev_data_capacity  = measurement_struct -> data_capacity;
    size_t new_data_capacity   = prev_data_capacity * INCREASE_NUM;

    measurement_struct -> data = (MeasurementData *) realloc (msr_data,
                                                              new_data_capacity * sizeof (MeasurementData));

    if (!msr_data)
        return MATH_FUNC_STATUS_FAIL;

    memset (msr_data + prev_data_capacity, 0, new_data_capacity - prev_data_capacity);

    return MATH_FUNC_STATUS_OK;
}

//------------------------------------------------------------------------

double MeasurementMeanEval (const Measurement *measurement_struct) {

    assert (measurement_struct);
    assert (measurement_struct -> data);

    double total_value = 0;

    for (size_t i = 0; i < measurement_struct -> measurement_number; i++)
        total_value += (measurement_struct -> data)[i].value;

    return total_value / ((double) (measurement_struct -> measurement_number));
}

//------------------------------------------------------------------------

double MeasurementRandErrEval (const Measurement *measurement_struct) {

    assert (measurement_struct);
    assert (measurement_struct -> data);

    const size_t msr_number = measurement_struct -> measurement_number;

    if (msr_number == 1)
        return NAN;

    const double mean_value     = MeasurementMeanEval (measurement_struct);
    double       sum_of_squares = 0;

    for (size_t i = 0; i < msr_number; i++)
        sum_of_squares += pow (((measurement_struct -> data)[i].value - mean_value), 2);

    return sqrt (sum_of_squares / ((double) (msr_number * (msr_number - 1))));
}

//------------------------------------------------------------------------

MathFuncStatus MeasurementSystErrInput (Measurement *measurement_struct) {

    assert (measurement_struct);
    assert (measurement_struct -> data);

    printf ("Enter systematic error of measurement:\n");

    int scanf_status = scanf (" %lg", &(measurement_struct -> systematic_error));

    while (scanf_status == 0) {

        printf ("Unknown error happened during input. Please, input again:\n");

        scanf_status = scanf (" %lg", &(measurement_struct -> systematic_error));
    }

    if (scanf_status == EOF) {

        printf ("Reached end-of-file. Systematic error won't be inputted.\n");
        return MATH_FUNC_STATUS_FAIL;
    }

    return MATH_FUNC_STATUS_OK;
}

//------------------------------------------------------------------------

double MeasurementTotalErrEval (const Measurement *measurement_struct) {

    assert (measurement_struct);
    assert (measurement_struct -> data);

    const double rand_err_value = MeasurementRandErrEval (measurement_struct);
    const double syst_err_value = measurement_struct -> systematic_error;

    if (DoublesAreBitsEqual (syst_err_value, MSR_POISON))
        return rand_err_value;

    return sqrt (pow (rand_err_value, 2) + pow (syst_err_value, 2));
}

//------------------------------------------------------------------------

bool DoublesAreBitsEqual (const double number_1, const double number_2) {

    const int are_bits_equal = memcmp (&number_1, &number_2, sizeof (double));

    return !are_bits_equal;
}
