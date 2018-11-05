#ifndef HITFINDER_BEAM_H
#define HITFINDER_BEAM_H

#include "WCOpReco/DataReader.h"
#include "WCOpReco/Opflash.h"
#include "WCOpReco/LassoModel.h"
#include "WCOpReco/ElasticNetModel.h"
#include "WCOpReco/LinearModel.h"
#include "WCOpReco/OpWaveformCollection.h"
#include "WCOpReco/Deconvolver.h"

//c++ includes
#include <vector>
#include <iostream>
#include <sstream>
#include <cmath>
#include <string>
#include <Eigen/Dense>

//root
#include "TMath.h"

namespace wcopreco{
  //Module for hit finding for beam
  // loop through all 32 channels of collection, fft with filters, ifft, and perform L1 fit

  class HitFinder_beam {
  public:
    HitFinder_beam(OpWaveformCollection deconvolved_beam);
    ~HitFinder_beam() {};

    void Perform_L1(std::vector<double> inverse_res1,
                       std::vector< std::vector<double> > &decon_vv,
                       std::vector<double> &totPE_v,
                       std::vector<double> &mult_v,
                       std::vector<double> &l1_totPE_v,
                       std::vector<double> &l1_mult_v,
                       int ch
                     );

     std::vector<double> get_totPE_v(){return totPE_v;}
     std::vector<double> get_mult_v(){return mult_v;}
     std::vector<double> get_l1_totPE_v(){return l1_totPE_v;}
     std::vector<double> get_l1_mult_v(){return l1_mult_v;}
     std::vector< std::vector<double> > get_decon_vv(){return decon_vv;}


  protected:
    int channel;
    std::vector<double> totPE_v;
    std::vector<double> mult_v;
    std::vector<double> l1_totPE_v;
    std::vector<double> l1_mult_v;
    std::vector< std::vector<double> > decon_vv;


  };

}

#endif