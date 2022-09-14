#ifndef DIMVECBANKMAP_H
#define DIMVECBANKMAP_H

#include "Partition.h"
#include "DimVec.h"
#include "DimVecLayer.h"



class dim_vec_bank : public std::vector<dim_vec_layer>
{
    const int_partition_bank& thePartnBank;
    nat K;
public:
    dim_vec_bank(const int_partition_bank& partnBank, nat k);
    void Build(dim_vec_pred pred);
    void BuildAll();

    void ComputeRootStatus();
    void ComputeRootStatusOfReducibles();
    void ComputeDominanceStatus();
    void ComputeEquiBrachialIndices();
    void BuildProperties();
    void CountReflectees();

    nat GetK() const;

    void Read();
    void Write() const;
};



#endif