#include "SteppingAction.hh"
#include "DetectorConstruction.hh"
#include "EventAction.hh"
#include "Run.hh"
#include "HistoManager.hh"

#include "G4SteppingManager.hh"
#include "G4RunManager.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(DetectorConstruction* det, EventAction* event)
:G4UserSteppingAction(),fDetector(det), fEventAction(event)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();   
void SteppingAction::UserSteppingAction(const G4Step* step)
{

  const G4ParticleDefinition* particle = step->GetTrack()->GetParticleDefinition();
  G4String name   = particle->GetParticleName();
  G4double energy = step->GetTrack()->GetKineticEnergy();
  auto volume = step->GetTrack()-> GetTouchableHandle() -> GetVolume();
  G4String VolumeName = volume -> GetName();
  if ((name == "neutron")&&(VolumeName == "LaBr3_Ce")&&(energy!=0)){
  	//G4cout <<name<<"  "<<energy<<G4endl;  
	G4AnalysisManager::Instance()->FillH1(3, energy);
  }
  // count processes
  // 
  const G4StepPoint* endPoint = step->GetPostStepPoint();
  const G4VProcess* process   = endPoint->GetProcessDefinedStep();
  Run* run = static_cast<Run*>(
        G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  run->CountProcesses(process);
  
 G4double edep = step->GetTotalEnergyDeposit();
 if (edep <= 0.) return;
 
 //G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();     
 
 //longitudinal profile of deposited energy
 //randomize point of energy deposition
 //
 G4StepPoint* prePoint  = step->GetPreStepPoint();
 G4StepPoint* postPoint = step->GetPostStepPoint(); 
 G4ThreeVector P1 = prePoint ->GetPosition();
 G4ThreeVector P2 = postPoint->GetPosition(); 
 G4ThreeVector point = P1 + G4UniformRand()*(P2 - P1);
 G4double x = point.x();
 G4double xshifted = x + 0.5*fDetector->GetAbsorSizeX();  
 analysisManager->FillH1(10, xshifted, edep);

/*
const G4ParticleDefinition* particle = step->GetTrack()->GetParticleDefinition();
auto volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
G4String VolumeName = volume->GetName();
  G4String name   = particle->GetParticleName();
  G4double energy = step->GetTrack()->GetKineticEnergy(); 
G4String ProcName= process->GetProcessName();*/
// G4cout<<VolumeName<<"\t\t\t\t"<<name<<"\t\t\t\t"<<energy<<"\t\t\t\t"<<ProcName<<G4endl;

 //total energy deposit in absorbers
 //
 G4int iabs = prePoint->GetTouchableHandle()->GetCopyNumber(0); 
 if (iabs > 0) fEventAction->AddEdep(iabs, edep);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

