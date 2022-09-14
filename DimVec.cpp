#include <vector>
#include <set>
#include <algorithm>
#include <numeric>
#include <map>
#include <iostream>
#include <functional>

#include "Partition.h"
#include "DimVec.h"

using namespace std;

dim_vec::dim_vec(nat n, const dim_vec_key& position) : theWeight(n), theArms(position), theReflection(NULL),  theRootStatus(RS_UNKNOWN), RefinementDegree(-1), NumReflectees(0) {}
nat dim_vec::GetWeight() const { return theWeight; }

//  SETUP
const dim_vec_key& dim_vec::GetArms() const { return theArms; }

//  Quadratic Form
void dim_vec::CalcQuadraticForm(const int_partition_layer& layer)
{
    /*q = accumulate(thePosition->begin(), thePosition->end(),
        (2 - (int)thePosition->size())*GetWeight()*GetWeight(),
        [layer](int total, itr_partn p){ return total + layer.GetProperties(p).QuadraticSum; }
    )/2;*/
    q = (2 - (int)theArms.size())*GetWeight()*GetWeight();
    for(dim_vec_key::const_iterator i = theArms.begin(); i != theArms.end(); ++i)
        q += layer.GetProperties(*i).QuadraticSum;
    q /= 2;
}
int dim_vec::GetQuadraticForm() const { return q; }

//  ROOT STATUS
void dim_vec::CalcRootStatus(const int_partition_layer& layer) {
    Reducible = false;

    if(GetWeight() == 1)
    {
        theRootStatus = RS_SIMPLE_ROOT;
        return;
    }

    chi = ((int)theArms.size() - 2)*GetWeight();
    /*accumulate(thePosition->begin(), thePosition->end(),
        ((int)thePosition->size() - 2)*GetWeight(),
        [](int total, itr_partn p){ return total - (int)p->GetFirstPart(); }
    );*/
    for(dim_vec_key::const_iterator i = theArms.begin(); i != theArms.end(); ++i) {
        chi -= (int)(*i)->GetFirstPart();
    }

    if(chi >= 0)
    {
        theRootStatus = RS_FUNDAMENTAL_IMAG_ROOT;
        return;
    }


    minEta = GetWeight();
    maxEta = 0;
    for(dim_vec_key::const_iterator i = theArms.begin(); i != theArms.end(); ++i) {
        nat p1 = (*i)->GetFirstPart();
        minEta = min<int>(minEta, p1);
        maxEta = max<int>(maxEta, p1);
    }

    if(chi < -minEta)
        theRootStatus = RS_NON_ROOT;
    else
        Reducible = true;
}
dim_vec::ROOT_STATUS dim_vec::GetRootStatus() const { return theRootStatus; }

int dim_vec::GetChi() const { return chi; }
int dim_vec::GetMaxEta() const { return maxEta; }

bool dim_vec::IsRoot() const { return theRootStatus != RS_NON_ROOT; }
bool dim_vec::IsReal() const { return theRootStatus == RS_REAL_ROOT || theRootStatus==RS_SIMPLE_ROOT; }
bool dim_vec::IsImaginary() const { return theRootStatus == RS_IMAG_ROOT || theRootStatus==RS_FUNDAMENTAL_IMAG_ROOT; }
bool dim_vec::IsFundamental() const { return theRootStatus == RS_FUNDAMENTAL_IMAG_ROOT; }
bool dim_vec::IsReducible() const { return Reducible; }

//  CENTRAL REFLECTION
void dim_vec::BuildCentralReflection(const int_partition_layer& partnLayer, dim_vec_key& out) const
{
    raw_dim_vec temp(theArms.size());
    theArms.CopyToRaw(temp);

    for(int_partition& i : temp)
    {
        int_partition::const_iterator i1 = i.GetFirstPartIterator();
        i.erase(i1);
        nat newP = *i1 + GetChi();
        if(newP > 0) i.insert(newP);
    }

    out.resize(theArms.size());
    out.CopyFromRaw(partnLayer,temp);
    sort(out.begin(),out.end());
}

