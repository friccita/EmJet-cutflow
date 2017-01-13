#include <iostream>
#include <iomanip>
#include <locale>

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#include "vector"
#include "vector"
using std::vector;

#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

TTree          *fChain;   //!pointer to the analyzed TTree or TChain               
Int_t           fCurrent; //!current Tree number in a TChain                       







int EMJselect(bool otfile, const char* inputfilename,const char* outputfilename,
	      float HTcut, float pt1cut, float pt2cut, float pt3cut, float pt4cut, float jetacut,float alphaMaxcut, float NemfracCut,float CemfracCut,int ntrk1cut, int NemergingCut) {
  // "ntuple.root", "histos.root"
  // suggest cuts 1000., 400.,200.,125.,50.,0.2,0.9,0.9,0,1
  // right now this code hard wires the jet pT cut and requires emerging jets to have at least
  // one track with pT> 1 GeV

  // read the Tree generated by tree1w and fill two histograms
  // note that we use "new" to create the TFile and TTree objects,
  // to keep them alive after leaving this function.
 
  int npass=0;

  TFile *f = new TFile(inputfilename);

  // get histogram of events before trigger
  TH1F *eventCountPreTrigger;
  if(otfile) eventCountPreTrigger = static_cast<TH1F*>(f->Get("eventCountPreTrigger/eventCountPreTrigger")->Clone());



  TTree *tt = (TTree*)f->Get("emJetAnalyzer/emJetTree");

  Int_t nVtx, event;
  Float_t met_pt, met_phi;


  vector<int> *jet_index=new vector<int>;
  vector<int> *jet_source=new vector<int>;
  vector<float> *jet_pt = new vector<float>;
  vector<float> *jet_eta = new vector<float>;
  vector<float> *jet_phi = new vector<float>;
  vector<float> *jet_alphaMax = new vector<float>;
  vector<float> *jet_cef = new vector<float>;
  vector<float> *jet_nef = new vector<float>;
  vector<float> *jet_chf = new vector<float>;
  vector<float> *jet_nhf = new vector<float>;
  vector<float> *jet_phf = new vector<float>;
  vector<vector<float> > *track_pt = 0;
  vector<vector<float> > *track_eta = 0;
  vector<vector<int> > *track_source = 0;
  vector<vector<int> > *track_index = 0;
  vector<vector<int> > *track_jet_index = 0;
  vector<vector<int> > *track_vertex_index = 0;
  vector<vector<int> > *track_algo = 0;
  vector<vector<float> > *track_vertex_weight =0;
  vector<vector<float> > *track_ipZ =0;

  //get event count pre trigger



  //for ntuple
  tt->SetBranchAddress("nVtx",&nVtx);
  tt->SetBranchAddress("event",&event);
  tt->SetBranchAddress("met_pt",&met_pt);
  tt->SetBranchAddress("met_phi",&met_phi);
  tt->SetBranchAddress("jet_index",&jet_index);
  tt->SetBranchAddress("jet_source",&jet_source);
  tt->SetBranchAddress("jet_pt",&jet_pt);
  tt->SetBranchAddress("jet_eta",&jet_eta);
  tt->SetBranchAddress("jet_phi",&jet_phi);
  tt->SetBranchAddress("jet_cef",&jet_cef);
  tt->SetBranchAddress("jet_nef",&jet_nef);
  tt->SetBranchAddress("jet_chf",&jet_chf);
  tt->SetBranchAddress("jet_nhf",&jet_nhf);
  tt->SetBranchAddress("jet_phf",&jet_phf);
  tt->SetBranchAddress("jet_alphaMax",&jet_alphaMax);
  tt->SetBranchAddress("track_pt",&track_pt);
  tt->SetBranchAddress("track_eta",&track_eta);
  tt->SetBranchAddress("track_source",&track_source);
  tt->SetBranchAddress("track_index",&track_index);
  tt->SetBranchAddress("track_jet_index",&track_jet_index);
  tt->SetBranchAddress("track_algo",&track_algo);
  tt->SetBranchAddress("track_vertex_index",&track_vertex_index);
  tt->SetBranchAddress("track_vertex_weight",&track_vertex_weight);
  tt->SetBranchAddress("track_ipZ",&track_ipZ);

  // create a histograms
  TH1F *acount,*count,*hjetcut,*hjetchf,*h_nemg,*hnjet,*hpt,*heta,*heta2,*halpha,*H_T,*H_T2,*hbcut_ntrkpt1,*hacut_ntrkpt1,*hbcut_nef,*hacut_nef,*hbcut_cef,*hacut_cef,*hbcut_alphamax,*hacut_alphamax,*hHTnm1,*hpt1nm1,*hpt2nm1,*hpt3nm1,*hpt4nm1,*halphanm1,*hnemnm1,*hpt1,*hpt2,*hpt3,*hpt4;
  if(otfile) {
  acount = new TH1F("acount","counts",20,0.,20.);
  count = new TH1F("count","counts",3,0,3);
  count->SetStats(0);
  count->SetCanExtend(TH1::kAllAxes);

  hjetcut = new TH1F("hjetcut","jetcut counts",20,0.,20.);
  hjetchf = new TH1F("hjetchf","jet charged hadron fr",20,0.,1.2);
  h_nemg = new TH1F("h_nemg","number of emerging jets",20,0.,20.);
  hnjet = new TH1F("hnjet","number of jets",20,0.,20.);
  hpt = new TH1F("hpt","jet pt distribution",200,0.,1000.);
  heta   = new TH1F("heta","jet eta distribution",100,-4.,4.);
  heta2   = new TH1F("heta2","jet eta distribution first 4 jets",100,-4.,4.);
  halpha   = new TH1F("halpha","jet alphaMax distribution",100,0.,1.5);
  H_T      = new TH1F("H_T"," HT distribution before cut", 100,0.,5000.);
  H_T2      = new TH1F("H_T2"," HT distribution after cut", 100,0.,5000.);
  hpt1 = new TH1F("hpt1"," pT of leading jet",200,0.,1000.);
  hpt2 = new TH1F("hpt2"," pT of second jet",200,0.,1000.);
  hpt3 = new TH1F("hpt3"," pT of third jet",200,0.,1000.);
  hpt4 = new TH1F("hpt4"," pT of fourth jet",200,0.,1000.);
  hbcut_ntrkpt1 = new TH1F("hbcut_ntrkpt1","number tracks pt>1 before cut",10,0.,20.);
  hacut_ntrkpt1 = new TH1F("hacut_ntrkpt1","number tracks pt>1 after cut",10,0.,20.);
  hbcut_nef = new TH1F("hbcut_nef","neutral em fraction before cut",10,0.,1.2);
  hacut_nef = new TH1F("hacut_nef","neutral em fraction after cut",10,0.,1.2);
  hbcut_cef = new TH1F("hbcut_cef","charged em fraction before cut",50,0.,1.2);
  hacut_cef = new TH1F("hacut_cef","charged em fraction after cut",50,0.,1.2);
  hbcut_alphamax = new TH1F("hbcut_alphamax","alphamax before cut",50,0.,1.5);
  hacut_alphamax = new TH1F("hacut_alphamax","alphamax after cut",50,0.,1.5);
  hHTnm1 = new TH1F("hHTnm1","HT n-1",100,0.,5000.);
  hpt1nm1 = new TH1F("hpt1nm1","pt1 n-1",200,0.,1000.);
  hpt2nm1 = new TH1F("hpt2nm1","pt2 n-1",200,0.,1000.);
  hpt3nm1 = new TH1F("hpt3nm1","pt3 n-1",200,0.,1000.);
  hpt4nm1 = new TH1F("hpt4nm1","pt4 n-1",200,0.,1000.);
  halphanm1 = new TH1F("halphanm1","alpha max n-1",50,0.,1.5);
  hnemnm1 = new TH1F("hnemnm1","N emerging jets n-1",10,0.,10.);
  }

  //read all entries and fill the histograms
  Int_t nentries = (Int_t)tt->GetEntries();


  // loop over events
  for (Int_t i=0; i<nentries; i++) {
    //    if(i%100 == 0) std::cout<<"event "<<i<<std::endl;
    if(otfile) count->Fill("All",1);  // count number of events
    if(otfile) acount->Fill(0.5);
    tt->GetEntry(i);
    //    std::cout<<"event number is "<<event<<" number of vertex is "<<nVtx<<std::endl;

    // make some basic plots on all events before any selections
    // jets
    vector<int> jet_ntrkpt1((*jet_index).size());
    if(otfile) hnjet->Fill((*jet_index).size()+0.5);
    int NNNjet = (*jet_index).size();
    for(Int_t j=0; j<NNNjet; j++) {
      if(otfile) hpt->Fill((*jet_pt)[j]);
      if(otfile) heta->Fill((*jet_eta)[j]);
      if(otfile) hjetchf->Fill((*jet_chf)[j]);
      if(otfile) if(j<4) heta2->Fill((*jet_eta)[j]);
      if(otfile) halpha->Fill((*jet_alphaMax)[j]);
      //      calculate  number of tracks with pt > 1
      jet_ntrkpt1[j]=0;
      vector<float> track_pts = track_pt->at(j);
      vector<int> track_sources = track_source->at(j);
      for (unsigned itrack=0; itrack<track_pts.size(); itrack++) {
	if(track_sources[itrack]==0) {
	  if(track_pts[itrack]>1) jet_ntrkpt1[j]+=1;
	}
      }
     }  // end of loop over jets



      //now look and see if any of the 4 lead jets are emerging

      bool emerging[4];
      emerging[0]=false;emerging[1]=false;emerging[2]=false;emerging[3]=false;
      int nemerging=0;
      int iijjkk = 4;
      if(NNNjet<4) iijjkk=NNNjet;
      for(int ij=0;ij<iijjkk;ij++) {
	if(otfile) hjetcut->Fill(0.5);
	if(otfile) hbcut_alphamax->Fill((*jet_alphaMax)[ij]);
	if((*jet_alphaMax)[ij]<alphaMaxcut) { // alpha max
	  if(otfile) hacut_alphamax->Fill((*jet_alphaMax)[ij]);
	  if(otfile) hjetcut->Fill(1.5);
	  if(otfile) hbcut_nef->Fill((*jet_nef)[ij]);
	  if((*jet_nef)[ij]<NemfracCut) {  // neutral fraction
	    if(otfile) hacut_nef->Fill((*jet_nef)[ij]);
	    if(otfile) hjetcut->Fill(2.5);
	    if(otfile) hbcut_ntrkpt1->Fill(jet_ntrkpt1[ij]);
	    if(jet_ntrkpt1[ij]>ntrk1cut) {  // tracks pt>1
	      if(otfile) hacut_ntrkpt1->Fill(jet_ntrkpt1[ij]);
	      if(otfile) hjetcut->Fill(3.5);
	      if(otfile) hbcut_cef->Fill((*jet_cef)[ij]);
	      if((*jet_cef)[ij]<CemfracCut) {  //charged fraction
	        if(otfile) hacut_cef->Fill((*jet_cef)[ij]);
	        emerging[ij]=true;
	        nemerging+=1.;
		//		std::cout<<" an emerging jet"<<std::endl;
	      }
	    }
	  }
        }
      }
      if(otfile) h_nemg->Fill(nemerging);


      //*************************************************************
    // now start the event selections
      //*************************************************************

    // require at least 4 jets
    bool C4jet=true;
    if(NNNjet<3) C4jet=false;
    // HT
    double HT = (*jet_pt)[0]+(*jet_pt)[1]+(*jet_pt)[2]+(*jet_pt)[3];
    if(otfile) H_T->Fill(HT);
    if(otfile) hpt1->Fill((*jet_pt)[0]);
    if(otfile) hpt2->Fill((*jet_pt)[1]);
    if(otfile) hpt3->Fill((*jet_pt)[2]);
    if(otfile) hpt4->Fill((*jet_pt)[3]);
    bool CHT=true;
    if(HT<HTcut) CHT=false;
    // jet pt
    bool Cpt1=true;
    bool Cpt2=true;
    bool Cpt3=true;
    bool Cpt4=true;
    if(((*jet_pt)[0]<pt1cut)&&(abs((*jet_eta)[0])<jetacut)) Cpt1=false;
    if(((*jet_pt)[1]<pt2cut)&&(abs((*jet_eta)[1])<jetacut)) Cpt2=false;
    if(((*jet_pt)[2]<pt3cut)&&(abs((*jet_eta)[2])<jetacut)) Cpt3=false;
    if(((*jet_pt)[3]<pt4cut)&&(abs((*jet_eta)[3])<jetacut)) Cpt4=false;
    // number emerging jets
    bool Cnem = true;
    if(nemerging<NemergingCut) Cnem=false;


    // do N-1 plots
    if(otfile) {
    if(C4jet&&Cpt1&&Cpt2&&Cpt3&&Cpt4&&Cnem) hHTnm1->Fill(HT);
    if(C4jet&&CHT&&Cpt2&&Cpt3&&Cpt4&&Cnem) hpt1nm1->Fill((*jet_pt)[0]);
    if(C4jet&&CHT&&Cpt1&&Cpt3&&Cpt4&&Cnem) hpt2nm1->Fill((*jet_pt)[1]);
    if(C4jet&&CHT&&Cpt1&&Cpt2&&Cpt4&&Cnem) hpt3nm1->Fill((*jet_pt)[2]);
    if(C4jet&&CHT&&Cpt1&&Cpt2&&Cpt3&&Cnem) hpt4nm1->Fill((*jet_pt)[3]);
    if(C4jet&&CHT&&Cpt1&&Cpt2&&Cpt3&&Cpt4) hnemnm1->Fill(nemerging);
    if(C4jet&&CHT&&Cpt1&&Cpt2&&Cpt3&&Cpt4) {
      for(int i=0;i<3;i++) halphanm1->Fill((*jet_alphaMax)[i]);
    }
    }

    // apply cuts sequentially

    if(!C4jet) continue;
    if(otfile) count->Fill("4 jets",1);
    if(otfile) acount->Fill(1.5);

    // calculate HT and require it greater than some cut value
    if(!CHT) continue;
    if(otfile) count->Fill("HT",1);
    if(otfile) acount->Fill(2.5);
    if(otfile) H_T2->Fill(HT);

    // do pT cuts on jets  
    if(!Cpt1) continue;
    if(otfile) count->Fill("jet pt1",1);
    if(otfile) acount->Fill(3.5);


    if(!Cpt2) continue;
    if(otfile) count->Fill("jet pt2",1);
    if(otfile) acount->Fill(4.5);


    if(!Cpt3) continue;
    if(otfile) count->Fill("jet pt3",1);
    if(otfile) acount->Fill(5.5);


    if(!Cpt4) continue;
    if(otfile) count->Fill("jet pt4",1);
    if(otfile) acount->Fill(6.5);



      // require at least N emerging jets
      if(!Cnem) continue;



    if(otfile) count->Fill("emerging",1);
    if(otfile) acount->Fill(7.5);
    npass+=1;




  }  // end of loop over events

  if(otfile) {
    TFile myfile(outputfilename,"RECREATE");
    count->LabelsDeflate();
    count->LabelsOption("v");
  //  count->LabelsOption("a");

    eventCountPreTrigger->Write();
    acount->Write();
    count->Write();
    hjetcut->Write();
    hpt->Write();
    hnjet->Write();
    heta->Write();
    heta2->Write();
    halpha->Write();
    H_T->Write();
    H_T2->Write();
    hpt1->Write();
    hpt2->Write();
    hpt3->Write();
    hpt4->Write();
    h_nemg->Write();
    hjetchf->Write();
    hbcut_ntrkpt1->Write();
    hacut_ntrkpt1->Write();
    hbcut_nef->Write();
    hacut_nef->Write();
    hbcut_cef->Write();
    hacut_cef->Write();
    hbcut_alphamax->Write();
    hacut_alphamax->Write();
    hHTnm1->Write();
    hpt1nm1->Write();
    hpt2nm1->Write();
    hpt3nm1->Write();
    hpt4nm1->Write();
    halphanm1->Write();
    hnemnm1->Write();
    myfile.Close();
  }

  tt->ResetBranchAddresses();
  
  delete jet_index;
  delete jet_source;
  delete jet_pt;
  delete jet_eta;
  delete jet_phi;
  delete jet_alphaMax;
  delete jet_cef;
  delete jet_nef;
  delete jet_chf;
  delete jet_phf;
  delete track_pt;
  delete track_eta;
  delete track_source;
  delete track_index;
  delete track_jet_index;
  delete track_vertex_index;
  delete track_algo;
  delete track_vertex_weight;
  delete track_ipZ;
  


  f->Close();
  
  /*  
  delete count;
  delete acount;
  delete hjetcut;
  delete h_nemg;
  delete hnjet;
  delete hpt;
  delete heta;
  delete heta2;
  delete halpha;
  delete H_T;
  delete H_T2;
  delete hbcut_ntrkpt1;
  delete hacut_ntrkpt1;
  delete hbcut_nef;
  delete hacut_nef;
  delete hbcut_cef;
  delete hacut_cef;
  delete hbcut_alphamax;
  delete hacut_alphamax;
  delete hHTnm1;
  delete hpt1nm1;
  delete hpt2nm1;
  delete hpt3nm1;
  delete hpt4nm1;
  delete halphanm1;
  delete hnemnm1;
  */


  return npass;
}
