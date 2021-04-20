#ifndef  _MMVII_nums_H_
#define  _MMVII_nums_H_

namespace MMVII
{

template <class Type> bool ValidFloatValue(const Type & aV)
{
   // return ! (   ((boost::math::isnan)(aV)) ||   ((boost::math::isinf)(aV)));
   return (std::isfinite)(aV) ;
}
template <class Type> bool ValidInvertibleFloatValue(const Type & aV)
{
    return ValidFloatValue(aV) && (aV!=0.0);
}


/** \file MMVII_nums.h
    \brief some numerical function

*/

/* ================= Random generator  ======================= */

    // === Basic interface, global function but use C++11 modern
    // === generator. By default will be deterministic, 


double RandUnif_0_1(); ///<  Uniform distribution in 0-1
std::vector<double> VRandUnif_0_1(int aNb); ///<  Uniform distribution in 0-1
double RandUnif_C();   ///<  Uniform distribution in  -1 1
bool   HeadOrTail();   ///< 1/2 , french "Pile ou Face"
double RandUnif_N(int aN); ///< Uniform disrtibution in [0,N[ 
double RandUnif_C_NotNull(double aEps);   ///<  Uniform distribution in  -1 1, but abs > aEps

/** Class for mapping object R->R */
class cFctrRR
{  
   public :
      virtual  double F (double) const;  ///< Default return 1.0
      static cFctrRR  TheOne;  ///< the object return always 1
};
/// Random permutation , Higer Bias => Higer average rank
std::vector<int> RandPerm(int aN,cFctrRR & aBias =cFctrRR::TheOne);
/// Random subset K among  N  !! Higher bias => lower proba of selection
std::vector<int> RandSet(int aK,int aN,cFctrRR & aBias =cFctrRR::TheOne);
///  Random modification of K Value in a set of N elem
std::vector<int> RandNeighSet(int aK,int aN,const std::vector<int> & aSet);
/// Complement of aSet in [0,1...., N[    ;  ]]
std::vector<int> ComplemSet(int aN,const std::vector<int> & aSet);


/* ============ Definition of numerical type ================*/

typedef float       tREAL4;
typedef double      tREAL8;
typedef long double tREAL16;

typedef signed char  tINT1;
typedef signed short tINT2;
typedef signed int   tINT4;
typedef long int     tINT8;



typedef unsigned char  tU_INT1;
typedef unsigned short tU_INT2;
typedef unsigned int   tU_INT4;


typedef int    tStdInt;  ///< "natural" int
typedef double tStdDouble;  ///< "natural" int

/* ================= rounding  ======================= */

/// return the smallest integral value >= r
template<class Type> inline Type Tpl_round_up(tREAL8 r)
{
       Type i = (Type) r;
       return i + (i < r);
}
inline tINT4 round_up(tREAL8 r)  { return Tpl_round_up<tINT4>(r); }
inline tINT8 lround_up(tREAL8 r) { return Tpl_round_up<tINT8>(r); }


/// return the smallest integral value > r
template<class Type> inline Type Tpl_round_Uup(tREAL8 r)
{
       Type i = (Type) r;
       return i + (i <= r);
}
inline tINT4 round_Uup(tREAL8 r) { return Tpl_round_Uup<int>(r); }


/// return the highest integral value <= r
template<class Type> inline Type Tpl_round_down(tREAL8 r)
{
       Type i = (Type) r;
       return i - (i > r);
}
inline tINT4  round_down(tREAL8 r) { return Tpl_round_down<tINT4>(r); }
inline tINT8 lround_down(tREAL8 r) { return Tpl_round_down<tINT8>(r); }

/// return the highest integral value < r
template<class Type> inline Type Tpl_round_Ddown(tREAL8 r)
{
       Type i = (Type) r;
       return i - (i >= r);
}
inline tINT4 round_Ddown(tREAL8 r) { return Tpl_round_Ddown<tINT4>(r); }

/// return the integral value closest to r , if r = i +0.5 (i integer) return i+1
template<class Type> inline Type Tpl_round_ni(tREAL8 r)
{
       Type i = (Type) r;
       i -= (i > r);
       // return i+ ((i+0.5) <= r) ; =>  2i+1<2r  => i < 2*r-i-1
       return i+ ((i+0.5) <= r) ;
}

inline tINT4  round_ni(tREAL8 r) { return Tpl_round_ni<tINT4>(r); }
inline tINT8 lround_ni(tREAL8 r) { return Tpl_round_ni<tINT8>(r); }

tINT4 EmbeddedIntVal(tREAL8 r); ///< When a real value is used for embedding a int, check that value is really int and return it
bool  EmbeddedBoolVal(tREAL8 r); ///< When a real value is used for embedding a bool, check that value is really bool and return it
bool  EmbeddedBoolVal(int V); ///< When a int value is used for embedding a bool, check that value is 0 or 1

/*  ==============  Traits on numerical type, usable in template function ===================   */
/*   tNumTrait => class to be used                                                              */
/*  tBaseNumTrait, tElemNumTrait   : accessory classes                                          */

