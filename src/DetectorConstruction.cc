//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
//
// $Id: DetectorConstruction.cc 77288 2013-11-22 10:52:58Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Trap.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4RotationMatrix.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4UniformMagField.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4UImanager.hh"
#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"
#include "G4RunManager.hh"
#include "G4VisAttributes.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
:G4VUserDetectorConstruction(),fDefaultMaterial(0),fSourceGuideMat(0),fGuideMat(0),fSandMaterial(0),fTargetMaterial(0),fDetectorMaterial(0),fSubmarineMaterial(0),fSubMaterial(0),fCollimatorMat(0),fCollimator2Mat(0),fCollimator3Mat(0),fPhysiWorld(0),fPhysiSand(0),
 fDetectorMessenger(0)
{
  // default parameter values of the absorbers
  fNbOfAbsor = 1;
  fAbsorThickness[0] = 0*mm;        //dummy, for initialization   
  fAbsorThickness[1] = 5*cm;  
  fAbsorSizeYZ = 7.5*mm;

  ComputeParameters();

  // materials
  DefineMaterials();
  SetAbsorMaterial(1,"LaBr3_Ce");

  // create commands for interactive definition of the calorimeter
  fDetectorMessenger = new DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
  delete fDetectorMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  return ConstructVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{
  G4NistManager* man = G4NistManager::Instance();
  man->FindOrBuildMaterial("G4_AIR");

  G4Element* H = man->FindOrBuildElement("H");
  G4Element* Li = man->FindOrBuildElement("Li");
  G4Element* B = man->FindOrBuildElement("B");
  G4Element* C = man->FindOrBuildElement("C");
  G4Element* N  = man->FindOrBuildElement("N");
  G4Element* O = man->FindOrBuildElement("O");
  G4Element* F = man->FindOrBuildElement("F"); 
  G4Element* Na = man->FindOrBuildElement("Na");
  G4Element* Cl = man->FindOrBuildElement("Cl");
  G4Element* Fe = man->FindOrBuildElement("Fe");
  G4Element* Al = man->FindOrBuildElement("Al");
  G4Element* Pb = man->FindOrBuildElement("Pb");
  G4Element* S = man->FindOrBuildElement("Si");
  G4Element* Si = man->FindOrBuildElement("S");
  G4Element* La = man->FindOrBuildElement("La");
  G4Element* Br = man->FindOrBuildElement("Br");
  G4Element* Ce = man->FindOrBuildElement("Ce");
  G4Element* Cd = man->FindOrBuildElement("Cd");

//////////////////////////// Air
G4int ncomponents;
G4double density,temperature,pressure,fractionmass; 
	
				density = 0.001225*g/cm3;
       				pressure = 98658.96*pascal;
				temperature = 273*kelvin; 
				G4Material* Air = new G4Material("Air", density, ncomponents=2, kStateGas, temperature, pressure);
				Air->AddElement(N, fractionmass=79.*perCent);
				Air->AddElement(O, fractionmass=21.*perCent);
				G4NistManager *nist_man = G4NistManager::Instance();
				G4Material *AIR_mat = nist_man->FindOrBuildMaterial("Air");
/////////////////////////////////////////////////
  G4Material* H2O = 
  new G4Material("Water", 1.000*g/cm3, 2);
  H2O->AddElement(H, 2);
  H2O->AddElement(O, 1);
  H2O->GetIonisation()->SetMeanExcitationEnergy(78.0*eV);
 
  G4Material* H3BO3 = 
  new G4Material("Boric_Acid", 1.44*g/cm3, 3);
  H3BO3->AddElement(H, 3);
  H3BO3->AddElement(B, 1);
  H3BO3->AddElement(O, 3);
  //GetIonisation()->SetMeanExcitationEnergy(78.0*eV);
 
  G4Material* NaCl = 
  new G4Material("Salt", 2.165*g/cm3, 2);
  NaCl->AddElement(Na, 1);
  NaCl->AddElement(Cl, 1);

  G4Material* LiF = 
  new G4Material("Lithium_Fluoride", 2.64*g/cm3, 2);
  LiF->AddElement(Li, 1);
  LiF->AddElement(F, 1);

  // example of vacuum				
 /* G4double density     = universe_mean_density;    //from PhysicalConstants.h
  G4double pressure    = 3.e-18*pascal;
  G4double temperature = 2.73*kelvin;
  G4Material* Galactic =   
  new G4Material("Galactic", 1., 1.008*g/mole, density,
                             kStateGas,temperature,pressure);
            */                 
  //fDefaultMaterial = Galactic;

	//SeaWater
  	G4Material* SeaWater = 
  	new G4Material("SeaWater", 1.027*g/cm3, 2);
  	SeaWater->AddMaterial(H2O,98.8*perCent);
  	SeaWater->AddMaterial(NaCl,1.2*perCent);\

    //  G4cout << *(G4Material::GetMaterialTable()) << G4endl;

	// LaBr3
	G4Material* LaBr3 = new G4Material("LaBr3", 5.07*g/cm3, 2);
	LaBr3->AddElement(La, 1);
	LaBr3->AddElement(Br, 3);

	// LaBr3_Ce
	G4Material* LaBr3_Ce = new G4Material("LaBr3_Ce", 5.08*g/cm3, 2);
	LaBr3_Ce->AddMaterial(LaBr3, 99.5*perCent);
	LaBr3_Ce->AddElement(Ce, 0.5*perCent);

	//  Mustard Gas 
	G4Material* C4H8Cl2S = new G4Material("Mustard_Gas", 1.27*g/cm3, 4);
	C4H8Cl2S->AddElement(C,4);
	C4H8Cl2S->AddElement(H,8);
	C4H8Cl2S->AddElement(Cl,2);
	C4H8Cl2S->AddElement(S,1);

	// SiO2 
	G4Material* SiO2 = new G4Material("Silicon_Dioxide", 2.196 *g/cm3, 2);
	SiO2->AddElement(Si,1);
	SiO2->AddElement(O,2);
	// Fe 
	G4Material* Iron = new G4Material("Iron", 7.850 *g/cm3, 1);
	Iron->AddElement(Fe,1);
	// Lead
	G4Material* Lead = new G4Material("Lead", 11.34 *g/cm3, 1);
	Lead->AddElement(Pb,1);
	// Paraffin
	G4Material* Paraffin = man->FindOrBuildMaterial("G4_PARAFFIN");
	//Aluminum
	G4Material* Aluminum = new G4Material("Al", 2.7 *g/cm3, 1);
	Aluminum->AddElement(Al,1);
    //Paraffin_Boric_Acid
  	G4Material* Paraffin_Boric_Acid = 
  	new G4Material("Paraffin_Boric_Acid", 0.972*g/cm3, 2);
  	Paraffin_Boric_Acid->AddMaterial(Paraffin,90*perCent);
  	Paraffin_Boric_Acid->AddMaterial(H3BO3,10*perCent);

	//Cadmium
	G4Material* Cadmium = new G4Material("Cadmium", 8.65 *g/cm3, 1);
	Cadmium->AddElement(Cd,1);
	//Carbon(graphite)
	G4Material* Carbon = new G4Material("Carbon", 2.3 *g/cm3, 1);
	Carbon->AddElement(C,1);
       
	//Low_Carbon_Stainless_Steel
	G4Material* Low_Carbon_Steel = new G4Material("Low_Carbon_Steel", 7.8499*g/cm3, 2);
  	Low_Carbon_Steel->AddMaterial(Iron,99.75*perCent);
  	Low_Carbon_Steel->AddMaterial(Carbon,0.25*perCent);

  fGuideMat = Low_Carbon_Steel;
  fSourceGuideMat = Low_Carbon_Steel;

  fCollimatorMat = Low_Carbon_Steel;//Lead;//Paraffin_Boric_Acid;Paraffin;//Iron;
  fCollimator2Mat = Paraffin_Boric_Acid;//LiF;//Aluminum;
  fCollimator3Mat = Paraffin_Boric_Acid;//LiF;//Paraffin_Boric_Acid;
  fCollimator4Mat = Cadmium;
  fSubmarineMaterial = Low_Carbon_Steel;

  fSubMaterial = AIR_mat;
  fSandMaterial = SeaWater;//SiO2;
  fDefaultMaterial = SeaWater;
  fTargetMaterial = C4H8Cl2S;//SiO2;//C4H8Cl2S;


  fDetectorMaterial = LaBr3;   // Forced on the messenger input sks

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* DetectorConstruction::MaterialWithSingleIsotope( G4String name,
                           G4String symbol, G4double density, G4int Z, G4int A)
{
 // define a material from an isotope
 //
 G4int ncomponents;
 G4double abundance, massfraction;

 G4Isotope* isotope = new G4Isotope(symbol, Z, A);
 
 G4Element* element  = new G4Element(name, symbol, ncomponents=1);
 element->AddIsotope(isotope, abundance= 100.*perCent);
 
 G4Material* material = new G4Material(name, density, ncomponents=1);
 material->AddElement(element, massfraction=100.*perCent);

 return material;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ComputeParameters()
{
  // Compute total thickness of absorbers
  fAbsorSizeX = 0.;
  for (G4int iAbs=1; iAbs<=fNbOfAbsor; iAbs++) {
    fAbsorSizeX += fAbsorThickness[iAbs];
  }

  fWorldSizeX = fWorldSizeYZ = 4.01*m;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::ConstructVolumes()
{
  // complete the Calor parameters definition
  ComputeParameters();

  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  static const double Pi = 3.14159265358979323846;

  //
  // World
  //
  G4Box* solidWorld =
    new G4Box("World",                                             //name
               fWorldSizeX/2,fWorldSizeYZ/2,fWorldSizeYZ/2);       //size

  G4LogicalVolume* logicWorld =
    new G4LogicalVolume(solidWorld,              //solid
                        fDefaultMaterial,        //material
                        "World");                //name

  fPhysiWorld = 
    new G4PVPlacement(0,                        //no rotation
                      G4ThreeVector(),          //at (0,0,0)
                      logicWorld,               //logical volume
                      "World",                  //name
                       0,                       //mother volume
                       false,                   //no boolean operation
                       0);                      //copy number

/////////////////////////////// Added by Vahagn /////////////////////////////////////////

 // rotation
        G4RotationMatrix* rotation = new G4RotationMatrix();
         //rotation->rotateZ(0*deg);
	 //rotation->rotateX(0*deg);
	 rotation->rotateY(90*deg);
G4RotationMatrix* rotation1 = new G4RotationMatrix();
         
	 rotation1->rotateX(90*deg);
	
G4RotationMatrix* rotation2 = new G4RotationMatrix();
         rotation2->rotateZ(0*deg);
	 rotation2->rotateX(-90*deg);
	 rotation2->rotateY(180*deg);
G4RotationMatrix* rotation3 = new G4RotationMatrix();
      //   rotation->rotateZ(3*deg);
     rotation3->rotateX(55*deg);
     rotation3->rotateY(90*deg);
 // SubMarine (Steel)
  G4Box* solidSubmarineVolume = new G4Box("SubmarineVolume",1.5*m+0.5*cm,1*m+0.5*cm,1.5*m+0.5*cm);//300 × 200 × 300 cm 3 and thickness is 0.5 cm
  G4LogicalVolume* logicSubmarineVolume = new G4LogicalVolume(solidSubmarineVolume,
                                                       fSubmarineMaterial,
                                                       "SubmarineVolume");
  new G4PVPlacement(rotation,G4ThreeVector(0.,60*cm,0),
                         logicSubmarineVolume,"SubmarineVolume",
                         logicWorld,false,0); 
 // Sub(Air) 
  G4Box* solidSubVolume = new G4Box("SubVolume",1.5*m,1*m,1.5*m);//300 × 200 × 300 cm 3
  G4LogicalVolume* logicSubVolume = new G4LogicalVolume(solidSubVolume,
                                                       fSubMaterial,
                                                       "SubVolume");
  new G4PVPlacement(rotation,G4ThreeVector(0.,0.,0.),
                         logicSubVolume,"SubVolume",
                         logicSubmarineVolume,false,0);

 // G4double Pi = 3.14159265358979324;

//SourceGuide
  G4Cons* solidSourceGuide = new G4Cons("SourceGuide",10*cm,10.5*cm,10*cm,10.5*cm,5.*cm,0.,twopi);
  G4LogicalVolume* logicSourceGuide = new G4LogicalVolume(solidSourceGuide,
                                                       fSourceGuideMat,
                                                       "SourceGuide");
  new G4PVPlacement(rotation1,G4ThreeVector(0.,-45.*cm,0),
                         logicSourceGuide,"SourceGuide",
                         logicWorld,false,0);
  //Added by sks
  G4VisAttributes* sourceGVisAtt= new G4VisAttributes(G4Colour(1.0,0.0,1.0));
  sourceGVisAtt->SetForceSolid(true);
  logicSourceGuide->SetVisAttributes(sourceGVisAtt);
   
  G4Cons* solidInsideSourceGuide = new G4Cons("InsideSourceGuide",0*cm,10*cm,0*cm,10*cm,5.*cm,0.,twopi);
  G4LogicalVolume* logicInsideSourceGuide = new G4LogicalVolume(solidInsideSourceGuide,
                                                       fSubMaterial,
                                                       "InsideSourceGuide");
  new G4PVPlacement(0,G4ThreeVector(0.,0.,0),
                         logicInsideSourceGuide,"InsideSourceGuide",
                         logicSourceGuide,false,0);
  //Added by sks
  G4VisAttributes* sourceInsVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  sourceInsVisAtt->SetForceWireframe(true);
  logicInsideSourceGuide->SetVisAttributes(sourceInsVisAtt);


//ParticleGuide

  
  G4Trap* solidParticleGuide = new G4Trap("ParticleGuide",8.5*cm,71*deg,90*deg,8.3*cm,7.5*cm,7.5*cm,0*deg,8.3*cm,7.5*cm,7.5*cm,0*deg);
  //G4Box* solidParticleGuide = new G4Box("ParticleGuide",8*cm,10*cm,10*cm);
  G4LogicalVolume* logicParticleGuide = new G4LogicalVolume(solidParticleGuide,
                                                       fGuideMat,
                                                       "ParticleGuide");
  new G4PVPlacement(rotation2,G4ThreeVector(0.,-41.5*cm,39*cm+5*cm),
                         logicParticleGuide,"ParticleGuide",
                         logicWorld,false,0);
  
  G4VisAttributes* particleVisAtt= new G4VisAttributes(G4Colour(0.0,0.0,0.0));
  particleVisAtt->SetForceWireframe(true);
  logicParticleGuide->SetVisAttributes(particleVisAtt);
  
  // Particle Guide -inner

  G4Trap* solidInsideParticleGuide = new G4Trap("InsideParticleGuide",8.5*cm,71*deg,90*deg,8.3*cm,6.5*cm,6.5*cm,0*deg,8.3*cm,6.5*cm,6.5*cm,0*deg);

 // G4Trap* solidInsideParticleGuide = new G4Trap("InsideParticleGuide",28*cm,42*deg,90*deg,10*cm,7.75*cm,7.75*cm,0*deg,10*cm,7.75*cm,7.75*cm,0*deg);
  
  G4LogicalVolume* logicInsideParticleGuide = new G4LogicalVolume(solidInsideParticleGuide,
                                                       fSubMaterial,
                                                       "InsideParticleGuide");
  new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),
                         logicInsideParticleGuide,"InsideParticleGuide",
                         logicParticleGuide,false,0);
  
  
  G4VisAttributes* partInsVisAtt= new G4VisAttributes(G4Colour(0.0,0.0,1.0));
  partInsVisAtt->SetForceWireframe(true);
  logicInsideParticleGuide->SetVisAttributes(partInsVisAtt);
  

// SAND (SiO2)

  G4Box* solidSANDVolume = new G4Box("SAND",2*m,50*cm,2*m); 
  
  G4LogicalVolume* logicSANDVolume = new G4LogicalVolume(solidSANDVolume,
                                                       fSandMaterial,
                                                       "SAND");
  fPhysiSand = new G4PVPlacement(0,G4ThreeVector(0.,-150.5*cm,0.),logicSANDVolume,"SAND",logicWorld,false,0); 
  
  //Added by sushil
  
  G4VisAttributes* sandVisAtt= new G4VisAttributes(G4Colour(0.5,0.5,0.5));
  sandVisAtt->SetForceSolid(true);
  logicSANDVolume->SetVisAttributes(sandVisAtt);

  
  

// Target (Mustard Gas)

  G4Box* solidTGVolume = new G4Box("TargetVolume",97*cm,25*cm,25*cm);
  
  G4LogicalVolume* logicTGVolume = new G4LogicalVolume(solidTGVolume,
                                                       fTargetMaterial,
                                                       "TargetVolume");
  new G4PVPlacement(rotation,G4ThreeVector(0.,-75.*cm,0.),
                         logicTGVolume,"TargetVolume",
                         /*logicSANDVolume*/logicWorld,false,0);
  
  
  G4VisAttributes* mustardVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,0));
  mustardVisAtt->SetForceSolid(true);
  logicTGVolume->SetVisAttributes(mustardVisAtt);

//++++++++++++++++++++++++++++ COLLIMATOR SECTION ++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Collimator (Iron)

//G4Box* solidColimatorVolume = new G4Box("CollimatorVolume",10*cm,7.5*cm,7.5*cm); //15 × 15 × 20 cm 3
  /*G4Tubs* solidCollimatorVolume = new G4Tubs("CollimatorVolume",2*cm,22*cm, 22*cm,0.,twopi);
  G4LogicalVolume* logicCollimatorVolume = new G4LogicalVolume(solidCollimatorVolume,
                                                       fCollimatorMat,
                                                        "CollimatorVolume");
  new G4PVPlacement(rotation1,G4ThreeVector(0.,5*cm,0.),
                         logicCollimatorVolume,"CollimatorVolume",
                         logicWorld,false,0);  */

  //G4Tubs* solidCollimatorVolume = new G4Tubs("CollimatorVolume",2*cm,/*CollimatorOuterRadius*/22*cm, 10*cm,0.,twopi);
/*sks 27.03.2018
  // Low Carbon Steel
  G4Box* solidCollimatorVolume = new G4Box("CollimatorVolume",23*cm,10*cm+4*cm,24*cm);
  G4LogicalVolume* logicCollimatorVolume = new G4LogicalVolume(solidCollimatorVolume,
                                                       fCollimatorMat,
                                                        "CollimatorVolume");
  new G4PVPlacement(rotation1,G4ThreeVector(0.,-75*cm,-10.5*cm-5*cm),
                         logicCollimatorVolume,"CollimatorVolume",
                         logicSubVolume,false,0); 
  
  G4VisAttributes* colFirVisAtt= new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  colFirVisAtt->SetForceSolid(true);
  logicCollimatorVolume->SetVisAttributes(colFirVisAtt);
  
  // Boric acid
  G4Box* solidCollimator2Volume = new G4Box("Collimator2Volume",23*cm,7.5*cm,24*cm);
  G4LogicalVolume* logicCollimator2Volume = new G4LogicalVolume(solidCollimator2Volume,
                                                       fCollimator2Mat,
                                                        "Collimator2Volume");
  new G4PVPlacement(rotation1,G4ThreeVector(0.,-75*cm,-28*cm-9*cm),
                         logicCollimator2Volume,"Collimator2Volume",
                         logicSubVolume,false,0); 
  
  G4VisAttributes* colSecVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,0.0));
  colSecVisAtt->SetForceSolid(true);
  logicCollimator2Volume->SetVisAttributes(colSecVisAtt);

  */
  // Boric Acid
  /*G4Box* solidCollimator3Volume = new G4Box("Collimator3Volume",25*cm,2.5*cm,25*cm);
  G4LogicalVolume* logicCollimator3Volume = new G4LogicalVolume(solidCollimator3Volume,
                                                       fCollimator3Mat,
                                                        "Collimator3Volume");
  new G4PVPlacement(rotation1,G4ThreeVector(0.,-75*cm,-37.5*cm),
                         logicCollimator3Volume,"Collimator3Volume",
                         logicSubVolume,false,0);  
  
  G4VisAttributes* colThreeVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,0.0));
  colThreeVisAtt->SetForceSolid(true);
  logicCollimator3Volume->SetVisAttributes(colThreeVisAtt);*/
  /*
  // Cadmium
  G4Box* solidCollimator4Volume = new G4Box("Collimator4Volume",25*cm,5*cm,25*cm);
  G4LogicalVolume* logicCollimator4Volume = new G4LogicalVolume(solidCollimator4Volume,
                                                       fCollimator4Mat,
                                                        "Collimator4Volume");
  new G4PVPlacement(rotation1,G4ThreeVector(0.,-75*cm,-40.5*cm-10*cm),
                         logicCollimator4Volume,"Collimator4Volume",
                         logicSubVolume,false,0); 
  
  G4VisAttributes* colFourVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  colFourVisAtt->SetForceSolid(true);
  logicCollimator4Volume->SetVisAttributes(colFourVisAtt);
  */
  
  // Collimator ---------------------- ENDS -------------------------------------------
  //-----------------------------------------------------------------------------------
  
  // Source Virtualization sks
  // Source virtualization

      G4double pRmin=0.*cm;
      G4double pRmax=2.5*cm;
      G4double pSPhi=0*deg;
      G4double pDPhi=2*Pi;
      G4double pSTheta=0*deg;
      G4double pDTheta=Pi;

      G4Sphere* solidShape1 =
              new G4Sphere("SOURCE",
                           pRmin, pRmax, pSPhi, pDPhi, pSTheta, pDTheta);

      G4LogicalVolume* logicShape1 =
              new G4LogicalVolume(solidShape1,         //its solid
                                  fSubmarineMaterial,          //its material
                                  "SOURCE");           //its name

      new G4PVPlacement(0,                       //no rotation
                        G4ThreeVector(0,-97*cm,0),                    //at position
                        logicShape1,             //its logical volume
                        "SOURCE",                //its name
                        logicSubVolume,
                        false,                   //no boolean operation
                        0);                       //copy number
      //GEOMETRY +++++++++++++++++++++++++++++++++++++++++++
            G4VisAttributes * sourceVisAtt = new G4VisAttributes(G4Colour(1.,1.,0.));
            sourceVisAtt->SetForceWireframe(true);
            logicShape1->SetVisAttributes(sourceVisAtt);



//Detector (LaBr3:Ce)
  // Absorbers
  //
  fXfront[0] = -0.5*fAbsorSizeX;
  //
  
  for (G4int k=1; k<=fNbOfAbsor; k++) {
    G4Material* material = fAbsorMaterial[k];
    G4String matname = material->GetName();
      
    G4Box* solidAbsor = new G4Box(matname,3.81*cm,3.81*cm,3.81*cm);
    G4LogicalVolume* logicAbsor =
      new G4LogicalVolume(solidAbsor,           // solid
                          material,             // material
                          matname);             // name
                                     
  
    G4ThreeVector position = G4ThreeVector(0.,-1*m+6.3*cm,-60.*cm-1*cm-5*cm);

  
      new G4PVPlacement(rotation3,        //no rotation
                        position,        //position
                        logicAbsor,            //logical volume        
                        matname,               //name
                        logicSubVolume,  //mother
                        false,                 //no boulean operat
                        k);                    //copy number
      
  G4VisAttributes* solidVisAtt= new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  solidVisAtt->SetForceSolid(true);
  logicAbsor->SetVisAttributes(solidVisAtt);
      
  }
  
  

  PrintParameters();

  //always return the physical World
  //
  return fPhysiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::PrintParameters()
{
  G4cout << "\n-------------------------------------------------------------"
         << "\n ---> The Absorber is " << fNbOfAbsor << " layers of:";
  for (G4int i=1; i<=fNbOfAbsor; i++)
     {
      G4cout << "\n \t" << std::setw(12) << fAbsorMaterial[i]->GetName() <<": "
              << std::setw(6) << G4BestUnit(fAbsorThickness[i],"Length");
     }
  G4cout << "\n-------------------------------------------------------------\n"
         << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetNbOfAbsor(G4int ival)
{
  // set the number of Absorbers
  //
  if (ival < 1 || ival > (kMaxAbsor-1))
    { G4cout << "\n ---> warning from SetfNbOfAbsor: "
             << ival << " must be at least 1 and and most " << kMaxAbsor-1
             << ". Command refused" << G4endl;
      return;
    }
  fNbOfAbsor = ival;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetAbsorMaterial(G4int iabs,const G4String& material)
{
  // search the material by its name
  //
  if (iabs > fNbOfAbsor || iabs <= 0)
    { G4cout << "\n --->warning from SetfAbsorMaterial: absor number "
             << iabs << " out of range. Command refused" << G4endl;
      return;
    }

  G4Material* pttoMaterial = 
    G4NistManager::Instance()->FindOrBuildMaterial(material);
  if (pttoMaterial) {
      fAbsorMaterial[iabs] = pttoMaterial;
      G4RunManager::GetRunManager()->PhysicsHasBeenModified();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetAbsorThickness(G4int iabs,G4double val)
{
  // change Absorber thickness
  //
  if (iabs > fNbOfAbsor || iabs <= 0)
    { G4cout << "\n --->warning from SetfAbsorThickness: absor number "
             << iabs << " out of range. Command refused" << G4endl;
      return;
    }
  if (val <= DBL_MIN)
    { G4cout << "\n --->warning from SetfAbsorThickness: thickness "
             << val  << " out of range. Command refused" << G4endl;
      return;
    }
  fAbsorThickness[iabs] = val;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetAbsorSizeYZ(G4double val)
{
  // change the transverse size
  //
  if (val <= DBL_MIN)
    { G4cout << "\n --->warning from SetfAbsorSizeYZ: thickness "
             << val  << " out of range. Command refused" << G4endl;
      return;
    }
  fAbsorSizeYZ = val;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField()
{
    if ( fFieldMessenger.Get() == 0 ) {
        // Create global magnetic field messenger.
        // Uniform magnetic field is then created automatically if
        // the field value is not zero.
        G4ThreeVector fieldValue = G4ThreeVector();
        G4GlobalMagFieldMessenger* msg =
        new G4GlobalMagFieldMessenger(fieldValue);
        //msg->SetVerboseLevel(1);
        G4AutoDelete::Register(msg);
        fFieldMessenger.Put( msg );
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

