// perceptron.cpp 
// software to test the perceptron learning algorithm


#include <cstdlib> // for rand, srand
#include <ctime> // for time() 
#include <iostream>
#include <fstream>
#include "LFD.h"
using namespace std;

int main ()
{
	int num_points = 100;
	int dense_points = 1000;
	int num_trials = 1;
	
	int total_iter = 0;
	int dense_mismatches = 0;
	
	//initialize random number seed
	srand(time(NULL));
	rand();
	
	// generate a new dataset if necessary
	char output[20] = "file.dat";
	generate_points(output, num_points);
	char densefile[20] = "denseset.dat";
	generate_points(densefile, dense_points);
	
	// read data file into local memory
	ifstream fin("file.dat");
	double x[num_points];
	double y[num_points];
	for (int i = 0; i < num_points; i++)
	{
		fin >> x[i] >> y[i]; 
	}
	fin.close();
	
	// read dense set file into local memory
	ifstream densein("denseset.dat");
	double dense_x[dense_points];
	double dense_y[dense_points];
	for (int i = 0; i < dense_points; i++)
	{
		densein >> dense_x[i] >> dense_y[i]; 
	}
	densein.close();
	
	
	for(int i = 0; i < num_trials; i++){
		// make target function
		double target[3];
		makeline(target);
		report_line("target.txt", target);
	
		//make hypothesis
		double w[3];
		makeline(w);
		report_line("hypothesis.txt", w);
	
		// do perceptron algorithm 
	
		total_iter += PLA_update(x,y,target,w,num_points);
		dense_mismatches +=
			 count_mismatches(dense_x,dense_y,target,w,dense_points);
	
		report_line("updated.txt", w);
	}
	
	double avg_iter = ((double) total_iter) / num_trials; 
	double prob_mismatch = ((double) dense_mismatches)/num_trials/dense_points;
	cout << "Average iterations needed: " << avg_iter << endl;
	cout << "Out of sample mismatch probability: " << prob_mismatch << endl;
	return 0;
}

