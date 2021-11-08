#include "common.hpp"
#include <iostream>
#include <cassert>
#include <iomanip>
#include <cstring>

int DoubleCmp (const double firstDouble, const double secondDouble) {

    if (std::abs (firstDouble - secondDouble) <= EPSILON)
        return 0;

    if (firstDouble > secondDouble)
        return 1;

    return -1;

}

MaxElem maxSubMatrixElem (double** tmpMatrix, int current, int size_) {

    MaxElem maxElem {current, current, tmpMatrix[current][current]};
    for (int i = current; i < size_; ++i)
        for (int j = current; j < size_; ++j)
            if ((DoubleCmp (std::abs(maxElem.max), std::abs(tmpMatrix[i][j]))) < 0 && DoubleCmp (tmpMatrix[i][j], 0) != 0 ) {

                maxElem.col = j;
                maxElem.row = i;
                maxElem.max = tmpMatrix[i][j];
            }

    return maxElem;
}

void swap_columns (double** tmpMatrix, int current, MaxElem *maxElem,  int size) {

    assert (tmpMatrix);
    assert (maxElem);
    double tmp {};

    for (int i = current; i < size; ++i) {

        tmp = tmpMatrix[i][current];
        tmpMatrix[i][current] = tmpMatrix[i][maxElem->col];
        tmpMatrix[i][maxElem->col] = tmp;
    }
}

void swap_rows (double** tmpMatrix, int current, MaxElem *maxElem,  int size) {

    assert (tmpMatrix);
    assert (maxElem);
    double tmp {};

    for (int i = current; i < size; ++i) {

        tmp = tmpMatrix[current][i];
        tmpMatrix[current][i] = tmpMatrix[maxElem->row][i];
        tmpMatrix[maxElem->row][i] = tmp;
    }
}

void eliminate (double** tmpMatrix, int current, int size) {

    assert (tmpMatrix);
    assert (tmpMatrix[current]);

    double coeff{};

    for (int i = current + 1; i < size; ++i) {

        coeff = tmpMatrix[i][current] / tmpMatrix[current][current];

        for (int j = 0; j < size; ++j) {

            tmpMatrix[i][j] -= tmpMatrix[current][j] * coeff;

            if (DoubleCmp (std::abs(tmpMatrix[i][j]), 0) == 0)
                tmpMatrix[i][j] = 0;
        }
    }
}

double CalcTrace (double** tmpMatrix, int size) {

    assert (tmpMatrix);

    double det{1};

    for (int i = 0; i < size; ++i) {

        det *= tmpMatrix[i][i];
        delete [] tmpMatrix[i];
    }

    delete [] tmpMatrix;

    // std::cout << std::setprecision(7) << det << std::endl;
    return det;
}
