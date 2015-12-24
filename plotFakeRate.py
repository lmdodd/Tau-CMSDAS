import math
import ROOT
from ROOT import TCanvas
from ROOT import TFile
from ROOT import TH1F
from ROOT import TGraph
from ROOT import gROOT
from ROOT import gStyle
from ROOT import gSystem
from ROOT import TGraphErrors
import array



Binning_PT = array.array("d",[0,20,25,30,40,55,75,95,120,150,200,300])
OutFile=TFile("outputFR.root")

HistoNum=OutFile.Get("histoNumerator")
HistoNum= HistoNum.Rebin(len(Binning_PT)-1,"",Binning_PT)

HistoDeNum=OutFile.Get("histoDenominator")
HistoDeNum= HistoDeNum.Rebin(len(Binning_PT)-1,"",Binning_PT)

fakeRate=ROOT.TGraphAsymmErrors(HistoNum, HistoDeNum, "")

canv = TCanvas("canv", "histograms", 0, 0, 600, 600)

fakeRate.SetMinimum(0.5)
fakeRate.GetXaxis().SetRangeUser(0,300)
fakeRate.GetXaxis().SetTitle("#tau p_{T} [GeV]")
fakeRate.GetYaxis().SetRangeUser(0,1)
fakeRate.SetMarkerStyle(20)


fakeRate.Draw()

canv.SaveAs("jetToTauFR.pdf")
