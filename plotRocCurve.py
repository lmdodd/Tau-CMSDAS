'''
Usage:python plotRocCurve.py label[optional]
Script to make some quick efficiency plots to test ntuple generation.
Author: L. Dodd, UW Madison
'''

from subprocess import Popen
from sys import argv, exit, stdout, stderr

import ROOT
import numpy
from array import array

#import plotRocCurve_def
import plot_def


# So things don't look like crap.
ROOT.gROOT.SetStyle("Plain")
ROOT.gROOT.SetBatch(True)
ROOT.gStyle.SetOptStat(0)

######## Load File #########
ntuple_file1 = ROOT.TFile("outputEfficiency1.root")
ntuple_file2 = ROOT.TFile("outputFakeRate1.root")
ntuple_file3 = ROOT.TFile("outputEfficiency2.root")
ntuple_file4 = ROOT.TFile("outputFakeRate2.root")
ntuple_file5 = ROOT.TFile("outputEfficiency3.root")
ntuple_file6 = ROOT.TFile("outputFakeRate3.root")

#####################################
#                                   #
#plotRocCurve_def.produce_roc_curve(
#                 ntuple_file, ntuple_file2,'id_1',  #id1_efficency_ntuple, id1_fakeRate_ntuple, id1 title for legend
#                 100, #integer number of bins for TGraph
#                 'ROC; tau Efficiency;tau Fake Rate', # 'title; x-axis title; y-axis title'
#                 'test' #title of saved png
#)
#####################################
#plotRocCurve_def.produce_roc_curve(
plot_def.produce_roc_curve(
			ntuple_file1,ntuple_file2,'byCombinedLoose',
			ntuple_file3,ntuple_file4,'byCombinedMedium',
			ntuple_file5,ntuple_file6,'byCombinedTight',
			'ROC; tau Efficiency;tau Fake Rate',
			'test'
)


#plot_def.produce_roc_curve(
#			 ntuple_fileeff,ntuple_filefr,
#    			 'histoNumeratorLoose','byCombinedLoose',
#			 'histoNumeratorMedium','byCombinedMedium',
#			 'histoNumeratorTight','byCombinedTight',
#			 ROC; tau Efficiency;tau Fake Rate',
#			 'test'
#)

