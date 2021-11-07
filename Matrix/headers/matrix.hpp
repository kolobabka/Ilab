#include <iostream>
#include <cassert>
#include <iomanip>
#include <cstring>

namespace Matrix {

    template <typename pType> class Matrix {
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
                matrix_[i] = new pType [size_];
                assert (matrix_[i]);
                std::copy (rhsMatrix, rhsMatrix + size_, matrix_[i]);
            }
        } 
        Matrix (Matrix &&rhs) noexcept : size_(rhs.size_), matrix_(rhs.matrix_) { //Move ctor 

            rhs.matrix_ = nullptr;
            rhs.size_ = 0;
        }

        Matrix& operator=(const Matrix &rhs) { //Assignment-operator

        std::cout << "Ass Op" << std::endl;

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
                matrix_[i] = new pType [size_];
                assert (matrix_[i]);
                
                for (int j = 0; j < size_; ++j)
                    matrix_[i][j] = rhsMatrix[j];
            }
            return *this; 
        }       
        Matrix& operator=(Matrix &&rhs) noexcept { //Move-operator
        
            if (&rhs == this)
                return *this;

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
        //TODO: add some important selectors
    
        Proxy operator[] (int x)  const {
            Proxy tmp {matrix_[x]};
            return tmp;
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