void dim_vec::SetReflection(const dim_vec* ref) {
    theReflection = ref;
    SetRootStatusFrom(theReflection);
}
void dim_vec::SetRootStatusFrom(const dim_vec* ref) {
    switch(ref->GetRootStatus()) {
        case RS_FUNDAMENTAL_IMAG_ROOT:
            theRootStatus = RS_IMAG_ROOT;
        break;
        case RS_SIMPLE_ROOT:
            theRootStatus = RS_REAL_ROOT;
        break;
        default:
            theRootStatus = ref->GetRootStatus();
        break;
    }
}



void dim_vec::RegisterReflectee() { ++NumReflectees; }
nat dim_vec::GetNumReflectees() const { return NumReflectees; }


const dim_vec* dim_vec::GetReflection() const { return theReflection; }

const dim_vec* dim_vec::GetFinalReduction() const
{   
    const dim_vec* cur;
    for(cur = this; cur->GetReflection(); cur = cur->GetReflection());
    return cur;
}





void dim_vec::BuildDescentSequence(vector_of_ptr& out) const {
    for(const dim_vec* cur = this; cur != NULL; cur = cur->GetReflection())
        out.push_back(cur);
}
void dim_vec::BuildDescentSequenceWeights(vector_of_ptr& refls, std::vector<nat>& out) const {
    out.resize(refls.size());
    std::transform(refls.begin(),refls.end(), out.begin(), [](const dim_vec* a){ return -a->GetChi(); } );
    out.pop_back();
}




void dim_vec::BuildCoveringCoarseningList(dim_vec_key::set_of& out, const int_partition_layer& partnLayer) const
{
    for(nat arm = 0; arm < theArms.size(); ++arm)
    {
        const int_partition_layer::properties& prop = partnLayer.GetProperties(theArms[arm]);
        build_arm_substitution_list(out, prop.theCoveringCoarsenings, arm);

    }
}
void dim_vec::BuildCoveringRefinementList(dim_vec_key::set_of& out, const int_partition_layer& partnLayer) const
{
    for(nat arm = 0; arm < theArms.size(); ++arm)
    {
        const int_partition_layer::properties& prop = partnLayer.GetProperties(theArms[arm]);
        build_arm_substitution_list(out, prop.theCoveringRefinements, arm);
    }
}


void dim_vec::BuildCoveringDominatorsList(dim_vec_key::set_of& out, const int_partition_layer& partnLayer) const
{
    for(nat arm = 0; arm < theArms.size(); ++arm)
    {
        const int_partition_layer::properties& prop = partnLayer.GetProperties(theArms[arm]);
        build_arm_substitution_list(out, prop.theCoveringDominators, arm);
    }
}

void dim_vec::BuildCoveringDomineesList(dim_vec_key::set_of& out, const int_partition_layer& partnLayer) const
{
    for(nat arm = 0; arm < theArms.size(); ++arm)
    {
        const int_partition_layer::properties& prop = partnLayer.GetProperties(theArms[arm]);
        build_arm_substitution_list(out, prop.theCoveringDominees, arm);
    }
}




void dim_vec::CalcMaximallyCoarse(const int_partition_layer& partnLayer, const vector_of& layer, const map<dim_vec_key,nat>& index)
{
    if(theRootStatus == RS_NON_ROOT)
    {
        MaximallyCoarse = false;
        return;
    }

    MaximallyCoarse = true;
    RefinementDegree = 0;

    for(nat arm = 0; arm < theArms.size(); ++arm)
        for(int_partition::vector_of::const_iterator i : partnLayer.GetProperties(theArms[arm]).theCoveringCoarsenings)
        {
            dim_vec_key temp(theArms);
            temp[arm] = i;
            sort(temp.begin(), temp.end());
            if(layer[index.at(temp)].IsRoot())
            {
                MaximallyCoarse = false;
                return;
            }
        }
}
bool dim_vec::IsMaximallyCoarse() const { return MaximallyCoarse; }



