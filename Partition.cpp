#include <vector>
#include <set>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <map>
#include <iostream>

#include "Partition.h"

using namespace std;

int_partition::int_partition() : theLayer(NULL) {}
int_partition::int_partition(const int_partition::vector_of& layer) : theLayer(&layer) {}

bool int_partition::lexico_next()
{
    nat num_units = count(1);

    //  First case: there are no non-unit parts
    if(num_units == size())
        return false;

    erase(1);
    nat smallest_nonunit = *begin();
    erase(begin());

    //(num_units + smallest_nonunit) = Q*(smallest_nonunit - 1) + R
    nat Q = (num_units + smallest_nonunit)/(smallest_nonunit - 1);
    nat R = (num_units + smallest_nonunit) % (smallest_nonunit - 1);
    for(nat q = 0; q < Q; ++q)
        insert(smallest_nonunit - 1);
    if(R != 0)
        insert(R);
    return true;
}

//  Is this a covering refinement of mu?
void int_partition::BuildListOfCoveringCoarsenings(int_partition::set_of& out) const
{
    for(const_iterator i1 = begin(); i1 != end(); ++i1)
    {
        for(const_iterator i2 = next(i1,1); i2 != end(); ++i2)
        {
            int_partition temp(*this);
            temp.erase(temp.find(*i1));
            temp.erase(temp.find(*i2));
            temp.insert(*i1 + *i2);
            out.insert(temp);
        }
    }
}

void int_partition::BuildListOfCoveringDominators(int_partition::set_of& out) const
{
    for(const_reverse_iterator i1 = rbegin(); i1 != rend(); ++i1)
    {
        for(const_reverse_iterator i2 = next(i1,1); i2 != rend(); ++i2)
        {
            int_partition temp(*this);
            temp.erase(temp.find(*i1));
            temp.erase(temp.find(*i2));
            temp.insert(*i1 + 1);
            temp.insert(*i2 - 1);
            temp.erase(0);
            out.insert(temp);
        }
    }
}

int int_partition::GetQuadraticSum() const {
    return inner_product(begin(),end(),begin(),0);
}
nat int_partition::GetWeight() const {
    return accumulate(begin(),end(),0);
}
nat int_partition::GetFirstPart() const { return empty() ? 0:*rbegin(); }
int_partition::const_iterator int_partition::GetFirstPartIterator() const { return empty() ? end():--end(); }

bool int_partition::IsContainedWithin(const int_partition& mu) const {
    for(const_iterator i = begin(); i != end(); ++i)
        if(mu.count(*i) < count(*i))
            return false;
    return true;
}


void int_partition::PrintInline(ostream& cout) const {
    for(auto i = rbegin(); i != rend(); ++i)
        cout << *i << " ";
}

void int_partition::Print(nat spaces) const
{
    for(nat s = 0; s < spaces;++s) cout << " ";
    
    cout << "( ";
    PrintInline(cout);
    cout << ")" << endl;
}








int_partition_layer::int_partition_layer() : theBank(NULL) {}
int_partition_layer::int_partition_layer(const int_partition_layer::vector_of& bank, nat n) : theBank(&bank), N(n) {}

void int_partition_layer::Build()
{
    int_partition temp(*this);
    temp.insert(N);
    push_back(temp);
    while(temp.lexico_next())
        push_back(temp);

    //sort(begin(), end(), [](const int_partition& a, const int_partition& b){ if(a.size() < b.size())return true; if(b.size() < a.size())return false; return a<b; } );
    BuildBins();
}
void int_partition_layer::BuildBins()
{
    ptrByLength.resize(N + 1);
    ptrByMaxPart.resize(N + 1);
    BeginByFirstPart.resize(N + 1,end());
    for(const_iterator i = begin(); i != end(); ++i)
    {
        if(BeginByFirstPart[i->GetFirstPart()] == end())
            BeginByFirstPart[i->GetFirstPart()] = i;

        ptrByLength[i->size()].push_back(i);
        ptrByMaxPart[i->GetFirstPart()].push_back(i);
    }
}
void int_partition_layer::BuildProperties()
{
    theProperties.clear();
    for(const_iterator i = begin(); i != end(); ++i)
        theProperties.push_back(properties());

    for(const_iterator i = begin(); i != end(); ++i)
    {
        int_partition::set_of temp;

        theProperties[i - begin()].QuadraticSum = i->GetQuadraticSum();
        /*i->BuildListOfCoveringCoarsenings(temp);
        for(const int_partition& p : temp)
        {
            const_iterator itr = std::find(begin(),end(),p);
            if(itr == end())
                cout << "Error" << std::endl;
            theProperties[i].theCoveringCoarsenings.push_back(itr);
            theProperties[itr].theCoveringRefinements.push_back(i);
        }
        temp.clear();*/
        i->BuildListOfCoveringDominators(temp);
        for(const int_partition& p : temp)
        {
            const_iterator itr = std::find(begin(),end(),p);
            if(itr == end())
                cout << "Error" << std::endl;
            theProperties[i - begin()].theCoveringDominators.push_back(itr);
            theProperties[itr - begin()].theCoveringDominees.push_back(i);
        }
    }
}
const int_partition_layer::properties&
    int_partition_layer::GetProperties(int_partition_layer::const_iterator i) const { return theProperties.at(i - begin()); }



int_partition_layer::const_iterator int_partition_layer::FindFromRaw(const int_partition& raw) const
{
    return find(BeginByFirstPart[raw.GetFirstPart()], end(), raw);
}



const vector<int_partition_layer::const_iterator>& int_partition_layer::GetHighestCommonRefinements(int_partition_layer::const_iterator i, int_partition_layer::const_iterator j) const {
    return thePairProperties.at(BuildPair(i,j)).theHighestCommonRefinements;
}
const vector<int_partition_layer::const_iterator>& int_partition_layer::GetLowestCommonCoarsenings(int_partition_layer::const_iterator i, int_partition_layer::const_iterator j) const {
    return thePairProperties.at(BuildPair(i,j)).theLowestCommonCoarsenings;
}

nat int_partition_layer::GetWeight() const { return N; }
int_partition_layer::operator nat() const { return N; }
const int_partition_layer& int_partition_layer::operator()(nat n) const { return theBank->at(n); }





int_partition_bank::int_partition_bank(nat n) : max_n(n) {
    for(nat n = 0; n <= max_n; ++n)
        push_back(int_partition_layer(*this, n));
}
void int_partition_bank::Build()
{
    at(0).push_back(int_partition(at(0)));
    for(nat n = 1; n <= max_n; ++n)
        at(n).Build();
}
void int_partition_bank::BuildProperties()
{
    for(iterator i = begin(); i != end(); ++i)
        i->BuildProperties();
}
void int_partition_bank::BuildPairBounds()
{
//    for(nat n = 0; n <= max_n; ++n)
//        at(n).BuildPairBounds();
}
nat int_partition_bank::GetMaxWeight() const { return max_n; }

void int_partition_bank::Print() const {
    for(const_iterator i = begin(); i != end(); ++i)
    {
        cout << (nat)*i << endl;
        for(auto j = i->begin(); j != i->end(); ++j)
        {
            j->Print(0);
        }
    }
}