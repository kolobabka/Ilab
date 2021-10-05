#include <iostream>
#include <new>
#include <iterator>
#include <cassert>
#include <algorithm>
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
namespace objects {
    
    //##############################################################################
    //                              VECTOR-CLASS PART
    //############################################################################## 

    template <typename PType> class Vector {

    private:
        PType x_;
        PType y_;
        PType z_;
    
    public:
        Vector (PType x = 0, PType y = 0, PType z = 0) :

            x_(x),
            y_(y),
            z_(z) {}

        void SetPointX (PType x) {
            x_ = x;
        }
        void SetPointY (PType y) {
            y_ = y;
        }
        void SetPointZ (PType z) {
            z_ = z;
        }
        PType GetPointX () const {
            return x_;
        }
        PType GetPointY () const {
            return y_;
        }
        PType GetPointZ () const {
            return z_;
        }
    
        Vector<PType> &operator += (const Vector<PType>& vec) {
            
            x_ += vec.GetPointX();
            y_ += vec.GetPointY();
            z_ += vec.GetPointZ();

            return *this;
        }
        Vector<PType> &operator -= (const Vector<PType>& vec) {
            
            x_ -= vec.GetPointX();
            y_ -= vec.GetPointY();
            z_ -= vec.GetPointZ();

            return *this;
        }
    };
    //##############################################################################
    //                         VECTOR-OVERLOAD PART
    //##############################################################################
    template <typename PType> 
    std::istream &operator >> (std::istream &in, Vector<PType> &point) {

        PType x, y, z;
        in >> x >> y >> z;

        point.SetPointX(x); 
        point.SetPointY(y); 
        point.SetPointZ(z);

        return in;
    }
    template <typename PType>
    std::ostream &operator << (std::ostream &out, const Vector<PType> &point) {

        out << "X = " << point.GetPointX() << std::endl; 
        out << "Y = " << point.GetPointY() << std::endl; 
        out << "Z = " << point.GetPointZ() << std::endl; 

        return out;
    }
    template <typename PType>
    Vector<PType> operator + (const Vector<PType>& vec1, const Vector<PType>& vec2) {
            
        PType x = vec1.GetPointX () + vec2.GetPointX ();
        PType y = vec1.GetPointY () + vec2.GetPointY ();
        PType z = vec1.GetPointY () + vec2.GetPointY ();

        return Vector<PType> (x, y, z);
    }
    template <typename PType>
    Vector<PType> operator - (const Vector<PType>& vec1, const Vector<PType>& vec2) {
            
        PType x = vec1.GetPointX () - vec2.GetPointX ();
        PType y = vec1.GetPointY () - vec2.GetPointY ();
        PType z = vec1.GetPointY () - vec2.GetPointY ();

        return Vector<PType> (x, y, z);
    }
    template <typename PType>
    Vector<PType> operator * (const Vector<PType>& vec1, const float num) {
            
        PType x = vec1.GetPointX () * num;
        PType y = vec1.GetPointY () * num;
        PType z = vec1.GetPointZ () * num;

        return Vector<PType> (x, y, z);
    }
    template <typename PType>
    Vector<PType> operator * (const float num, const Vector<PType>& vec1) {
            
        PType x = vec1.GetPointX () * num;
        PType y = vec1.GetPointY () * num;
        PType z = vec1.GetPointZ () * num;

        return Vector<PType> (x, y, z);
    }
    template <typename PType>
    Vector<PType> operator/ (const Vector<PType>& vec1, const float num) {
            
        PType x = vec1.GetPointX () / num;
        PType y = vec1.GetPointY () / num;
        PType z = vec1.GetPointZ () / num;

        return Vector<PType> (x, y, z);
    }
    template <typename PType>
    Vector<PType> operator - (const Vector<PType>& vec1) {
            
        PType x = -vec1.GetPointX ();
        PType y = -vec1.GetPointY ();
        PType z = -vec1.GetPointZ ();

        return Vector<PType> (x, y, z);
    }
    //##############################################################################
    //                         TRIANGLE-CLASS PART
    //##############################################################################
    template <typename PType> class Triangle {

    private:
        Vector<PType> rVec1_;
        Vector<PType> rVec2_;
        Vector<PType> rVec3_;

    public:
        Triangle () :

            rVec1_{0, 0, 0},
            rVec2_{0, 0, 0},
            rVec3_{0, 0, 0} {}

        void SetVec1 (Vector<PType> &vec1) {
            rVec1_ = vec1;
        }
        void SetVec2 (Vector<PType> &vec2) {
            rVec2_ = vec2;
        }
        void SetVec3 (Vector<PType> &vec3) {
            rVec3_ = vec3;
        }
        Vector<PType> GetVec1 () const {
            return rVec1_;
        }
        Vector<PType> GetVec2 () const {
            return rVec2_;
        }
        Vector<PType> GetVec3 () const {
            return rVec3_;
        }

        PType MaxCoord () {
            return (std::max ({(const PType) std::abs(rVec1_.GetPointX()), (const PType) std::abs(rVec1_.GetPointY()), (const PType) std::abs(rVec1_.GetPointZ()), 
                              (const PType) std::abs(rVec2_.GetPointX()), (const PType) std::abs(rVec2_.GetPointY()), (const PType) std::abs(rVec2_.GetPointZ()), 
                              (const PType) std::abs(rVec3_.GetPointX()), (const PType) std::abs(rVec3_.GetPointY()), (const PType) std::abs(rVec3_.GetPointZ())}));
        }
    };
    //##############################################################################
    //                         TRIANGLE-OVERLOAD PART
    //##############################################################################
    template <typename PType> 
    std::istream &operator >> (std::istream &in, Triangle<PType> &triangle) {

        Vector<PType> vec1;
        Vector<PType> vec2;
        Vector<PType> vec3;

        in >> vec1 >> vec2 >> vec3;

        triangle.SetVec1(vec1); 
        triangle.SetVec2(vec2); 
        triangle.SetVec3(vec3);

        return in;
    }
    template <typename PType> 
    std::ostream &operator << (std::ostream &out, const Triangle<PType> &triangle) {

        out << "First: " << std::endl << triangle.GetVec1() << std::endl;
        out << "Second: " << std::endl << triangle.GetVec2() << std::endl;
        out << "Third: " << std::endl << triangle.GetVec3() << std::endl;

        return out;
    }
    //##############################################################################
    //                         OCTREE-CLASS PART
    //##############################################################################
    template <typename PType>
    
}


