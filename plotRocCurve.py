'''
Usage:python plotRocCurve.py 
Script to plot a Roc curve based on 3 IDs from 2 root files with historgrams.
Author: L. Dodd, UW Madison
'''

from subprocess import Popen
from sys import argv, exit, stdout, stderr

import ROOT
import numpy
from array import array

import plotRocCurve_def

# So things don't look like crap.
ROOT.gROOT.SetStyle("Plain")
ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)

######## Load Files #########
ntuple_fileeff = ROOT.TFile("outputEffi.root")
ntuple_filefr = ROOT.TFile("outputFR.root")

#####################################
plotRocCurve_def.produce_roc_curve(
			 ntuple_fileeff,ntuple_filefr, #ntuple files with histograms
    			 'histoNumeratorLoose','byCombinedLoose', # 'histogramName', ' legend Title'
			 'histoNumeratorMedium','byCombinedMedium', # 
			 'histoNumeratorTight','byCombinedTight',
			 'ROC; tau Efficiency;tau Fake Rate',
			 'roc'
)