    // =========================================================
    //  tBaseNumTrait : Base trait, separate int and float
    // =========================================================

template <class Type> class tBaseNumTrait
{
    public :
        typedef tStdInt  tBase;
};
template <> class tBaseNumTrait<tStdInt>
{
    public :
 
        // For these type rounding mean something
        static int RoundDownToType(const double & aV) {return round_down(aV);}
        static int RoundNearestToType(const double & aV) {return round_ni(aV);}

        static bool IsInt() {return true;}
        typedef tStdInt  tBase;
        typedef tINT8    tBig;
};
template <> class tBaseNumTrait<tINT8>
{
    public :
        // For these type rounding mean something
        static tINT8 RoundDownToType(const double & aV) {return lround_down(aV);}
        static tINT8 RoundNearestToType(const double & aV) {return lround_ni(aV);}

        static bool IsInt() {return true;}
        typedef tINT8  tBase;
        typedef tINT8    tBig;
};
template <> class tBaseNumTrait<tStdDouble>
{
    public :
        // By default rounding has no meaning
        static double RoundDownToType(const double & aV) {return aV;}
        static double RoundNearestToType(const double & aV) {return aV;}

        static bool IsInt() {return false;}
        typedef tStdDouble  tBase;
        typedef tStdDouble  tBig;
};
template <> class tBaseNumTrait<tREAL16>
{
    public :
        static bool IsInt() {return false;}
        typedef tREAL16  tBase;
        typedef tREAL16  tBig;
};

    // ========================================================================
    //  tElemNumTrait : declare what must be specialized for each type
    // ========================================================================

template <class Type> class tElemNumTrait
{
    public :
};

      // Unsigned int 

template <> class tElemNumTrait<tU_INT1> : public tBaseNumTrait<tStdInt>
{
    public :
        static bool   Signed() {return false;}
        static eTyNums   TyNum() {return eTyNums::eTN_U_INT1;}
        typedef tREAL4   tFloatAssoc;
};
template <> class tElemNumTrait<tU_INT2> : public tBaseNumTrait<tStdInt>
{
    public :
        static bool   Signed() {return false;}
        static eTyNums   TyNum() {return eTyNums::eTN_U_INT2;}
        typedef tREAL4   tFloatAssoc;
};
template <> class tElemNumTrait<tU_INT4> : public tBaseNumTrait<tINT8>
{
    public :
        static bool   Signed() {return false;}
        static eTyNums   TyNum() {return eTyNums::eTN_U_INT4;}
        typedef tREAL8   tFloatAssoc;
};

