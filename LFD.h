#ifndef LFD_H_
#define LFD_H_

#include <string>
using namespace std;

double drand();
void generate_points(string filename, int num_pairs);

// The dependent variable is the classification (1 or -1)
// of a given point (x1,x2) according to the target function.
// This function writes a three column file.  The first two columns are 
// the coordinates x1, x2.  The third column is the dependent variable y.
//
void generate_points_with_dependent(string filename, int num_pairs, double * target);


void makeline(double *);
int test_point(double x1, double x2, double * hypo);
int count_mismatches(double * data_x1, double * data_x2, double * target, double * hypothesis, int);
void report_line(string filename, double * line);
int PLA_update(double * x1, double * x2, double *target, double * hypothesis, int num_pts);
double sqdifference(double * w1, double * w2, int dim);
void generate_rand_perm(int n, int * x);
double cross_entropy(double * x1, double *x2, double *w, double *y, int num_pts);

#endif