#ifndef UB_RC_H
#define UB_RC_H

#include "WCOpReco/kernel_fourier.h"

#include <vector>
#include <iostream>
#include <sstream>
#include <cmath>
#include <string>

namespace wcopreco {

  class UB_rc : public kernel_fourier {
  public:
     UB_rc(bool mult_flag, bool bad_ch);
     virtual ~UB_rc() {};

    std::vector<double> Get_wfm(int nbins, float tick_width_ns);
    //std::vector<double> Get_pow_spec(int nbins, float tick_width_ns, std::vector<double>* mag, std::vector<double>* phase){};

    std::string name;
    // 0 = divide 1 = multiply
    bool bad_ch;
    

  protected:

  };

}
#endif