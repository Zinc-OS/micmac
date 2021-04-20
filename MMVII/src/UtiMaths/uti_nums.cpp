#include "include/MMVII_all.h"

namespace MMVII
{

int BinomialCoeff(int aK,int aN)
{
  if ((aK<0) || (aK>aN)) 
     return aK;
  if (aK> (aN/2)) 
     aK= aN-aK;

  tINT8 aNum = 1;
  tINT8 aDenom = 1;

  for (int aP = 1 ; aP<=aK ; aP++)
  {
      aDenom*= aP;
      aNum *= (aN+1-aP);
  }
  return aNum / aDenom;
}

double  RelativeDifference(const double & aV1,const double & aV2,bool * aResOk)
{
    double aSom =  std::abs(aV1) +  std::abs(aV2);
    bool Ok = (aSom!=0);
    if (aResOk!=nullptr)
       *aResOk = Ok;
    if (!Ok)
    {
        MMVII_INTERNAL_ASSERT_strong(aResOk,"Null values in RelativeDifference");
        return std::nan("");
    }
    return std::abs(aV1-aV2) / aSom;
}


tINT4 HCF(tINT4 a,tINT4 b)
{
   a = std::abs(a);
   b = std::abs(b);

   OrderMinMax(a,b);
   while (a!=0)
   {
      tINT4 aInit = a;
      a = b%a;
      b = aInit;
   }
   return b;
}

template <class Type> const tNumTrait<Type>   tNumTrait<Type>::TheOnlyOne;
// const tNumTrait<tINT1>   tNumTrait<tINT1>::TheOnlyOne;
/*
template <> const tNumTrait<tINT2>   tNumTrait<tINT2>::TheOnlyOne;
template <> const tNumTrait<tINT4>   tNumTrait<tINT4>::TheOnlyOne;
template <> const tNumTrait<tINT8>   tNumTrait<tINT8>::TheOnlyOne;

template <> const tNumTrait<tU_INT1>   tNumTrait<tU_INT1>::TheOnlyOne;
template <> const tNumTrait<tU_INT2>   tNumTrait<tU_INT2>::TheOnlyOne;
template <> const tNumTrait<tU_INT4>   tNumTrait<tU_INT4>::TheOnlyOne;

template <> const tNumTrait<tREAL4>   tNumTrait<tREAL4>::TheOnlyOne;
template <> const tNumTrait<tREAL8>   tNumTrait<tREAL8>::TheOnlyOne;
template <> const tNumTrait<tREAL16>  tNumTrait<tREAL16>::TheOnlyOne;
*/


static const cVirtualTypeNum & SwitchFromEnum(eTyNums aTy)
{
   switch (aTy)
   {
      case eTyNums::eTN_INT1 : return tNumTrait<tINT1>::TheOnlyOne;
      case eTyNums::eTN_INT2 : return tNumTrait<tINT2>::TheOnlyOne;
      case eTyNums::eTN_INT4 : return tNumTrait<tINT4>::TheOnlyOne;
      case eTyNums::eTN_INT8 : return tNumTrait<tINT8>::TheOnlyOne;

      case eTyNums::eTN_U_INT1 : return tNumTrait<tU_INT1>::TheOnlyOne;
      case eTyNums::eTN_U_INT2 : return tNumTrait<tU_INT2>::TheOnlyOne;
      case eTyNums::eTN_U_INT4 : return tNumTrait<tU_INT4>::TheOnlyOne;

      case eTyNums::eTN_REAL4 :  return tNumTrait<tREAL4>::TheOnlyOne;
      case eTyNums::eTN_REAL8 :  return tNumTrait<tREAL8>::TheOnlyOne;
      case eTyNums::eTN_REAL16 : return tNumTrait<tREAL16>::TheOnlyOne;
      // case eTyNums::eTN_UnKnown : return tNumTrait<eTN_UnKnown>::TheOnlyOne;

      default : ;
   }
   MMVII_INTERNAL_ASSERT_strong(false,"Uknown value in cVirtualTypeNum::FromEnum");
   return tNumTrait<tINT1>::TheOnlyOne;
}

const cVirtualTypeNum & cVirtualTypeNum::FromEnum(eTyNums aTy)
{
    static std::vector<const cVirtualTypeNum *> aV;
    if (aV.empty())
    {
        // for (int aK=0 ; aK<int(eTyNums::eNbVals) ; aK++)
        for (int aK=0 ; aK<int(eTyNums::eTN_UnKnown) ; aK++)
        {
            aV.push_back(&SwitchFromEnum(eTyNums(aK)));
        }
    }
    return *(M_VectorAt(aV,int(aTy)));
}



template <class Type> void TplBenchTraits()
{
    //typename tNumTrait<Type>::tBase aVal=0;
    StdOut()  << "    "
              << E2Str(tNumTrait<Type>::TyNum() )
              << " Max=" << tNumTrait<Type>::MaxValue() 
              << " Min=" <<  tNumTrait<Type>::MinValue() 
              << " IsInt=" <<  tNumTrait<Type>::IsInt() 
              << "\n";

}


void BenchTraits()
{
   TplBenchTraits<tU_INT1>();
   TplBenchTraits<tU_INT2>();
   TplBenchTraits<tINT1>();
   TplBenchTraits<tINT2>();
   TplBenchTraits<tINT4>();
   TplBenchTraits<tREAL4>();
   for (int aK=0 ; aK<int(eTyNums::eTN_UnKnown) ; aK++)
   {
       const cVirtualTypeNum & aVTN =  cVirtualTypeNum::FromEnum(eTyNums(aK));
       MMVII_INTERNAL_ASSERT_bench (int(aVTN.V_TyNum())==aK,"Bench cVirtualTypeNum::FromEnum");
   }
   // getchar();
}

tINT4 EmbeddedIntVal(tREAL8 aRealVal)
{
    int aIntVal = round_ni(aRealVal);
    MMVII_INTERNAL_ASSERT_tiny(tREAL8(aIntVal)==aRealVal,"EmbeddedIntVal");

    return aIntVal;
}

bool  EmbeddedBoolVal(tINT4 aIV)
{
    MMVII_INTERNAL_ASSERT_tiny((aIV==0)||(aIV==1),"Enbedde In to Bool");
    return aIV==1;
}

bool  EmbeddedBoolVal(tREAL8 aRealVal)
{
    return EmbeddedBoolVal(EmbeddedIntVal(aRealVal));
}




/// Bench that aModb is the mathematicall definition
void BenchMod(int A,int B,int aModb)
{
     MMVII_INTERNAL_ASSERT_bench(aModb>=0,"BenchMod-1");
     MMVII_INTERNAL_ASSERT_bench(aModb<std::abs(B),"BenchMod-2");

     int AmB = A - aModb;  // AmB => A multiple de B
     MMVII_INTERNAL_ASSERT_bench((AmB/B)*B == AmB ,"BenchMod-3");
}


double NormalisedRatio(double aI1,double aI2)
{
    // X = I1/I2
    if (aI1 < aI2)   // X < 1
        return aI1/aI2 -1;   // X -1
    // 0<= aI2 <= aI1
    if (aI1==0)
    {
       return 0;
    }

    return 1-aI2/aI1;  // 1 -1/X
}


template <class Type> void  TplBenchMinMax(int aNb)
{

    std::vector<Type> aVVals;
    Type aV0 = tNumTrait<Type>::RandomValueCenter();
    cWhitchMinMax<int,Type> aWMM(0,aV0);
    aVVals.push_back(aV0);
    for (int aK=0 ; aK<aNb-1 ; aK++)
    {
       Type aVal = tNumTrait<Type>::RandomValueCenter();
       aVVals.push_back(aVal);
       aWMM.Add(aK+1,aVal);
    }
    int aKMin = aWMM.Min().Index();
    int aKMax = aWMM.Max().Index();
    MMVII_INTERNAL_ASSERT_bench (aVVals.at(aKMin)==aWMM.Min().Val(),"Bench MinMax");
    MMVII_INTERNAL_ASSERT_bench (aVVals.at(aKMax)==aWMM.Max().Val(),"Bench MinMax");
    for (const auto & aV : aVVals)
    {
        MMVII_INTERNAL_ASSERT_bench (aV>=aWMM.Min().Val(),"Bench MinMax");
        MMVII_INTERNAL_ASSERT_bench (aV<=aWMM.Max().Val(),"Bench MinMax");
    }
   
}

void BenchMinMax()
{
   for (int aK=0 ; aK<=100 ; aK++)
   {
       TplBenchMinMax<tU_INT1>(1+aK);
       TplBenchMinMax<tREAL4>(1+aK);
   }
}

template <class Type> void BenchFuncAnalytique(int aNb,double aEps,double EpsDer)
{
   for (int aK=0 ; aK< aNb ; aK++)
   {
       Type aEps = 1e-2;
       // Generate smal teta in [-2E,2E] , avoid too small teta for explicite atan/x
       Type Teta =  2 * aEps * RandUnif_C_NotNull(1e-3);
       // generate also big teta
       if ((aK%4)==0)
          Teta =  3.0 * RandUnif_C_NotNull(1e-3);
       Type aRho = std::max(EpsDer*10,10*RandUnif_0_1());

       Type aX = aRho * std::sin(Teta);
       Type aY = aRho * std::cos(Teta);

       Type aTeta2Sx = AtanXsY_sX(aX,aY,aEps);
       Type aTeta1Sx = Teta / aX;

       // std::cout << RelativeDifference(aTeta2Sx,aTeta1Sx) << " " << (aTeta2Sx-aTeta1Sx)*1e10  << "\n";
       MMVII_INTERNAL_ASSERT_bench(std::abs(aTeta2Sx-aTeta1Sx)<aEps,"Bench binom");

       Type aDDif = aRho * 3e-3;
       Type aDerDifX = (AtanXsY_sX(aX+aDDif,aY,aEps)-AtanXsY_sX(aX-aDDif,aY,aEps)) / (2*aDDif);
       Type aDerX = DerXAtanXsY_sX(aX,aY,aEps);

       MMVII_INTERNAL_ASSERT_bench(RelativeDifference(aDerDifX,aDerX) <EpsDer,"Der AtanXsY_SX");

       Type aDerDifY = (AtanXsY_sX(aX,aY+aDDif,aEps)-AtanXsY_sX(aX,aY-aDDif,aEps)) / (2*aDDif);
       Type aDerY = DerYAtanXsY_sX(aX,aY);
       // std::cout << "DDyyy " << RelativeDifference(aDerDifY,aDerY) << "\n";
       MMVII_INTERNAL_ASSERT_bench(RelativeDifference(aDerDifY,aDerY) <EpsDer,"Der AtanXsY_SX");
   }
   // std::cout << atan2(0,1)  << " " << atan2(1,0) << "\n";
   // getchar();
}

void Bench_Nums(cParamExeBench & aParam)
{
   if (! aParam.NewBench("BasicNum")) return;

   {
      int aNb=10000;
      BenchFuncAnalytique<tREAL4> (aNb,1e-3,100);
      BenchFuncAnalytique<tREAL8> (aNb,1e-5,1e-2);
      BenchFuncAnalytique<tREAL16>(aNb,1e-7,1e-2);
   }

   BenchMinMax();

   //for (
   MMVII_INTERNAL_ASSERT_bench (BinomialCoeff(2,10)==45,"Bench binom");
   {
      int aS=0;
      for (int aK=0 ; aK<=10 ; aK++)
      {
         aS += BinomialCoeff(aK,10);
      }
      MMVII_INTERNAL_ASSERT_bench (aS==(1<<10),"Bench binom");
   }
   // This function dont make test, but prints value on numerical types
   if (aParam.Show())
      BenchTraits(); 

   MMVII_INTERNAL_ASSERT_bench (sizeof(tREAL4)==4,"Bench size tREAL4");
   MMVII_INTERNAL_ASSERT_bench (sizeof(tREAL8)==8,"Bench size tREAL8");

   MMVII_INTERNAL_ASSERT_bench (sizeof(tREAL16)==16,"Bench size tREAL16");

   MMVII_INTERNAL_ASSERT_bench (sizeof( tINT1)==1,"Bench size tINT1");
   MMVII_INTERNAL_ASSERT_bench (sizeof( tINT2)==2,"Bench size tINT2");
   MMVII_INTERNAL_ASSERT_bench (sizeof( tINT4)==4,"Bench size tINT4");
   // MMVII_INTERNAL_ASSERT_bench (sizeof( tINT8)==8,"Bench round_up");
   /// Bench modulo

   for (int A=-20 ; A<=20 ; A++)
   {
      for (int B=-20 ; B<=20 ; B++)
      {
         if (B!=0)
         {
            // BenchMod(A,B,mod(A,B));
            // BenchMod(A,B,mod_gen(A,B));
            double aRatio = double(A) / double(B);

            int rup = round_up(aRatio);
            MMVII_INTERNAL_ASSERT_bench ((rup>=aRatio) &&((rup-1)<aRatio),"Bench round_up");
            int ruup = round_Uup(aRatio);
            MMVII_INTERNAL_ASSERT_bench ((ruup>aRatio) &&((ruup-1)<=aRatio),"Bench round_up");
            
            int rd = round_down(aRatio);
            MMVII_INTERNAL_ASSERT_bench ((rd<=aRatio) &&((rd+1)>aRatio),"Bench round_up");
            int rdd = round_Ddown(aRatio);
            MMVII_INTERNAL_ASSERT_bench ((rdd<aRatio) &&((rd+1)>=aRatio),"Bench round_up");

            int ri = round_ni(aRatio);
            MMVII_INTERNAL_ASSERT_bench ((ri<=aRatio+0.5) &&(ri>aRatio-0.5),"Bench round_up");

            BenchMod(A,B,mod_gen(A,B));
            if (B>0)
               BenchMod(A,B,mod(A,B));

            {
                double aFrac = FracPart(aRatio);
                MMVII_INTERNAL_ASSERT_bench ((aFrac>=0) &&( aFrac<1),"Bench Frac");
                double I  = aRatio - aFrac;
                MMVII_INTERNAL_ASSERT_bench(round_ni(I)==I,"Bench Frac");
            }
         }
      }
   }

   for (int aK=0 ; aK<1000 ; aK++)
   {
        double aA = 5.0 * RandUnif_C();
        double aB = 1e-2 +  RandUnif_0_1();
        double aR = mod_real(aA,aB);
        MMVII_INTERNAL_ASSERT_bench((aR>=0)&&(aR<aB),"Bench Modreal");
        double aDiv = (aA-aR) / aB;
        double aDif = aDiv - round_ni(aDiv);
        MMVII_INTERNAL_ASSERT_bench(std::abs(aDif)<1e-5,"Bench Frac");

        double aI1 = 1e-2 +  RandUnif_0_1();
        double aI2 = 1e-2 +  RandUnif_0_1();
        double aR12 =  NormalisedRatio(aI1,aI2);
        double aR21 =  NormalisedRatio(aI2,aI1); // Anti sym

        double aMul =  (1e-2+ RandUnif_0_1()) *10;
        double aRM12 =  NormalisedRatio(aI1*aMul,aI2*aMul); // Scale inv

        double aI1G = aI1+ 1e-6+ 1e-2 *  RandUnif_0_1(); 
        double aR1G2 =  NormalisedRatio(aI1G,aI2); // growing
        MMVII_INTERNAL_ASSERT_bench( (aR12>=-1) && (aR12<=1),"Bench NormRat");
        MMVII_INTERNAL_ASSERT_bench( std::abs(aR12+aR21)<1e-5,"Bench NormRat");
        MMVII_INTERNAL_ASSERT_bench( std::abs(aR12-aRM12)<1e-5,"Bench NormRat");
        MMVII_INTERNAL_ASSERT_bench( aR1G2>aR12,"Bench NormRat");
   }

   aParam.EndBench();
}

template <class Type> Type  NonConstMediane(std::vector<Type> & aV)
{
   std::sort(aV.begin(),aV.end());
   return aV.at(aV.size()/2);
}

template <class Type> Type  ConstMediane(const std::vector<Type> & aV)
{
    std::vector<Type> aDup(aV);
    return NonConstMediane(aDup);
}

template  double NonConstMediane(std::vector<double> &);
template  double ConstMediane(const std::vector<double> &);


bool SignalAtFrequence(tREAL8 anIndex,tREAL8 aFreq,tREAL8  aCenterPhase)
{
   tREAL8 aCoord0 = 0.5 + (anIndex-aCenterPhase + 0.5) * aFreq;
   tREAL8 aCoord1 = 0.5 + (anIndex-aCenterPhase - 0.5) * aFreq;

   return lround_ni(aCoord0) != lround_ni(aCoord1);
}


};

