// logreg.cpp
// logistic regression for classification
// 
// compile with: g++ logreg.cpp LFD.cpp

#include <cstdlib> //for rand, srand
#include <ctime>
#include <iostream>
#include <fstream>
#include <cmath>
#include "LFD.h"
using namespace std;



int main()
{
	int num_points = 100;
	int dense_points = 1000;
	int num_trials = 100;
	double accum_dense_xentropy = 0;
	double accum_in_sample_xentropy = 0;
	int total_iterations = 0;
	double rate = 0.01;
	
	srand(time(NULL));
	rand();
	// generate a new dataset if necessary
	generate_points("file.dat", num_points);
	generate_points("denseset.dat", dense_points);
	
	// read data file into local memory
	ifstream fin("file.dat");
	double x1[num_points];
	double x2[num_points];
	for (int i = 0; i < num_points; i++)
	{
		fin >> x1[i] >> x2[i]; 
	}
	fin.close();
	
	// read dense set file into local memory
	ifstream densein("denseset.dat");
	double dense_x1[dense_points];
	double dense_x2[dense_points];
	for (int i = 0; i < dense_points; i++)
	{
		densein >> dense_x1[i] >> dense_x2[i]; 
	}
	densein.close();
	
	// for recording classification
	double y[num_points];
	double dense_y[dense_points];
	
	double target[3];
	double hypothesis[3]={0,0,0};
	double updated[3]={0,0,0};
	double tmp_hypo[3];

	
	// Do num_trials choices of target function
	for(int i = 0; i < num_trials; i++){
		// make target function
		makeline(target);
		report_line("target.txt", target);
		
		// assign values for y
		for(int k =0; k < num_points; k++)
		{
			if(target[1]*x1[k] + target[2]*x2[k] >= -target[0])
				y[k] = 1;
			else
				y[k] = -1;
		}
		for(int k =0; k < dense_points; k++)
		{
			if(target[1]*dense_x1[k] + target[2]*dense_x2[k] >= -target[0])
				dense_y[k] = 1;
			else
				dense_y[k] = -1;
		}

	
		// logistic regression
		// GOES HERE
		int p[num_points];
		int ct = 0;
		do
		{
			generate_rand_perm(num_points, p);
			hypothesis[0] = updated[0];
			hypothesis[1] = updated[1];
			hypothesis[2] = updated[2];

			
			for(int j = 0; j < num_points; j++){
				tmp_hypo[0] = updated[0];
				tmp_hypo[1] = updated[1];
				tmp_hypo[2] = updated[2];
				
				double denom =(1 + exp(y[p[j]]*(tmp_hypo[0] + x1[p[j]]*tmp_hypo[1] + x2[p[j]]*tmp_hypo[2])));

				updated[0] = tmp_hypo[0] + rate*y[p[j]]/denom;
				updated[1] = tmp_hypo[1] + rate*x1[p[j]]*y[p[j]]/denom;
				updated[2] = tmp_hypo[2] + rate*x2[p[j]]*y[p[j]]/denom;
			}
			total_iterations++;
		
		}while(sqdifference(hypothesis, updated,3) > 0.0001);
		accum_in_sample_xentropy += cross_entropy(x1,x2,updated,y,num_points);
		accum_dense_xentropy += cross_entropy(dense_x1,dense_x2,updated,dense_y,dense_points);

	
		report_line("updated.txt", updated);
		
	}
	
	double Ein = accum_in_sample_xentropy/num_trials ;
	double Eout = accum_dense_xentropy/num_trials ;
	double iterations = ((double) total_iterations)/num_trials;
	cout << "Estimated in sample cross entropy error for logistic regression: "
	<< Ein << endl;
	cout << "Estimated out of sample cross entropy error for logistic regression: "
	<< Eout << endl;
	cout << "Avg number of SGD epochs: " << iterations << endl;
	return 0;
}