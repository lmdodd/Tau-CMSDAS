
from subprocess import Popen
from sys import argv, exit, stdout, stderr

import ROOT
from array import array


# So things don't look like crap.
ROOT.gROOT.SetStyle("Plain")
ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)


bins=[0,20, 30, 40, 50, 60, 70, 80, 100, 120, 160,260,360]
saveWhere='~/myAnalysis/CMSSW_7_4_0/src/RecoTauTag/tauAnalysis/outputs'
canvas = ROOT.TCanvas("asdf", "adsf", 800, 800)


###### generic make plots ############
def make_plot(tree, variable, selection, binning, xaxis='', title=''):
    ''' Plot a variable using draw and return the histogram '''
    draw_string = "%s>>htemp(%s)" % (variable, ", ".join(str(x) for x in binning))
    tree.Draw(draw_string, selection, "goff")
    output_histo = ROOT.gDirectory.Get("htemp").Clone()
    output_histo.GetXaxis().SetTitle(xaxis)
    output_histo.SetTitle(title)
    return output_histo

def make_efficiency(denom, num):
    ''' Make an efficiency graph with the style '''
    eff = ROOT.TGraphAsymmErrors(num, denom)
    eff.SetMarkerStyle(20)
    eff.SetMarkerSize(1.5)
    eff.SetLineColor(ROOT.kBlack)
    return eff

def get_integral(denom, num):
    ''' Make an efficiency graph with the style '''
    effi = num.Integral() / denom.Integral()
    return effi



#Efficiencies
def make_num(ntuple, variable,PtCut,binning):
    num = make_plot(
        ntuple, variable,
        "tauPt > %0.2f &&fabs(tauEta)<2.3&& dmf>0 &&genMatchedTau==1&&passDiscr>0" % (PtCut),
        binning
    )
    num_rebin = num.Rebin(12,"num_rebin",array('d',bins))
    return num_rebin

def make_denom(ntuple, variable,PtCut,binning):
    denom = make_plot(
        ntuple, variable,
        "dmf>0&&fabs(tauEta)<2.3&&genMatchedTau==1&&tauPt> %0.2f"%(PtCut), #
        binning
    )
    denom_rebin = denom.Rebin(12,"denom_rebin",array('d',bins)) 
    return denom_rebin

#FAKE RATE
def make_num_FR(ntuple, variable,PtCut,binning):
    num = make_plot(
        ntuple, 'tauPt',
        "jetIDLoose>0&&tauPt > %0.2f&&fabs(tauEta)<2.3&& dmf>0&&passDiscr>0" % (PtCut),
        binning
    )
    num_rebin = num.Rebin(12,"num_rebin",array('d',bins))
    return num_rebin

def make_denom_FR(ntuple, variable,PtCut,binning):
    denom = make_plot(
        ntuple, 'jetPt',
        "jetPt> %0.2f&&jetIDLoose"%(PtCut), #
        binning
    )
    denom_rebin = denom.Rebin(12,"denom_rebin",array('d',bins)) 
    return denom_rebin


#DIVIDE
def produce_FR(ntuple, variable, PtCut,binning, filename,color):
    denom = make_denom_FR(ntuple, variable,PtCut,binning)
    num = make_num_FR(ntuple,variable,PtCut,binning)
    l1 = make_efficiency(denom,num)
    l1.SetMarkerColor(color)
    return l1

def produce_efficiency(ntuple, variable, PtCut,binning, filename,color):
    denom = make_denom(ntuple, variable,PtCut,binning)
    num = make_num(ntuple,variable,PtCut,binning)
    l1 = make_efficiency(denom,num)
    l1.SetMarkerColor(color)
    return l1

#PLOT
def compare_FR(ntuple1,legend1,ntuple2,legend2,ntuple3,legend3,ntuple4,legend4,ntuple5,legend5, variable, PtCut, binning, filename,
                         title='', xaxis='',yaxis=''):
    frame = ROOT.TH1F("frame", "frame", *binning)
    l1 = produce_FR(ntuple1,variable, PtCut,binning, filename,ROOT.kMagenta-3)
    l2 = produce_FR(ntuple2,variable, PtCut,binning, filename,ROOT.kBlue-9)
    l3 = produce_FR(ntuple3,variable, PtCut,binning, filename,ROOT.kOrange-9)
    l4 = produce_FR(ntuple4,variable, PtCut,binning, filename,ROOT.kBlue+2)
    l5 = produce_FR(ntuple5,variable, PtCut,binning, filename,ROOT.kGreen-3)
    frame.SetMaximum(1.2)
    canvas.SetLogy()
    frame.SetTitle(title)
    frame.GetXaxis().SetTitle(xaxis)
    frame.GetYaxis().SetTitle(yaxis)
    frame.Draw()
    l1.Draw('pe')
    l2.Draw('pesame')
    l3.Draw('pesame')
    l4.Draw('pesame')
    l5.Draw('pesame')
    legend = ROOT.TLegend(0.5, 0.6, 0.89, 0.8, "", "brNDC")
    legend.SetFillColor(ROOT.kWhite)
    legend.SetBorderSize(1)
    legend.AddEntry(l1,legend1, "pe")
    legend.AddEntry(l2,legend2, "pe")
    legend.AddEntry(l3,legend3, "pe")
    legend.AddEntry(l4,legend4, "pe")
    legend.AddEntry(l5,legend5, "pe")
    legend.Draw()
    saveas = saveWhere+filename+'.png'
    print saveas
    canvas.SaveAs(saveas)

def compare_efficiencies(ntuple1,legend1,ntuple2,legend2,ntuple3,legend3,ntuple4,legend4,ntuple5,legend5, variable, PtCut, binning, filename,
                         title='', xaxis='',yaxis=''):
    frame = ROOT.TH1F("frame", "frame", *binning)
    l1 = produce_efficiency(ntuple1,variable, PtCut,binning, filename,ROOT.kMagenta-3)
    l2 = produce_efficiency(ntuple2,variable, PtCut,binning, filename,ROOT.kBlue-9)
    l3 = produce_efficiency(ntuple3,variable, PtCut,binning, filename,ROOT.kOrange-9)
    l4 = produce_efficiency(ntuple4,variable, PtCut,binning, filename,ROOT.kBlue+2)
    l5 = produce_efficiency(ntuple5,variable, PtCut,binning, filename,ROOT.kGreen-3)
    frame.SetMaximum(1.2)
    frame.SetTitle(title)
    frame.GetXaxis().SetTitle(xaxis)
    frame.GetYaxis().SetTitle(yaxis)
    frame.Draw()
    l1.Draw('pe')
    l2.Draw('pesame')
    l3.Draw('pesame')
    l4.Draw('pesame')
    l5.Draw('pesame')
    legend = ROOT.TLegend(0.5, 0.1, 0.89, 0.4, "", "brNDC")
    legend.SetFillColor(ROOT.kWhite)
    legend.SetBorderSize(1)
    legend.AddEntry(l1,legend1, "pe")
    legend.AddEntry(l2,legend2, "pe")
    legend.AddEntry(l3,legend3, "pe")
    legend.AddEntry(l4,legend4, "pe")
    legend.AddEntry(l5,legend5, "pe")
    legend.Draw()
    saveas = saveWhere+filename+'.png'
    print saveas
    canvas.SaveAs(saveas)



