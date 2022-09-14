#ifndef PYTHON_H
#define PYTHON_H


#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <fstream>
#include <iterator>

#include "Partition.h"
#include "DimVec.h"
#include "DimVecBank.h"
#include "DimVecAnalyser.h"

class PythonEngine : public dim_vec::vector_of_ptr
{
    std::ofstream myfile;
    const dim_vec_layer_analyser* pAnalyser;
public:
    void Init(const dim_vec_layer_analyser& analyser) { pAnalyser = &analyser; }

    bool Python2DInit();
    void Python2D(const int_partition_bank& thePartnBank, const int_partition_layer::vector_of_ptr_bin& dots, char col, char mark);

    bool Python3DInit();
    void PythonAxis(nat idx, char var);
    void Python3D(char col, char mark);
    void PythonEnd();

    void GetDimVecs(const dim_vec_pred pred);
};

#endif