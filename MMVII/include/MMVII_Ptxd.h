#ifndef  _MMVII_Ptxd_H_
#define  _MMVII_Ptxd_H_
namespace MMVII
{


/** \file MMVII_Ptxd.h
    \brief Basic N-dimensionnal point facilities

   Don't know exactly where we go ...  Probably will contain :

      - point in N dim template
      - specialization to 1,2,3,4  dims
      - dynamic dyn
      - boxes on others "small" connected classes

*/


template <class Type,const int Dim> class cPtxd;

template <class Type> class cDenseMatrix;
template <class Type> class cDenseVect;


///  template class for Points


template <class Type,const int Dim> class cPtxd
{
    public :
       typedef typename  tNumTrait<Type>::tBig  tBigNum ;

       static const int TheDim = Dim;
       /// Maybe some function will require generic access to data
       Type * PtRawData() {return mCoords;}
       const Type * PtRawData() const {return mCoords;}

       /// Safe acces to generik data
       Type & operator[] (int aK) 
       {
          MMVII_INTERNAL_ASSERT_tiny((aK>=0) && (aK<Dim),"Bad point access");
          return mCoords[aK];
       }
       /// (const variant) Safe acces to generik data
       const Type & operator[] (int aK)  const
       {
          MMVII_INTERNAL_ASSERT_tiny((aK>=0) && (aK<Dim),"Bad point access");
          return mCoords[aK];
       }

       /// Some function requires default constructor (serialization ?)
       cPtxd() {}

       /* I would prefer not inline but : cannot make work explicit instance of a 
          a specific method,  and explicit of the whole class create problem
         with the static asser ; 
       */
       // static cPtxd<Type,Dim> T_PCste(const Type & aVal) ;

       /// Initialisation with constants
       static cPtxd<Type,Dim>  PCste(const Type & aVal) ;
       /// Initialisation random
       static cPtxd<Type,Dim>  PRand();
       /// Initialisation random
       static cPtxd<Type,Dim>  PRandC();
       /// Initialisation random VUnit
       static cPtxd<Type,Dim>  PRandUnit();
       /// Initialisation random VUnit not too close to P1
       static cPtxd<Type,Dim>  PRandUnitDiff(const cPtxd<Type,Dim>&,const Type &aDist = 1e-2);

        static cPtxd<Type,Dim> Col(const cDenseMatrix<Type>&,int aCol);  ///< Init with colum of matrix
        static cPtxd<Type,Dim> Line(int aLine,const cDenseMatrix<Type>&); ///< Init with line of matrix
        static cPtxd<Type,Dim> FromVect(const cDenseVect<Type>&); ///< Init with line of matrix
        static cPtxd<Type,Dim> FromStdVector(const std::vector<Type>&); ///< Init with line of matrix

       /// Contructor for 1 dim point, statically checked
       explicit cPtxd(const Type & x) :  mCoords{x} {static_assert(Dim==1,"bad dim in cPtxd initializer");}
       /// Contructor for 2 dim point, statically checked
       cPtxd(const Type & x,const Type &y) :  mCoords{x,y} {static_assert(Dim==2,"bad dim in cPtxd initializer");}
       /// Contructor for 3 dim point, statically checked
       cPtxd(const Type & x,const Type &y,const Type &z) :  mCoords{x,y,z} {static_assert(Dim==3,"bad dim in cPtxd initializer");}
       /// Contructor for 4 dim point, statically checked
       cPtxd(const Type & x,const Type &y,const Type &z,const Type &t) :  mCoords{x,y,z,t} {static_assert(Dim==4,"bad dim in cPtxd initializer");}


        inline Type & x()             {static_assert(Dim>=1,"bad dim in cPtxd initializer");return mCoords[0];}
        inline const Type & x() const {static_assert(Dim>=1,"bad dim in cPtxd initializer");return mCoords[0];}

        inline Type & y()             {static_assert(Dim>=2,"bad dim in cPtxd initializer");return mCoords[1];}
        inline const Type & y() const {static_assert(Dim>=2,"bad dim in cPtxd initializer");return mCoords[1];}

        inline Type & z()             {static_assert(Dim>=3,"bad dim in cPtxd initializer");return mCoords[2];}
        inline const Type & z() const {static_assert(Dim>=3,"bad dim in cPtxd initializer");return mCoords[2];}

        inline Type & t()             {static_assert(Dim>=4,"bad dim in cPtxd initializer");return mCoords[3];}
        inline const Type & t() const {static_assert(Dim>=4,"bad dim in cPtxd initializer");return mCoords[3];}

        cDenseVect<Type> ToVect() const; ///< conversion
        std::vector<Type> ToStdVector() const; ///< conversion

    protected :
       Type mCoords[Dim];
};

template <class Type> inline bool IsNotNull (const cPtxd<Type,2> & aP1) { return (aP1.x() !=0) || (aP1.y()!=0);}

#if (The_MMVII_DebugLevel>=The_MMVII_DebugLevel_InternalError_tiny )
template <class Type> inline void AssertNonNul(const cPtxd<Type,2> &aP1) 
{
   MMVII_INTERNAL_ASSERT_tiny(IsNotNull(aP1),"Unexpected null point");
}
#else
#define AssertNonNul(aP) {} 
#endif


///  operator + on points
template <class Type> inline cPtxd<Type,1> operator + (const cPtxd<Type,1> & aP1,const cPtxd<Type,1> & aP2) 
{ return cPtxd<Type,1>(aP1.x() + aP2.x()); }
template <class Type> inline cPtxd<Type,2> operator + (const cPtxd<Type,2> & aP1,const cPtxd<Type,2> & aP2) 
{ return cPtxd<Type,2>(aP1.x() + aP2.x(),aP1.y() + aP2.y()); }
template <class Type> inline cPtxd<Type,3> operator + (const cPtxd<Type,3> & aP1,const cPtxd<Type,3> & aP2) 
{ return cPtxd<Type,3>(aP1.x() + aP2.x(),aP1.y() + aP2.y(),aP1.z()+aP2.z()); }
template <class Type> inline cPtxd<Type,4> operator + (const cPtxd<Type,4> & aP1,const cPtxd<Type,4> & aP2) 
{ return cPtxd<Type,4>(aP1.x() + aP2.x(),aP1.y() + aP2.y(),aP1.z()+aP2.z(),aP1.t()+aP2.t()); }

///  binary operator - on points
template <class Type> inline cPtxd<Type,1> operator - (const cPtxd<Type,1> & aP1,const cPtxd<Type,1> & aP2) 
{ return cPtxd<Type,1>(aP1.x() - aP2.x()); }
template <class Type> inline cPtxd<Type,2> operator - (const cPtxd<Type,2> & aP1,const cPtxd<Type,2> & aP2) 
{ return cPtxd<Type,2>(aP1.x() - aP2.x(),aP1.y() - aP2.y()); }
template <class Type> inline cPtxd<Type,3> operator - (const cPtxd<Type,3> & aP1,const cPtxd<Type,3> & aP2) 
{ return cPtxd<Type,3>(aP1.x() - aP2.x(),aP1.y() - aP2.y(),aP1.z()-aP2.z()); }
template <class Type> inline cPtxd<Type,4> operator - (const cPtxd<Type,4> & aP1,const cPtxd<Type,4> & aP2) 
{ return cPtxd<Type,4>(aP1.x() - aP2.x(),aP1.y() - aP2.y(),aP1.z()-aP2.z(),aP1.t()-aP2.t()); }

///  MulCByC multiplication coordinates by coordinates
template <class Type> inline cPtxd<Type,1>  MulCByC (const cPtxd<Type,1> & aP1,const cPtxd<Type,1> & aP2) 
{ return cPtxd<Type,1>(aP1.x() * aP2.x()); }
template <class Type> inline cPtxd<Type,2>  MulCByC (const cPtxd<Type,2> & aP1,const cPtxd<Type,2> & aP2) 
{ return cPtxd<Type,2>(aP1.x() * aP2.x(),aP1.y() * aP2.y()); }
template <class Type> inline cPtxd<Type,3> MulCByC (const cPtxd<Type,3> & aP1,const cPtxd<Type,3> & aP2) 
{ return cPtxd<Type,3>(aP1.x() * aP2.x(),aP1.y() * aP2.y(),aP1.z()*aP2.z()); }

///  DivCByC division coordinates by coordinates !! => INT Division; see also RDivCByC
template <class Type> inline cPtxd<Type,1>  DivCByC (const cPtxd<Type,1> & aP1,const cPtxd<Type,1> & aP2) 
{ return cPtxd<Type,1>(aP1.x() / aP2.x()); }
template <class Type> inline cPtxd<Type,2>  DivCByC (const cPtxd<Type,2> & aP1,const cPtxd<Type,2> & aP2) 
{ return cPtxd<Type,2>(aP1.x() / aP2.x(),aP1.y() / aP2.y()); }
template <class Type> inline cPtxd<Type,3> DivCByC (const cPtxd<Type,3> & aP1,const cPtxd<Type,3> & aP2) 
{ return cPtxd<Type,3>(aP1.x() / aP2.x(),aP1.y() / aP2.y(),aP1.z()/aP2.z()); }


/// Some time int division is not what is wanted !!
template <class T,const int Dim> inline cPtxd<double,Dim> RDivCByC(const cPtxd<T,Dim> & aP1,const cPtxd<T,Dim> & aP2) 
{
   return DivCByC(ToR(aP1),ToR(aP2));
}

///  unary operator - on points
template <class Type> inline cPtxd<Type,1> operator - (const cPtxd<Type,1> & aP) {return  cPtxd<Type,1>(-aP.x());}
template <class Type> inline cPtxd<Type,2> operator - (const cPtxd<Type,2> & aP) {return  cPtxd<Type,2>(-aP.x(),-aP.y());}
template <class Type> inline cPtxd<Type,3> operator - (const cPtxd<Type,3> & aP) {return  cPtxd<Type,3>(-aP.x(),-aP.y(),-aP.z());}


///  operator * scalar - points
template <class Type> inline cPtxd<Type,1> operator * (const Type & aVal ,const cPtxd<Type,1> & aP) 
{return  cPtxd<Type,1>(aP.x()*aVal);}
template <class Type> inline cPtxd<Type,1> operator * (const cPtxd<Type,1> & aP,const Type & aVal) 
{return  cPtxd<Type,1>(aP.x()*aVal);}
template <class Type> inline cPtxd<Type,2> operator * (const Type & aVal ,const cPtxd<Type,2> & aP) 
{return  cPtxd<Type,2>(aP.x()*aVal,aP.y()*aVal);}
template <class Type> inline cPtxd<Type,2> operator * (const cPtxd<Type,2> & aP,const Type & aVal) 
{return  cPtxd<Type,2>(aP.x()*aVal,aP.y()*aVal);}
template <class Type> inline cPtxd<Type,3> operator * (const Type & aVal ,const cPtxd<Type,3> & aP) 
{return  cPtxd<Type,3>(aP.x()*aVal,aP.y()*aVal,aP.z()*aVal);}
template <class Type> inline cPtxd<Type,3> operator * (const cPtxd<Type,3> & aP,const Type & aVal) 
{return  cPtxd<Type,3>(aP.x()*aVal,aP.y()*aVal,aP.z()*aVal);}

///  operator /  points-scalar
template <class Type> inline cPtxd<Type,1> operator / (const cPtxd<Type,1> & aP,const Type & aVal) 
{
   MMVII_INTERNAL_ASSERT_NotNul(aVal);
   return  cPtxd<Type,1>(aP.x()/aVal);
}
template <class Type> inline cPtxd<Type,2> operator / (const cPtxd<Type,2> & aP,const Type & aVal) 
{
    MMVII_INTERNAL_ASSERT_NotNul(aVal);
    return  cPtxd<Type,2>(aP.x()/aVal,aP.y()/aVal);
}
template <class Type> inline cPtxd<Type,3> operator / (const cPtxd<Type,3> & aP,const Type & aVal) 
{
    MMVII_INTERNAL_ASSERT_NotNul(aVal);
    return  cPtxd<Type,3>(aP.x()/aVal,aP.y()/aVal,aP.z()/aVal);
}
template <class Type> inline cPtxd<Type,4> operator / (const cPtxd<Type,4> & aP,const Type & aVal) 
{
    MMVII_INTERNAL_ASSERT_NotNul(aVal);
    return  cPtxd<Type,4>(aP.x()/aVal,aP.y()/aVal,aP.z()/aVal,aP.t()/aVal);
}


///  Norms on points
template <class T,const int Dim> double NormK(const cPtxd<T,Dim> & aP,double anExp) ;
template <class T,const int Dim> T Norm1(const cPtxd<T,Dim> & aP);
template <class T,const int Dim> T NormInf(const cPtxd<T,Dim> & aP);
template <class T,const int Dim> double Norm2(const cPtxd<T,Dim> & aP);

template <class T,const int Dim> typename  tNumTrait<T>::tBig Scal(const cPtxd<T,Dim> &,const cPtxd<T,Dim> &);

template <class T,const int Dim> T Cos(const cPtxd<T,Dim> &,const cPtxd<T,Dim> &);


/*
template <class T,const int Dim> inline T Norm2(const cPtxd<T,Dim> & aP) {return std::sqrt(SqN2(aP));}
template <class T> inline T Norm1(const cPtxd<T,1> & aP) {return std::abs(aP.x());}
template <class T> inline T Norm1(const cPtxd<T,2> & aP) {return std::abs(aP.x())+std::abs(aP.y());}
template <class T> inline T NormInf(const cPtxd<T,1> & aP) {return std::abs(aP.x());}
template <class T> inline T NormInf(const cPtxd<T,2> & aP) {return std::max(std::abs(aP.x()),std::abs(aP.y()));}
// template <class T> inline T SqN2(const cPtxd<T,1> & aP) {return Square(aP.x());}
*/
   /// Currently, the L2 norm is used for comparaison, no need to extract square root
template <class T> inline T SqN2(const cPtxd<T,1> & aP) {return Square(aP.x());}
template <class T> inline T SqN2(const cPtxd<T,2> & aP) {return Square(aP.x())+Square(aP.y());}
/// Sort vector by norm, typically dont need to compute square root
template <class Type,const int Dim> bool CmpN2(const cPtxd<Type,Dim> &aP1,const  cPtxd<Type,Dim> & aP2) 
{
    return SqN2(aP1) < SqN2(aP2);
}





///  operator == on points
template <class Type> inline bool operator == (const cPtxd<Type,1> & aP1,const cPtxd<Type,1> & aP2) 
{return  (aP1.x()==aP2.x());}
template <class Type> inline bool operator == (const cPtxd<Type,2> & aP1,const cPtxd<Type,2> & aP2) 
{return  (aP1.x()==aP2.x()) && (aP1.y()==aP2.y());}
template <class Type> inline bool operator == (const cPtxd<Type,3> & aP1,const cPtxd<Type,3> & aP2) 
{return  (aP1.x()==aP2.x()) && (aP1.y()==aP2.y()) && (aP1.z()==aP2.z());}

///  operator != on points
template <class Type> inline bool operator != (const cPtxd<Type,1> & aP1,const cPtxd<Type,1> & aP2) 
{return  (aP1.x()!=aP2.x());}
template <class Type> inline bool operator != (const cPtxd<Type,2> & aP1,const cPtxd<Type,2> & aP2) 
{return  (aP1.x()!=aP2.x()) || (aP1.y()!=aP2.y());}
template <class Type> inline bool operator != (const cPtxd<Type,3> & aP1,const cPtxd<Type,3> & aP2) 
{return  (aP1.x()!=aP2.x()) || (aP1.y()!=aP2.y()) ||  (aP1.z()!=aP2.z());}

///  SupEq  :  P1.k() >= P2.k() for all coordinates
template <class Type> inline bool SupEq  (const cPtxd<Type,1> & aP1,const cPtxd<Type,1> & aP2) 
{return  (aP1.x()>=aP2.x());}
template <class Type> inline bool SupEq  (const cPtxd<Type,2> & aP1,const cPtxd<Type,2> & aP2) 
{return  (aP1.x()>=aP2.x()) && (aP1.y()>=aP2.y());}
template <class Type> inline bool SupEq  (const cPtxd<Type,3> & aP1,const cPtxd<Type,3> & aP2) 
{return  (aP1.x()>=aP2.x()) && (aP1.y()>=aP2.y()) && (aP1.z()>=aP2.z());}


/// PtSupEq   : smallest point being SupEq to
template <class Type> inline cPtxd<Type,1> PtSupEq  (const cPtxd<Type,1> & aP1,const cPtxd<Type,1> & aP2) 
{ return cPtxd<Type,1> (std::max(aP1.x(),aP2.x())); }
template <class Type> inline cPtxd<Type,2> PtSupEq  (const cPtxd<Type,2> & aP1,const cPtxd<Type,2> & aP2) 
{ return cPtxd<Type,2> (std::max(aP1.x(),aP2.x()),std::max(aP1.y(),aP2.y())); }
template <class Type> inline cPtxd<Type,3> PtSupEq  (const cPtxd<Type,3> & aP1,const cPtxd<Type,3> & aP2) 
{ return cPtxd<Type,3> (std::max(aP1.x(),aP2.x()),std::max(aP1.y(),aP2.y()),std::max(aP1.z(),aP2.z())); }

template <class TypePt> void SetSupEq(TypePt & aP1,const TypePt & aP2) {aP1 = PtSupEq(aP1,aP2);}

/// PtInfEq   : bigeest point being InfEq to
template <class Type> inline cPtxd<Type,1> PtInfEq  (const cPtxd<Type,1> & aP1,const cPtxd<Type,1> & aP2) 
{ return cPtxd<Type,1> (std::min(aP1.x(),aP2.x())); }
template <class Type> inline cPtxd<Type,2> PtInfEq  (const cPtxd<Type,2> & aP1,const cPtxd<Type,2> & aP2) 
{ return cPtxd<Type,2> (std::min(aP1.x(),aP2.x()),std::min(aP1.y(),aP2.y())); }
template <class Type> inline cPtxd<Type,3> PtInfEq  (const cPtxd<Type,3> & aP1,const cPtxd<Type,3> & aP2) 
{ return cPtxd<Type,3> (std::min(aP1.x(),aP2.x()),std::min(aP1.y(),aP2.y()),std::min(aP1.z(),aP2.z())); }

template <class TypePt> void SetInfEq(TypePt & aP1,const TypePt & aP2) {aP1 = PtInfEq(aP1,aP2);}

template <class Type> inline cPtxd<Type,2> Transp  (const cPtxd<Type,2> & aP) {return  cPtxd<Type,2>(aP.y(),aP.x());}

///  InfStr  :  P1.k() < P2.k() for all coordinates
template <class Type> inline bool InfStr  (const cPtxd<Type,1> & aP1,const cPtxd<Type,1> & aP2) 
{return  (aP1.x()<aP2.x());}
template <class Type> inline bool InfStr  (const cPtxd<Type,2> & aP1,const cPtxd<Type,2> & aP2) 
{return  (aP1.x()<aP2.x()) && (aP1.y()<aP2.y());}
template <class Type> inline bool InfStr  (const cPtxd<Type,3> & aP1,const cPtxd<Type,3> & aP2) 
{return  (aP1.x()<aP2.x()) && (aP1.y()<aP2.y()) && (aP1.z()<aP2.z());}

/**  PtInfSTr : bigets point beg=ing InfStr (definition valide for integer types) 
  Warn non symetric function;  strictness is relative to P2, not P1 */
template <class Type> inline cPtxd<Type,1> PtInfStr  (const cPtxd<Type,1> & aP1,const cPtxd<Type,1> & aP2) 
{ return cPtxd<Type,1> (std::min(aP1.x(),aP2.x()-1)); }
template <class Type> inline cPtxd<Type,2> PtInfStr  (const cPtxd<Type,2> & aP1,const cPtxd<Type,2> & aP2) 
{ return cPtxd<Type,2> (std::min(aP1.x(),aP2.x()-1),std::min(aP1.y(),aP2.y()-1)); }
template <class Type> inline cPtxd<Type,3> PtInfStr  (const cPtxd<Type,3> & aP1,const cPtxd<Type,3> & aP2) 
{ return cPtxd<Type,3> (std::min(aP1.x(),aP2.x()-1),std::min(aP1.y(),aP2.y()-1),std::min(aP1.z(),aP2.z()-1)); }


/// InfEq  :  P1.k() <= P2.k() for all coordinates
template <class Type> inline bool InfEq  (const cPtxd<Type,1> & aP1,const cPtxd<Type,1> & aP2) 
{return  (aP1.x()<=aP2.x());}
template <class Type> inline bool InfEq  (const cPtxd<Type,2> & aP1,const cPtxd<Type,2> & aP2) 
{return  (aP1.x()<=aP2.x()) && (aP1.y()<=aP2.y());}
template <class Type> inline bool InfEq  (const cPtxd<Type,3> & aP1,const cPtxd<Type,3> & aP2) 
{return  (aP1.x()<=aP2.x()) && (aP1.y()<=aP2.y()) && (aP1.z()<=aP2.z());}


template<class T,const int Dim> cPtxd<T,Dim>  VUnit(const cPtxd<T,Dim> & aP);
// template<const int Dim> cPtxd<tREAL8 ,Dim>  VUnit(const cPtxd<tREAL8 ,Dim> & aP);
// template<const int Dim> cPtxd<tREAL16,Dim>  VUnit(const cPtxd<tREAL16,Dim> & aP);


///  operator << 
template <class Type,const int Dim> std::ostream & operator << (std::ostream & OS,const cPtxd<Type,Dim> &aP);


