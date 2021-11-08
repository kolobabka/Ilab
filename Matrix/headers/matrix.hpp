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
    public:
        Matrix (int size = 0) : size_(size) {

            matrix_ = new pType* [size_];
            assert (matrix_);
            for (int i = 0; i < size_; ++i) {

                matrix_[i] = new pType [size_]{};
                assert (matrix_[i]);
            }
        }
        Matrix (const Matrix &rhs) : size_(rhs.size_) { //Copy ctor

            matrix_ = new pType* [size_];
            assert (matrix_);

            for (int i = 0; i < size_; ++i) {

                pType* rhsMatrix = rhs.matrix_[i];
                matrix_[i] = new pType [size_]{};
                assert (matrix_[i]);
                std::copy (rhsMatrix, rhsMatrix + size_, matrix_[i]);
            }
        }
        Matrix (Matrix &&rhs) noexcept : size_(rhs.size_), matrix_(rhs.matrix_) { //Move ctor

            rhs.matrix_ = nullptr;
            rhs.size_ = 0;
        }

        Matrix (int size, int det) : size_(size) {  //PseudoRandomMatrixCtor with a given determinant

            matrix_ = new pType* [size_];
            assert (matrix_);

            for (int i = 0; i < size_; ++i) {

                matrix_[i] = new pType [size_]{};
                assert (matrix_[i]);
            }
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

            for (int i = 0; i < size_; ++i) {

                pType* rhsMatrix = rhs.matrix_[i];
                matrix_[i] = new pType [size_]{};
                assert (matrix_[i]);

                for (int j = 0; j < size_; ++j)
                    matrix_[i][j] = rhsMatrix[j];
            }
            return *this;
        }
        Matrix& operator=(Matrix &&rhs) noexcept { //Move-operator

            if (&rhs == this)
                return *this;

            size_ = rhs.size_;
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

    public:
        int getSize() const {

            return size_;
        };

        Proxy operator[] (int x)  const {
            Proxy tmp {matrix_[x]};
            return tmp;
        }



        pType determinant () {

            // Matrix <double> tmpMatrix = *this;
            double** tmpMatrix = new double* [size_];
            int sign {};

            for (int i = 0; i < size_; ++i) {

                tmpMatrix[i] = new double [size_];
                for (int j = 0; j < size_; ++j)
                    tmpMatrix[i][j] = static_cast<double> (matrix_[i][j]);
            }

            for (int current = 0; current < size_; ++current) {


                // for (int i = 0; i < size_; ++i) {

                //     for (int j = 0; j < size_; ++j)
                //         std::cout << std::setw(6) << tmpMatrix[i][j];

                //     std::cout << std::endl;
                // }

                MaxElem maxElem = maxSubMatrixElem(tmpMatrix, current, size_);

                swap_columns (tmpMatrix, current, &maxElem, size_);
                if (maxElem.col != current) {

                    ++sign;
                    maxElem.col = current;
                }

                swap_rows (tmpMatrix, current, &maxElem, size_);
                if (maxElem.row != current) {

                    ++sign;
                    maxElem.row = current;
                }

                // std::cout << "max elem = " << maxElem.max << std::endl;

                if (DoubleCmp (std::abs(tmpMatrix[current][current]), 0) == 0) {

                    for (int i = 0; i < size_; ++i)
                        delete [] tmpMatrix[i];

                    delete [] tmpMatrix;
                    return 0;
                }

                eliminate (tmpMatrix, current, size_);
            }

            if (sign % 2 == 0)
                return pType (CalcTrace (tmpMatrix, size_));

            return pType (-CalcTrace (tmpMatrix, size_));
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
}
