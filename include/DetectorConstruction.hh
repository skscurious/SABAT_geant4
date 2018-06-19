

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4Cache.hh"

class G4Box;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class DetectorMessenger;

const G4int kMaxAbsor = 10;                        // 0 + 9

class G4GlobalMagFieldMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  
  DetectorConstruction();
 ~DetectorConstruction();

public:

  G4Material* 
  MaterialWithSingleIsotope(G4String, G4String, G4double, G4int, G4int);
  
  void SetNbOfAbsor     (G4int);      
  void SetAbsorMaterial (G4int,const G4String&);     
  void SetAbsorThickness(G4int,G4double);
                
  void SetAbsorSizeYZ   (G4double);          
     
  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();
     
public:

  G4int       GetNbOfAbsor()             {return fNbOfAbsor;}     
  G4Material* GetAbsorMaterial (G4int i) {return fAbsorMaterial[i];};
  G4double    GetAbsorThickness(G4int i) {return fAbsorThickness[i];};      
  G4double    GetXfront        (G4int i) {return fXfront[i];};
            
  G4double GetAbsorSizeX()               {return fAbsorSizeX;}; 
  G4double GetAbsorSizeYZ()              {return fAbsorSizeYZ;};
  
  G4double GetWorldSizeX()               {return fWorldSizeX;}; 
  G4double GetWorldSizeYZ()              {return fWorldSizeYZ;}; 
  
  void PrintParameters();
   
private:

  G4int              fNbOfAbsor;
  G4Material*        fAbsorMaterial [kMaxAbsor];
  G4double           fAbsorThickness[kMaxAbsor];
  G4double           fXfront[kMaxAbsor];  

  G4double           fAbsorSizeX;
  G4double           fAbsorSizeYZ;
  
  G4double           fWorldSizeX;
  G4double           fWorldSizeYZ; 
  
  G4Material*        fDefaultMaterial;
  G4Material* 	     fSourceGuideMat; 
  G4Material* 	     fGuideMat;
  G4Material* 	     fSandMaterial;
  G4Material*        fTargetMaterial;
  G4Material*        fDetectorMaterial;
  G4Material*	     fSubmarineMaterial;
  G4Material*	     fSubMaterial;
  G4Material*	     fCollimatorMat;
  G4Material*	     fCollimator2Mat;
  G4Material*	     fCollimator3Mat;
  G4Material*	     fCollimator4Mat;

  G4VPhysicalVolume* fPhysiWorld;
  G4VPhysicalVolume* fPhysiSand;

  DetectorMessenger* fDetectorMessenger;
  G4Cache<G4GlobalMagFieldMessenger*> fFieldMessenger;

private:

  void DefineMaterials();
  void ComputeParameters();
  G4VPhysicalVolume* ConstructVolumes();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

