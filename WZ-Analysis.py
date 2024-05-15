import ROOT
import math

# ROOT dosyasını aç
#file = ROOT.TFile.Open("mc_363358.WqqZll.2lep.root")
file = ROOT.TFile.Open("data_D.2lep.root")

# Çıktı dosyasını oluştur
#output_file = ROOT.TFile("mc-WZ.root", "RECREATE")
output_file = ROOT.TFile("Data-WZ.root", "RECREATE")


# treeyi al
tree = file.Get("mini")

#Histogramlar
de1_mass_hist = ROOT.TH1F("de1_mass_hist", "De1 quark mass", 50, 0, 400)
de2_mass_hist = ROOT.TH1F("de2_mass_hist", "De2 quark mass", 50, 0, 400)
w_mass_hist = ROOT.TH1F("w_mass_hist", "W boson mass", 50, 0, 200)
zee_mass_hist = ROOT.TH1F("zee_mass_hist", "Z boson mass", 50, 0, 200)
zmumu_mass_hist = ROOT.TH1F("zmumu_mass_hist", "Z boson mass", 50, 0, 200)


zee_mass_hist_selected = ROOT.TH1F("zee_mass_hist_selected", "Selected Z boson mass", 50, 0, 200)
zmumu_mass_hist_selected = ROOT.TH1F("zmumu_mass_hist_selected", "Selected Z boson mass", 50, 0, 200)
w_mass_hist_selected = ROOT.TH1F("w_mass_hist_selected", "Selected W boson mass", 50, 0, 200)

expected_Zmass = 91.2
sigma = 5
expected_Wmass = 80.4

eta_cut = 2.47
pt_cut = 25

# Electron Kanalı (Elektron-Pozitron Çifti)
for event in tree:
    electrons = []  # Elektron leptonlarını tutacak liste

    # Elektron leptonlarını seçme
    for i in range(event.lep_n):
        if abs(event.lep_type[i]) == 11 and abs(event.lep_eta[i]) < eta_cut and event.lep_pt[i] / 1000. > pt_cut:
            electrons.append(i)

    # En az iki elektron varsa histogramı doldurma
    if len(electrons) >= 2:
        for i in range(len(electrons)):
            for j in range(i + 1, len(electrons)):
                if event.lep_charge[electrons[i]] * event.lep_charge[electrons[j]] < 0 and abs(event.lep_eta[j]) < eta_cut and event.lep_pt[j] / 1000. > pt_cut:
                    
                    l1 = ROOT.TLorentzVector()
                    l2 = ROOT.TLorentzVector()

                    l1.SetPtEtaPhiE(event.lep_pt[electrons[i]] / 1000., event.lep_eta[electrons[i]], event.lep_phi[electrons[i]], event.lep_E[electrons[i]] / 1000.)
                    l2.SetPtEtaPhiE(event.lep_pt[electrons[j]] / 1000., event.lep_eta[electrons[j]], event.lep_phi[electrons[j]], event.lep_E[electrons[j]] / 1000.)

                    Z1 = l1 + l2


                    zee_mass_hist.Fill(Z1.M())


                    # Chi^2 hesaplama
                    def chi2(m, expected_Zmass, sigma):
                        return math.pow((m - expected_Zmass), 2) / math.pow(sigma, 2)


                    chi2_value = chi2(Z1.M(), expected_Zmass, sigma)
                    if chi2_value < 5:
                        zee_mass_hist_selected.Fill(Z1.M())

