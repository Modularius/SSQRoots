#include "Partition.h"
#include "DimVec.h"
#include "DimVecBank.h"

#include <fstream>

using namespace std;


dim_vec_bank::dim_vec_bank(const int_partition_bank& partnBank, nat k) : thePartnBank(partnBank), K(k) {
    for(nat n = 0; n <= thePartnBank.GetMaxWeight(); ++n)
        push_back(dim_vec_layer(thePartnBank[n], K));
}

void dim_vec_bank::Build(dim_vec_pred pred) {
    for(nat n = 1; n <= thePartnBank.GetMaxWeight(); ++n)
        at(n).Build(pred);
}

void dim_vec_bank::BuildAll() {
    for(nat n = 1; n <= thePartnBank.GetMaxWeight(); ++n)
        at(n).BuildAll();
}

void dim_vec_bank::ComputeRootStatus() {
    for(nat n = 1; n <= thePartnBank.GetMaxWeight(); ++n)
        at(n).ComputeRootStatus();
}

void dim_vec_bank::ComputeRootStatusOfReducibles() {
    for(nat n = 1; n <= thePartnBank.GetMaxWeight(); ++n)
        at(n).ComputeRootStatusOfReducibles(*this);
}

void dim_vec_bank::ComputeEquiBrachialIndices() {
    for(iterator i = begin(); i != end(); ++i)
        i->ComputeEquiBrachialIndices();
}

void dim_vec_bank::ComputeDominanceStatus() {
    for(iterator i = begin(); i != end(); ++i)
        i->ComputeDominanceStatus();
}
void dim_vec_bank::BuildProperties() {
    for(iterator i = begin(); i != end(); ++i)
        i->BuildProperties();
}

void dim_vec_bank::CountReflectees() {
    for(const_iterator i = begin(); i != end(); ++i)
        for(dim_vec_layer::const_iterator j = i->begin(); j != i->end(); ++j)
            if(j->GetReflection())
            {
                dim_vec_layer& layer = at(i->GetN() + j->GetChi());
                layer(j->GetReflection()->GetArms()).RegisterReflectee();
            }
}

nat dim_vec_bank::GetK() const { return K; }

void dim_vec_bank::Read()
{
    ifstream myfile;
    char filename[20] = "data/00-dimvecs.dat";
    for(nat n = 0; n < size(); ++n)
    {
        filename[5] = (char)(n/10 + 48);
        filename[6] = (char)(n%10 + 48);
        myfile.open(filename, ios::in | ios::binary);
        if(myfile.is_open())
        {
            this->at(n).Read(myfile);
            myfile.close();
        }
        else {
            cout << "Failed to open file " << filename << endl;
        }
    }
}


void dim_vec_bank::Write() const
{
    ofstream myfile;
    char filename[20] = "data/00-dimvecs.dat";
    for(nat n = 0; n < size(); ++n)
    {
        filename[5] = (char)(n/10 + 48);
        filename[6] = (char)(n%10 + 48);
        myfile.open(filename, ios::out | ios::binary | ios::trunc);
        if(myfile.is_open())
        {
            this->at(n).Write(myfile);
            myfile.close();
        }
        else {
            cout << "Failed to open file " << filename << endl;
        }
    }
}