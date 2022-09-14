#ifndef DIMVECBANK_H
#define DIMVECBANK_H

#include <cmath>
#include <iostream>
#include <unordered_map>

#include "Partition.h"

typedef int_partition::vector_of raw_dim_vec;
typedef int_partition_layer::const_iterator itr_partn;

class dim_vec_key : public std::vector<itr_partn>
{
public:
    typedef std::set<dim_vec_key> set_of;

    dim_vec_key(nat K);

    void MakeInitial(const int_partition_layer& layer);
    void MakeFinal(const int_partition_layer& layer);
    bool lexico_prev(const int_partition_layer& P);
    bool lexico_next(const int_partition_layer& P);


    void CopyToRaw(raw_dim_vec& out) const;
    void CopyFromRaw(const int_partition_layer& layer, const raw_dim_vec& in);
    void GetIndices (const int_partition_layer& partnLayer, std::vector<nat>& out) const;

    std::size_t GetIndex(const int_partition_layer& partnLater) const;


    //  Printing
    void Print (nat spaces, std::ostream& cout) const;
    void PrintIndex (nat spaces, const int_partition_layer& partnLayer, std::ostream& cout) const;

    void Write(std::ostream& cout, const int_partition_layer& partnLayer) const;
    void Read(std::istream& cin, const int_partition_layer& partnLayer, nat k);
};


namespace std {
    template<>
    class hash<dim_vec_key>
    {
        const int_partition_layer* thePartnLayer;
    public:
        hash(const int_partition_layer& partnLayer) : thePartnLayer(&partnLayer) {}

        std::size_t operator()(const dim_vec_key& alpha) const {
            return alpha.GetIndex(*thePartnLayer);
        }
    };
}


class dim_vec {
public:
    typedef std::unordered_map<dim_vec_key,dim_vec,std::hash<dim_vec_key>> map_of;
    //typedef std::map<dim_vec_key,dim_vec> map_of;
    typedef std::vector<dim_vec> vector_of;
    typedef std::vector<const dim_vec*> vector_of_ptr;
    typedef std::set<dim_vec> set_of;
    typedef std::pair<dim_vec_key,dim_vec> pair_of;
    typedef std::vector<pair_of> vector_of_pair;
    typedef std::pair<const dim_vec_key*,dim_vec&> ptr_pair_of;
    typedef std::map<const dim_vec_key*,dim_vec&> map_of_ptr;

    // bit 1 = is root?
    // bit 2 = is real? [TODO]
    // bit 3 = is fundamental?
    enum ROOT_STATUS {RS_UNKNOWN=0, RS_NON_ROOT=1, RS_REAL_ROOT=2, RS_SIMPLE_ROOT=3, RS_IMAG_ROOT=4, RS_FUNDAMENTAL_IMAG_ROOT};
private:
    nat theWeight;
    const dim_vec_key theArms;
    const dim_vec* theReflection;
    ROOT_STATUS theRootStatus;
    int chi, q, minEta, maxEta;
    bool MaximallyCoarse;
    bool MaximallyDominant;
    bool MinimallyDominant;
    bool Reducible;
    int RefinementDegree;
    nat NumReflectees;
    int EquiBrachialIndexHCR, EquiBrachialIndexLCC, theDiversity;
    int_partition_layer::const_iterator theHCR, theLCC;

    void build_arm_substitution_list(dim_vec_key::set_of& out, const int_partition_layer::properties::vector_of& subs, nat arm) const;
public:
    dim_vec(nat n, const dim_vec_key& position);
    nat GetWeight() const;

    //  SETUP
    const dim_vec_key& GetArms() const;

    //  ROOT STATUS
    void CalcRootStatus(const int_partition_layer& layer);
    ROOT_STATUS GetRootStatus() const;

    //  QUADRATIC FORM
    void CalcQuadraticForm(const int_partition_layer& layer);
    int GetQuadraticForm() const;

    //  Get Central Excess
    int GetChi() const;
    int GetMaxEta() const;

    bool IsRoot() const;
    bool IsReal() const;
    bool IsImaginary() const;
    bool IsFundamental() const;
    bool IsReducible() const;

    //  CENTRAL REFLECTION
    void BuildCentralReflection(const int_partition_layer& partnLayer, dim_vec_key& out) const;
    void SetReflection(const dim_vec* ref);
    void SetRootStatusFrom(const dim_vec* ref);

    void RegisterReflectee();
    nat GetNumReflectees() const;

    const dim_vec* GetReflection() const;
    const dim_vec* GetFinalReduction() const;
    void BuildDescentSequence(vector_of_ptr& out) const;
    void BuildDescentSequenceWeights(vector_of_ptr& refls, std::vector<nat>& out) const;

    void BuildCoveringCoarseningList(dim_vec_key::set_of& out, const int_partition_layer& partnLayer) const;
    void BuildCoveringRefinementList(dim_vec_key::set_of& out, const int_partition_layer& partnLayer) const;

    void BuildCoveringDominatorsList(dim_vec_key::set_of& out, const int_partition_layer& partnLayer) const;
    void BuildCoveringDomineesList(dim_vec_key::set_of& out, const int_partition_layer& partnLayer) const;

    //  Maximal Coarseness
    void CalcMaximallyCoarse(const int_partition_layer& partnLayer, const vector_of& layer, const std::map<dim_vec_key,nat>& index);
    bool IsMaximallyCoarse() const;

    //  Maximal Dominance
    void CalcMaximallyDominant(const int_partition_layer& partnLayer, const vector_of& layer, const std::map<dim_vec_key,nat>& index);
    bool IsMaximallyDominant() const;

    //  Lattice Computation
    int_partition_layer::const_iterator GetHighestCommonRefinement(const int_partition_layer& partnLayer) const;
    int_partition_layer::const_iterator GetLowestCommonCoarsening(const int_partition_layer& partnLayer) const;

    //  Equibrachial
    bool IsEquibrachial() const;

    //  Assorted
    void SetRefinementDegree(int deg);
    int GetRefinementDegree() const;

    double GetArmLengthToWeightRatios() const;
    void CalcEquiBrachialIndex(const int_partition_layer& partnLayer);
    void BuildEquiBrachialIndexSequence(std::vector<std::pair<nat,nat>>& out) const;
    //  Odd Stuff
    double GetIndexCosineSqr(const int_partition_layer& partnLayer) const;


    //  PRINTING
    void Print(nat spaces, std::ostream& cout) const;
    void PrintArms(nat spaces, std::ostream& cout) const;
    void PrintArmIndices(nat spaces, const int_partition_layer& partnLayer, std::ostream& cout) const;
    void PrintArmLengthToWeightRatios(nat spaces, std::ostream& cout) const;
    void PrintRootStatus(std::ostream& cout) const;
    void Write(std::ostream& cout) const;
    void Read(std::istream& cin);
};

#endif