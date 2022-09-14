#include "Partition.h"
#include "DimVec.h"
#include "DimVecBank.h"
#include "DimVecAnalyser.h"

using namespace std;

dim_vec_layer_analyser::dim_vec_layer_analyser(const dim_vec_layer& layer) : theLayer(layer),
    num_roots(0), num_real(0), num_imag(0), num_simple(0), num_fundamental(0),
    num_max_coarse(0), num_max_coarse_imag(0), num_reducible(0)
        {}

nat dim_vec_layer_analyser::count_pred(const dim_vec_pred pred) const {
    return count_if(theLayer.begin(), theLayer.end(), [pred](const dim_vec& i){ return pred(i); });
}
void dim_vec_layer_analyser::copy_pred(const dim_vec_pred pred, dim_vec::vector_of_ptr& out) const {
    for(const dim_vec& i : theLayer)
        if(pred(i))
            out.push_back(&i);
}

void dim_vec_layer_analyser::ComputeStatistics()
{
    for(dim_vec_layer::const_iterator i = theLayer.begin(); i != theLayer.end(); ++i)
    {
        const dim_vec& alpha = *i;
        switch(alpha.GetRootStatus())
        {
        case dim_vec::RS_SIMPLE_ROOT:
            ++num_simple;
            case dim_vec::RS_REAL_ROOT:
                ++num_roots;
                ++num_real;
        break;
        case dim_vec::RS_FUNDAMENTAL_IMAG_ROOT:
            ++num_fundamental;
            case dim_vec::RS_IMAG_ROOT:
                ++num_roots;
                ++num_imag;
                if(alpha.IsMaximallyCoarse())
                    ++num_max_coarse_imag;
        break;
        case dim_vec::RS_UNKNOWN:
            cout << "Unknown" << endl;
        break;
        default:
        break;
        }
        if(alpha.IsMaximallyCoarse())
            ++num_max_coarse;
        if(alpha.IsReducible())
            ++num_reducible;
    }
}


void dim_vec_layer_analyser::PrintSummary(std::ostream& cout) const {
    cout << "Found " << theLayer.size() << " dimension vectors" << std::endl;
    cout << "  Found " << num_roots << " roots" << std::endl;
    cout << "    Found " << num_real << " real roots: ";
    cout << num_simple << " simple" << std::endl;
    cout << "    Found " << num_imag << " imag roots: ";
    cout << num_fundamental << " fundamental" << std::endl;
    cout << "    Found " << num_max_coarse << " maximally coarse roots: ";
    cout << num_max_coarse_imag << " imag" << std::endl;
    cout << "  Found " << theLayer.size() - num_roots << " non-roots" << std::endl;
    cout << "  Found " << num_reducible << " reducible" << std::endl;
}

void dim_vec_layer_analyser::PrintAll(std::ostream& cout) const {

    cout << "Dimension Vectors of Central Size " << theLayer.GetN() << std::endl;
    for(const dim_vec& i : theLayer)
    {
        i.PrintArms(0, cout);
        i.Print(0, cout);
        i.PrintArmLengthToWeightRatios(0, cout);
        for(auto refl = i.GetReflection(); refl; refl = refl->GetReflection())
        {
            refl->Print(2, cout);
            refl->PrintArmLengthToWeightRatios(2, cout);
            //refl->PrintArms(4, cout);
        }
        cout << std::endl;
    }
}
void dim_vec_layer_analyser::PrintTitle(ostream& cout) const {
    cout << "Dimension Vectors of Central Size " << theLayer.GetN() << endl;
}

void dim_vec_layer_analyser::PrintPred(const dim_vec_pred pred, std::ostream& cout) const
{
    for(const dim_vec& i : theLayer)
    {
        if(!pred(i))
            continue;
            
        i.PrintArms(0, cout);
        i.Print(0, cout);
        //i->second.PrintArmLengthToWeightRatios(0, cout);
        continue;
        for(auto refl = i.GetReflection(); refl; refl = refl->GetReflection())
        {
            refl->Print(2, cout);
            //refl->PrintArmLengthToWeightRatios(2, cout);
            //refl->PrintArms(4, cout);
        }
        cout << std::endl;
    }
}
void dim_vec_layer_analyser::PrintReductionSequence(nat spaces, const dim_vec& alpha, std::ostream& cout) const
{
    alpha.PrintArms(spaces,cout);
    PrintReductionSequenceWeights(spaces, alpha, cout);
    alpha.Print(spaces,cout);

    auto prop = theLayer.GetProperties(&alpha);
    for(auto j = prop.theDescentSequence.begin(); j != prop.theDescentSequence.end(); ++j)
    {
        (*j)->PrintArms(spaces + 4, cout);
        (*j)->Print(spaces + 4,cout);
    }

}
void dim_vec_layer_analyser::PrintReductionSequenceWeights(nat spaces, const dim_vec& alpha, std::ostream& cout) const
{
    for(nat s = 0; s < spaces;++s) cout << " ";

    auto prop = theLayer.GetProperties(&alpha);
    cout << "[ ";
    for(auto j = prop.theDescentSequenceWeights.begin(); j != prop.theDescentSequenceWeights.end(); ++j)
        cout << *j << " ";
    cout << "]" << std::endl;
}