void dim_vec::CalcMaximallyDominant(const int_partition_layer& partnLayer, const vector_of& layer, const map<dim_vec_key,nat>& index)
{
    if(theRootStatus == RS_NON_ROOT)
    {
        MaximallyDominant = false;
        return;
    }

    MaximallyDominant = true;

    for(nat arm = 0; arm < theArms.size(); ++arm)
        for(int_partition::vector_of::const_iterator i : partnLayer.GetProperties(theArms[arm]).theCoveringDominators)
        {
            dim_vec_key temp(theArms);
            temp[arm] = i;
            sort(temp.begin(), temp.end());
            //const dim_vec& dv = layer[temp.GetIndex(partnLayer)];
            const dim_vec& dv = layer[index.at(temp)];
            if(dv.IsRoot())
            {
                MaximallyDominant = false;
                return;
            }
        }
}
bool dim_vec::IsMaximallyDominant() const { return MaximallyDominant; }


int_partition_layer::const_iterator dim_vec::GetHighestCommonRefinement(const int_partition_layer& partnLayer) const {

    int_partition_layer::ptr_bin temp;
    partnLayer.BuildHighestCommonRefinements(temp, theArms);
    return temp.front();
}
int_partition_layer::const_iterator dim_vec::GetLowestCommonCoarsening(const int_partition_layer& partnLayer) const {
    int_partition_layer::ptr_bin temp;
    partnLayer.BuildLowestCommonCoarsenings(temp, theArms);
    return temp.front();
}




void dim_vec::build_arm_substitution_list(dim_vec_key::set_of& out, const int_partition_layer::properties::vector_of& subs, nat arm) const
{
    for(int_partition::vector_of::const_iterator i : subs)//.begin(); i != subs.end(); ++i)
    {
        dim_vec_key temp(theArms);
        temp[arm] = i;
        sort(temp.begin(), temp.end());
        out.insert(temp);
    }
}





void dim_vec::SetRefinementDegree(int deg) { RefinementDegree = deg; }
int dim_vec::GetRefinementDegree() const { return RefinementDegree; }




double dim_vec::GetIndexCosineSqr(const int_partition_layer& partnLayer) const {
    if(theReflection)
    {
        vector<nat> a, b;
        theArms.GetIndices(partnLayer, a);
        theReflection->GetArms().GetIndices(partnLayer(partnLayer + chi), b);
        double dot = inner_product(a.begin(),a.end(),b.begin(),0);
        double aa = inner_product(a.begin(),a.end(),a.begin(),0);
        double bb = inner_product(b.begin(),b.end(),b.begin(),0);
        return dot/sqrt(aa*bb);
    }
    return 2;
}


void dim_vec::CalcEquiBrachialIndex(const int_partition_layer& partnLayer) {
    theDiversity = 0;
    int_partition_layer::set_of_ptr arms(theArms.begin(),theArms.end());
    theDiversity = (*arms.begin())->GetFirstPart() - (*arms.rbegin())->GetFirstPart();
    /*for(auto i = arms.begin(); i != arms.end(); ++i)
    {
        for(auto j = arms.begin(); j != i; ++j)
        {
            int_partition_layer::ptr_bin hcr, lcc;
            partnLayer.BuildHighestCommonRefinements(hcr, {*i,*j});
            partnLayer.BuildLowestCommonCoarsenings(lcc, {*i,*j});
            theDiversity += hcr.front()->size() - lcc.front()->size();
        }
    }*/
    theHCR = GetHighestCommonRefinement(partnLayer);
    EquiBrachialIndexHCR = accumulate(theArms.begin(), theArms.end(), 0, [this](int maxSoFar, itr_partn p) { return maxSoFar + this->theHCR->size() - p->size(); } );
    theLCC = GetLowestCommonCoarsening(partnLayer);
    EquiBrachialIndexLCC = accumulate(theArms.begin(), theArms.end(), 0, [this](int maxSoFar, itr_partn p) { return maxSoFar + p->size() - this->theLCC->size(); } );
}

void dim_vec::BuildEquiBrachialIndexSequence(std::vector<pair<nat,nat>>& out) const
{
    for(const dim_vec* cur = this; cur != NULL; cur = cur->GetReflection())
        out.push_back(make_pair(cur->theDiversity,0));
        //out.push_back(make_pair(cur->EquiBrachialIndexHCR,cur->EquiBrachialIndexLCC));
}


