#include "include/MMVII_all.h"

namespace MMVII
{

template<class T> cPtxd<T,3>  PFromNumAxe(int aNum);
template<class T> cDenseMatrix<T> MatFromCols(const cPtxd<T,3>&aP0,const cPtxd<T,3>&aP1,const cPtxd<T,3>&aP2);
template<class T> cDenseMatrix<T> MatFromLines(const cPtxd<T,3>&aP0,const cPtxd<T,3>&aP1,const cPtxd<T,3>&aP2);
template <class T>  cPtxd<T,3> operator ^ (const cPtxd<T,3> & aP1,const cPtxd<T,3> & aP2);
// Return one vector orthog, as the choice is not univoque just avoid degenerency
template<class T> cPtxd<T,3>  VOrthog(const cPtxd<T,3> & aP);  
/*
*/

template<class T> cPtxd<T,3>  PFromNumAxe(int aNum)
{
   static const cDenseMatrix<T> anId3x3(3,3,eModeInitImage::eMIA_MatrixId);
   return cPtxd<T,3>::Col(anId3x3,aNum);
}

template<class T> cDenseMatrix<T> MatFromCols(const cPtxd<T,3>&aP0,const cPtxd<T,3>&aP1,const cPtxd<T,3>&aP2)
{
   cDenseMatrix<T> aRes(3,3);

   SetCol(aRes,0,aP0);
   SetCol(aRes,1,aP1);
   SetCol(aRes,2,aP2);

   return aRes;
}

template<class T> cDenseMatrix<T> MatFromLines(const cPtxd<T,3>&aP0,const cPtxd<T,3>&aP1,const cPtxd<T,3>&aP2)
{
   cDenseMatrix<T> aRes(3,3);

   SetLine(0,aRes,aP0);
   SetLine(1,aRes,aP1);
   SetLine(2,aRes,aP2);

   return aRes;
}


template <class T>  cPtxd<T,3> operator ^ (const cPtxd<T,3> & aP1,const cPtxd<T,3> & aP2)
{
   return cPtxd<T,3>
          (
               aP1.y() * aP2.z() -aP1.z()*aP2.y(),
               aP1.z() * aP2.x() -aP1.x()*aP2.z(),
               aP1.x() * aP2.y() -aP1.y()*aP2.x()
          );
}

template<class T> cPtxd<T,3>  VOrthog(const cPtxd<T,3> & aP)
{
   // we make a vect product with any vector, just avoid one too colinear  to P
   // test I and J, as P cannot be colinear to both, its sufficient 
   // (i.e : we are sur to maintain the biggest of x, y and z)
   if (std::abs(aP.x()) > std::abs(aP.y()))
      return cPtxd<T,3>( aP.z(), 0, -aP.x());

  return cPtxd<T,3>(0,aP.z(),-aP.y());
}



/* ========================== */
/*          ::                */
/* ========================== */


#define MACRO_INSTATIATE_PTXD(TYPE)\
template cPtxd<TYPE,3>  operator ^ (const cPtxd<TYPE,3> & aP1,const cPtxd<TYPE,3> & aP2);\
template cDenseMatrix<TYPE> MatFromCols(const cPtxd<TYPE,3>&,const cPtxd<TYPE,3>&,const cPtxd<TYPE,3>&);\
template cDenseMatrix<TYPE> MatFromLines(const cPtxd<TYPE,3>&,const cPtxd<TYPE,3>&,const cPtxd<TYPE,3>&);\
template cPtxd<TYPE,3>  PFromNumAxe(int aNum);\
template cPtxd<TYPE,3>  VOrthog(const cPtxd<TYPE,3> & aP);


MACRO_INSTATIATE_PTXD(tREAL4)
MACRO_INSTATIATE_PTXD(tREAL8)
MACRO_INSTATIATE_PTXD(tREAL16)



};
