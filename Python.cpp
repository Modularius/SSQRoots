#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <fstream>
#include <iterator>

#include "Partition.h"
#include "DimVec.h"
#include "DimVecBank.h"
#include "DimVecAnalyser.h"
#include "Python.h"


using namespace std;


void PythonEngine::GetDimVecs(const dim_vec_pred pred) {
    clear();
    pAnalyser->copy_pred(pred,*this);
}

bool PythonEngine::Python2DInit() {
    char filename[8] = "plot.py";
    myfile.open(filename, ios::out | ios::trunc);
    if(!myfile.is_open())
        return false;

    myfile << "import matplotlib.pyplot as plt \n";
    myfile << "import matplotlib.colors as col \n";
    myfile << "fig = plt.figure(figsize = (10,10)) \n";
    myfile << "ax = fig.add_subplot(111) \n";

    return true;
}


void PythonEngine::Python2D(const int_partition_bank& thePartnBank, const int_partition_layer::vector_of_ptr_bin& dots, char col, char mark)
{
    myfile << "X = [] \n";
    myfile << "Y = [] \n";
    //const int_partition_layer& partnLayer = pAnalyser->GetDimVecLayer().GetPartitionLayer();
    for(nat n = 0; n < thePartnBank.size(); ++n)
    {
        for(int_partition_layer::vector_of_ptr::const_iterator i = dots[n].begin(); i != dots[n].end(); ++i)
        {
            myfile << "X.append("  << distance(thePartnBank[n].begin(), *i) << ") \n";
            myfile << "Y.append(" << (*i)->size() << ") \n";
        }
        //myfile << "Y.extend([" << n << "]*" << dots[n].size() << ") \n";
    }
    myfile << "ax.scatter(X,Y) \n";
}

bool PythonEngine::Python3DInit() {
    char filename[8] = "plot.py";
    myfile.open(filename, ios::out | ios::trunc);
    if(!myfile.is_open())
        return false;

    myfile << "import matplotlib.pyplot as plt \n";
    myfile << "import matplotlib.colors as col \n";
    myfile << "fig = plt.figure(figsize = (10,10)) \n";
    myfile << "ax = fig.add_subplot(111, projection = '3d') \n";

    return true;
}

void PythonEngine::PythonAxis(nat idx, char var) {
    dim_vec::vector_of_ptr::const_iterator i;
    itr_partn arm;

    const int_partition_layer& partnLayer = pAnalyser->GetDimVecLayer().GetPartitionLayer();
    
    i = begin();
    arm = (*begin())->GetArms()[idx];
    myfile << var << ".extend([" << distance(partnLayer.begin(), arm);
    for(++i; i != end(); ++i)
    {
        arm = (*i)->GetArms()[idx];
        myfile << ", " << distance(partnLayer.begin(), arm);
    }
    myfile << "]) \n";
}

void PythonEngine::Python3D(char col, char mark) {
    if(empty())
        return;

    myfile << "X = [] \n";
    myfile << "Y = [] \n";
    myfile << "Z = [] \n";
    myfile << "C = [] \n";
    PythonAxis(0, 'X');
    PythonAxis(1, 'Y');
    PythonAxis(2, 'Z');

    PythonAxis(0, 'X');
    PythonAxis(2, 'Y');
    PythonAxis(1, 'Z');

    PythonAxis(1, 'X');
    PythonAxis(0, 'Y');
    PythonAxis(2, 'Z');

    PythonAxis(1, 'X');
    PythonAxis(2, 'Y');
    PythonAxis(0, 'Z');

    PythonAxis(2, 'X');
    PythonAxis(0, 'Y');
    PythonAxis(1, 'Z');

    PythonAxis(2, 'X');
    PythonAxis(1, 'Y');
    PythonAxis(0, 'Z');
    
    myfile << "C.extend(['" << col << "']*" << 6*size() << ") \n";
    myfile << "ax.scatter(X, Y, Z, marker = '" << mark << "', c = C) \n";
}
void PythonEngine::PythonEnd() {
    myfile.close();
}