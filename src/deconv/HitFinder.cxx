#include "HitFinder.h"

namespace wcopreco {

  HitFinder::HitFinder(OpWaveformCollection* merged_beam, OpWaveformCollection* merged_cosmic){
    //Module for hit finding
    //input should be the output of the saturation merger - but I'm not confident in this at all

    //Much of this code can be left the way it is in WC

    //COphitSelection = vector of COphits
    std::vector<COphitSelection> ophits_group;
    std::vector<COphitSelection> left_ophits;

      //I'm confused about what  fop_femch is. It's made in dumpPmtVec of ToyLightReco.
      //but I'm confused about what's in it and how it relates to COphit

      // for (int i=32;i!=fop_femch->size();i++){
      //   // our code: COphit(int ch_no, OpWaveform *wfm, double time, double gain, double gain_err)
      //   // original WC: COphit *op_hit = new COphit(fop_femch->at(i), (TH1S*)fop_wf->At(i), fop_timestamp->at(i) - triggerTime, op_gain->at(fop_femch->at(i)), op_gainerror->at(fop_femch->at(i)));
      //   // My attempt:
      //   OpWaveform wfm_beam = merged_beam.at(i);
      //   int channel = wfm_beam.get_ch_no();
      //   double timestamp = wfm_beam.get_time_from_trigger();
      //   //not sure what gain and gain error should be
      //   COphit *op_hit = new COphit(channel, *wfm_beam, timestamp, op_gain->at(fop_femch->at(i)), op_gainerror->at(fop_femch->at(i)));
      //
      //
      //    op_hits.push_back(op_hit);
      //
      //   if (op_hit->get_type()){ // what type  good baseline ???
      //     bool flag_used = false;
      //     if (ophits_group.size()==0){
      //     	COphitSelection ophits;
      //     	ophits.push_back(op_hit);
      //      //ophits_group = a vector of COphitSelections?
      //     	ophits_group.push_back(ophits);
      //     	flag_used = true;
      //     }
      //     else{
      //     	for (size_t j=0; j!=ophits_group.size();j++){
      //     	  for (size_t k=0; k!= ophits_group.at(j).size(); k++){
      //     	    if (fabs(op_hit->get_time() - ophits_group.at(j).at(k)->get_time()) < 0.1 ){  // time unit??? 0.1 us?
      //     	      ophits_group.at(j).push_back(op_hit);
      //     	      flag_used = true;
      //     	      break;
      //     	    }
      //     	  }
      //     	  if (flag_used)
      //     	    break;
      //     	}
      //     }
      //     if (!flag_used){
      //     	COphitSelection ophits;
      //     	ophits.push_back(op_hit);
      //     	ophits_group.push_back(ophits);
      //     }
      //   }
      //
      //
      //   else{
      //     left_ophits.push_back(op_hit);
      //   }
      // }
      //
      // for (size_t i=0;i!=left_ophits.size();i++){
      //   bool flag_used = false;
      //   for (size_t j=0; j!=ophits_group.size();j++){
      //     for (size_t k=0; k!= ophits_group.at(j).size(); k++){
      //     	if (fabs(left_ophits.at(i)->get_time() - ophits_group.at(j).at(k)->get_time())<0.1){ // time unit??? 0.1 us?
      //     	  ophits_group.at(j).push_back(left_ophits.at(i));
      //     	  flag_used = true;
      //     	  break;
      //     	}
      //     }
      //     if (flag_used)
      // 	     break;
      //   }
      // }
      //
      // for (size_t j=0; j!=ophits_group.size();j++){
      //   Opflash *flash = new Opflash(ophits_group.at(j));
      //   if (flash->get_total_PE()!=0){
      //     cosmic_flashes.push_back(flash);
      //   }
      //   else{
      //     delete flash;
      //   }
      //   //    std::cout << ophits_group.at(j).size() << " " << flash->get_time() << std::endl;
      // }
      //
      // // std::cout << cosmic_flashes.size() << std::endl;
      //
      // // for (auto flash : cosmic_flashes){
      // //   std::cout << flash->get_time() << std::endl;
      // // }
      //
      //
      //
      // for (int i=0;i!=32;i++){
      //   TH1S *hsignal = (TH1S*)fop_wf->At(i);
      //   for (int j=0;j!=1500;j++){
      //     hraw[i]->SetBinContent(j+1,hsignal->GetBinContent(j+1)-2050);
      //   }
      //   gain[i] = op_gain->at(i);
      //   beam_dt[i] = fop_timestamp->at(i) - triggerTime;
      // }

  }

}
