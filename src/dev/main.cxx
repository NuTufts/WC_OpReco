#include "WCOpReco/OpWaveform.h"
#include "WCOpReco/OpWaveformCollection.h"
#include "WCOpReco/EventOpWaveforms.h"
#include "WCOpReco/UBEventWaveform.h"
#include "WCOpReco/datareader_j.h"
#include "WCOpReco/DataReader.h"


//root includes
#include "TH1S.h"
#include "TF1.h"
#include "TVirtualFFT.h"
#include "TTree.h"
#include "TFile.h"

#include <iostream>
#include <sstream>

int main(){

  std::cout << "Hello world" << std::endl;

  wcopreco::DataReader reader;

  //std::string file = "/home/jmills/workdir/WCOpReco/src/data/celltree.root";
  std::string file = "../data/celltree.root";
  std::cout << "Filepath is set to:   " << file << std::endl;
  reader.Reader(file);
  //reader_ub.read_in_data(file);
};
