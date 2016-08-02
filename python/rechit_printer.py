#!/usr/bin/env python


import ROOT
import sys

ROOT.PyConfig.IgnoreCommandLineOptions = True
ROOT.gStyle.SetOptStat(0)
ROOT.gROOT.SetBatch(1)
ROOT.gStyle.SetPaintTextFormat(".2f")

f = ROOT.TFile(sys.argv[1])
fd = f.Get("recHitAnalyser")

for evt_dir in fd.GetListOfKeys():
    dir_name = evt_dir.GetName()
    print dir_name
    fd_evt = fd.Get(dir_name)
    hist = fd_evt.Get("recHitMap")
    c = ROOT.TCanvas("c"+dir_name, "", 800, 600)
    c.SetLogz()
    c.SetTicks(1, 1)
    hist.Draw("COLZ TEXT")
    hist.SetMarkerSize(0.2)
    hist.GetZaxis().SetRangeUser(0.5, 4000)
    c.SaveAs(dir_name + "_recHitMap.pdf")