      // Signed int 

template <> class tElemNumTrait<tINT1> : public tBaseNumTrait<tStdInt>
{
    public :
        static bool   Signed() {return true;}
        static eTyNums   TyNum() {return eTyNums::eTN_INT1;}
        typedef tREAL4   tFloatAssoc;
};
template <> class tElemNumTrait<tINT2> : public tBaseNumTrait<tStdInt>
{
    public :
        static bool   Signed() {return true;}
        static eTyNums   TyNum() {return eTyNums::eTN_INT2;}
        typedef tREAL4   tFloatAssoc;
};
template <> class tElemNumTrait<tINT4> : public tBaseNumTrait<tStdInt>
{
    public :
        static bool   Signed() {return true;}
        static eTyNums   TyNum() {return eTyNums::eTN_INT4;}
        typedef tREAL8   tFloatAssoc;
};
template <> class tElemNumTrait<tINT8> : public tBaseNumTrait<tStdInt>
{
    public :
        static bool      Signed() {return true;}
        static eTyNums   TyNum() {return eTyNums::eTN_INT8;}
        typedef tREAL8   tFloatAssoc;
};

      // Floating type

template <> class tElemNumTrait<tREAL4> : public tBaseNumTrait<tStdDouble>
{
    public :
        static tREAL4 Accuracy() {return 1e-2;} 
        static bool   Signed() {return true;} ///< Not usefull but have same interface
        static eTyNums   TyNum() {return eTyNums::eTN_REAL4;}
        typedef tREAL4   tFloatAssoc;
};
template <> class tElemNumTrait<tREAL8> : public tBaseNumTrait<tStdDouble>
{
    public :
        static tREAL8 Accuracy() {return 1e-4;} 
        static bool   Signed() {return true;} ///< Not usefull but have same interface
        static eTyNums   TyNum() {return eTyNums::eTN_REAL8;}
        typedef tREAL8   tFloatAssoc;
};
template <> class tElemNumTrait<tREAL16> : public tBaseNumTrait<tREAL16>
{
    public :
        static tREAL16 Accuracy() {return 1e-6;} 
        static bool      Signed() {return true;} ///< Not usefull but have same interface
        static eTyNums   TyNum() {return eTyNums::eTN_REAL16;}
        typedef tREAL16  tFloatAssoc;
};

    // ========================================================================
    //  tNumTrait class to be used
    // ========================================================================

/** Sometime it may be usefull to manipulate the caracteristic (size, int/float ...)  of an unknown
  numerical type (for example read in a tiff file spec), this can be done using cVirtualTypeNum */


class cVirtualTypeNum
{
    public :
       virtual bool V_IsInt()  const = 0;
       virtual bool V_Signed() const = 0;
       virtual int  V_Size()   const = 0;
       virtual eTyNums  V_TyNum() const = 0; ///< Used to check FromEnum, else ??
  

