// linreg.cpp
// linear regression for classification
// 
// compile with: g++ linreg.cpp LFD.cpp -l armadillo

#include <cstdlib> //for rand, srand
#include <ctime>
#include <iostream>
#include <fstream>
#include <armadillo> //for linear algebra packages
#include "LFD.h"
using namespace std;
using namespace arma;

int main()
{
	int num_points = 100;
	int dense_points = 1000;
	int num_trials = 1000;
	int dense_mismatches = 0;
	int in_sample_mismatches = 0;
	int total_perceptron_iter = 0;
	
	
	
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
	
	vec y(num_points);
	
	// make the matrix X
	mat X_ia(num_points,3);
	for(int i = 0; i < num_points; i++)
	{
		X_ia(i,0) = 1;
		X_ia(i,1) = x1[i];
		X_ia(i,2) = x2[i];
	}
		
	
	// read dense set file into local memory
	ifstream densein("denseset.dat");
	double dense_x1[dense_points];
	double dense_x2[dense_points];
	for (int i = 0; i < dense_points; i++)
	{
		densein >> dense_x1[i] >> dense_x2[i]; 
	}
	densein.close();
	
	for(int i = 0; i < num_trials; i++){
		// make target function
		double target[3];
		makeline(target);
		report_line("target.txt", target);
		
		for(int k =0; k < num_points; k++)
		{
			if(target[1]*x1[k] + target[2]*x2[k] >= -target[0])
				y(k) = 1;
			else
				y(k) = -1;
		}
	
		//do linear regression
		
		
		double w[3];
		
		vec h(3);
		h = inv(trans(X_ia)*X_ia)*trans(X_ia)*y;
		for(int j = 0; j < 3; j++)
			w[j] = h(j);

		report_line("hypothesis.txt",w);
		in_sample_mismatches +=
			count_mismatches(x1,x2,target,w,num_points);
		dense_mismatches +=
			 count_mismatches(dense_x1,dense_x2,target,w,dense_points);
		
	
		// do perceptron algorithm 
	
		total_perceptron_iter += PLA_update(x1,x2,target,w,num_points);
		//dense_mismatches +=
		//	 count_mismatches(dense_x1,dense_x2,target,w,dense_points);
	
		report_line("updated.txt", w);
		
	}
	
	double Ein = ((double) in_sample_mismatches)/num_trials/num_points;
	double Eout = ((double) dense_mismatches)/num_trials/dense_points;
	// compare hw2, prob 5 with num_points = 100
	cout << "In sample mismatch probability for linear regression: " 
		<< Ein << endl;
	//compare hw2, prob 6
	cout << "Out of sample mismatch probability for linear regression: " 
		<< Eout << endl;
	double avg_iter = ((double) total_perceptron_iter) / num_trials; 
	//compare hw2, prob 7 with num_points = 10
	cout << "Average perceptron iterations needed: " << avg_iter << endl;

	return 0;
}