#ifndef FLASHES_COSMIC_H
#define FLASHES_COSMIC_H

//data
#include "WCOpReco/OpWaveform.h"
#include "WCOpReco/OpWaveformCollection.h"
#include "WCOpReco/EventOpWaveforms.h"
#include "WCOpReco/Opflash.h"
#include "HitFinder_cosmic.h"

#include "WCOpReco/Config_Opflash.h"

namespace wcopreco{
  //class to find flashes in cosmic data from hits
  //makes use of the OpFlash class

  class Flashes_cosmic {
  public:
    Flashes_cosmic(std::vector<COphitSelection> *ophits_group, const Config_Opflash &configOpF);
    ~Flashes_cosmic() {};

    OpflashSelection get_cosmic_flashes(){return cosmic_flashes;};

  protected:

    OpflashSelection cosmic_flashes;
    Config_Opflash _cfgOpF;

  };

}

#endif
