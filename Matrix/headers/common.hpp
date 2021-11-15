#include <iostream>
#include <cassert>

const double EPSILON = 10E-16;

struct MaxElem {

  int col;
  int row;
  double max;
};

int DoubleCmp (const double firstDouble, const double secondDouble);

