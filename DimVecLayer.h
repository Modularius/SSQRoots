#ifndef DIMVECLAYER_H
#define DIMVECLAYER_H

#include "Partition.h"
#include "DimVec.h"
#include "DimVecSpace.h"



template<class C_ITR>
struct dim_vec_properties {
    typedef std::vector<C_ITR> vector_of;

    std::vector<nat> theDescentSequenceWeights;
    std::vector<std::vector<nat>> theDescentSequenceTops;
    std::vector<C_ITR> theDescentSequence;
    std::vector<C_ITR> theCoveringCoarsenings;
    std::vector<C_ITR> theCoveringRefinements;


    void PrintDescentSequenceWeights(std::ostream& cout) const {
        cout << "[ ";
        for(auto j = theDescentSequenceWeights.begin(); j != theDescentSequenceWeights.end(); ++j)
            cout << *j << " ";
        cout << "]" << std::endl;
    }
    template<class C_PTR>
    void PrintDescentSequenceTops(std::ostream& cout) const {;
        for(auto i = theDescentSequenceTops.begin(); i != theDescentSequenceTops.end(); ++i)
        {
            cout << "{ ";
            for(auto j = i->begin(); j != i->end(); ++j)
                cout << *j << " ";
            cout << "}" << std::endl;
        }
    }
};


class dim_vec_layer : public dim_vec_space
{
public:
    typedef std::vector<dim_vec_layer> vector_of;
    typedef dim_vec_properties<const dim_vec*> properties;
private:

    std::map<const dim_vec*, properties> theProperties;

    const dim_vec_layer::properties& build_properties_of(const dim_vec& alpha);
public:
    dim_vec_layer(const int_partition_layer& partnLayer, nat k);

    void ComputeRootStatus();
    void ComputeRootStatusOfReducibles(const vector_of& theBank);
    void ComputeDominanceStatus();
    void ComputeRefinementStatus_Quick();
    void ComputeEquiBrachialIndices();

    void BuildProperties();
    const properties& GetProperties(const dim_vec* i) const;

    void Write(std::ostream& cout) const;
    void Read(std::istream& cin);
};



#endif