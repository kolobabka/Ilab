#include <iostream>
#include <cassert>

const double EPSILON = 10E-16;

struct MaxElem {

  int col;
  int row;
  double max;
};

double CalcTrace (double** tmpMatrix, int size);
void eliminate (double** tmpMatrix, int current, int size);
int DoubleCmp (const double firstDouble, const double secondDouble);
MaxElem maxSubMatrixElem (double** tmpMatrix, int current, int size_);
void swap_rows (double** tmpMatrix, int current, MaxElem *maxElem,  int size);
void swap_columns (double** tmpMatrix, int current, MaxElem *maxElem,  int size);
