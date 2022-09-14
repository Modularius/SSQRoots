#include <vector>
#include <set>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <map>
#include <iostream>
#include <functional>

#include "Partition.h"
#include "DimVec.h"

using namespace std;

dim_vec_key::dim_vec_key(nat K) : vector<itr_partn>(K) {}

/*
    Suppse P = {0, 1, 2} and K = 3
    For a given input I, this produces output O
    [2,2,2] -> [1,2,2]
    [1,2,2] -> [0,2,2]
    [0,2,2] -> [1,1,2]
    [1,1,2] -> [0,1,2]
    [0,1,2] -> [0,0,2]
    [0,0,2] -> [1,1,1]
    [1,1,1] -> [0,1,1]
    [0,1,1] -> [0,0,1]
    [0,0,1] -> [0,0,0]
*/
void dim_vec_key::MakeInitial(const int_partition_layer& layer) {
    fill(begin(),end(), layer.begin());
}
void dim_vec_key::MakeFinal(const int_partition_layer& layer) {
    fill(begin(),end(), --layer.end());
}
bool dim_vec_key::lexico_prev(const int_partition_layer& P)
{
    if(empty())
        return false;

    if(P.empty())
        return true;

    // This finds the position of the first element not equal to P.begin()
    iterator iFirstNonInitial = upper_bound(begin(), end(), P.begin());
    
    //  If no element is not equal to P.begin() then we have reached the end
    if(iFirstNonInitial == end())
        return false;

    // Decrements the first element not equal to P.begin() by 1
    *iFirstNonInitial = *iFirstNonInitial - 1;
    //  Set all values before this (which are equal to P.begin()) to the same value
    fill(begin(),iFirstNonInitial,*iFirstNonInitial);
    return true;
}
bool dim_vec_key::lexico_next(const int_partition_layer& P)
{
    if(empty())
        return false;

    if(P.empty())
        return true;

    // This finds the position of the last element not equal to P.rend()
    iterator iFirstFinal = lower_bound(begin(), end(), P.end() - 1);
    
    //  If all elements are equal to (P.end() - 1) then we have reached the end
    if(iFirstFinal == begin())
        return false;

    iterator iLastNonFinal = iFirstFinal - 1;

    // Increments the previous element by 1
    *iLastNonFinal = *iLastNonFinal + 1;
    //  Set all values from iFirstFinal to the end (which are equal to P.end() - 1) to the new value of *iLastNonFinal
    fill(iFirstFinal,end(),*iLastNonFinal);
    return true;
}
void dim_vec_key::CopyToRaw(raw_dim_vec& out) const {
    transform(begin(), end(), out.begin(), [](const itr_partn& i){ return *i; });
}
void dim_vec_key::CopyFromRaw(const int_partition_layer& layer, const raw_dim_vec& in) {
    for(nat i = 0; i < in.size(); ++i)
        at(i) = layer.FindFromRaw(in[i]);
}


nat nCk(nat n, nat k)
{
    if(k > n)
        return 0;

    nat f = 1;
    for(nat i = 0; i < k; ++i)
        f *= n - i;
    for(nat i = 2; i <= k; ++i)
        f /= i;
    return f;
}
 
size_t dim_vec_key::GetIndex(const int_partition_layer& partnLayer) const {
    int_partition_layer::const_iterator begin = partnLayer.begin();
    int_partition_layer::const_iterator end = partnLayer.end();

    nat idx = 0;
    for(nat a = 0; a < size(); ++ a)
    {
        for(nat i = 0; i < at(a) - begin; ++i)
            idx += nCk((end - begin) - i + size() - a - 2, size() - a - 1);
        begin = at(a);
    }
    return idx;

}


void dim_vec_key::GetIndices (const int_partition_layer& partnLayer, vector<nat>& out) const {
    out.clear();
    for(const_iterator i = begin(); i != end(); ++i)
        out.push_back(*i - partnLayer.begin());
}


//  Printing
void dim_vec_key::Print(nat spaces, ostream& cout) const
{
    for(nat s = 0; s < spaces;++s) cout << " ";

    cout << "[DIMVEC]\n";
    for(const_iterator i = begin(); i != end(); ++i)
        (*i)->Print(spaces);
}
void dim_vec_key::PrintIndex (nat spaces, const int_partition_layer& partnLayer, ostream& cout) const {
    for(nat s = 0; s < spaces;++s) cout << " ";

    cout << "[" << partnLayer << "]" << "{ ";
    for(const_iterator i = begin(); i != end(); ++i)
        cout << *i - partnLayer.begin() << " ";
    cout << "}" << endl;
}




void dim_vec_key::Write(ostream& cout, const int_partition_layer& partnLayer) const
{
    nat idx;
    for(const_iterator i = begin(); i != end(); ++i) {
        idx = distance(partnLayer.begin(), *i);
        cout.write((char*)&idx,sizeof(nat));
    }
}

void dim_vec_key::Read(istream& cin, const int_partition_layer& partnLayer, nat k)
{
    clear();
    nat idx;
    for(nat i = 0; i < k; ++i) {
        cin.read((char*)&idx,sizeof(nat));
        push_back(next(partnLayer.begin(),idx));
    }
}

