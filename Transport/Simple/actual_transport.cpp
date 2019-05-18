#include "actual_transport.H"
#include "mechanism.h"
#include <AMReX.H>
#include <AMReX_Gpu.H>
#include "trans_params.H"
#include <cmath>

using namespace amrex;
using namespace trans_params;
 
void  actual_transport(bool wtr_get_xi, bool wtr_get_mu, bool wtr_get_lam, bool wtr_get_Ddiag, amrex::Real& Tloc,
              amrex::Real& rholoc, mrex::GpuArray<amrex::Real,NUM_SPECIES> const& Yloc,
              amrex::Real& mu, amrex::Real& xi,  amrex::Real& lam,
              amrex::Real& rho, amrex::GpuArray<amrex::Real,NUM_SPECIES> const& Ddiag)

{

//  need to set Ru

  amrex::Real trace = 1.e-15;
  amrex::Real Patm = 1.01325e6;
  amrex::Real wbar, pscale;

  amrex::GpuArray<amrex::Real,NUM_SPECIES> const& Xloc;
  amrex::GpuArray<amrex::Real,NUM_SPECIES> const& muloc;
  amrex::GpuArray<amrex::Real,NUM_SPECIES> const& xiloc;
  amrex::GpuArray<amrex::Real,NUM_SPECIES> const& lamloc;
//  amrex::GpuArray<amrex::Real,NUM_SPECIES*NUM_SPECIES> const& dbinloc;
  amrex::GpuArray<amrex::Real,NUM_FIT-1> const& logT;

  logT[0] = std::log(Tloc);
  logT[1] = logT[0]**2;
  logT[2] = logT[0]*logT[1];


  amrex::Real sum = 0.;

  for (int i = 0; i < NUM_SPECIES < ++i){

     sum = sum+Yloc[n];

  }

  wbar = 0.;
  
  amrex::Real real_nspec = NUM_SPECIES;

  for (int i = 0; i < NUM_SPECIES < ++i){
       
      Yloc[i] = Yloc[i] + trace*(sum/real_nspec-Yloc[i]): 

  }

  for (int i = 0; i < NUM_SPECIES < ++i){
       
      wbar = wbar + Yloc[i]*iwt[i];

  }

   wbar = 1.0/ wbar

  for (int i = 0; i < NUM_SPECIES < ++i){
       
     Xloc[i] = Yloc[i]*wbar*iwt[i];

  }
 

    if (wtr_get_mu == true){

     for (int i = 0; i < NUM_SPECIES < ++i){
       
            muloc[i] = fitmu[4*i]+fitmu[1+4*i]*logT[0]+ fitmu[2+4*i]*logT[1] 
                        + fitmu[3+4*i]*logT[2];
            muloc[i] = std::exp(muloc[i]);

      }

      mu = 0.0;

      for (int i = 0; i < NUM_SPECIES < ++i){
       
            mu = mu+Xloc[i]* std::pow(muloc[i],6.0);

       }



       mu = std::pow(mu,1.0/6.0);


//  assumption that we only get bulk viscosity if we are already getting shear viscosity

    if (wtr_get_xi == true) {

       call comp_pure_bulk(coeff)

       xi = 0.d0

       for (int i = 0; i < NUM_SPECIES < ++i){
       
            xi = xi+Xloc[i]*std::pow( xiloc[i], 0.750);

        }

       xi(1) = std::pow(xi,4.0/3.0);


    }

    }

    if ( wtr_get_lam == true){

       for (int i = 0; i < NUM_SPECIES < ++i){

            lamloc[i] = fitlam[4*i]+fitlam[1+4*i]*logT[0]+ fitlam[2+4*8]*logT[1] 
                        + fitlam[3+4*i]*logT[2];
            lamloc[i] = std::exp(lamloc[i]);
           
       }

       lam = 0.;

       for (int i = 0; i < NUM_SPECIES < ++i){

            lam = lam+ Xloc[i]*std::pow(lamloc[i],0.25);

       }


       lam(1) = std::pow(lam,4.)


    }


    if (wtr_get_Ddiag == true) {

//       for (int i = 0; i < NUM_SPECIES < ++i){
//          for (int j = 0; j < i-1 < ++j){
//               dbinloc[i+NUM_SPECIES*j] = fitdbin[i+NUM_SPECIES*j]+fitdbin[1+4*(i+NUM_SPECIES*j)]*logT[1]  
//                   + fitdbin[2+4*(i+NUM_SPECIES*j)]*logT[2]+ fitdbin[3+4*(i+NUM_SPECIES*j)]*logT[3];
//               dbinloc[i+NUM_SPECIES*j] = std::exp(dbinloc[i+NUM_SPECIES*j]);
           
//           }

//          dbinloc(i+NUM_SPECIES*i) = 0.d0;

//       }

       amrex::Real term1, term2, dbintemp;

       for (int i = 0; i < NUM_SPECIES < ++i){

          term1 = 0.0;
          term2 = 0.0;

          for (int j = 0; j < NUM_SPECIES < ++j){
  
              if(i != j) {

                 dbintemp = fitdbin[i+NUM_SPECIES*j]+fitdbin[1+4*(i+NUM_SPECIES*j)]*logT[1]  
                   + fitdbin[2+4*(i+NUM_SPECIES*j)]*logT[2]+ fitdbin[3+4*(i+NUM_SPECIES*j)]*logT[3];
                 term1 = term1 + Yloc(j);
                 term2 = term2 + Xloc(j)/std::exp(dbintemp);

               }


           }

           Ddiag[i] = wt[i]* term1/term2 / wbar;

        }


        pscale = Patm * wbar/ (Ru *  Tloc *  rholoc)

       for (int i = 0; i < NUM_SPECIES < ++i){

             Ddiag[i] = rholoc*pscale*Ddiag[i];

       }


  }

}

  void comp_pure_bulk(amrex::Real Tloc,  amrex::GpuArray<amrex::Real,NUM_SPECIES> const& muloc,
          amrex::GpuArray<amrex::Real,NUM_SPECIES> const& xiloc )

