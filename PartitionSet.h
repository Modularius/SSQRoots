#ifndef PARTNSET_H
#define PARTNSET_H

#include <iostream>
#include <iomanip>

#include "Partition.h"
#include "DimVecBank.h"
#include "DimVecAnalyser.h"

class PartitionSet {
    const dim_vec_bank& theDimVecBank;

    double k;
    nat weight;
    nat max;
    int_partition mustContain;

    std::vector<PartitionSet> redux;
public:
    PartitionSet(const dim_vec_bank& dvBank, nat n, nat m, const int_partition& must)
     : theDimVecBank(dvBank), k(dvBank.GetK()), weight(n), max(m), mustContain(must) {}
    bool test() const { return mustContain.GetWeight() <= weight && mustContain.GetFirstPart() <= max; }

    bool IsPartitionIn(const int_partition& p) const {
        return p.GetFirstPart() <= max && mustContain.IsContainedWithin(p);
    }

    nat GetSize() const {
        nat num = 0;
        for(auto i = theDimVecBank[weight].GetPartitionLayer().begin(); i != theDimVecBank[weight].GetPartitionLayer().end(); ++i)
            if(IsPartitionIn(*i))
                ++num;
        return num;
    }
    nat GetNumFundamental() const {
        nat num = 0;
        for(auto i = theDimVecBank[weight].GetPartitionLayer().begin(); i != theDimVecBank[weight].GetPartitionLayer().end(); ++i)
            if(i->GetFirstPart() <= floor(weight*(1 - 2/k)) && mustContain.IsContainedWithin(*i))
                ++num;
        return num;
    }
    nat GetNumFundamentalFromSource() const {
        nat num = 0;
        for(auto i = theDimVecBank[weight].begin(); i != theDimVecBank[weight].end(); ++i)
            if(i->IsFundamental())
                if(IsPartitionIn(*i->GetArms()[0]))
                    ++num;
        return num;
    }
    nat GetNumSimple() const {
        return (weight == 1)?1:0;
    }
    nat GetNumNegatable() const {
        if(weight == 1)
            return 0;
        nat num = 0;
        for(auto i = theDimVecBank[weight].GetPartitionLayer().begin(); i != theDimVecBank[weight].GetPartitionLayer().end(); ++i)
            if(i->GetFirstPart() > weight*(k - 2)/(k - 1) && IsPartitionIn(*i))
                ++num;
        return num;
    }
    nat GetNumNegatableFromSource() const {
        nat num = 0;
        for(auto i = theDimVecBank[weight].begin(); i != theDimVecBank[weight].end(); ++i)
            if((!i->IsRoot() && !i->IsReducible()) || i->GetRootStatus()==dim_vec::RS_SIMPLE_ROOT)
                if(IsPartitionIn(*i->GetArms()[0]))
                    ++num;
        return num;
    }
    nat GetNumReducibleFromSource() const {
        nat num = 0;
        for(auto i = theDimVecBank[weight].begin(); i != theDimVecBank[weight].end(); ++i)
            if(i->IsReducible())
                if(IsPartitionIn(*i->GetArms()[0]))
                    ++num;
        return num;
    }
    nat  GetReducibleRangeMin() const {
        return ceil(weight/(k - 1));
    }
    nat  GetReducibleRangeMax() const {
        return ceil(2*weight/k);
    }
    nat  GetReducibleRange() const {
        return GetReducibleRangeMax() - (int)GetReducibleRangeMin();
    }

    nat FindNumImaginary() const {
        return accumulate(redux.begin(),redux.end(),GetNumFundamental(),[]
            (nat total, const PartitionSet& p){ return total + p.FindNumImaginary(); });
    }

    nat FindNumNonRoots() const {
        return accumulate(redux.begin(),redux.end(),GetNumNegatable(),[]
            (nat total, const PartitionSet& p){ return total + p.FindNumNonRoots(); });
    }

    void decompose() {
        for(nat r = GetReducibleRangeMin(); r < GetReducibleRangeMax(); ++r)
        {
            int_partition p(mustContain);
            auto f = p.find(weight - r);
            if(f != p.end())
            {
                std::cout << "erasing " << *f << " from ";
                PrintHeader(std::cout);
                std::cout << std::endl;
                p.erase(f);
            }
            p.insert(r*(k - 1) - weight);
            p.erase(0);
            redux.push_back(PartitionSet(theDimVecBank, r*k - weight, weight - r, p));
            if(!redux.back().test())
            {
                std::cout << "rejecting ";
                redux.back().PrintHeader(std::cout);
                std::cout << " from ";
                PrintHeader(std::cout);
                std::cout << std::endl;
                redux.pop_back();
            }
        }
        for(auto i = redux.begin(); i != redux.end(); ++i)
            i->decompose();
    }

    void PrintHeader(std::ostream& cout) const {
        cout << "P_" << max << "(" << weight << ", { ";
        mustContain.PrintInline(cout);
        cout << "}) ";
    }
    void PrintHeading(std::ostream& cout) const {
        cout << std::setw(8) << "size"
             << std::setw(8) << "red"
             << std::setw(8) << "fndml"
             << std::setw(8) << "neg"
             << std::setw(8) << "smpl"
             << std::setw(8) << "range"
             << std::setw(0) << std::endl;
    }
    void PrintSummary(std::ostream& cout) const {
        cout
         << std::setw(8) << GetSize()
         << std::setw(8) << GetNumReducibleFromSource()
         << std::setw(8) << GetNumFundamental()
         << std::setw(8) << GetNumNegatable()
         << std::setw(8) << GetNumSimple()
         << std::setw(8) << GetReducibleRange();
    }
    void Print(std::ostream& cout, nat spaces) const {
        //for(nat s = 0; s < spaces;++s) std::cout << " ";
        PrintSummary(cout);
        std::cout << std::setw(2 + 2*spaces) << "";
        PrintHeader(cout);
        //for(nat s = 0; s < spaces;++s) std::cout << " ";
        cout << std::endl;
        for(auto i = redux.begin(); i != redux.end(); ++i)
            i->Print(cout, spaces + 2);
    }
    void PrintComputation(std::ostream& cout) const {
        dim_vec_layer_analyser analyser(theDimVecBank[weight]);
        cout << "Num Imag: " << FindNumImaginary() << std::endl;
        cout << "Num Imag: " << analyser.count_pred([](const dim_vec& a){return a.IsImaginary();}) << std::endl;

        cout << "Num Nonroots: " << FindNumNonRoots() << std::endl;
        cout << "Num Nonroots: " << analyser.count_pred([](const dim_vec& a){return !a.IsRoot();}) << std::endl;
    }
};


#endif