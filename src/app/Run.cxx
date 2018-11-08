#include "Run.h"

namespace wcopreco{

  wcopreco::Run::Run(){

      //Set the filepath
      std::string file = "src/data/celltree.root";
      std::cout << "\n\nFilepath is set to:   " << file << std::endl;

      //Open the reader, choose event number, create the UBEventWaveform _UB_Ev_wfm
      wcopreco::DataReader reader(&file);
      wcopreco::UBEventWaveform _UB_Ev_wfm;

      //change with a config parameter eventually?
      int EVENT_NUM =51;

      _UB_Ev_wfm = reader.Reader(EVENT_NUM);
      std::vector<float> op_gain = _UB_Ev_wfm.get_op_gain();
      std::vector<float> op_gainerror = _UB_Ev_wfm.get_op_gainerror();

      const Config_Saturation_Merger cfg_SM;
      const Config_Deconvolver cfg_DC;
      const Config_Hitfinder_Beam cfg_HB;
      //create the merger and then get a Merged UBEventWaveform UB_Ev_Merged
      wcopreco::Saturation_Merger *merger = new wcopreco::Saturation_Merger(_UB_Ev_wfm, cfg_SM);
      wcopreco::OpWaveformCollection merged_beam = merger->get_merged_beam(); //This is inside UB_Ev_Merged
      wcopreco::OpWaveformCollection merged_cosmic = merger->get_merged_cosmic(); //This is inside UB_Ev_Merged
      wcopreco::UBEventWaveform UB_Ev_Merged = merger->get_merged_UB_Ev();

      //Construct the vector of kernel containers (one container per channel)
      const Config_UB_rc cfg_RC;
      const Config_UB_spe cfg_SPE;
      std::vector<wcopreco::kernel_fourier_container> kernel_container_v;
      kernel_container_v.resize(32);
      wcopreco::UB_rc *rc_good_ch = new wcopreco::UB_rc(true, false, cfg_RC);
      wcopreco::UB_rc *rc_bad_ch = new wcopreco::UB_rc(true, true, cfg_RC);
      for (int i =0 ; i<32; i++){
        wcopreco::UB_spe *spe = new wcopreco::UB_spe(true, op_gain.at(i), cfg_SPE); //Place UB_spe on heap, so object not deleted
        kernel_container_v.at(i).add_kernel(spe);
        if (i == 28){
          kernel_container_v.at(i).add_kernel(rc_bad_ch);
        }
        else{
          kernel_container_v.at(i).add_kernel(rc_good_ch);
        }
      }

      //do beam hitfinding
      wcopreco::HitFinder_beam hits_found_beam(merged_beam, kernel_container_v, cfg_HB, cfg_DC);

      // do beam flash finding
      std::vector<double> totPE_v = hits_found_beam.get_totPE_v();
      std::vector<double> mult_v =hits_found_beam.get_mult_v();
      std::vector<double> l1_totPE_v =hits_found_beam.get_l1_totPE_v();
      std::vector<double> l1_mult_v = hits_found_beam.get_l1_mult_v();
      std::vector< std::vector<double> > decon_vv = hits_found_beam.get_decon_vv();
      double beam_start_time =merged_beam.at(0).get_time_from_trigger();

      const Config_FlashesBeam cfg_FB;
      const Config_Opflash cfg_OpF;
      wcopreco::Flashes_beam flashfinder_beam( &totPE_v,
                                              &mult_v,
                                              &l1_totPE_v,
                                              &l1_mult_v,
                                              decon_vv,
                                              beam_start_time,
                                              cfg_FB,
                                              cfg_OpF);

      flashes_beam = flashfinder_beam.get_beam_flashes();
      // std::cout << "\n\n" << flashes_beam.size() << " Beam Flashes in Event\n";

      // //Create the Hitfinder for COSMICS
      const Config_COpHit cfg_COpH;
      const Config_Hitfinder_Cosmic cfg_HC;
      wcopreco::HitFinder_cosmic hits_found(&merged_cosmic, &op_gain, &op_gainerror, cfg_HC, cfg_COpH);

      //flashes for cosmics
      std::vector<COphitSelection>  hits = hits_found.get_ophits_group();
      wcopreco::Flashes_cosmic flashfinder_cosmic(&hits, cfg_OpF);
      flashes_cosmic = flashfinder_cosmic.get_cosmic_flashes();
      // std::cout << flashes_cosmic.size() << " Cosmic Flashes in Event\n";

      // std::cout << flashes.size() << " flashes were found in the cosmic selection\n";

      //flash filtering
      const Config_FlashFiltering cfg_FF;
      wcopreco::FlashFiltering flashesfiltered(&flashes_cosmic, &flashes_beam, cfg_FF);
      flashes = flashesfiltered.get_flashes();
      // std::cout << flashes.size() << " Matched Flashes in Event\n\n";

      for (int i =0 ; i<flashes.size(); i++) {
          if (flashes.at(i)->get_type() == 2) std::cout << flashes.at(i)->get_total_PE() << "\n";
      }

      delete merger;


  }

}
