#ifndef DIMVECSPACE_H
#define DIMVECSPACE_H

#include <map>

#include "Partition.h"
#include "DimVec.h"

typedef bool(*dim_vec_pred)(const dim_vec&);
class dim_vec_space : public dim_vec::vector_of
{
    //dim_vec::vector_of theDimVecs;

    std::map<int_partition_layer::const_iterator,iterator> beginByFirstArm;
    std::map<int_partition_layer::const_iterator,const_iterator> cbeginByFirstArm;
    std::map<dim_vec_key,const_iterator> cmap;

    void remap();
public:
protected:
    const int_partition_layer& thePartnLayer;

    nat K;

    std::map<dim_vec_key,nat> index;

    void insert_if(dim_vec_pred pred, const dim_vec_key& temp_key);
public:
    dim_vec_space(const int_partition_layer& partnLayer, nat k);

    const int_partition_layer& GetPartitionLayer() const;
    nat GetN() const;
    nat GetK() const;
    
    dim_vec& operator()(const dim_vec_key& arms);
    const dim_vec& operator()(const dim_vec_key& arms) const;

    void BuildAll();
    void Build(dim_vec_pred pred);
    void BuildEqualArms();

    void Write(std::ostream& cout) const;
    void Read(std::istream& cin);
};




#endif