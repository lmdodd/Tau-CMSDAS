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

######## File #########
if len(argv) < 2:
   print 'Usage:python plot.py RootFile.root label[optional]'
   exit()


infile = argv[1]
infile2= argv[2]
#infile3= argv[3]
#infile4= argv[4]
#infile5= argv[5]
#infile6= argv[6]
ntuple_file = ROOT.TFile(infile)
ntuple_file2 = ROOT.TFile(infile2)
#ntuple_file3 = ROOT.TFile(infile3)
#ntuple_file4 = ROOT.TFile(infile4)
#ntuple_file5 = ROOT.TFile(infile5)
#ntuple_file6 = ROOT.TFile(infile6)

#####################################
#Get Effi NTUPLE                 #
#####################################
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

def produce_roc_pair(ntupleEff,ntupleFR,color,N):
    ''' Add a point to  a roc curve (TGraph) '''
    id_Effi = get_ratio(ntupleEff)
    eff = numpy.array([id_Effi],dtype=float)
    id_FakeRate = get_ratio(ntupleFR)
    fr = numpy.array([id_FakeRate],dtype=float)
    n = numpy.array([N],dtype=int)
    tgraph = ROOT.TGraph()
    tgraph.SetPoint(N,eff,fr)
    tgraph.SetMarkerStyle(20)
    tgraph.SetMarkerColor(color)
    return tgraph

def produce_tgraph(ntuple1,ntuple2,legend1, N,title=''):
    frame = ROOT.TMultiGraph()
    frame.SetTitle(title)
    tg = produce_roc_pair(ntuple1,ntuple2, ROOT.kBlue-9, N)
    frame.Add(tg)
    frame.Draw("AP")
    frame.GetXaxis().SetLimits(0.,1.)
    frame.GetYaxis().SetRangeUser(0.,1.)
    legend = ROOT.TLegend(0.7, 0.7, 0.89, 0.8, "", "brNDC")
    legend.SetFillColor(ROOT.kWhite)
    legend.SetBorderSize(1)
    legend.AddEntry(tg,legend1,"pe")
    legend.Draw()
    saveas = 'test.png'
    print saveas
    canvas.SaveAs(saveas)

################################################################################
#  for a
################################################################################

produce_tgraph(ntuple_file,ntuple_file2,'id_1',100,'ROC;tauEfficiency;tauFakeRate')

