'''
Definitions for plotRocCurve.py so that ear can stay cleaner
Author: L. Dodd, UW Madison
'''

from subprocess import Popen
from sys import argv, exit, stdout, stderr

import ROOT
import numpy
from array import array

# So things don't look like crap.
ROOT.gROOT.SetStyle("Plain")
ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)

####################################
canvas = ROOT.TCanvas("asdf", "adsf", 800, 800)

def get_histo(ntuple_file, histname=''):
    hist =ntuple_file.Get(histname)
    return hist

def get_ratio(histoId, ntuple):
    ''' Get the effi given one ntuple and two histos '''
    num = get_histo(ntuple,histoId)
    denom = get_histo(ntuple,"histoDenumerator")
    effi = num.Integral() / denom.Integral()
    return effi

def produce_tgraph(histoId, ntupleEff,ntupleFR,color,N,MarkerStyle=20):
    ''' Create a TGraph with one point(TGraph) '''
    id_Effi = get_ratio(histoId, ntupleEff)
    eff = numpy.array([id_Effi],dtype=float)
    id_FakeRate = get_ratio(histoId, ntupleFR)
    fr = numpy.array([id_FakeRate],dtype=float)
    n = numpy.array([N],dtype=int)
    tgraph = ROOT.TGraph(N,eff,fr)
    tgraph.SetMarkerStyle(MarkerStyle)
    tgraph.SetMarkerColor(color)
    return tgraph

def produce_pair(histoId, ntupleEff,ntupleFR,N,tgline):
    ''' Add a point to  a roc curve (TGraph) '''
    id_Effi = get_ratio(histoId, ntupleEff)
    eff = numpy.array([id_Effi],dtype=float)
    id_FakeRate = get_ratio(histoId, ntupleFR)
    fr = numpy.array([id_FakeRate],dtype=float)
    tgline.SetPoint(N,eff,fr)

def produce_roc_curve(ntupleEff,ntupleFR,histoId1, legend1, histoId2, legend2, histoId3, legend3, title='',label='roc'):
    frame = ROOT.TMultiGraph()
    frame.SetTitle(title)
    #Create a TGraph to draw a line behind the other points
    tgline = produce_tgraph(histoId1, ntupleEff, ntupleFR, ROOT.kBlack, 3,1)
    produce_pair(histoId2, ntupleEff, ntupleFR, 1, tgline)
    produce_pair(histoId3,ntupleEff, ntupleFR, 2, tgline)
    #Create TGraphs to add to the TMultiGraph
    tg1 = produce_tgraph(histoId1, ntupleEff, ntupleFR, ROOT.kBlue-9, 1)
    tg2 = produce_tgraph(histoId2, ntupleEff, ntupleFR, ROOT.kRed-9, 1)
    tg3 = produce_tgraph(histoId3, ntupleEff, ntupleFR, ROOT.kOrange-2, 1)
    #Add the TGraphs to the TMultigraph 
    frame.Add(tgline)
    frame.Add(tg1)
    frame.Add(tg2)
    frame.Add(tg3)
    #Draw Axis,Line,Points
    frame.Draw("ALP")
    frame.GetXaxis().SetLimits(0.,1.)
    frame.GetYaxis().SetRangeUser(0.,1.)
    #Add Legend for the IDs
    legend = ROOT.TLegend(0.7, 0.7, 0.89, 0.8, "", "brNDC")
    legend.SetFillColor(ROOT.kWhite)
    legend.SetBorderSize(1)
    legend.AddEntry(tg1,legend1,"pe")
    legend.AddEntry(tg2,legend2,"pe")
    legend.AddEntry(tg3,legend3,"pe")
    legend.Draw()
    #Save with a specific file name
    saveas = label+'.png'
    print saveas
    canvas.SaveAs(saveas)

