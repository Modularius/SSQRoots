#ifndef PARTITION_H
#define PARTITION_H

#include <vector>
#include <set>
#include <algorithm>
#include <numeric>
#include <functional>
#include <iostream>
#include <map>

typedef unsigned int nat;

class int_partition : public std::multiset<nat> {
public:
    typedef std::vector<int_partition> vector_of;
    typedef std::set<int_partition> set_of;
private:
    const vector_of* theLayer;
public:
    int_partition();
    int_partition(const vector_of& layer);

    bool lexico_next();

    //  Is this a covering refinement of mu?
    void BuildListOfCoveringCoarsenings(set_of& out) const;
    void BuildListOfCoveringDominators(set_of& out) const;

    int GetQuadraticSum() const;
    nat GetWeight() const;


    void Print(nat spaces) const;
    void PrintInline(std::ostream& cout) const;
    nat GetFirstPart() const;
    const_iterator GetFirstPartIterator() const;
    bool IsContainedWithin(const int_partition& mu) const;
};





template<class C_ITR>
struct int_partition_properties {
    typedef std::vector<C_ITR> vector_of;
    vector_of theCoveringCoarsenings;
    vector_of theCoveringRefinements;
    vector_of theCoveringDominators;
    vector_of theCoveringDominees;
    int QuadraticSum;
};

template<class C_ITR>
struct int_partition_pair_properties {
    std::vector<C_ITR> theLowestCommonCoarsenings, theHighestCommonRefinements;
};


class int_partition_layer : public int_partition::vector_of
{
public:
    typedef std::vector<int_partition_layer> vector_of;
    typedef int_partition_properties<const_iterator> properties;
    typedef int_partition_pair_properties<const_iterator> pair_properties;
    typedef std::vector<const_iterator> vector_of_ptr;
    typedef std::set<const_iterator> set_of_ptr;
    typedef vector_of_ptr ptr_bin;
    typedef std::vector<ptr_bin> vector_of_ptr_bin;
    typedef std::pair<const_iterator,const_iterator> ptr_pair;
    typedef std::set<const_iterator>*(*collect_function)(std::set<const_iterator>*, const_iterator);
private:
    const vector_of* theBank;
    nat N;

    std::vector<properties> theProperties;
    std::map<ptr_pair, pair_properties> thePairProperties;

    vector_of_ptr_bin ptrByLength;
    vector_of_ptr_bin ptrByMaxPart;

    vector_of_ptr BeginByFirstPart;

    //void build_bound_recursively(const_iterator i, const_iterator j, const int_partition& intersection);
    //void build_bound_naively(const_iterator i1, const_iterator i2);

    const std::vector<set_of_ptr>& init_collections(std::vector<set_of_ptr>& collection, const vector_of_ptr& ptrs) const;
    void build_intersection(set_of_ptr& intersection, const std::vector<set_of_ptr>& collection) const;
    void test_sizes_are_equal(const set_of_ptr& intersection) const;
    //const_iterator collect_naively(const_iterator i1, const_iterator i2, std::function<collect_function> collect) const;
    //const_iterator find_union_partition(const_iterator i, const int_partition& intersection, nat m) const;
public:
    int_partition_layer();
    int_partition_layer(const vector_of& bank, nat n);

    void Build();
    void BuildBins();
    void BuildProperties();
    
    //  Algorithms
    ptr_pair BuildPair(const_iterator i, const_iterator j) const;
    //void BuildPairBounds();
    void BuildHighestCommonRefinements(vector_of_ptr& out, const vector_of_ptr& ptrs) const;
    void BuildLowestCommonCoarsenings(vector_of_ptr& out, const vector_of_ptr& ptrs) const;

    //  Property Getters
    const properties& GetProperties(const_iterator i) const;
    const std::vector<const_iterator>& GetHighestCommonRefinements(const_iterator i, const_iterator j) const;
    const std::vector<const_iterator>& GetLowestCommonCoarsenings(const_iterator i, const_iterator j) const;

    //  Lattice Getters
    const_iterator GetMax() const { return begin(); }
    const_iterator GetMin() const { return empty()?end():end() - 1; }

    const_iterator FindFromRaw(const int_partition& raw) const;

    //  Weight Getters
    nat GetWeight() const;
    operator nat() const;

    //  Get Other Layers
    const int_partition_layer& operator()(nat n) const;
};





class int_partition_bank : public int_partition_layer::vector_of
{
    nat max_n;
public:
    int_partition_bank(nat n);
    void Build();
    void BuildProperties();
    void BuildPairBounds();
    nat GetMaxWeight() const;
    void Print() const;
};

#endif