bool dim_vec::IsEquibrachial() const {
    return all_of(theArms.begin() + 1,theArms.end(),[this](const int_partition_layer::const_iterator i){return i == this->GetArms().front();});
}

//  PRINTING
void dim_vec::PrintArms (nat spaces, std::ostream& cout) const { theArms.Print(spaces, cout); }
void dim_vec::PrintArmIndices (nat spaces, const int_partition_layer& partnLayer, std::ostream& cout) const { theArms.PrintIndex(spaces, partnLayer, cout); }

double dim_vec::GetArmLengthToWeightRatios() const {

    double sum = 0;
    for(dim_vec_key::const_iterator i = theArms.begin(); i != theArms.end(); ++i) {
        sum += (1000*(*i)->size()/GetWeight())/1000.0;
    }
    return sum;
}

void dim_vec::PrintArmLengthToWeightRatios(nat spaces, std::ostream& cout) const {
    for(nat s = 0; s < spaces;++s) cout << " ";
    
    cout << "[";
    double sum = 0;
    for(dim_vec_key::const_iterator i = theArms.begin(); i != theArms.end(); ++i) {
        double val = (1000*(*i)->size()/GetWeight())/1000.0;
        sum += val;
        cout << " " << val;
    }
    cout << " = " << sum << " ]" << endl;
}
void dim_vec::PrintRootStatus (std::ostream& cout) const
{
    switch(theRootStatus) {
        case RS_IMAG_ROOT:
            cout << "Imaginary";
        break;
        case RS_FUNDAMENTAL_IMAG_ROOT:
            cout << "Fundamental";
        break;
        case RS_REAL_ROOT:
            cout << "Real";
        break;
        case RS_SIMPLE_ROOT:
            cout << "Simple";
        break;
        case RS_NON_ROOT:
            cout << "Nonroot";
        break;
        default:
            
        break;
    }
    
}

void dim_vec::Print(nat spaces, ostream& cout) const
{
    for(nat s = 0; s < spaces;++s) cout << " ";

    cout << "n = " << GetWeight()
                << ", q = " << GetQuadraticForm()
                << ", chi = " << GetChi()
                << ", num-reflectees " << GetNumReflectees()
                << ", max-coarse = " << IsMaximallyCoarse()
                << ", max-dom = " << IsMaximallyDominant()
                << ", root = ";
            PrintRootStatus(cout);
    cout << std::endl;
    //theHCR->Print(spaces + 2);
    //theLCC->Print(spaces + 2);
    cout << std::endl;
}




void dim_vec::Write(ostream& cout) const
{
    cout.write((char*)&theRootStatus, sizeof(ROOT_STATUS));
    cout.write((char*)&chi, sizeof(int));
    cout.write((char*)&q, sizeof(int));
    cout.write((char*)&minEta, sizeof(nat));
    cout.write((char*)&maxEta, sizeof(nat));
    cout.write((char*)&MaximallyCoarse, sizeof(bool));
    cout.write((char*)&Reducible, sizeof(bool));
    cout.write((char*)&RefinementDegree, sizeof(nat));
    cout.write((char*)&NumReflectees, sizeof(nat));
    cout.write((char*)&EquiBrachialIndexHCR, sizeof(int));
    cout.write((char*)&EquiBrachialIndexLCC, sizeof(int));
    cout.write((char*)&theDiversity, sizeof(int));
}

void dim_vec::Read(istream& cin)
{
    cin.read((char*)&theRootStatus, sizeof(ROOT_STATUS));
    cin.read((char*)&chi, sizeof(int));
    cin.read((char*)&q, sizeof(int));
    cin.read((char*)&minEta, sizeof(nat));
    cin.read((char*)&maxEta, sizeof(nat));
    cin.read((char*)&MaximallyCoarse, sizeof(bool));
    cin.read((char*)&Reducible, sizeof(bool));
    cin.read((char*)&RefinementDegree, sizeof(nat));
    cin.read((char*)&NumReflectees, sizeof(nat));
    cin.read((char*)&EquiBrachialIndexHCR, sizeof(int));
    cin.read((char*)&EquiBrachialIndexLCC, sizeof(int));
    cin.read((char*)&theDiversity, sizeof(int));
}