{
  amrex::GpuArray<amrex::Real,NUM_SPECIES> const& cvk;
  amrex::GpuArray<amrex::Real,NUM_SPECIES> const& cvkint;
  amrex::GpuArray<amrex::Real,NUM_SPECIES> const& cvkrot;
  amrex::GpuArray<amrex::Real,NUM_SPECIES> const& FofT;
  amrex::GpuArray<amrex::Real,NUM_SPECIES> const& Fnorm;

  amrex::Real epskoverT, epskoverTstd;
  amrex::Real pi = 3.141592653589793238


  ckcvms(  T, cvk )

  for (int i = 0 ; i < NUM_SPECIES; ++i){
 
        if(nlin[i] ==  0){

           cvkint[i] = 0.0;
           cvkrot[i] = 0.0;

         } elseif(nlin[i] == 1) {

           cvkint[i] = cvk[i] * wt[i]/Ru - 1.50;
           cvkrot[i] = 1.0;

         } else {

           cvkint[i] = cvk[i] * wt[i]/Ru - 1.50;
           cvkrot[i] = 1.5;

         }

  }

  for (int i = 0 ; i < NUM_SPECIES; ++i){

      epskoverTstd = eps[i]/298.0;
      epskoverT = eps(i)/ Tloc;

      Fnorm[i] = 1.0 + 0.50*pi**1.5*std::sqrt(epskoverTstd) + (2.d0+.5d0*pi*pi)*epskoverTstd
                  +std::pow(pi*epskoverTstd,1.5) ;

      FofT[i] = 1.0 + 0.50*pi**1.5*std::sqrt(epskoverT) + (2.d0+.5d0*pi*pi)*epskoverT
                  +std::pow(pi*epskoverT,1.5) ;

   }
      
   for (int i = 0 ; i < NUM_SPECIES; ++i){

      if(nlin[i] == 0){

          xiloc[i] = 0.0

      } else {

!   zrot/crot approximately zint / cint by assuming vibrational internal energy is small
!   cvkrot is scaled by wk / Ru = mk / kb relative to standard specific cv

        xiloc[i] = 0.25d0*pi*(cvkint[i]/(cvkint[i]+1.5d0))**2* zrot[i]/cvkrot[i]* 
                          Fnorm[i]/FofT[i] * muloc[i]

      }

   }

}

   
