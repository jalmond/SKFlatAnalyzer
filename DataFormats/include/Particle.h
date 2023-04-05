#ifndef Particle_h
#define Particle_h

#include "TLorentzVector.h"
#include <iostream>

using namespace std;

class Particle: public TLorentzVector{

public:

  void  PrintObject(TString label);


  //==== Default Constructor with p4
  Particle();
  //==== TLorentzVector copy constructor.
  Particle(const TLorentzVector& p);
  //==== Copy constructor.
  Particle(const Particle& p);
  //==== Construct from 4-vector components
  Particle(double px, double py, double pz, double e);

  //==== Add Charge
  Particle& operator+=(const Particle& p);

  //==== Assignment operator uses KParticle copy constructor
  Particle& operator=(const Particle& p);

  ~Particle();

  void SetCharge(double q);
  inline double Charge() const {return j_Charge;}

  inline TString PIDToString(int pid) const {

    if(fabs(pid)  < 7){
      TString sign="";
      if (pid < 0) sign="bar";
      
      if(fabs(pid) < 4) return "q";
      if(fabs(pid) == 5) return "b";
      if(fabs(pid) == 4) return "c";

    }

    if(fabs(pid) == 11) return "electron";
    if(fabs(pid) == 12) return "nue";
    if(fabs(pid) == 13) return "muon";
    if(fabs(pid) == 14) return "numu";
    if(fabs(pid) == 15) return "tau";
    if(fabs(pid) == 16) return "nutau";

    if(fabs(pid) == 21) return "g";
    if(fabs(pid) == 22) return "ph";
    if(fabs(pid) == 23) return "Z";
    if(fabs(pid) == 24) return "W";

    if(fabs(pid) == 2212) return "proton";

    if(fabs(pid) == 111) return "pi0";
    if(fabs(pid) == 130) return "K0L";
    
    if(fabs(pid) == 211) return "pi+";
    if(fabs(pid) == 213) return "ρ(770)+";
    
    if(fabs(pid) == 310) return "K0s";
    if(fabs(pid) == 311) return "K0";
    if(fabs(pid) == 321) return "K+";
    if(fabs(pid) == 323) return "K";

    if(fabs(pid) == 421) return "D0";
    if(fabs(pid) == 423) return "D*";
    if(fabs(pid) == 411) return "D+";
    if(fabs(pid) == 425) return "D∗";
    if(fabs(pid) == 431) return "Ds+";
    if(fabs(pid) == 433) return "Ds+";
    
    if(fabs(pid) == 511) return "B0";
    if(fabs(pid) == 513) return "B0*";
    if(fabs(pid) == 521) return "B+";
    if(fabs(pid) == 523) return "B*";
    if(fabs(pid) == 531) return "B0s";
    if(fabs(pid) == 533) return "B0s";

    if(fabs(pid) == 10313) return "K1(1270)0";

    if(fabs(pid) == 221) return "LightMeson";
    if(fabs(pid) == 333) return "LightMeson";
    if(fabs(pid) == 223) return "LightMeson";

    if(fabs(pid) > 4000 && fabs(pid) < 5000) return "CBaryon";
    if(fabs(pid) > 5000 && fabs(pid) < 6000) return "BBaryon";

    return to_string(pid);

  }



  //==== Print four vector
  virtual void Print();

private:
  double j_Charge;

  ClassDef(Particle,1)

};

#endif
