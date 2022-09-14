#include <vector>
#include <set>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <map>
#include <iostream>

#include "Partition.h"

using namespace std;

int_partition_layer::ptr_pair int_partition_layer::BuildPair(int_partition_layer::const_iterator i, int_partition_layer::const_iterator j) const {
    if(i <= j)
        return make_pair(i,j);
    else
        return make_pair(j,i);
}
/*
void int_partition_layer::BuildPairBounds() {
    for(const_iterator i = begin(); i != end(); ++i)
    {
        thePairProperties[make_pair(i,i)].theLowestCommonCoarsenings.push_back(i);
        thePairProperties[make_pair(i,i)].theHighestCommonRefinements.push_back(i);
    }
    for(const_iterator i = begin(); i != end(); ++i)
    {
        for(const_iterator j = begin(); j != end(); ++j)
        {
            if(i == j) continue;
            if(thePairProperties.find(make_pair(i,j)) != thePairProperties.end())
                continue;
            build_bound_naively(i,j);
            continue;
            int_partition intersection;
            set_intersection(i->begin(),i->end(),j->begin(),j->end(),inserter(intersection,intersection.begin()));
            if(intersection.empty())
                build_bound_naively(i,j);
            //else
                //build_bound_recursively(i,j,intersection);
            // 3 + 2 + 2 /\ 4 + 1 + 1 + 1
        }
    }
}*/
/*void int_partition_layer::build_bound_recursively(int_partition_layer::const_iterator i, int_partition_layer::const_iterator j, const int_partition& intersection)
{
    int_partition diff_i, diff_j;
    set_difference(i->begin(),i->end(),intersection.begin(),intersection.end(),inserter(diff_i,diff_i.begin()));
    set_difference(j->begin(),j->end(),intersection.begin(),intersection.end(),inserter(diff_j,diff_j.begin()));

    nat m = accumulate(intersection.begin(),intersection.end(),0);
    cout << N - m << endl;
    intersection.Print(0);
    diff_i.Print(0);
    diff_j.Print(0);
    const_iterator ii = find(theBank->at(N - m).begin(),theBank->at(N - m).end(),diff_i);
    const_iterator jj = find(theBank->at(N - m).begin(),theBank->at(N - m).end(),diff_j);

    thePairProperties[BuildPair(i,j)].theLowestCommonCoarsenings = find_union_partition(theBank->at(N - m).GetLowestCommonCoarsenings(ii,jj), intersection, m);
    thePairProperties[BuildPair(i,j)].theHighestCommonRefinements = find_union_partition(theBank->at(N - m).GetHighestCommonRefinements(ii,jj), intersection, m);
}

int_partition_layer::const_iterator
int_partition_layer::find_union_partition(int_partition_layer::const_iterator i, const int_partition& intersection, nat m) const {
    if(i == theBank->at(N - m).end())
    {
        cout << "Eeek" << endl;
        return end();
    }
    int_partition temp;
    set_union(i->begin(), i->end(),intersection.begin(),intersection.end(),inserter(temp,temp.begin()));
    return find(begin(),end(),temp);
}

void int_partition_layer::build_bound_naively(const_iterator i1, const_iterator i2)
{
    pair_properties& pprops = thePairProperties[make_pair(i1,i2)];

    set<const_iterator> collection1, collection2, collection_intersection;
    collection1.insert(i1);
    collection2.insert(i2);
    set_intersection(collection1.begin(),collection1.end(),collection2.begin(),collection2.end(),inserter(collection_intersection,collection_intersection.begin()));

    auto collectRefinements = [this](set<const_iterator>* out, const_iterator i) {
        const properties&  prop = this->GetProperties(i);
        out->insert(prop.theCoveringRefinements.begin(),prop.theCoveringRefinements.end());
        return out;
    };

    while(collection_intersection.empty())
    {
        set<const_iterator> tmp1, tmp2;
        accumulate(collection1.begin(), collection1.end(), &tmp1, collectRefinements);
        collection1.insert(tmp1.begin(),tmp1.end());

        accumulate(collection2.begin(), collection2.end(), &tmp2, collectRefinements);
        collection2.insert(tmp2.begin(),tmp2.end());
        
        collection_intersection.clear();
        set_intersection(collection1.begin(),collection1.end(),collection2.begin(),collection2.end(),inserter(collection_intersection,collection_intersection.begin()));
    }
    vector<nat> sizes;
    transform(collection_intersection.begin(), collection_intersection.end(), inserter(sizes,sizes.begin()), [](const_iterator i){ return i->size(); });
    if(!equal(sizes.begin() + 1,sizes.end(),sizes.begin()))
    {
        cout << "ERROR!!" << endl;
    }
    pprops.theHighestCommonRefinements.assign(collection_intersection.begin(),collection_intersection.end());




    collection1.clear();
    collection2.clear();

    collection1.insert(i1);
    collection2.insert(i2);

    collection_intersection.clear();
    set_intersection(collection1.begin(),collection1.end(),collection2.begin(),collection2.end(),inserter(collection_intersection,collection_intersection.begin()));

    auto collectCoarsenings = [this](set<const_iterator>* out, const_iterator i) {
        const properties&  prop = this->GetProperties(i);
        out->insert(prop.theCoveringCoarsenings.begin(),prop.theCoveringCoarsenings.end());
        return out;
    };

    while(collection_intersection.empty())
    {
        set<const_iterator> tmp1, tmp2;
        accumulate(collection1.begin(), collection1.end(), &tmp1, collectCoarsenings);
        collection1.insert(tmp1.begin(),tmp1.end());

        accumulate(collection2.begin(), collection2.end(), &tmp2, collectCoarsenings);
        collection2.insert(tmp2.begin(),tmp2.end());
        
        collection_intersection.clear();
        set_intersection(collection1.begin(),collection1.end(),collection2.begin(),collection2.end(),inserter(collection_intersection,collection_intersection.begin()));
    }
    sizes.clear();
    transform(collection_intersection.begin(), collection_intersection.end(), inserter(sizes,sizes.begin()), [](const_iterator i){ return i->size(); });
    if(!equal(sizes.begin() + 1,sizes.end(),sizes.begin()))
    {
        cout << "ERROR!!" << endl;
    }
    pprops.theLowestCommonCoarsenings.assign(collection_intersection.begin(),collection_intersection.end());
}*/