    ///  1 dimension specializatio,
typedef cPtxd<double,1>  cPt1dr ;
typedef cPtxd<int,1>     cPt1di ;
typedef cPtxd<float,1>   cPt1df ;

    ///  2 dimension specialization
typedef cPtxd<double,2>  cPt2dr ;
typedef cPtxd<int,2>     cPt2di ;
typedef cPtxd<float,2>   cPt2df ;



template <class T,const int Dim> inline double RatioMax(const cPtxd<T,Dim> & aP1,const cPtxd<T,Dim> & aP2)
{
   return NormInf(RDivCByC(aP1,aP2));
}


// cPt2dr operator / (const cPt2dr &aP1,const cPt2dr & aP2) {return (aP1*conj(aP)

    ///  3 dimension specialization
typedef cPtxd<double,3>  cPt3dr ;
typedef cPtxd<int,3>     cPt3di ;
typedef cPtxd<float,3>   cPt3df ;

// Most frequent conversion
inline cPt2di ToI(const cPt2dr & aP) {return cPt2di(round_ni(aP.x()),round_ni(aP.y()));}
inline cPt2dr ToR(const cPt2di & aP) {return cPt2dr(aP.x(),aP.y());}

template <class Type,int Dim,int aKth> bool  CmpCoord(const cPtxd<Type,Dim> & aP1,const cPtxd<Type,Dim> & aP2)
{
   static_assert((aKth>=0) && (aKth<Dim),"CmpCoord");
   return aP1[aKth] < aP2[aKth];
}


/**  CByC operator, apply an operator coordinate by coordinate, first version with one points */
template <class Type,const int Dim,class TypeFctr>
cPtxd<Type,Dim> CByC1P
                (
                   const cPtxd<Type,Dim>  & aP1,
                   const TypeFctr &         aFctr
                )
{
    cPtxd<Type,Dim> aRes;
    for (int aK=0 ; aK<Dim ; aK++)
        aRes[aK] = aFctr(aP1[aK]);
    return aRes;
}

/** Idem CByC when we need to change & force to int the result */
template <class Type,const int Dim,class TypeFctr>
cPtxd<int,Dim>  ICByC1P
                (
                   const cPtxd<Type,Dim>  & aP1,
                   const TypeFctr &         aFctr
                )
{
    cPtxd<int,Dim> aRes;
    for (int aK=0 ; aK<Dim ; aK++)
        aRes[aK] = aFctr(aP1[aK]);
    return aRes;
}
template <class Type,const int Dim>  cPtxd<int,Dim> Pt_round_down(const cPtxd<Type,Dim>  aP);
template <class Type,const int Dim>  cPtxd<int,Dim> Pt_round_up(const cPtxd<Type,Dim>  aP);
template <class Type,const int Dim>  cPtxd<int,Dim> Pt_round_ni(const cPtxd<Type,Dim>  aP);
// template <class Type,const int Dim>  cPtxd<Type,Dim> PCste(const Type & aVal);


/**  CByC version with 2 points */
template <class Type,const int Dim,class TypeFctr>
cPtxd<Type,Dim> CByC2P
                (
                   const cPtxd<Type,Dim>  & aP1,
                   const cPtxd<Type,Dim>  & aP2,
                   const TypeFctr &         aFctr
                )
{
    cPtxd<Type,Dim> aRes;
    for (int aK=0 ; aK<Dim ; aK++)
        aRes[aK] = aFctr(aP1[aK],aP2[aK]);
    return aRes;
}

/// Number of pixel in square window
int NbPixVign(const int & aVign); 
/// Number of pixel in a non square window
template <const int Dim> int NbPixVign(const cPtxd<int,Dim> & aVign); 


/// Order coordinate so that it can define a box
template <class Type,const int Dim> void MakeBox(cPtxd<Type,Dim> & aP0,cPtxd<Type,Dim> & aP1)
{
    for (int aK=0 ; aK<Dim ; aK++)
        OrderMinMax(aP0[aK],aP1[aK]);
}

/// Return pixel between two radius, the order make them as sparse as possible (slow method in N^3) => To implement ???? No longer know what I wanted to do ???
//std::vector<cPt2di> SparsedVectOfRadius(const double & aR0,const double & aR1); // > R0 et <= R1
/// Implemented
std::vector<cPt2di> SortedVectOfRadius(const double & aR0,const double & aR1); // > R0 et <= R1


//  === TABULATION OF NEIGHBOORING

extern cPt2di  TAB4Corner[4] ; ///< {{1,1},{-1,1},{-1,-1},{1,-1}};



/**  Class for box, they are template as typically :
       - double will be used in geometric indexes QdTree or tiling
       - int will be used in bitmap manipulation
*/
template <class Type,const int Dim>  class cTplBox 
{
    public : 
        typedef Type                             tNum ;
        typedef typename  tNumTrait<Type>::tBig  tBigNum ;
        typedef cTplBox<Type,Dim>                tBox;
        typedef cPtxd<Type,Dim>                  tPt;
        typedef cPtxd<tBigNum,Dim>               tBigPt;

        cTplBox(const tPt & aP0,const tPt & aP1,bool AllowEmpty=false);
        cTplBox(const tPt & aSz,bool AllowEmpty=false); // Create a box with origin in 0,0,..
        static cTplBox Empty();
        


        const tPt & P0() const {return mP0;} ///< Origin of object
        const tPt & P1() const {return mP1;} ///< End of object
        const tPt & Sz() const {return mSz;} ///< Size of object

        const tBigNum & NbElem() const {return mNbElem;}  ///< Surface  / Volume

        //const tPt & SzCum() const; ///< Cumulated size, rather internal use

        // Boolean operators
           /// Is this point/pixel/voxel  inside
        bool Inside(const tPt & aP) const  {return SupEq(aP,mP0) && InfStr(aP,mP1);}
           /// Specialistion 1D
        bool Inside(const tNum & aX) const  
        {
           // static_assert(Dim==1,"Bas dim for integer access");
           return (aX>=mP0.x()) && (aX<mP1.x());
        }
        /// Return closest point inside the box
        tPt  Proj(const tPt & aP) const {return PtInfStr(PtSupEq(aP,mP0),mP1);}
        /// Are the two box equals
        bool operator == (const tBox & aR2) const ;
        /// Is  this included in aB
        bool  IncludedIn(const  tBox & aB)const;
        /// Sometime we need to represent the empty box explicitely
        bool IsEmpty() const;
        tBox   Translate(const tPt & aPt)const;

        // tBox Sup(const tBox & aBox)const;
        tBox Inter(const tBox & aBox)const; ///< Intersction handle empty case
        tBox Dilate(const tPt & aPt)const;  ///< Dilatation, as in morpho math : mP0-P mP1+P
        tBox Dilate(const Type & aVal)const;  ///< Dilatation with constant coordinate

        /// Assert that it is inside
        template <class TypeIndex> void AssertInside(const TypeIndex & aP) const
        {
             MMVII_INTERNAL_ASSERT_tiny(Inside(aP),"Point out of image");
        }
        void AssertSameArea(const tBox & aV) const; ///<  Assert object are identic
        void AssertSameSz(const   tBox & aV) const;   ///<  Check only size

        //  ---  object generation inside box ----------------

        tPt  FromNormaliseCoord(const cPtxd<double,Dim> &) const;  ///< [0,1] * => Rect
        cPtxd<double,Dim> ToNormaliseCoord(const tPt & aP) const;  ///< Rect => [0,1] *

        static cPtxd<double,Dim>  RandomNormalised() ;     ///<  Random point in "hyper cube" [0,1] ^ Dim
        tPt   GeneratePointInside() const;   ///< Random point in integer rect
        tBox  GenerateRectInside(double aPowSize=1.0) const; ///< Hig Power generate "small" rect, never empty



    protected :
        tPt       mP0;         ///< "smallest"
        tPt       mP1;         ///< "highest"
        tPt       mSz;         ///<  Size
        tBigPt    mSzCum;      ///< Cumlated size : Cum[aK] = Cum[aK-1] * Sz[aK-1]
        tBigNum   mNbElem;     ///< Number of pixel = Cum[Dim-1]
    private :
};

typedef cTplBox<int,2>  cBox2di; 
typedef cTplBox<double,2>  cBox2dr; 
cBox2dr ToR(const cBox2di &);  ///< Basic conversion
cBox2di ToI(const cBox2dr &);  ///< Convert in englobing mode
cBox2dr operator * (const cBox2dr & aBox,double aScale); ///< just multiply each coord

template <class Type,const int Dim> std::ostream & operator << (std::ostream & OS,const cTplBox<Type,Dim> &aBox)
{ return  OS << "{" << aBox.P0() <<   " :: " << aBox.P1()<< "}"; }

/**  Class for computing box of a set of points by iteratively adding them.
     Is ok with empty case (!= cTplBox) 
     Can be converted to a "regular" box (cTplBox)
*/

template <class Type,const int Dim>  class cTplBoxOfPts
{
    public :
        typedef cPtxd<Type,Dim>                  tPt;

        cTplBoxOfPts();
        int NbPts() const;  ///< Use to check acces that are forbidden when empty
        const tPt & P0() const;
        const tPt & P1() const;
        cTplBox<Type,Dim> CurBox() const;

        void Add(const tPt &);
    private :
        int  mNbPts;  ///< Number of points, to check access
        tPt  mP0;
        tPt  mP1;
};




};

#endif  //  _MMVII_Ptxd_H_