# Muon Kanalı
for event in tree:
    muons = []  #muonlari tutacak liste

    # muonlari seçme
    for i in range(event.lep_n):
        if abs(event.lep_type[i]) == 13:
            muons.append(i)

    # En az iki muon varsa histogramı doldurma
    if len(muons) >= 2:
        for i in range(len(muons)):
            for j in range(i + 1, len(muons)):
                if event.lep_charge[muons[i]] * event.lep_charge[muons[j]] < 0 and abs(event.lep_eta[j]) < eta_cut and event.lep_pt[j] / 1000. > pt_cut:

                    
                    l1 = ROOT.TLorentzVector()
                    l2 = ROOT.TLorentzVector()

                    l1.SetPtEtaPhiE(event.lep_pt[muons[i]] / 1000., event.lep_eta[muons[i]], event.lep_phi[muons[i]], event.lep_E[muons[i]] / 1000.)
                    l2.SetPtEtaPhiE(event.lep_pt[muons[j]] / 1000., event.lep_eta[muons[j]], event.lep_phi[muons[j]], event.lep_E[muons[j]] / 1000.)

                    Z2 = l1 + l2


                    zmumu_mass_hist.Fill(Z2.M())

                    # Chi^2 hesaplama
                    def chi2(m, expected_Zmass, sigma):
                        return math.pow((m - expected_Zmass), 2) / math.pow(sigma, 2)

                    chi2_value = chi2(Z2.M(), expected_Zmass, sigma)
                    if chi2_value < 5:
                        zmumu_mass_hist_selected.Fill(Z2.M())

# Jet Kanalı
for event in tree:
    jets = []  # Jetleri tutacak liste

    # Jetleri seçme
    for i in range(event.jet_n):
        if event.jet_pt[i] / 1000. > pt_cut and abs(event.jet_eta[i]) < eta_cut:
            jets.append(i)

    # En uygun jetleri seçme ve histogramı doldurma
    if len(jets) >= 4:
        for i in range(len(jets)):
            for j in range(i + 1, len(jets)):
                for k in range(j + 1, len(jets)):
                    for l in range(k + 1, len(jets)):
                        if jets[i] != jets[j] and jets[i] != jets[k] and jets[i] != jets[l] and jets[j] != jets[k] and jets[j] != jets[l] and jets[k] != jets[l]:

                            wj1 = ROOT.TLorentzVector()
                            wj2 = ROOT.TLorentzVector()
                            de_wj = ROOT.TLorentzVector()
                            de_zj = ROOT.TLorentzVector()

                            wj1.SetPtEtaPhiE(event.jet_pt[jets[i]] / 1000., event.jet_eta[jets[i]], event.jet_phi[jets[i]], event.jet_E[jets[i]] / 1000.)
                            wj2.SetPtEtaPhiE(event.jet_pt[jets[j]] / 1000., event.jet_eta[jets[j]], event.jet_phi[jets[j]], event.jet_E[jets[j]] / 1000.)
                            de_wj.SetPtEtaPhiE(event.jet_pt[jets[k]] / 1000., event.jet_eta[jets[k]], event.jet_phi[jets[k]], event.jet_E[jets[k]] / 1000.)
                            de_zj.SetPtEtaPhiE(event.jet_pt[jets[l]] / 1000., event.jet_eta[jets[l]], event.jet_phi[jets[l]], event.jet_E[jets[l]] / 1000.)


                            w = wj1 + wj2
                            w_mass_hist.Fill(w.M())
                            deW = w + de_wj
                            deZ = l1 + l2 + de_zj
                        
                            # Chi^2 hesaplama
                            def chi2(m, expected_Wmass, sigma):
                                return math.pow((m - expected_Wmass), 2) / math.pow(sigma, 2)

                            chi2_value = chi2(w.M(), expected_Wmass, sigma)
                            if chi2_value < 5:
                                w_mass_hist_selected.Fill(w.M())
                                de1_mass_hist.Fill(deW.M())
                                de2_mass_hist.Fill(deZ.M())

        

# Histogram çiz
zee_mass_hist_selected.Write()
zmumu_mass_hist_selected.Write()
w_mass_hist_selected.Write()
de1_mass_hist.Write()
de2_mass_hist.Write()
w_mass_hist.Write()
zee_mass_hist.Write()
zmumu_mass_hist.Write()
output_file.Close()

