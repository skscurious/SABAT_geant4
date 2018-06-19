
#include "StepMaxMessenger.hh"

#include "StepMaxProcess.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StepMaxMessenger::StepMaxMessenger(StepMaxProcess* stepM)
:G4UImessenger(),fStepMax(stepM),
 fStepMax1Cmd(0),    
 fStepMax2Cmd(0)
{
  fStepMax1Cmd = new G4UIcmdWithADoubleAndUnit("/testhadr/stepMax",this);
  fStepMax1Cmd->SetGuidance("Set max allowed step length");
  fStepMax1Cmd->SetParameterName("mxStep1",false);
  fStepMax1Cmd->SetRange("mxStep1>0.");
  fStepMax1Cmd->SetUnitCategory("Length");
   
  fStepMax2Cmd = new G4UIcmdWithABool("/testhadr/applyAutomaticStepMax",this);
  fStepMax2Cmd->SetGuidance("apply StepMax computed from histograms");
  fStepMax2Cmd->SetParameterName("mxStep2",true);
  fStepMax2Cmd->SetDefaultValue(true);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StepMaxMessenger::~StepMaxMessenger()
{
  delete fStepMax1Cmd;
  delete fStepMax2Cmd;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void StepMaxMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 
  if (command == fStepMax1Cmd)
    { fStepMax->SetMaxStep1(fStepMax1Cmd->GetNewDoubleValue(newValue));}
    
  if (command == fStepMax2Cmd)
    { fStepMax->ApplyMaxStep2(fStepMax2Cmd->GetNewBoolValue(newValue));}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
