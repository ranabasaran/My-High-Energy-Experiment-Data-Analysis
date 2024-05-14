import ROOT

# ROOT dosyasını aç
#file = ROOT.TFile.Open("mc_363490.llll.2lep.root")
file = ROOT.TFile.Open("data_D.2lep.root")

# Çıktı dosyasını oluştur
#output_file = ROOT.TFile("ZtoEeMumu.root", "RECREATE")
output_file = ROOT.TFile("Data-ZtoEeMumu.root", "RECREATE")


# treeyi al
tree = file.Get("mini")

# Histogram oluşturma
ZeeMass = ROOT.TH1F("ZeeMass", "Electron Channel - Z boson mass", 50, 0, 200)
ZmumuMass = ROOT.TH1F("ZmumuMass", "Muon Channel - Z boson mass", 50, 0, 200)

eventnumber = ROOT.TH1F("eventnumber", "Number of Events", 5,0,5)

# Z bosonunu oluşturan l1 ve l2 leptonlarına ait eta ve pt kriterleri
eta_cut = 2.47
pt_cut = 25

# Electron Kanalı (Elektron-Pozitron Çifti)
for event in tree:
    if event.lep_n >= 2:  # En az iki lepton kontrolü
        if event.lep_charge[0] * event.lep_charge[1] < 0:
            if abs(event.lep_type[0]) == 11 and abs(event.lep_eta[0]) < eta_cut and event.lep_pt[0] / 1000. > pt_cut:
                if abs(event.lep_type[1]) == 11 and abs(event.lep_eta[1]) < eta_cut and event.lep_pt[1] / 1000. > pt_cut:
                    l1 = ROOT.TLorentzVector()
                    l2 = ROOT.TLorentzVector()

                    l1.SetPtEtaPhiE(event.lep_pt[0] / 1000., event.lep_eta[0], event.lep_phi[0], event.lep_E[0] / 1000.)
                    l2.SetPtEtaPhiE(event.lep_pt[1] / 1000., event.lep_eta[1], event.lep_phi[1], event.lep_E[1] / 1000.)

                    Z = l1 + l2

                    ZeeMass.Fill(Z.M())

# Muon Kanalı (Muon-Antimuon Çifti)
for event in tree:
    if event.lep_n >= 2:  # En az iki lepton kontrolü
        if event.lep_charge[0] * event.lep_charge[1] < 0:
            if abs(event.lep_type[0]) == 13 and abs(event.lep_eta[0]) < eta_cut and event.lep_pt[0] / 1000. > pt_cut:
                if abs(event.lep_type[1]) == 13 and abs(event.lep_eta[1]) < eta_cut and event.lep_pt[1] / 1000. > pt_cut:
                    l1 = ROOT.TLorentzVector()
                    l2 = ROOT.TLorentzVector()

                    l1.SetPtEtaPhiE(event.lep_pt[0] / 1000., event.lep_eta[0], event.lep_phi[0], event.lep_E[0] / 1000.)
                    l2.SetPtEtaPhiE(event.lep_pt[1] / 1000., event.lep_eta[1], event.lep_phi[1], event.lep_E[1] / 1000.)

                    Z = l1 + l2

                    ZmumuMass.Fill(Z.M())



# Histogram çiz
ZeeMass.Write()
ZmumuMass.Write()
eventnumber.Write()
output_file.Close()

