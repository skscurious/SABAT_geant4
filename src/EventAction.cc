
#include "EventAction.hh"

#include "Run.hh"
#include "HistoManager.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(DetectorConstruction* det)
:G4UserEventAction(), fDetector(det)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{
  //energy deposited per event
  for (G4int k=0; k<kMaxAbsor; k++) { fEdepAbsor[k] = 0.0; }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event*)
{
  //get Run
  Run* run = static_cast<Run*>(
             G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  
  //plot energy deposited per event
  //
  G4double TotalEdep(0.);
  for (G4int k=1; k<=fDetector->GetNbOfAbsor(); k++) {
    if (fEdepAbsor[k] > 0.) {
      run->AddEdep(k,fEdepAbsor[k]);
      G4AnalysisManager::Instance()->FillH1(k, fEdepAbsor[k]);
      TotalEdep += fEdepAbsor[k];
    }
  }

  
  if (TotalEdep > 0.) {
    run->AddTotEdep(TotalEdep);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

