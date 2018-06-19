#include "StepMaxProcess.hh"
#include "StepMaxMessenger.hh"
#include "HistoManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StepMaxProcess::StepMaxProcess(const G4String& processName, G4ProcessType type)
 : G4VDiscreteProcess(processName,type),fMess(0)
{
  fMaxStep1 = fMaxStep2 = DBL_MAX;
  fApplyMaxStep2 = true;
  fMess = new StepMaxMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StepMaxProcess::~StepMaxProcess() { delete fMess; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool StepMaxProcess::IsApplicable(const G4ParticleDefinition& particle)
{
  return (particle.GetPDGCharge() != 0. && !particle.IsShortLived());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void StepMaxProcess::SetMaxStep1(G4double step) {fMaxStep1 = step;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void StepMaxProcess::ApplyMaxStep2(G4bool value) {fApplyMaxStep2 = value;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double StepMaxProcess::PostStepGetPhysicalInteractionLength( const G4Track&,
                                                   G4double,
                                                   G4ForceCondition* condition)
{
  // condition is set to "Not Forced"
  *condition = NotForced;

  if (fApplyMaxStep2) {
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    G4int ih = 10;
    if (analysisManager->GetH1Activation(ih))
     fMaxStep2 = analysisManager->GetH1Width(ih)*analysisManager->GetH1Unit(ih);
    return fMaxStep2;
  }
  else  return fMaxStep1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VParticleChange* StepMaxProcess::PostStepDoIt(const G4Track& aTrack,
                                                const G4Step&)
{
   // do nothing
   aParticleChange.Initialize(aTrack);
   return &aParticleChange;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
