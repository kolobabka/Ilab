#include <iostream>
#include <cassert>
#include <iomanip>
#include <cstring>
#include "common.hpp"

namespace Matrix {

    template <typename pType> class Matrix final {
    private:
        int size_;
        pType** matrix_;

        template  <typename someType = pType> 
        void copyMatrix (const Matrix<someType> &rhs) & {

            for (int i = 0; i < size_; ++i) {
                
                matrix_[i] = new pType [size_]{};
                assert (matrix_[i]);                   

                for (int j = 0; j < size_; ++j) 
                    matrix_[i][j] = rhs[i][j];
            }
        }
    public:
        Matrix (int size = 0) : size_(size), matrix_(new pType* [size_]) {
            
            assert (matrix_);
            for (int i = 0; i < size_; ++i) {

                matrix_[i] = new pType [size_]{};
                assert (matrix_[i]);
            }
        }
        Matrix (const Matrix &rhs) : size_(rhs.getSize()), matrix_(new pType* [size_]) { //Copy ctor

            assert (matrix_);
            copyMatrix (rhs);
        }

        template <typename someType> 
        Matrix (const Matrix<someType> &rhs) : size_(rhs.getSize()), matrix_(new pType* [size_]) {
            
            assert (matrix_);
            copyMatrix (rhs);
        }

        Matrix (Matrix &&rhs) noexcept : size_(rhs.size_), matrix_(rhs.matrix_) { //Move ctor

            rhs.matrix_ = nullptr;
            rhs.size_ = 0;
        }

        Matrix (int size, int det) : Matrix (size) {  //PseudoRandomMatrixCtor with a given determinant // I don't like it :(

            int j = 0;
            for (int i = 0; i < size_; i++) {

                j = i;

            for (j; j < size_; j++) {

            if (j == i)
                matrix_[i][j] = 1;
            else
                matrix_[size_ - 1 - i][size_ - 1 - j] = rand () % 100;
            }

            if (i == size_ - 1)
                matrix_[i][j - 1] = det;
            }

            std::srand(time(nullptr));
            int randRange = rand () % 5 ;

            for (int i = 0; i < randRange; ++i) {
                for   (int j = 0; j < size_ - 1; ++j) {

                    int randCoeff = (rand () % 4);
                    for (int k = 0; k < size_; ++k) 
                        matrix_[j][k] += (matrix_[size_ - 1][k] * randCoeff) ;
                }
            }
        }

        Matrix& operator=(const Matrix &rhs) { //Assignment-operator

            if (this == &rhs)
                return *this;
            for (int i = 0; i < size_; ++i)
                delete [] matrix_[i];
            size_ = rhs.size_;
            delete [] matrix_;

            matrix_ = new pType* [size_];
            assert (matrix_);

            copyMatrix (rhs);
            return *this;
        }
        Matrix& operator=(Matrix &&rhs) noexcept { //Move-operator

            if (&rhs == this)
                return *this;

            std::swap (size_, rhs.size_);
            std::swap (matrix_, rhs.matrix_);
            return *this;
        }

        struct Proxy {

            pType* row;
            const pType& operator[](int y) const { return row[y]; }
            pType& operator[](int y) { return row[y]; }
        };

        ~Matrix() { //Destructor

            for (int i = 0; i < size_; ++i)
                delete [] matrix_[i];

            delete [] matrix_;
        };

        MaxElem maxSubMatrixElem (int current) const {

            MaxElem maxElemT {current, current, matrix_[current][current]};
            for (int i = current; i < size_; ++i)
                for (int j = current; j < size_; ++j)
                    if ((DoubleCmp (std::abs(maxElemT.max), std::abs(matrix_[i][j]))) < 0  && DoubleCmp (std::abs(matrix_[i][j]), 0) != 0) {

                        maxElemT.col = j;
                        maxElemT.row = i;
                        maxElemT.max = matrix_[i][j];
                    }

            return maxElemT;
        }

        void swap_columns (int current, MaxElem *maxElem) {

            assert (maxElem);
            double tmp {};

            for (int i = current; i < size_; ++i) {

                tmp = matrix_[i][current];
                matrix_[i][current] = matrix_[i][maxElem->col];
                matrix_[i][maxElem->col] = tmp;
            }
        }

        void swap_rows (int current, MaxElem *maxElem) {

            assert (maxElem);
            double tmp {};

            for (int i = current; i < size_; ++i) {

                tmp = matrix_[current][i];
                matrix_[current][i] = matrix_[maxElem->row][i];
                matrix_[maxElem->row][i] = tmp;
            }
        }

        void eliminate (int current) {

            double coeff{};

            for (int i = current + 1; i < size_; ++i) {

                coeff = matrix_[i][current] / matrix_[current][current];

                for (int j = 0; j < size_; ++j) {

                    matrix_[i][j] -= matrix_[current][j] * coeff;

                    if (DoubleCmp (std::abs(matrix_[i][j]), 0) == 0)
                        matrix_[i][j] = 0;
                }
            }
        }

        double calcTrace () {

            double det{1};

            for (int i = 0; i < size_; ++i) 
                det *= matrix_[i][i];
        
            return det;
        }

    public:
        int getSize() const {

            return size_;
        };

        Proxy operator[] (int x)  const {
            Proxy tmp {matrix_[x]};
            return tmp;
        }

        pType determinant () {
            
            Matrix <double> testMatrix = *this;
            int sign {};
            
            for (int current = 0; current < size_; ++current) {
                
                MaxElem maxElem = testMatrix.maxSubMatrixElem(current);

                if (maxElem.col != current) {

                    testMatrix.swap_columns(current, &maxElem);
                    ++sign;
                    maxElem.col = current;
                }
                
                if (maxElem.row != current) {
                    
                    testMatrix.swap_rows(current, &maxElem);
                    ++sign;
                    maxElem.row = current;
                }

                if (DoubleCmp (std::abs(testMatrix[current][current]), 0) == 0) {
                    
                    return 0;
                }
                
                testMatrix.eliminate(current);
            }

            if (sign % 2 == 0)
                return pType (testMatrix.calcTrace());

            return pType (-testMatrix.calcTrace ());
        }
    };

    template <typename pType> std::ostream& operator << (std::ostream &out, const Matrix<pType> &thou) {

        int size = thou.getSize();
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j)
                out << std::setw (5) << thou[i][j];

            out << std::endl;
        }
        return out;
    }

    template <typename pType> std::istream& operator >> (std::istream &in, Matrix<pType> &thou) {

        int size = thou.getSize();
        for (int i = 0; i < size; ++i)
            for (int j = 0; j < size; ++j)
                in >> thou[i][j];

        return in;
    }

    template <typename pType> bool operator == (const Matrix<pType> &first, const Matrix<pType> &second) {

        int firstSize = first.getSize();
        int secondSize = second.getSize();
        if (firstSize == secondSize)
            for (int i = 0; i < firstSize; ++i) 
                for (int j = 0; j < firstSize; ++j)
                    if (first[i][j] != second[i][j])
                        return false;
            
        return true;

    }
}
