#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <cmath>
#include <fstream>
#include <iterator>
#include <algorithm>

#include "Partition.h"
#include "DimVec.h"
#include "DimVecBank.h"
#include "DimVecAnalyser.h"
#include "Python.h"

#include "test.h"

#include "PartitionSet.h"

using namespace std;

int_partition_bank thePartnBank(17);
dim_vec_bank theDimVecBank(thePartnBank, 3);
auto predT = [](const dim_vec&){ return true; };
auto predRoot = [](const dim_vec& dv){ return dv.IsRoot(); };
auto predReal = [](const dim_vec& dv){ return dv.IsReal(); };
auto predRedux = [](const dim_vec& dv){ return dv.IsReducible(); };
auto predEqArms = [](const dim_vec& dv){ return dv.IsEquibrachial(); };
auto predFinalEqArms = [](const dim_vec& dv){ return dv.GetFinalReduction()->IsEquibrachial();};

PythonEngine thePython;

void build_dim_vecs_from_scratch()
{
    clock_t time = clock();
    cout << "Dimension Vectors Building" << endl;

    theDimVecBank.BuildAll();//(predRedux);//(predT);
    //for(nat n = 0; n < theDimVecBank.size(); ++n)
    //    theDimVecBank[n].BuildEqualArms();
    cout << "Dimension Vectors Built in " << (clock() - time)/1000000.0 << endl;
    time = clock();

    theDimVecBank.ComputeRootStatus();
    cout << "Dimension Vector Statuses Found " << (clock() - time)/1000000.0 << endl;
    time = clock();

    theDimVecBank.ComputeRootStatusOfReducibles();
    cout << "Reducible Dimension Vector Statuses Found " << (clock() - time)/1000000.0 << endl;
    time = clock();

    theDimVecBank.ComputeDominanceStatus();
    cout << "Dimension Vector Dominance Statuses Found " << (clock() - time)/1000000.0 << endl;
    time = clock();
/*
    theDimVecBank.BuildProperties();
    cout << "Dimension Vector Properties Found " << clock() - time << endl;
    time = clock();

    theDimVecBank.CountReflectees();
    cout << "Dimension Vector Reflectees Counted " << clock() - time << endl;
    time = clock();
    */
}

void write_dim_vecs() {
    clock_t time = clock();
    theDimVecBank.Write();
    cout << "Dimension Vectors Written to File " << (clock() - time)/1000000.0 << endl;
    time = clock();
}

void read_dim_vecs() {
    clock_t time = clock();
    theDimVecBank.Read();
    cout << "Dimension Vectors Read from File " << (clock() - time)/1000000.0 << endl;
    time = clock();
}

int main()
{
    //TestDimVecBank2();
    thePartnBank.Build();
    thePartnBank.BuildProperties();
    //thePartnBank.Print();
    /*for(const int_partition_layer& layer : thePartnBank)
    {
        for(auto ip = layer.begin(); ip != layer.end(); ++ip)
        {
            ip->Print(0);
            for(auto q : layer.GetProperties(ip).theCoveringDominees)
                q->Print(2);
            for(auto q : layer.GetProperties(ip).theCoveringDominators)
                q->Print(4);
        }

    }*/
    //thePartnBank.BuildPairBounds();

    build_dim_vecs_from_scratch();
    //read_dim_vecs();
    
    //write_dim_vecs();

    for(const auto& layer : theDimVecBank)
    {
        dim_vec_layer_analyser analyser(layer);

        //const dim_vec_layer& layer = analyser.GetDimVecLayer();
        auto pred = [](const dim_vec& dv){ return dv.IsMaximallyDominant() && dv.IsImaginary(); };
        analyser.copy_pred(pred, analyser);

        printf("Num(%i): %lu\n", layer.GetN(), analyser.size());
        
        /*for(auto i = analyser.begin(); i != analyser.end(); ++i)
        {
            const dim_vec& dv = **i;
            if(dv.IsFundamental())
                dv.PrintArms(0,cout);
            dv.Print(0,cout);
        }*/
    }
    return 0;

    PartitionSet P1(theDimVecBank, 20,20,int_partition());
    P1.decompose();
    P1.PrintHeading(cout);
    P1.Print(cout, 0);

    PartitionSet P2(theDimVecBank, 19,19,int_partition());
    P2.decompose();
    P2.PrintHeading(cout);
    P2.Print(cout, 0);


    PartitionSet P3(theDimVecBank, 18,18,int_partition());
    P3.decompose();
    P3.PrintHeading(cout);
    P3.Print(cout, 0);
    P3.PrintComputation(cout);

    return 0;

    if(thePython.Python2DInit())
    {
        vector<vector<itr_partn>> dots1(thePartnBank.size()), dots2(thePartnBank.size()), dots3(thePartnBank.size());
        vector<nat> weights({5,8,10});
        for(auto i = weights.begin(); i != weights.end(); ++i)
        //for(nat n = 9; n < thePartnBank.size(); ++n)
        {
            nat n = *i;
            dim_vec_layer_analyser analyser(theDimVecBank[n]);
            thePython.Init(analyser);

            thePython.GetDimVecs([](const dim_vec& a){ return a.IsReal(); });
            for(auto i = thePython.begin(); i != thePython.end(); ++i)
                dots1[n].push_back((*i)->GetArms()[0]);

            thePython.Python2D(thePartnBank, dots1, 'b', '.');

            /*analyser.copy_pred([](const dim_vec& a){ return a.IsImaginary(); },dimvecs);
            for(auto i = dimvecs.begin(); i != dimvecs.end(); ++i)
                dots2[n].push_back((*i)->GetArms()[0]);

            analyser.copy_pred([](const dim_vec& a){ return !a.IsRoot(); },dimvecs);
            for(auto i = dimvecs.begin(); i != dimvecs.end(); ++i)
                dots3[n].push_back((*i)->GetArms()[0]);
            dimvecs.clear();*/

        }

        //Python2D(myfile, thePartnBank, dots3, 'y', '.');
        //Python2D(myfile, thePartnBank, dots2, 'r', '.');
        //Python2D(myfile, thePartnBank, dots1, 'b', '.');

        thePython.PythonEnd();
    }
    return 0;

    //dim_vec_layer_analyser analyser(theDimVecBank[thePartnBank.size() - 1]);
    //thePython.Init(analyser);

    if(thePython.Python3DInit())
    {
        /*dimvecs.clear();
        analyser.copy_pred([](const dim_vec& a){ return a.IsReal(); },dimvecs);
        Python(myfile, thePartnBank.back(), dimvecs, 'r', ',');*/

        thePython.GetDimVecs([](const dim_vec& a){ return predFinalEqArms(a); });
        thePython.Python3D('g', ',');

        thePython.GetDimVecs([](const dim_vec& a){ return !predFinalEqArms(a) && a.IsReducible() && ((2*a.GetQuadraticForm() - 2*a.GetWeight()*a.GetWeight()) % 3 != 0); });
        thePython.Python3D('r', ',');

        thePython.GetDimVecs([](const dim_vec& a){ return !predFinalEqArms(a) && a.IsReducible() && ((2*a.GetQuadraticForm() - 2*a.GetWeight()*a.GetWeight()) % 3 == 0); });
        thePython.Python3D('b', ',');

        /*dimvecs.clear();
        analyser.copy_pred([](const dim_vec& a){ return a.IsRoot() && !a.IsReducible() && !a.IsMaximallyCoarse(); },dimvecs);
        Python(myfile, thePartnBank.back(), dimvecs, 'b', '.');*/

        thePython.PythonEnd();
    }
}