//##############################################################################
//                         IMPLEMENTATION PART
//##############################################################################
template <typename PType> PType GetTriangles (objects::Triangle<PType>* Triangles, int number);
template <typename PType> void DumpTriangles (objects::Triangle<PType>* Triangles, int number);
template <typename PType> long long IntersectCount ();
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
template <typename PType>
long long IntersectCount () {

    int number = 0;
    std::cin >> number;
    assert (std::cin.good());

    objects::Triangle<PType>* Triangles = new objects::Triangle<PType> [number];

    PType max = GetTriangles (Triangles, number);
    DumpTriangles (Triangles, number);

    std::cout << "Max coord is " << max << std::endl;

    return 10;   
}
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
template <typename PType>
PType GetTriangles (objects::Triangle<PType>* Triangles, int number) {

    assert (Triangles);
    PType max = 0;

    for (int index = 0; index < number; ++index) { 

        std::cin >> Triangles[index];
        assert (std::cin.good());

        if (Triangles[index].MaxCoord() > max)
            max = Triangles[index].MaxCoord();
        
        assert(std::cin.good());
    }
    
    return max;
}
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
template <typename PType>
void DumpTriangles (objects::Triangle<PType>* Triangles, int number) {

    assert (Triangles);

    for (int index = 0; index < number; ++index) { 

        std::cout << "\t\t\t####### " << index + 1 << std::endl;
        std::cout << Triangles[index];
        
    }
}