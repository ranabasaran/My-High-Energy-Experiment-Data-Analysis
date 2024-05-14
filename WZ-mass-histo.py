import ROOT

# MC

mc_file = ROOT.TFile.Open("mc-WZ.root","READ")
#mc_hist = mc_file.Get("zee_mass_hist_selected")
#mc_hist = mc_file.Get("zmumu_mass_hist_selected")
mc_hist = mc_file.Get("w_mass_hist_selected")
#mc_hist = mc_file.Get("de1_mass_hist")
#mc_hist = mc_file.Get("de2_mass_hist")


# Data

data_file = ROOT.TFile.Open("Data-WZ.root","READ")
#data_hist = data_file.Get("zee_mass_hist_selected")
#data_hist = data_file.Get("zmumu_mass_hist_selected")
data_hist = data_file.Get("w_mass_hist_selected")
#data_hist = data_file.Get("de1_mass_hist")
#data_hist = data_file.Get("de2_mass_hist")

mc_hist.SetDirectory(0)
data_hist.SetDirectory(0)

mc_file.Close()
data_file.Close()

other_comments = "ATLAS Open Data"

comments = ROOT.TLatex(0.15, 0.82, other_comments)
comments.SetNDC()
comments.SetTextFont(132)
comments.SetTextSize(0.05)

#histogram cizimi

canvas = ROOT.TCanvas("canvas")
#canvas.cd()

mc_hist.SetStats(0)
mc_hist.SetStats(0)

#renkler
mc_hist.SetLineColor(ROOT.kRed)
data_hist.SetLineColor(ROOT.kBlack)

#etiketler

#mc_hist.GetXaxis().SetTitle("m_{Z_ee} (GeV)")
#mc_hist.GetXaxis().SetTitle("m_{Z_#mu#mu} (GeV)")
#mc_hist.GetYaxis().SetTitle("Events")
#data_hist.GetXaxis().SetTitle("m_{Z_ee} (GeV)")
#data_hist.GetXaxis().SetTitle("m_{Z_#mu#mu} (GeV)")
#data_hist.GetYaxis().SetTitle("Events")

mc_hist.GetXaxis().SetTitle("m_{W} (GeV)")
mc_hist.GetYaxis().SetTitle("Events")
data_hist.GetXaxis().SetTitle("m_{W} (GeV)")
data_hist.GetYaxis().SetTitle("Events")

#mc_hist.GetXaxis().SetTitle("m_{De1} (GeV)")
#mc_hist.GetYaxis().SetTitle("Events")
#data_hist.GetXaxis().SetTitle("m_{De1} (GeV)")
#data_hist.GetYaxis().SetTitle("Events")


#mc_hist.GetXaxis().SetTitle("m_{De2} (GeV)")
#mc_hist.GetYaxis().SetTitle("Events")
#data_hist.GetXaxis().SetTitle("m_{De2} (GeV)")
#data_hist.GetYaxis().SetTitle("Events")


mc_hist.SetLineWidth(2)
data_hist.SetLineWidth(2)

data_hist.SetMarkerSize(0.6)
data_hist.SetMarkerStyle(20)
data_hist.SetMarkerColor(ROOT.kBlack)

mc_hist.SetMarkerStyle(1)
mc_hist.SetMarkerSize(0.4)
mc_hist.SetMarkerColor(ROOT.kRed)

#mc_hist.SetFillColor(ROOT.kRed)

#Logaritmik ölçek
canvas.SetLogy(True)

#yeniden ölçeklendirme
mc_hist.Scale(data_hist.Integral()/mc_hist.Integral())

mc_hist.Draw()
#mc_hist.Draw("same, hist")
data_hist.Draw("pe, same")
comments.Draw("same")

legend = ROOT.TLegend(0.7, 0.8, 0.87, 0.87)
legend.SetBorderSize(0)
legend.AddEntry(data_hist, "Data")
legend.AddEntry(mc_hist, "MC")
legend.Draw("same")

text = ROOT.TLatex()
text.SetNDC()
text.SetTextSize(0.04)
text.DrawLatex(0.7, 0.75,"#sqrt{s} = 13 TeV")

canvas.SaveAs("W-histogram.pdf")
#canvas.SaveAs("Zee-histogram.pdf")
#canvas.SaveAs("Zmumu-histogram.pdf")
#canvas.SaveAs("De1-histogram.pdf")
#canvas.SaveAs("De2-histogram.pdf")