       static const cVirtualTypeNum & FromEnum(eTyNums);
};

template <class Type> class tNumTrait : public tElemNumTrait<Type> ,
                                        public cVirtualTypeNum
{
    public :
 
      // ===========================
         typedef Type  tVal;
         typedef tElemNumTrait<Type>  tETrait;
         typedef typename  tETrait::tBase tBase;
         typedef typename  tETrait::tBig  tBig ;
      // ===========================
         bool V_IsInt()  const override {return  tBaseNumTrait<tBase>::IsInt();}
         bool V_Signed() const override {return  tETrait::Signed();}
         int  V_Size()   const override {return  sizeof(Type);}
         eTyNums  V_TyNum() const override {return  tETrait::TyNum();}

      //==============
         static const tBase MaxValue() {return  std::numeric_limits<tVal>::max();}
         static const tBase MinValue() {return  std::numeric_limits<tVal>::min();}

         static bool ValueOk(const tBase & aV)
         {
               if (tETrait::IsInt())
                  return (aV>=MinValue()) && (aV<=MaxValue());
               return ValidFloatValue(aV);
         }
         static Type Trunc(const tBase & aV)
         {
               if (tETrait::IsInt())
               {
                  if  (aV<MinValue()) return MinValue();
                  if  (aV>MaxValue()) return MaxValue();
               }
               return Type(aV);
         }
         static Type RandomValue()
         {
              if (tETrait::IsInt())
                 return MinValue() + RandUnif_0_1() * (int(MaxValue())-int(MinValue())) ;
              return RandUnif_0_1();
         }
         static Type RandomValueCenter()
         {
              if (tETrait::IsInt())
                 return MinValue() + RandUnif_0_1() * (int(MaxValue())-int(MinValue())) ;
              return RandUnif_C();
         }
         static Type Eps()
         {
              if (tETrait::IsInt())
                 return 1;
              return  std::numeric_limits<Type>::epsilon();
         }

         static const tNumTrait<Type>  TheOnlyOne;
         static const std::string & NameType() {return E2Str(TheOnlyOne.V_TyNum());}
};


// This traits type allow to comppute a temporary variable having the max
// precision between 2 floating types

template <class T1,class T2> class tMergeF { public : typedef tREAL16  tMax; };
template <> class tMergeF<tREAL4,tREAL4> { public : typedef tREAL4  tMax; };
template <> class tMergeF<tREAL8,tREAL4> { public : typedef tREAL8  tMax; };
template <> class tMergeF<tREAL4,tREAL8> { public : typedef tREAL8  tMax; };
template <> class tMergeF<tREAL8,tREAL8> { public : typedef tREAL8  tMax; };




// typedef unsigned char tU_INT1;
// typedef unsigned char tU_INT1;

/* ================= Modulo ======================= */

/// work only when b > 0
inline tINT4 mod(tINT4 a,tINT4 b)
{
    tINT4 r = a%b;
    return (r <0) ? (r+b) : r;
}

/// work only also when b < 0
inline tINT4 mod_gen(tINT4 a,tINT4 b)
{
    tINT4 r = a%b;
    return (r <0) ? (r+ ((b>0) ? b : -b)) : r;
}

///  Modulo with real value, same def as with int but build-in C support
inline tREAL8 mod_real(tREAL8 a,tREAL8 b)
{
   MMVII_INTERNAL_ASSERT_tiny(b>0,"modreal");
   tREAL8 aRes = a - b * round_ni(a/b);
   return (aRes<0) ? aRes+b : aRes;
}

template<class Type> Type DivSup(const Type & a,const Type & b) 
{
    MMVII_INTERNAL_ASSERT_tiny(b>0,"DivSup");
    return (a+b-1)/b; 
}

/// Return a value depending only of ratio, in [-1,1], eq 0 if I1=I2, and invert sign when swap I1,I2
double NormalisedRatio(double aI1,double aI2);


tINT4 HCF(tINT4 a,tINT4 b); ///< = PGCD = Highest Common Factor
int BinomialCoeff(int aK,int aN);
double  RelativeDifference(const double & aV1,const double & aV2,bool * Ok=nullptr);

template <class Type> int SignSupEq0(const Type & aV) {return (aV>=0) ? 1 : -1;}

inline tREAL8 FracPart(tREAL8 r) {return r - round_down(r);}


template <class Type> Type Square(const Type & aV) {return aV*aV;}
template <class Type> Type Cube(const Type & aV) {return aV*aV*aV;}
template <class Type,class TCast> TCast TSquare(const Type & aV) {return aV* TCast(aV);}
template <class Type> tREAL8  R8Square(const Type & aV) {return TSquare<Type,tREAL8>(aV);} ///< To avoid oveflow with int type


template <class Type> void OrderMinMax(Type & aV1,Type & aV2)
{
   if (aV1>aV2)
      std::swap(aV1,aV2);
}

// 4 now use sort, will enhance with boost or home made
template <class Type> Type NonConstMediane(std::vector<Type> & aV);
template <class Type> Type ConstMediane(const std::vector<Type> & aV);

/*  ********************************* */
/*       Kernels                      */
/* ********************************** */

/// A kernel, approximating "gauss"

/**  a quick kernel, derivable, with support in [-1,1], coinciding with bicub in [-1,1] 
     not really gauss but has a "bell shape"
     1 +2X^3 -3X^2  , it's particular of bicub with Deriv(1) = 0
*/
/*
class cCubAppGauss
{
     public :
         tREAL8 Value(const tREAL8);
     private :
};
*/

/// If we dont need any kernel interface keep it simple 
tREAL8 CubAppGaussVal(const tREAL8&);   

/*  ********************************* */
/*       Witch Min and Max            */
/* ********************************** */

template <class TypeIndex,class TypeVal> class cWhitchMin
{
     public :
         cWhitchMin(const TypeIndex & anIndex,const TypeVal & aVal) :
             mIndexMin (anIndex),
             mVMin     (aVal)
         {
         }
         void Add(const TypeIndex & anIndex,const TypeVal & aVal)
         {
              if (aVal<mVMin)
              {     
                    mVMin = aVal;
                    mIndexMin = anIndex;
              }
         }
         const TypeIndex & Index() const {return mIndexMin;}
         const TypeVal   & Val  () const {return mVMin;}
     private :
         TypeIndex mIndexMin;
         TypeVal   mVMin;
};
template <class TypeIndex,class TypeVal> class cWhitchMax
{
     public :
         cWhitchMax(const TypeIndex & anIndex,const TypeVal & aVal) :
             mIndexMax (anIndex),
             mVMax     (aVal)
         {
         }
         void Add(const TypeIndex & anIndex,const TypeVal & aVal)
         {
              if (aVal>mVMax)
              {
                    mVMax = aVal;
                    mIndexMax = anIndex;
              }
         }
         const TypeIndex & Index() const {return mIndexMax;}
         const TypeVal   & Val  () const {return mVMax;}
     private :
         TypeIndex mIndexMax;
         TypeVal   mVMax;
};
template <class TypeIndex,class TypeVal> class cWhitchMinMax
{
     public  :
         cWhitchMinMax(const TypeIndex & anIndex,const TypeVal & aVal) :
             mMin(anIndex,aVal),
             mMax(anIndex,aVal)
         {
         }
         void Add(const TypeIndex & anIndex,const TypeVal & aVal)
         {
             mMin.Add(anIndex,aVal);
             mMax.Add(anIndex,aVal);
         }
         const cWhitchMin<TypeIndex,TypeVal> & Min() const {return  mMin;}
         const cWhitchMax<TypeIndex,TypeVal> & Max() const {return  mMax;}

     private :
         cWhitchMin<TypeIndex,TypeVal> mMin;
         cWhitchMax<TypeIndex,TypeVal> mMax;
};

// This rather "strange" function returns a value true at frequence  as close as possible
// to aFreq, and with the warantee that it is true for last index

bool SignalAtFrequence(tREAL8 anIndex,tREAL8 aFreq,tREAL8  aCenterPhase);

/*  ****************************************** */
/*       Analytical function used with fisheye */
/* ******************************************* */

// Sinus cardinal with caution on tiny values
template <typename Type> Type sinC(const Type & aTeta);
template <typename Type> Type sinC(const Type & aTeta,const Type & aEps);

  //  ----- Function used for equilinear fisheye ----

   /// Arctan(x,y)/x  but stable when x->0,  !!! NOT C++ CONVENTION WHICH ARE atan2(y,x)
template <typename Type> Type AtanXsY_sX(const Type & X,const Type & Y);
  /// Derivate upon x of AtanXY_sX(x,y),  stable when x->0
template <typename Type> Type DerXAtanXsY_sX(const Type & X,const Type & Y);
  /// Derivate upon y of AtanXY_sX(x,y),  noting to care  when x->0
template <typename Type> Type DerYAtanXsY_sX(const Type & X,const Type & Y);

   /// Same as AtanXY_sX but user fix the "tiny" value, used for bench
template <typename Type> Type AtanXsY_sX(const Type & X,const Type & Y,const Type & aEps);
   /// Same as DerXAtanXY_sX ...  ... bench
template <typename Type> Type DerXAtanXsY_sX(const Type & X,const Type & Y,const Type & aEps);





};

#endif  //  _MMVII_nums_H_
