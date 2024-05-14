#define Analysis_cxx
#include "Analysis.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

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

    TFile *outputfile = new TFile("hz-son-output.root", "RECREATE");

   TH1F *h_eventnumber = new TH1F("h_eventnumber", "NumberofEvents", 10, 0, 10);

   TH1F *hHmass = new TH1F("hHmass", "H Mass", 100, 0, 200);

   TH1F *hZmass = new TH1F("hZmass", "Z Mass", 100, 0, 200);

   TH1F *hDe1mass = new TH1F("hDe1mass", "De1 Mass", 100, 0, 1300);

   TH1F *hDe2mass = new TH1F("hDe2mass", "De2 Mass", 100, 0, 1300);

   TH1F *j1mass = new TH1F("j1mass", "j1 Mass", 100, 0., 0.02);

   TH1F *j2mass = new TH1F("j2mass", "j2 Mass", 100, 0., 0.01);


   TH1F *Zeta, *Zphi, *ZPT;

   Zeta= new TH1F("Zeta", "Z #eta", 50, -5., 5.);
   Zphi= new TH1F("Zphi", "Z #phi", 50, -5., 5.);
   ZPT= new TH1F("ZPT", "Z p_{T}", 300, 0., 300);

   TH1F *Heta, *Hphi, *HPT;
   Heta= new TH1F("Heta", "H #eta", 50, -5., 5.);
   Hphi= new TH1F("Hphi", "H #phi", 50, -5., 5.);
   HPT= new TH1F("HPT", "H p_{T}", 300, 0., 600);

   TH1F *j1eta, *j1phi, *j1PT;
   j1eta= new TH1F("j1eta", "j1 #eta", 50, -5., 5.);
   j1phi= new TH1F("j1phi", "j1 #phi", 50, -5., 5.);
   j1PT= new TH1F("j1PT", "j1 p_{T}", 300, 0., 300);

   TH1F *j2eta, *j2phi, *j2PT;
   j2eta= new TH1F("j2eta", "j2 #eta", 50, -5., 5.);
   j2phi= new TH1F("j2phi", "j2 #phi", 50, -5., 5.);
   j2PT= new TH1F("j2PT", "j2 p_{T}", 300, 0., 300);



   int eventnumber=0;


   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      
      eventnumber++;

      if(eventnumber%1000 == 0) cout<<"event: "<<eventnumber<<endl;

      h_eventnumber->Fill(1);

      TLorentzVector H, Z, j1, j2;

      TLorentzVector De1, De2;

      Z.SetPtEtaPhiM(Particle_PT[2], Particle_Eta[2], Particle_Phi[2], Particle_M[2]);
      j2.SetPtEtaPhiM(Particle_PT[3], Particle_Eta[3], Particle_Phi[3], Particle_M[3]);

      H.SetPtEtaPhiM(Particle_PT[5], Particle_Eta[5], Particle_Phi[5], Particle_M[5]);
      j1.SetPtEtaPhiM(Particle_PT[4], Particle_Eta[4], Particle_Phi[4], Particle_M[4]);

      De1 = H + j1;
      De2 = Z + j2;

      hHmass->Fill(H.M());
      HPT->Fill(H.Pt());
      Heta->Fill(H.Eta());
      Hphi->Fill(H.Phi());

      hZmass->Fill(Z.M());
      ZPT->Fill(Z.Pt());
      Zeta->Fill(Z.Eta());
      Zphi->Fill(Z.Phi());


      hDe1mass->Fill(De1.M());
      
      j1mass->Fill(j1.M());
      j1PT->Fill(j1.Pt());
      j1eta->Fill(j1.Eta());
      j1phi->Fill(j1.Phi());


      hDe2mass->Fill(De2.M());

      j2mass->Fill(j2.M());
      j2PT->Fill(j2.Pt());
      j2eta->Fill(j2.Eta());
      j2phi->Fill(j2.Phi());

   }

   outputfile->cd();
   hHmass->Write();
   HPT->Write();
   Heta->Write();
   Hphi->Write();
   hZmass->Write();
   ZPT->Write();
   Zeta->Write();
   Zphi->Write();
   j1mass->Write();
   j2mass->Write();
   j1PT->Write();
   j1eta->Write();
   j1phi->Write();
   hDe1mass->Write();
   hDe2mass->Write();
   j2PT->Write();
   j2eta->Write();
   j2phi->Write();

   outputfile->Close();
}
