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


void TestPartitionBank()
{

}


void TestDimVecBank2()
{
    nat maxWeight = 12;
    nat numArms = 3;
    int_partition_bank thePartnBank(maxWeight);
    
    thePartnBank.Build();

    dim_vec_bank theDimVecBank(thePartnBank,numArms);
    theDimVecBank.BuildAll();
    for(const dim_vec_layer& layer : theDimVecBank)
    {
        std::cout << "Layer " << layer.GetN() << std::endl;
        for(const dim_vec& alpha : layer)
        {
            //alpha.PrintArms(0,std::cout);
            //alpha.PrintArmIndices(0, layer.GetPartitionLayer(), std::cout);
            if(alpha.GetArms() != layer(alpha.GetArms()).GetArms())
            {
                std::cout << "Dimension Vector out of place!" << std::endl;
                /*std::cout
                 << alpha.GetArms().GetIndex(layer.GetPartitionLayer()) << " "
                 << layer(alpha.GetArms()).GetArms().GetIndex(layer.GetPartitionLayer())
                 << std::endl;
                alpha.PrintArms(0,std::cout);
                layer(alpha.GetArms()).PrintArms(0,std::cout);*/
            }
        }
    }
}


void TestDimVecBank()
{
    nat maxWeight = 8;
    nat numArms = 3;
    int_partition_bank thePartnBank(maxWeight);
    
    thePartnBank.Build();

    std::set<dim_vec_key> v1, v2;
    dim_vec_key temp_key(numArms);

    for(nat n = 1; n <= thePartnBank.GetMaxWeight(); ++n)
    {
        std::cout << "Creating lexicographical and reverse lexicographical dimension vectors of weight " << n << std::endl;

        temp_key.MakeInitial(thePartnBank[n]);

        v1.clear();
        v1.insert(temp_key);
        while(temp_key.lexico_next(thePartnBank[n]))
            v1.insert(temp_key);
        

        std::cout << "Lexicographical dimension vectors finished " << std::endl;


        temp_key.MakeFinal(thePartnBank[n]);

        v2.clear();
        v2.insert(temp_key);
        while(temp_key.lexico_prev(thePartnBank[n]))
            v2.insert(temp_key);

        std::cout << "Reverse lexicographical dimension vectors finished " << std::endl;

        if(v1.size() == v2.size())
        {
            std::cout << "  " << "Number of dimension vectors of weight " << n << " are the same" << std::endl;
            if(v1 == v2)
                std::cout << "  " << "Dimension vectors of weight " << n << " are equal" << std::endl;
            else
                std::cout << "  " << "Dimension vectors of weight " << n << " are not equal" << std::endl;
        }
        else
            std::cout << "  " << "Number of dimension vectors of weight " << n << " are not the same" << std::endl;
    }
}