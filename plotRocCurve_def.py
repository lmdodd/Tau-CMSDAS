'''
Usage:python plot.py RootFile.root label[optional]
Script to make some quick efficiency plots to test ntuple generation.
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

def get_ratio(ntuple):
    ''' Get the effi given two histos '''
    num = get_histo(ntuple,"histoNumerator")
    denom = get_histo(ntuple,"histoDenumerator")
    effi = num.Integral() / denom.Integral()
    return effi

def produce_tgraph(ntupleEff,ntupleFR,color,N):
    ''' Add a point to  a roc curve (TGraph) '''
    id_Effi = get_ratio(ntupleEff)
    eff = numpy.array([id_Effi],dtype=float)
    id_FakeRate = get_ratio(ntupleFR)
    fr = numpy.array([id_FakeRate],dtype=float)
    n = numpy.array([N],dtype=int)
    tgraph = ROOT.TGraph(N,eff,fr)
    tgraph.SetMarkerStyle(20)
    tgraph.SetMarkerColor(color)
    return tgraph

def produce_pair(ntupleEff,ntupleFR,color,N):
    ''' Add a point to  a roc curve (TGraph) '''
    id_Effi = get_ratio(ntupleEff)
    eff = numpy.array([id_Effi],dtype=float)
    id_FakeRate = get_ratio(ntupleFR)
    fr = numpy.array([id_FakeRate],dtype=float)
    n = numpy.array([N],dtype=int)
    tgraph = ROOT.TGraph(N,eff,fr)
    tgraph.SetMarkerStyle(20)
    tgraph.SetMarkerColor(color)
    return tgraph


def produce_roc_curve(ntuple1,ntuple2,legend1,ntuple3,ntuple4,legend2,ntuple5,ntuple6,legend3, N,title='',label='roc'):
    frame = ROOT.TMultiGraph()
    frame.SetTitle(title)
    tg1 = produce_tgraph(ntuple1,ntuple2, ROOT.kBlue-9, N)
    tg2 = produce_tgraph(ntuple3,ntuple4, ROOT.kRed-9, N)
    tg3 = produce_tgraph(ntuple5,ntuple6, ROOT.kOrange+1, N)
    frame.Add(tg1)
    frame.Add(tg2)
    frame.Add(tg3)
    frame.Draw("AP")
    frame.GetXaxis().SetLimits(0.,1.)
    frame.GetYaxis().SetRangeUser(0.,1.)
    legend = ROOT.TLegend(0.7, 0.7, 0.89, 0.8, "", "brNDC")
    legend.SetFillColor(ROOT.kWhite)
    legend.SetBorderSize(1)
    legend.AddEntry(tg1,legend1,"pe")
    legend.AddEntry(tg2,legend2,"pe")
    legend.AddEntry(tg3,legend3,"pe")
    legend.Draw()
    saveas = label+'.png'
    print saveas
    canvas.SaveAs(saveas)


