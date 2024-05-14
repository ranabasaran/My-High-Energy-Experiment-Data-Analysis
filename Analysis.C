#define Analysis_cxx
#include "Analysis.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <Math/Vector4D.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include <iostream>
#ifndef PI
    #define PI 3.14159265358979323846
#endif
#ifndef deltaR
    #define deltaR
#endif

void Analysis::Loop()
{
//   In a ROOT session, you can do:
//      root> .L Analysis.C
//      root> Analysis t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();
   TFile *outputfile = new TFile("WZanalysis.root", "RECREATE");

   TH1F *h_eventnumber = new TH1F("h_eventnumber","Number of Events", 5,0,5);

   TH1F *muonsize = new TH1F("muonsize","Muon Size", 20,0,20);
   TH1F *muoncharge = new TH1F("muoncharge","Muon Charge", 100,-2,2);
   TH1F *muonEta = new TH1F("muonEta","Muon Eta", 24,-2.4,2.4);
   TH1F *muonPhi = new TH1F("muonPhi","Muon Phi", 20,-PI,PI);
   TH1F *muonPT = new TH1F("muonPT","Muon PT", 30,0,1500);

   TH1F *electronsize = new TH1F("electronsize","Electron Size", 20,0,20);
   TH1F *electroncharge = new TH1F("electroncharge","Elctron Charge", 100,-2,2);
   TH1F *electronEta = new TH1F("electronEta","Elctron Eta", 24,-2.4,2.4);
   TH1F *electronPT = new TH1F("electronPT","Electron PT", 30,0,1500);
   TH1F *electronPhi = new TH1F("electronPhi","Electron Phi", 20,-PI,PI);

   TH1F *ZtoMumuMass = new TH1F("ZtoMumuMass","Reconstructed Mass of Z", 100,0,200);
   ZtoMumuMass->GetXaxis()->SetTitle("m_{Z}^{#mu#mu} (GeV)");
   ZtoMumuMass->GetYaxis()->SetTitle("Events");

   TH1F *ZtoEeMass = new TH1F("ZtoEeMass","Reconstructed Mass of Z", 100,0,200);
   ZtoEeMass->GetXaxis()->SetTitle("m_{Z}^{#e#e} (GeV)");
   ZtoEeMass->GetYaxis()->SetTitle("Events");
  
   TH1F *WtoHmass = new TH1F("WtoHmass","Reconstructed Mass of W",100,0,200);
   WtoHmass->GetXaxis()->SetTitle("m_{W} (GeV)");
   WtoHmass->GetYaxis()->SetTitle("Events");
   
   TH1F *DeZMass = new TH1F("DeZMass","Reconstructed Mass of DeZ",100,0,1300);
   DeZMass->GetXaxis()->SetTitle("m_{De}^{Z} (GeV)");
   DeZMass->GetYaxis()->SetTitle("Events");

   TH1F *DeWMass = new TH1F("DeWMass","Reconstructed Mass of DeW",100,0,1300);
   DeWMass->GetXaxis()->SetTitle("m_{De}^{W} (GeV)");
   DeWMass->GetYaxis()->SetTitle("Events");

   TH1F *jetsize = new TH1F("jetsize","Jet Size", 20,0,20);
   TH1F *jetEta = new TH1F("jetEta","Jet Eta", 24,-2.4,2.4);
   TH1F *jetPT = new TH1F("jetPT","Jet PT", 30,0,1500);
   TH1F *jetPhi = new TH1F("jetPhi","Jet Phi", 20,-PI,PI);


   TH1F *CutEfficiency = new TH1F("CutEfficiency", "Cut Efficiency",10,0,10);

   int eventnumber = 0;
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      if(jentry%5000==0) cout<<"Entries: "<<jentry<<endl;
      eventnumber++;

      h_eventnumber->Fill(1);
      CutEfficiency->Fill(0);

      TLorentzVector zLep1,zLep2;

      int nMuon = Muon_size;
      int countM = 0;
      bool ZfromM = false;
      float mumudR = 0;

      for (int i = 0 ; i < nMuon ; i++) {
              countM++;
              muonPT->Fill(Muon_PT[i]);
              muonEta->Fill(Muon_Eta[i]);
              muonPhi->Fill(Muon_Phi[i]);
              muoncharge->Fill(Muon_Charge[i]);
       
              for (int j = 0 ; j < i ; j++) {
		      if((Muon_Charge[i] * Muon_Charge[j]) < 0) {
			      if((Muon_PT[i] < 30 && Muon_PT[j] < 30)) continue;
			      zLep1.SetPtEtaPhiM(Muon_PT[i], Muon_Eta[i], Muon_Phi[i], 0.106);
			      zLep2.SetPtEtaPhiM(Muon_PT[j], Muon_Eta[j], Muon_Phi[j], 0.106);
			      double ZMumuMass = (zLep1 + zLep2).M();
			      ZfromM = true;
			      ZtoMumuMass->Fill(ZMumuMass);
		      }
	      }
   }
      muonsize->Fill(countM);

      int nElectron = Electron_size;
      int countE = 0;
      bool ZfromE = false;
      float eedR = 0;

      for (int i = 0 ; i < nElectron ; i++) {
	      countE++;
	      electronPT->Fill(Electron_PT[i]);
	      electronEta->Fill(Electron_Eta[i]);
	      electronPhi->Fill(Electron_Phi[i]);
	      electroncharge->Fill(Electron_Charge[i]);

	      for (int j=0 ; j < i ; j++) {
                      if((Electron_Charge[i] * Electron_Charge[j]) < 0) {
			      if((Electron_PT[i] < 30 && Electron_PT[j] < 30)) continue;
                              zLep1.SetPtEtaPhiM(Electron_PT[i], Electron_Eta[i], Electron_Phi[i], 0.000511);
                              zLep2.SetPtEtaPhiM(Electron_PT[j], Electron_Eta[j], Electron_Phi[j], 0.000511);
                              double ZeeMass = (zLep1 + zLep2).M();
                              ZfromE = true;
                              ZtoEeMass->Fill(ZeeMass);
		        }
	      }
   }
      electronsize->Fill(countE);

      if(!(ZfromM || ZfromE))continue;
      CutEfficiency->Fill(1);

      TLorentzVector Wj1, Wj2, DeWj, DeZj;
      int nJet = Jet_size;
      int countJ = 0;
      bool WfromJ = false;
      for(int i = 0 ; i < nJet ; i++) {
	      countJ++;
	      jetPT->Fill(Jet_PT[i]);
              jetEta->Fill(Jet_Eta[i]);
              jetPhi->Fill(Jet_Phi[i]);

      }
      jetsize->Fill(countJ);

      if(countJ < 4) continue;
      CutEfficiency->Fill(2);



      for(unsigned int jet1 = 0 ; jet1 < nJet ; jet1++) {
	      if(Jet_PT[jet1] < 25) continue;
	        for(unsigned int jet2 = jet1 + 1; jet2 < nJet ; jet2++) {
			if(Jet_PT[jet2] < 25) continue;

			Wj1.SetPtEtaPhiM(Jet_PT[jet1], Jet_Eta[jet1], Jet_Phi[jet1], Jet_Mass[jet1]);
			Wj2.SetPtEtaPhiM(Jet_PT[jet2], Jet_Eta[jet2], Jet_Phi[jet2], Jet_Mass[jet2]);
	       		double Wmass = (Wj1 + Wj2).M();
                        WfromJ; true;
			WtoHmass->Fill(Wmass);


			for(unsigned int jet3 = 0 ; jet3 < nJet ; jet3++) {
				if(jet3==jet2 || jet3==jet1) continue;
				  if(Jet_PT[jet3] < 25) continue;
				       for(unsigned int jet4 = 0 ; jet4 < nJet; jet4++) {
					  if(jet4==jet1 || jet4==jet2 || jet4==jet3) {
						  if(Jet_PT[jet4] < 25) continue;
						  if(Jet_PT[jet3] < 100 && Jet_PT[jet4] < 100) continue;
						  DeWj.SetPtEtaPhiM(Jet_PT[jet3], Jet_Eta[jet3], Jet_Phi[jet3], Jet_Mass[jet3]);
						  DeZj.SetPtEtaPhiM(Jet_PT[jet4], Jet_Eta[jet4], Jet_Phi[jet4], Jet_Mass[jet4]);

						  double DeWmass = (Wj1 + Wj2 + DeWj).M();
						  double DeZmass = (zLep1 + zLep2 + DeZj).M();
			 
                                                  DeZMass->Fill(DeZmass);
						  DeWMass->Fill(DeWmass);
 
						  
				          }
				}
			}
		}	
		}
   }
  outputfile->cd();
  h_eventnumber->Write();
  muonsize->Write();
  muoncharge->Write();
  muonPT->Write();
  muonEta->Write();
  muonPhi->Write();
  electronsize->Write();
  electroncharge->Write();
  electronPT->Write();
  electronEta->Write();
  electronPhi->Write();
  jetsize->Write();
  jetPT->Write();
  jetEta->Write();
  jetPhi->Write();
  ZtoMumuMass->Write();
  ZtoEeMass->Write();
  WtoHmass->Write();
  DeZMass->Write();
  DeWMass->Write();
  CutEfficiency->Write();
  outputfile->Close();

  cout <<"Histograms written to file. Done." << endl;
  

  }
