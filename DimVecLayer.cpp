#include "Partition.h"
#include "DimVec.h"
#include "DimVecLayer.h"

#include <fstream>

using namespace std;


dim_vec_layer::dim_vec_layer(const int_partition_layer& partnLayer, nat k)
 : dim_vec_space(partnLayer,k) {}

// Public   //
void dim_vec_layer::ComputeRootStatus() {
    for(iterator i = begin(); i != end(); ++i) {
        i->CalcQuadraticForm(thePartnLayer);
        i->CalcRootStatus(thePartnLayer);
        //alpha.CalcEquiBrachialIndex(thePartnLayer);
    }
}

void dim_vec_layer::ComputeRootStatusOfReducibles(const vector_of& theBank) {
    for(iterator i = begin(); i != end(); ++i) {
        dim_vec& alpha = *i;

        if(!alpha.IsReducible())
            continue;

        //  Aliases
        const dim_vec_layer& newDimVecLayer = theBank[thePartnLayer + alpha.GetChi()];

        //  We centrally reflect to get new arms
        dim_vec_key refl_arms(K);
        alpha.BuildCentralReflection(newDimVecLayer.GetPartitionLayer(), refl_arms);

        //  We try to find this new dim vec in the bank
        //refl_arms.Print(0,cout);
        //cout << this->operator()(refl_arms) << endl;

        alpha.SetReflection(&newDimVecLayer(refl_arms));
        
        /* iRefl = find_if(newDimVecLayer.begin(),newDimVecLayer.end(),[refl_arms](const dim_vec& a){ return refl_arms==a.GetArms(); });
        //  If we find this dimension vector in the bank we set it to be the reflection
        if(iRefl != newDimVecLayer.end())
            alpha.SetReflection(&*iRefl);
        else {  //  Otherwise we try to determine the root status from it, then discard it
            dim_vec refl(newDimVecLayer.GetPartitionLayer(),refl_arms);
            //refl.InitArms(refl_arms);
            refl.CalcRootStatus(newDimVecLayer.GetPartitionLayer());
            alpha.SetRootStatusFrom(&refl);
        }*/
    }
}

void dim_vec_layer::ComputeRefinementStatus_Quick() {
    /*dim_vec_key temp_key(GetK() - 1);
    temp_key.MakeInitial(thePartnLayer);


    for(int_partition_layer::const_iterator i = temp_key.back(); i != thePartnLayer.end(); ++i)
    {
        dim_vec_key temp_key2(temp_key);
        temp_key2.push_back(i);
        dim_vec dv(GetN());
        dv.InitArms(temp_key2);
        dv.CalcMaximallyCoarse(thePartnLayer,*this);
        if(dv.IsMaximallyCoarse())
        {

        }
    }
    while(temp_key.lexico_next(thePartnLayer))
    
    for(iterator i = begin(); i != end(); ++i)
        i->second.InitArms(i->first);*/
}

void dim_vec_layer::ComputeDominanceStatus() {
    //for(iterator i = begin(); i != end(); ++i)
    //    i->CalcMaximallyCoarse(thePartnLayer,*this, index);
    for(iterator i = begin(); i != end(); ++i)
        i->CalcMaximallyDominant(thePartnLayer,*this, index);
}

void dim_vec_layer::ComputeEquiBrachialIndices() {
    for(iterator i = begin(); i != end(); ++i)
        i->CalcEquiBrachialIndex(thePartnLayer);
}


void dim_vec_layer::BuildProperties()
{
    for(const_iterator i = begin(); i != end(); ++i)
    {
        const dim_vec& alpha = *i;
        dim_vec_key::set_of coarsenings;
        alpha.BuildCoveringCoarseningList(coarsenings, thePartnLayer);
        for(auto j = coarsenings.begin(); j != coarsenings.end(); ++j)
        {
            /*const dim_vec& itr = at(this->operator()(*j));
            //if(itr == end())
            //    continue;

            theProperties[&alpha].theCoveringCoarsenings.push_back(&itr);
            theProperties[&itr].theCoveringRefinements.push_back(&alpha);*/
        }
        alpha.BuildDescentSequence(theProperties[&alpha].theDescentSequence);
        alpha.BuildDescentSequenceWeights(theProperties[&alpha].theDescentSequence,theProperties[&alpha].theDescentSequenceWeights);

        for(auto j = theProperties[&alpha].theDescentSequence.begin(); j != theProperties[&alpha].theDescentSequence.end(); ++j)
        {
            const dim_vec_key& arms = (*j)->GetArms();
            vector<vector<nat>>& tops = theProperties[&alpha].theDescentSequenceTops;
            tops.push_back(vector<nat>(K,0));
            for(nat i = 0; i < K; ++i)
            {
                nat arm_p = arms[i]->GetFirstPart() + (*j)->GetChi();
                tops.back()[i] = count_if(arms[i]->begin(),arms[i]->end(), [arm_p](nat p) { return p > arm_p; } ) - 1;
            }
        }
    }
}

const dim_vec_layer::properties&
    dim_vec_layer::GetProperties(const dim_vec* i) const { return theProperties.at(i); }





void dim_vec_layer::Write(ostream& cout) const
{
    dim_vec_space::Write(cout);
}

void dim_vec_layer::Read(istream& cin)
{
    dim_vec_space::Read(cin);
}
