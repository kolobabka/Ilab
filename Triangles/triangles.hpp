#include <iostream>
#include <new>
#include <iterator>
#include <cassert>
#include <algorithm>
#include <list>
#include <cmath>
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
namespace objects {
    
    //##############################################################################
    //                              VECTOR-CLASS PART
    //############################################################################## 

    template <typename PType> class Vector {

    private:
        

        PType points_[3]{};
    
    public:
        Vector (PType x = 0, PType y = 0, PType z = 0) {

            points_[0] = x;
            points_[1] = y;
            points_[2] = z;
        }

        void SetPoint (PType point, int num) {

            points_[num] = point;
        }

        PType GetPoint (int num) const {

            return points_[num];
        }
    
        Vector<PType> &operator += (const Vector<PType>& vec) {

            for (int i = 0; i < 3; ++i)
                points_[i] += vec.points_[i];

            return *this;
        }
        Vector<PType> &operator -= (const Vector<PType>& vec) {

            for (int i = 0; i < 3; ++i)
                points_[i] -= vec.points_[i];

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

        point.SetPoint(x, 0); 
        point.SetPoint(y, 1); 
        point.SetPoint(z, 2);

        return in;
    }
    template <typename PType>
    std::ostream &operator << (std::ostream &out, const Vector<PType> &point) {

        out << "X = " << point.GetPoint(0) << std::endl; 
        out << "Y = " << point.GetPoint(1) << std::endl; 
        out << "Z = " << point.GetPoint(2) << std::endl; 

        return out;
    }
    template <typename PType>
    Vector<PType> operator + (const Vector<PType>& vec1, const Vector<PType>& vec2) {
            
        PType x = vec1.GetPoint(0) + vec2.GetPoint(0);
        PType y = vec1.GetPoint(1) + vec2.GetPoint(1);
        PType z = vec1.GetPoint(2) + vec2.GetPoint(2);

        return Vector<PType> (x, y, z);
    }
    template <typename PType>
    Vector<PType> operator - (const Vector<PType>& vec1, const Vector<PType>& vec2) {
            
        PType x = vec1.GetPoint(0) - vec2.GetPoint(0);
        PType y = vec1.GetPoint(1) - vec2.GetPoint(1);
        PType z = vec1.GetPoint(2) - vec2.GetPoint(2);

        return Vector<PType> (x, y, z);
    }
    template <typename PType>
    Vector<PType> operator * (const Vector<PType>& vec1, const float num) {
            
        PType x = vec1.GetPoint(0) * num;
        PType y = vec1.GetPoint(1) * num;
        PType z = vec1.GetPoint(2) * num;

        return Vector<PType> (x, y, z);
    }
    template <typename PType>
    Vector<PType> operator * (const float num, const Vector<PType>& vec1) {
            
        PType x = vec1.GetPoint(0) * num;
        PType y = vec1.GetPoint(1) * num;
        PType z = vec1.GetPoint(2) * num;

        return Vector<PType> (x, y, z);
    }
    template <typename PType>
    Vector<PType> operator/ (const Vector<PType>& vec1, const float num) {
            
        PType x = vec1.GetPoint(0) / num;
        PType y = vec1.GetPoint(1) / num;
        PType z = vec1.GetPoint(2) / num;

        return Vector<PType> (x, y, z);
    }
    template <typename PType>
    Vector<PType> operator - (const Vector<PType>& vec1) {
            
        PType x = -vec1.GetPoint(0);
        PType y = -vec1.GetPoint(1);
        PType z = -vec1.GetPoint(2);

        return Vector<PType> (x, y, z);
    }
    template <typename PType>
    PType ScalarProduct (const Vector<PType>& vec1, const Vector<PType>& vec2) {

        return ((vec1.GetPoint(0) * vec2.GetPoint(0)) + 
            (vec1.GetPoint(1) * vec2.GetPoint(1)) +
            (vec1.GetPoint(2) * vec2.GetPoint(2)));
    }

    //##############################################################################
    //                         TRIANGLE-CLASS PART
    //##############################################################################
    template <typename PType> class Triangle {

    private:
        Vector<PType> rVec1_;
        Vector<PType> rVec2_;
        Vector<PType> rVec3_;

        Vector<PType> rVecs_[3]{};

    public:
        
        void SetVec (Vector<PType> &vec, int num) { 
            if (num < 0 || num >= 3) {

                std::cout << "Wrong number!" << std::endl;
                return;
            }
            rVecs_[num] = vec;
        }

        Vector<PType> GetVec (int num) const{ 
            if (num < 0 || num >= 3) {

                std::cout << "Wrong number!" << std::endl;
                return rVecs_[0];
            }
            return rVecs_[num];
        }

        PType MaxCoord () {

            return (std::max ({(const PType) std::abs(rVecs_[0].GetPoint(0)), (const PType) std::abs(rVecs_[0].GetPoint(1)), (const PType) std::abs(rVecs_[0].GetPoint(2)), 
                              (const PType) std::abs(rVecs_[1].GetPoint(0)), (const PType) std::abs(rVecs_[1].GetPoint(1)), (const PType) std::abs(rVecs_[1].GetPoint(2)), 
                              (const PType) std::abs(rVecs_[2].GetPoint(0)), (const PType) std::abs(rVecs_[2].GetPoint(1)), (const PType) std::abs(rVecs_[2].GetPoint(2))}));
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

        triangle.SetVec(vec1, 0); 
        triangle.SetVec(vec2, 1); 
        triangle.SetVec(vec3, 2);

        return in;
    }
    template <typename PType> 
    std::ostream &operator << (std::ostream &out, const Triangle<PType> &triangle) {

        out << "First: " << std::endl << triangle.GetVec(0) << std::endl;
        out << "Second: " << std::endl << triangle.GetVec(1) << std::endl;
        out << "Third: " << std::endl << triangle.GetVec(2) << std::endl;

        return out;
    }
}
    //##############################################################################
    //                         OCTREE-CLASS PART
    //##############################################################################  
namespace tree {

    template <typename PType> struct Cube {

        objects::Vector<PType> rMinVec_;
        objects::Vector<PType> rMaxVec_;

    public:
        Cube (objects::Vector<PType>& rMinVec, objects::Vector<PType>& rMaxVec) :

            rMinVec_(rMinVec),
            rMaxVec_(rMaxVec) {};
    };

    template <typename PType> struct Octree {

        Octree* childs_[8]{};
        
        std::list<objects::Triangle<PType>> Triangles_;
        Cube<PType> space_;

    public:
        Octree (objects::Vector<PType> rMinVecs, objects::Vector<PType> rMaxVecs) :

            space_{rMinVecs, rMaxVecs} {};

        ~Octree () {};
    };
        
}


//##############################################################################
//                         IMPLEMENTATION PART
//##############################################################################
template <typename PType>
void AddTriangle (tree::Octree<PType>& octree, typename std::list<objects::Triangle<PType>>::iterator Tr, int whereIs);
template <typename PType> PType GetTriangles (tree::Octree<PType> &octree, int number);
template <typename PType> void DumpTree (tree::Octree<PType> &octree);
template <typename PType> long long IntersectCount ();
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
template <typename PType>
int TrWhereIs (objects::Triangle<PType> &triangle, tree::Cube<PType> &space) {

    PType center = (space.rMinVec_.GetPoint(0) + space.rMaxVec_.GetPoint(0)) / 2; 

    int whereis[3]{};

    for (int i = 0; i < 3; ++i) {

        for (int j = 0; j < 3; ++j) {

            if (triangle.GetVec(i).GetPoint(j) > center)
                whereis[i] |= 1 << j;
            
            else if (triangle.GetVec(i).GetPoint(j) == center)
                return -1;
        }

        if (i) 
            if (whereis[i] != whereis[i - 1])  
                return -1;
    }

    return whereis[0];    
}
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
template <typename PType>
void DivideSpace (tree::Octree<PType>& octree) {

    if (octree.Triangles_.size() <= 2)
        return;

    using ListIt = typename std::list<objects::Triangle<PType>>::iterator;

    ListIt listCur = octree.Triangles_.begin();

    ListIt listEnd = octree.Triangles_.end();

    while (listCur != listEnd) {

        ListIt listNext = listCur;
        ++listNext;

        int whereIs = TrWhereIs (*listCur, octree.space_);  
    
        if (whereIs == -1) {

            ++listCur;
            continue;  
        }

        AddTriangle (octree, listCur, whereIs);
        octree.Triangles_.erase(listCur);

        listCur = listNext;
    }

    for (int index = 0; index < 8; ++index) {
        
        if (octree.childs_[index])
            DivideSpace (*(octree.childs_[index]));
    }
}
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
template <typename PType>
void AddTriangle (tree::Octree<PType>& octree, typename std::list<objects::Triangle<PType>>::iterator Tr, int whereIs) {

    if (octree.childs_[whereIs] == nullptr) 
        octree.childs_[whereIs] = new tree::Octree<PType>{{0,0,0}, {0,0,0}};
    
    tree::Octree<PType>* child = octree.childs_[whereIs];

    child->Triangles_.push_back(*Tr);
    child->space_.rMinVec_ = octree.space_.rMinVec_;
    child->space_.rMaxVec_ = octree.space_.rMaxVec_;

    for (int index = 0; index < 3; ++index) {

        if (((whereIs >> index) & 1u)) 
            child->space_.rMinVec_.SetPoint((octree.space_.rMinVec_.GetPoint(index) + octree.space_.rMaxVec_.GetPoint(index)) / 2, index);
        
        if (!((whereIs >> index) & 1u))
            child->space_.rMaxVec_.SetPoint((octree.space_.rMinVec_.GetPoint(index) + octree.space_.rMaxVec_.GetPoint(index)) / 2, index);
    }
}
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
template <typename PType>
long long IntersectCount () {

    int number = 0;
    std::cin >> number;
    assert (std::cin.good());

    tree::Octree<PType> octree{{0,0,0}, {0,0,0}};

    GetTriangles (octree, number);

    objects::Vector<PType> vexxx{-4, -4, -4};
    objects::Vector<PType> veccc{4, 4, 4};

    tree::Cube<PType> cube{vexxx, veccc};
    

    DivideSpace (octree);

    std::cout << std::endl;

    DumpTree (octree);

    // DeleteTree (octree);
    
    std::cout << std::endl;

    return 10;   
}
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
template <typename PType>
PType GetTriangles (tree::Octree<PType> &octree, int number) {

    PType max = 0;

    for (int index = 0; index < number; ++index) { 

        objects::Triangle<PType> triangle;

        std::cin >> triangle;
        assert (std::cin.good());

        octree.Triangles_.push_back(triangle);

        if (triangle.MaxCoord() > max)
            max = triangle.MaxCoord();
    }

    for (int i = 0; i < 3; ++i) {

        octree.space_.rMaxVec_.SetPoint(max, i);
        octree.space_.rMinVec_.SetPoint(-max, i);
    }
    
    return max;
}
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
template <typename PType>
void DumpTree (tree::Octree<PType> &octree) {

    std::cout << "Cube :" << std::endl;
    std::cout << "Max: " << std::endl;
    std::cout << octree.space_.rMaxVec_;
    std::cout << "Min: " << std::endl;
    std::cout << octree.space_.rMinVec_;

    for (auto v : octree.Triangles_)
        std::cout << v;
    
    for (int i = 0; i < 8; ++i) {

        if (octree.childs_[i])
            DumpTree (*(octree.childs_[i]));
    }
}