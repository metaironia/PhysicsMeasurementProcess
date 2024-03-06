#include <assert.h>

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

/*
MathFuncStatus MeasurementValueArrayInput (FILE *array_file, MeasuredDataPoint *data_x) {

    assert (array_file);



    for (size_t i = 0; !feof (array_file); i++) {

        fscanf (array_file, " %lg", (data_x -> value)[i]);
    }

    return MATH_FUNC_STATUS_OK;
}

MathFuncStatus MeasuredDataRealloc (MeasuredData *data) {

    assert (data);

    if (data -> measurement_number == data -> data_capacity)
        data -> data_point = (MeasurementDataPoint *) realloc (data -> data_point, data_capacity * INCREASE_NUM);



}

MathFuncStatus MeasurementErrorArrayInput ()

MathFuncStatus Measurement

MathFuncStatus MeasurementTotalErrorEval

MathFuncStatus MeasurementRandomErrorEval
*/
