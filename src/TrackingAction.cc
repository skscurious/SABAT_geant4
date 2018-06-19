#include "TrackingAction.hh"

//#include "G4Gamma.hh"
#include "Run.hh"
#include "HistoManager.hh"

#include "G4Track.hh"
#include "G4StepStatus.hh"
#include "G4RunManager.hh"
//sks
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::TrackingAction()
:G4UserTrackingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PreUserTrackingAction(const G4Track* track)
{  
  //count secondary particles
G4int N_Gamma = 0; 
G4int N_neutron = 0; 
//const G4StepPoint* endPoint = track->GetStep()->GetPostStepPoint();
//const G4VProcess* process   = endPoint->GetProcessDefinedStep();
//  if (track->GetTrackID() == 1) return; // switched off by SKSHARMA ( WAS ON IN ORIGINAL VERSION)

  // SKS ++++++++++++++++

  G4double EvolTime = track->GetGlobalTime()/ns;

  if(EvolTime<2000) return;
  //-------------------------------
  G4int iabs = track->GetTouchableHandle()->GetCopyNumber();

/////////////////////////////// added by Vahagn //////////////////////////////
  auto volume = track-> GetTouchableHandle() -> GetVolume();
 G4String VolumeName = volume -> GetName();
  G4String name   = track->GetDefinition()->GetParticleName();
  G4double energy = track->GetKineticEnergy();

if(name == "gamma")
 N_Gamma = 1;
if(name == "neutron")
 N_neutron = 1;
if ((N_Gamma == 1)&&(VolumeName == "LaBr3_Ce"))
 {
  G4AnalysisManager::Instance()->FillH1(2, energy);
 }
if ((N_neutron == 1)&&(VolumeName == "LaBr3_Ce"))
 {
  G4AnalysisManager::Instance()->FillH1(4, energy);
 }
  Run* run = static_cast<Run*>(
        G4RunManager::GetRunManager()->GetNonConstCurrentRun());    
  if(iabs > 0) run->ParticleCount(iabs,name,energy);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PostUserTrackingAction(const G4Track* track)
{
  // get Run
  Run* run 
    = static_cast<Run*>(
        G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  
  // where are we ?
  const G4ParticleDefinition* particle = track->GetParticleDefinition();
  G4String name   = particle->GetParticleName();
  G4double energy = track->GetKineticEnergy();
  G4StepStatus status = track->GetStep()->GetPostStepPoint()->GetStepStatus();
  //auto volume = track-> GetTouchableHandle() -> GetVolume();



  //++++++++++++++++++++++++


 /* G4String VolumeName = volume -> GetName();
 	 G4cout <<VolumeName<<G4endl;
 }*/
  //status of primary particle : absorbed, transmited, reflected ?
  if (track->GetTrackID() == 1) {
    G4int flag = 0;
    if (status == fWorldBoundary) {
      if (track->GetMomentumDirection().x() > 0.) flag = 1;
      else                                        flag = 2;
    }
    run->AddTrackStatus(flag);
  }
  
  // keep only emerging particles
  if (status != fWorldBoundary) return;
  run->ParticleCount(0,name,energy);

 ////G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

