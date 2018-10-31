#ifndef FLASHFILTERING_H
#define FLASHFILTERING_H

#include "Flashes_beam.h"
#include "Flashes_cosmic.h"
#include "WCOpReco/Opflash.h"

namespace wcopreco{


  class FlashFiltering {
  public:
    FlashFiltering(OpflashSelection &c_flashes, OpflashSelection &b_flashes);
    ~FlashFiltering() {};

    OpflashSelection& get_flashes(){return flashes;};
    OpflashSelection& get_beam_flashes(){return beam_flashes;};
    OpflashSelection& get_cosmic_flashes(){return cosmic_flashes;};

    OpFlashSet& get_all_set(){return all_set;};
    OpFlashSet& get_beam_set(){return beam_set;};
    OpFlashSet& get_cosmic_set(){return cosmic_set;};

    void sort_flashes();
    void update_pmt_map();

  protected:

    OpflashSelection flashes;
    OpflashSelection beam_flashes;
    OpflashSelection cosmic_flashes;
    //all set is of the form std::set<Opflash*, OpFlashCompare> OpFlashSet
    OpFlashSet all_set;
    OpFlashSet beam_set;
    OpFlashSet cosmic_set;

  };

}

#endif