#ifndef DIMVECANAL_H
#define DIMVECANAL_H

#include "Partition.h"
#include "DimVec.h"
#include "DimVecBank.h"


class dim_vec_layer_analyser : public dim_vec::vector_of_ptr
{
    const dim_vec_layer& theLayer;
    //  Stats
    nat num_roots, num_real, num_imag, num_simple, num_fundamental, num_max_coarse, num_max_coarse_imag, num_reducible;
public:
    dim_vec_layer_analyser(const dim_vec_layer& layer);
    
    nat count_pred(const dim_vec_pred pred) const;
    void copy_pred(const dim_vec_pred pred, dim_vec::vector_of_ptr& out) const;

    const dim_vec_layer& GetDimVecLayer() const { return theLayer; }

    void ComputeStatistics();

    void PrintTitle(std::ostream& cout) const;
    void PrintSummary(std::ostream& cout) const;
    void PrintAll(std::ostream& cout) const;
    void PrintPred(const dim_vec_pred pred, std::ostream& cout) const;

    void PrintReductionSequence(nat spaces, const dim_vec&, std::ostream& cout) const;
    void PrintReductionSequenceWeights(nat space, const dim_vec&, std::ostream& cout) const;
};


#endif