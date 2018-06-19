

#ifndef StepMaxProcess_h
#define StepMaxProcess_h 1

#include "globals.hh"
#include "G4VDiscreteProcess.hh"
#include "G4ParticleDefinition.hh"
#include "G4Step.hh"

class StepMaxMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class StepMaxProcess : public G4VDiscreteProcess
{
  public:

     StepMaxProcess(const G4String& processName = "UserMaxStep",
                   G4ProcessType type    = fUserDefined);
    ~StepMaxProcess();

     virtual G4bool IsApplicable(const G4ParticleDefinition&);

     void SetMaxStep1(G4double);
     void ApplyMaxStep2(G4bool);

     virtual G4double PostStepGetPhysicalInteractionLength(const G4Track& track,
                                               G4double previousStepSize,
                                               G4ForceCondition* condition);

     virtual G4VParticleChange* PostStepDoIt(const G4Track&, const G4Step&);

     virtual G4double GetMeanFreePath(const G4Track&,G4double,G4ForceCondition*)
     {return DBL_MAX;};    

  private:

     G4double fMaxStep1;
     G4double fMaxStep2;
     G4bool   fApplyMaxStep2;
     
     StepMaxMessenger* fMess;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

