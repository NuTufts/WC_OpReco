#include "WCOpReco/OpWaveform.h"
#include "WCOpReco/OpWaveformCollection.h"
#include "WCOpReco/EventOpWaveforms.h"
#include "WCOpReco/UBEventWaveform.h"
#include "WCOpReco/DataReader.h"
#include "WCOpReco/Deconvolver.h"
#include "WCOpReco/saturation_merger.h"
#include "WCOpReco/HitFinder_cosmic.h"
#include "WCOpReco/Flashes_cosmic.h"
#include "WCOpReco/Flashes_beam.h"
#include "WCOpReco/HitFinder_beam.h"
#include "WCOpReco/FlashFiltering.h"

//root includes
#include "TH1S.h"
#include "TF1.h"
#include "TVirtualFFT.h"
#include "TTree.h"
#include "TFile.h"
#include "TCanvas.h"

#include <iostream>
#include <sstream>

int main(){


  std::cout << "Hello world" << std::endl;

  //Set the filepath
  std::string file = "src/data/celltree.root";
  std::cout << "\n\nFilepath is set to:   " << file << std::endl;

  //Open the reader, choose event number, create the UBEventWaveform _UB_Ev_wfm
  wcopreco::DataReader reader(file);
  reader.IAMTHENIGHT();
  wcopreco::UBEventWaveform _UB_Ev_wfm;
  bool do_loop = false;
  int EVENT_NUM =16;
  int size = EVENT_NUM+1;

  if (do_loop){
    EVENT_NUM=0;
    size = 52;
  }

  for (EVENT_NUM;EVENT_NUM<size;EVENT_NUM++){
    _UB_Ev_wfm = reader.Reader(EVENT_NUM);
    std::vector<float> op_gain = _UB_Ev_wfm.get_op_gain();
    std::vector<float> op_gainerror = _UB_Ev_wfm.get_op_gainerror();
    // std::cout << "\n";
    // std::cout << _UB_Ev_wfm.get__wfm_v().at(2).size() << " Number of Waveform in Cosmic HG\n";
    // std::cout << _UB_Ev_wfm.get__wfm_v().at(3).size() << " Number of Waveform in Cosmic LG\n";



    //create the merger and then get a Merged UBEventWaveform UB_Ev_Merged
    wcopreco::saturation_merger merger(_UB_Ev_wfm);
    wcopreco::OpWaveformCollection merged_beam = merger.get_merged_beam(); //This is inside UB_Ev_Merged
    wcopreco::OpWaveformCollection merged_cosmic = merger.get_merged_cosmic(); //This is inside UB_Ev_Merged
    wcopreco::UBEventWaveform UB_Ev_Merged = merger.get_merged_UB_Ev();
    //std::cout << UB_Ev_Merged.get__wfm_v().at(5).size() << " Number of Waveform in Cosmic LG\n";
    //std::cout << UB_Ev_Merged.get__wfm_v().at(5).size() << " Number of Waveform in Cosmic LG\n";
    // std::cout << UB_Ev_Merged.get__wfm_v().at(5).size() << " Number of Waveform in Cosmic Merged\n";
    // std::cout << merged_cosmic.size() << " Number of Waveform in Cosmic Merged (CONFIRMED)\n\n";




    //Diagnosis Code -J
    // for (int i=0; i< 2; i++){
    //   for (int j=0; j<merged_beam.at(i).size(); j++){
    //
    //     if (j%1==0) {std::cout << merged_beam.at(i).at(j) << "   Is value in " << i << " wfm in " << j << " bin\n";}
    //   }
    //   std::cout << "\n\n";
    // }


    // Create the Deconvolver (This should just deconvolves the BEAM)
    // wcopreco::Deconvolver tester(&merged_beam, true, true);
    // // std::cout << "Deconvolver declared!" << std::endl;
    // std::vector<wcopreco::kernel_fourier_container> kernel_container_v = tester.get_kernel_container_v();
    //
    // wcopreco::OpWaveform wfm = merged_beam.at(0);
    // std::cout << kernel_container_v.size() << " Size of kernel container_v \n\n\n\n";
    //
    //
    //
    //
    // //do beam hitfinding
    //wcopreco::OpWaveformCollection deconvolved_wfm = tester.Deconvolve_Collection(& merged_beam);
    wcopreco::HitFinder_beam hits_found_beam(merged_beam);
    std::vector<double> totPE_v = hits_found_beam.get_totPE_v();
    std::vector<double> mult_v =hits_found_beam.get_mult_v();
    std::vector<double> l1_totPE_v =hits_found_beam.get_l1_totPE_v();
    std::vector<double> l1_mult_v = hits_found_beam.get_l1_mult_v();
    std::vector< std::vector<double> > decon_vv = hits_found_beam.get_decon_vv();
    double beam_start_time =merged_beam.at(0).get_time_from_trigger();


    wcopreco::Flashes_beam flashfinder_beam( totPE_v, mult_v, l1_totPE_v, l1_mult_v, decon_vv, beam_start_time );
    wcopreco::OpflashSelection flashes_beam = flashfinder_beam.get_beam_flashes();
    // std::cout << "\n\n" << flashes_beam.size() << " Beam Flashes in Event\n";

    // //Create the Hitfinder for COSMICS (Currently this also does the flashes for cosmics)
    wcopreco::HitFinder_cosmic hits_found(&merged_cosmic, &op_gain, &op_gainerror);
    std::vector<wcopreco::COphitSelection> hits = hits_found.get_ophits_group();
    wcopreco::Flashes_cosmic flashfinder_cosmic(hits);
    wcopreco::OpflashSelection flashes_cosmic = flashfinder_cosmic.get_cosmic_flashes();
    // std::cout << flashes_cosmic.size() << " Cosmic Flashes in Event\n";

    // std::cout << flashes.size() << " flashes were found in the cosmic selection\n";

    //flash matching
    wcopreco::FlashFiltering flashesfiltered(flashes_cosmic, flashes_beam);
    wcopreco::OpflashSelection flashes = flashesfiltered.get_flashes();
    // std::cout << flashes.size() << " Matched Flashes in Event\n\n";
    for (int i =0 ; i<flashes.size(); i++) {
      // std::cout << flashes.at(i)->get_total_PE() << "\n";
    }
    // Code to test plotting lines on a waveform
    int bin_low;
    int bin_high;
    wcopreco::Deconvolver testplotter(&merged_cosmic, true,true);
    wcopreco::OpWaveform plottedwfm = merged_beam.at(0);
    char a1;
    char a2;
    char a3;
    std::string str;
    std::vector<double> vec(250,0);
    vec.at(43)  = 10;
    vec.at(121) = 10;
    for (int i = 0; i < merged_beam.size();i++){
      a1 = (char)((i-i%10)/10+48);
      a2 = (char)(i%10+48);
      str = "Plot_with_flash_";
      str +=a1;
      str +=a2;
      std::cout << str<< "\n";
      testplotter.testPlot(str , merged_beam.at(i),vec, 5);

      // std::cout << merged_beam.at(i).get_time_from_trigger() << "\n";
    }
    for (int j=0;j< flashes_beam.size() ; j++) {
      std::cout << flashes_beam.at(j)->get_low_time() <<"    "<< flashes_beam.at(j)->get_high_time()<< "\n";
      bin_low = flashes_beam.at(j)->get_low_time() / (1.0/64.0);
      bin_high = flashes_beam.at(j)->get_high_time() / (1.0/64.0);
      std::cout << bin_low <<"    "<< bin_high << "\n";

    }



    // testplotter.testPlot("Plot With Flash" +(std::string)i,merged_beam.at(i));
    // if (plottedwfm.get_ChannelNum() ==0) std::cout << "Channel num is 0!\n";
}








  // // // // // // Here is deconvolver test code // // // // // // //
  // std::cout << "\n";
  // // Create the Deconvolver (This should just deconvolves the BEAM)
  // wcopreco::Deconvolver Does_Nothing(&merged_beam, false, false);
  // // wcopreco::OpWaveformCollection DN = Does_Nothing.Deconvolve_Collection(& merged_beam);
  //
  // std::cout << "\n";
  // // Create the Deconvolver (This should just deconvolves the BEAM)
  // wcopreco::Deconvolver All_At_Once(&merged_beam, true, true);
  // wcopreco::OpWaveformCollection AAO = All_At_Once.Deconvolve_Collection(& merged_beam);
  // std::cout << "\n";
  //
  // // create the deconvolver in 2 steps
  // wcopreco::Deconvolver Step_One(&merged_beam, true, false);
  // wcopreco::OpWaveformCollection S1 = Step_One.Deconvolve_Collection(& merged_beam);
  // std::cout << "\n";
  //
  // wcopreco::Deconvolver Step_Two(&S1, false, true);
  // wcopreco::OpWaveformCollection S2 = Step_Two.Deconvolve_Collection(& S1);
  // std::cout << "\n";
  //
  //Remove Baselines from Original
  // wcopreco::OpWaveform orig = merged_beam.at(0);
  // All_At_Once.Remove_Baseline_Leading_Edge(&orig);
  // All_At_Once.Remove_Baseline_Secondary(&orig);

  // Do Nothing Deconvolver
  // std::vector<wcopreco::kernel_fourier_container> kernel_container_v_dn = Does_Nothing.get_kernel_container_v();
  // wcopreco::OpWaveform dn = Does_Nothing.Deconvolve_One_Wfm(orig, kernel_container_v_dn.at(orig.get_ChannelNum()));




  // wcopreco::OpWaveform aao = AAO.at(0);
  // wcopreco::OpWaveform s2 = S2.at(0);

  // All_At_Once.Remove_Baseline_Leading_Edge(&aao);
  // All_At_Once.Remove_Baseline_Secondary(&aao);

  // All_At_Once.testPlot("Original", orig);
  // All_At_Once.testPlot("Do Nothing Dec", dn);
  // All_At_Once.testPlot("All at Once from Collection", aao);
  // Step_Two.testPlot("Two Step Deconv", s2);
  //
  // double difference;
  // // std::cout << "=================================================\n";
  // for (int i=0; i<1500;i++){
  //   // if (i%100 ==0) {
  //     difference = s2.at(i) - aao.at(i);
  //     // std::cout << orig.at(i) - dn.at(i) << "\n";
  //     // std::cout << orig.at(i) << " Original Waveform\n";
  //     // std::cout << dn.at(i) << " 'Does Nothing' Waveform\n";
  //     if (difference > 0.05){
  //       std::cout << difference/aao.at(i)*100.0 << "   Percent difference relative to all at once\n";
  //       std::cout << aao.at(i) << " All at once method\n";
  //       std::cout << s2.at(i) << " Two Step\n";
  //       std::cout << difference << "   Difference\n\n";
  //     }
  //   // }
  // }




};
