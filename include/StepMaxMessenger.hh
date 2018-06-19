

#ifndef StepMaxMessenger_h
#define StepMaxMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class StepMaxProcess;
class G4UIcmdWithABool;
class G4UIcmdWithADoubleAndUnit;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class StepMaxMessenger: public G4UImessenger
{
  public:
    StepMaxMessenger(StepMaxProcess*);
   ~StepMaxMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);
    
  private:
    StepMaxProcess*            fStepMax;
    G4UIcmdWithADoubleAndUnit* fStepMax1Cmd;    
    G4UIcmdWithABool*          fStepMax2Cmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
