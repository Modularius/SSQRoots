#include "Partition.h"
#include "DimVec.h"
#include "DimVecSpace.h"

#include <fstream>
#include <assert.h>

using namespace std;


dim_vec_space::dim_vec_space(const int_partition_layer& partnLayer, nat k) : 
    //std::hash<dim_vec_key>(thePartnLayer),
    thePartnLayer(partnLayer), K(k) {}

const int_partition_layer&
    dim_vec_space::GetPartitionLayer() const { return thePartnLayer; }
nat dim_vec_space::GetN() const { return thePartnLayer; }
nat dim_vec_space::GetK() const { return K; }

void dim_vec_space::BuildAll() {
    reserve(pow(thePartnLayer.size(),K));
    dim_vec_key temp_key(GetK());
    temp_key.MakeInitial(thePartnLayer);
    /*
    //temp_key.MakeFinal(thePartnLayer);

    iterator hint = emplace(temp_key, dim_vec(thePartnLayer)).first;
    while(temp_key.lexico_next(thePartnLayer))
        hint = emplace_hint(hint, temp_key, dim_vec(thePartnLayer));
    
    for(iterator i = begin(); i != end(); ++i)
        i->second.InitArms(i->first);
    */

    //this->operator[](temp_key.GetIndex(thePartnLayer)) = dim_vec(thePartnLayer,temp_key);
    push_back(dim_vec(thePartnLayer,temp_key));
    while(temp_key.lexico_next(thePartnLayer))
        push_back(dim_vec(thePartnLayer,temp_key));
    
    remap();
    
}
void dim_vec_space::remap()
{
    index.clear();
    for(const_iterator i = begin(); i != end(); ++i)
        index[i->GetArms()] = i - begin();
}
void dim_vec_space::BuildEqualArms() {
    dim_vec_key temp_key(GetK());
    for(int_partition_layer::const_iterator i = thePartnLayer.begin(); i != thePartnLayer.end(); ++i)
    {
        fill(temp_key.begin(),temp_key.end(),i);
        push_back(dim_vec(thePartnLayer,temp_key));
    }
}
void dim_vec_space::Build(dim_vec_pred pred) {
    /*dim_vec_key temp_key(GetK());
    temp_key.MakeInitial(thePartnLayer);
    //temp_key.MakeFinal(thePartnLayer);

    insert_if(pred,temp_key);
    while(temp_key.lexico_next(thePartnLayer))
        insert_if(pred,temp_key);
    
    for(iterator i = begin(); i != end(); ++i)
        i->second.InitArms(i->first);*/
}


dim_vec& dim_vec_space::operator()(const dim_vec_key& arms) {
    return at(arms.GetIndex(GetPartitionLayer()));
    //return at(index.at(arms));
}
const dim_vec& dim_vec_space::operator()(const dim_vec_key& arms) const { 
    return at(arms.GetIndex(GetPartitionLayer()));
    //return at(index.at(arms));
 }

// Private   //
void dim_vec_space::insert_if(dim_vec_pred pred, const dim_vec_key& temp_key) {
    /*dim_vec temp(thePartnLayer);
    
    if(pred(temp))
        emplace(temp_key, temp);*/
}




// Public   //
void dim_vec_space::Write(ostream& cout) const
{
    nat numDimVecs = size();
    cout.write((char*)&numDimVecs, sizeof(nat));
    for(dim_vec::vector_of::const_iterator i = begin(); i != end(); ++i)
    {
        i->GetArms().Write(cout, thePartnLayer);
        i->Write(cout);
    }
}

void dim_vec_space::Read(istream& cin)
{
    clear();

    nat numDimVecs;
    cin.read((char*)&numDimVecs, sizeof(nat));
    for(nat i = 0; i < numDimVecs; ++i)
    {
        dim_vec_key temp_key(K);
        temp_key.Read(cin, thePartnLayer, K);
        push_back(dim_vec(thePartnLayer,temp_key));
        back().Read(cin);
    }
    remap();
}