const vector<int_partition_layer::set_of_ptr>& int_partition_layer::init_collections(vector<set_of_ptr>& collection, const vector_of_ptr& ptrs) const {
    for(ptr_bin::const_iterator i = ptrs.begin(); i != ptrs.end(); ++i)
    {
        collection.push_back(set<const_iterator>());
        collection.back().insert(*i);
    }
    return collection;
}
void int_partition_layer::build_intersection(set_of_ptr& intersection, const vector<set_of_ptr>& collection) const {
    intersection.clear();
    intersection.insert(collection[0].begin(),collection[0].end());
    accumulate(collection.begin() + 1, collection.end(), &intersection, [](set_of_ptr* out, const set_of_ptr& c){
        set_of_ptr tmp;
        set_intersection(c.begin(),c.end(),out->begin(),out->end(),inserter(tmp,tmp.begin()));
        out->clear();
        out->insert(tmp.begin(), tmp.end());
        return out;
    });
}

void int_partition_layer::test_sizes_are_equal(const set_of_ptr& intersection) const
{
    vector<nat> sizes;
    transform(intersection.begin(), intersection.end(), inserter(sizes,sizes.begin()), [](const_iterator i){ return i->size(); });
    if(!equal(sizes.begin() + 1,sizes.end(),sizes.begin()))
    {
        cout << "ERROR!!" << endl;
    }
}

void int_partition_layer::BuildHighestCommonRefinements(vector_of_ptr& out, const vector_of_ptr& ptrs) const
{
    vector<set_of_ptr> collection;
    set_of_ptr intersection;

    build_intersection(intersection, init_collections(collection, ptrs));

    auto collectRefinements = [this](set_of_ptr* out, const_iterator i) {
        const properties&  prop = this->GetProperties(i);
        out->insert(prop.theCoveringRefinements.begin(),prop.theCoveringRefinements.end());
        return out;
    };

    while(intersection.empty())
    {
        for_each(collection.begin(),collection.end(), [collectRefinements](set_of_ptr& c){
            set_of_ptr tmp;
            accumulate(c.begin(), c.end(), &tmp, collectRefinements);
            c.insert(tmp.begin(),tmp.end());
        });

        intersection.clear();
        build_intersection(intersection, collection);
    }
    test_sizes_are_equal(intersection);
    out.assign(intersection.begin(),intersection.end());
}
void int_partition_layer::BuildLowestCommonCoarsenings(vector_of_ptr& out, const vector_of_ptr& ptrs) const
{
    vector<set_of_ptr> collection;
    set_of_ptr intersection;

    build_intersection(intersection, init_collections(collection, ptrs));

    auto collectCoarsenings = [this](set_of_ptr* out, const_iterator i) {
        const properties&  prop = this->GetProperties(i);
        out->insert(prop.theCoveringCoarsenings.begin(),prop.theCoveringCoarsenings.end());
        return out;
    };

    while(intersection.empty())
    {
        for_each(collection.begin(),collection.end(), [collectCoarsenings](set_of_ptr& c){
            set_of_ptr tmp;
            accumulate(c.begin(), c.end(), &tmp, collectCoarsenings);
            c.insert(tmp.begin(),tmp.end());
        });

        intersection.clear();
        build_intersection(intersection, collection);
    }
    test_sizes_are_equal(intersection);
    out.assign(intersection.begin(),intersection.end());
}
/*
int_partition_layer::const_iterator int_partition_layer::collect_naively(const_iterator i1, const_iterator i2, function<collect_function> collect) const {
    set<const_iterator> refs_i1, refs_i2, refs_intersection;
    refs_i1.insert(i1);
    refs_i2.insert(i2);
    set_intersection(refs_i1.begin(),refs_i1.end(),refs_i2.begin(),refs_i2.end(),inserter(refs_intersection,refs_intersection.begin()));
    while(refs_intersection.empty())
    {
        set<const_iterator> tmp1, tmp2;
        accumulate(refs_i1.begin(), refs_i1.end(), &tmp1, collect);
        refs_i1.insert(tmp1.begin(),tmp1.end());

        accumulate(refs_i2.begin(), refs_i2.end(), &tmp2, collect);
        refs_i2.insert(tmp2.begin(),tmp2.end());
        
        refs_intersection.clear();
        set_intersection(refs_i1.begin(),refs_i1.end(),refs_i2.begin(),refs_i2.end(),inserter(refs_intersection,refs_intersection.begin()));
    }
    if(refs_intersection.size() != 1)
    {
        cout << "error" << endl;
        i1->Print(0);
        i2->Print(0);
        for(auto i = refs_intersection.begin(); i != refs_intersection.end(); ++i)
            (*i)->Print(4);
    }
    return *refs_intersection.begin();
}
*/