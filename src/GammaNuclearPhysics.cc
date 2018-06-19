
#include "GammaNuclearPhysics.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"

// Processes

#include "G4PhotoNuclearProcess.hh"
#include "G4CascadeInterface.hh"

#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GammaNuclearPhysics::GammaNuclearPhysics(const G4String& name)
:  G4VPhysicsConstructor(name)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GammaNuclearPhysics::~GammaNuclearPhysics()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GammaNuclearPhysics::ConstructProcess()
{
   G4ProcessManager* pManager = G4Gamma::Gamma()->GetProcessManager();
   //
   G4PhotoNuclearProcess* process = new G4PhotoNuclearProcess();
   //
   G4CascadeInterface* bertini = new G4CascadeInterface();
   bertini->SetMaxEnergy(10*GeV);
   process->RegisterMe(bertini);
   //
   pManager->AddDiscreteProcess(process);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
