#include <iostream>
#include <string>
#include <map>

void QCDhists(float goalintlum,int nbin, float* xsec, int* nfiles, std::string* binnames,std::string* aaname,std::string ohname, int dooptk, int doopta) ;



int main(int argc, char *argv[])
{ 
  int dooptk =*(argv[1])-'0';
  int doopta =*(argv[2])-'0';
  int imode=*(argv[3])-'0';

  if(dooptk==0) {
    std::cout<<"not doing kinematic optimization"<<std::endl;
  } else if(dooptk==1) {
    std::cout<<"doing kinematic optimization"<<std::endl;
  } else {
    std::cout<<"invalid choice"<<std::endl;
  }

  if(doopta==0) {
    std::cout<<"not doing optimization on an emerging jet variable"<<std::endl;
  } else if(doopta==1) {
    std::cout<<"doing alphamax optimization"<<std::endl;
  } else if(doopta==2) {
    std::cout<<"doing maxIP optimization"<<std::endl;
  } else {
    std::cout<<"invalid choice"<<std::endl;
  }

  if(imode==0) {
    std::cout<<"doing background"<<std::endl;
  } else if(imode==1) {
    std::cout<<"doing modelA"<<std::endl;
  } else if(imode==2) {
    std::cout<<"doing modelB"<<std::endl;
  } else if(imode==3) {
    std::cout<<"doing debug sample"<<std::endl;
  } else {
    std::cout<<"invalid choice"<<std::endl;
  }


float goalintlum=20; // fb-1                                                                                        

// aaname is the vector of names of the config files with input file lists
//std::string aaname = "/data/users/eno/outputQCD/";  // area containing subdirectors with sarah's ntuples
//std::string aaname = "/mnt/hadoop/cms/store/DARKQCD/YHShin_ntuples/";  // area containing subdirectors with young's ntuples

// for background
//const int nbin=5; // 500-700,700-1000,1000-1500,1500-2000,200toInf
 const int nbin=3;
 //float xsec[nbin]={29370000,6524000,1064000,121500,25420}; // fb 
float xsec[nbin]={6524000,1064000,121500}; // fb 
 int nfiles[nbin]={152,66,46};
//int nfiles[nbin]={138,133,50,40,23}; //sarah's
 //int nfiles[nbin]={20,20,20,20,20}; //young's

 std::string aaname_qcd[nbin] = {/*"configs/QCD_HT500to700_74X_config.txt",*/"configs/QCD_HT700to1000_74X_config.txt","configs/QCD_HT1000to1500_74X_config.txt","configs/QCD_HT1500to2000_74X_config.txt"/*,"configs/QCD_HT2000toInf_74X_config.txt"*/};
 //std::string binnames[nbin]={"QCD_HT500to700","QCD_HT700to1000","QCD_HT1000to1500","QCD_HT1500to2000","QCD_HT2000toInf"};
 std::string binnames[nbin]={"QCD_HT700to1000","QCD_HT1000to1500","QCD_HT1500to2000"};



// for signal models A.  mediat mass is 1000
const int anbin=1; 
float axsec[nbin]={18.45}; // fb 
//int anfiles[nbin]={71}; 
int anfiles[anbin]={56};
std::string aaname_modA[anbin]={"configs/ModelA_74X_config.txt"}; 
std::string abinnames[anbin]={"modelA"}; //sarah's
//std::string abinnames[nbin]={"ModelA"}; //young's

// for signal models B.  mediat mass is 1000
const int bnbin=1; 
float bxsec[nbin]={18.45}; // fb 
//int bnfiles[nbin]={50}; 
int bnfiles[bnbin]={99}; 
std::string aaname_modB[bnbin]={"configs/ModelB_74X_config.txt"}; 
std::string bbinnames[bnbin]={"modelB"}; //sarah's
//std::string bbinnames[nbin]={"ModelB"}; //young's


// for debugging
const int dnbin=1; 
float dxsec[nbin]={18.45}; // fb 
int dnfiles[nbin]={2}; 
std::string aaname_d[dnbin]={"configs/ModelB_74X_config.txt"}; 
std::string dbinnames[dnbin]={"modelB"};



 if(imode==0) {
   QCDhists(goalintlum,nbin,xsec,nfiles,binnames,aaname_qcd,"SumHistsQCD.root",dooptk,doopta);
 } else if (imode==1) {
   QCDhists(goalintlum,anbin,axsec,anfiles,abinnames,aaname_modA,"SumHistsModelA.root",dooptk,doopta);
 } else if (imode==2) {
   QCDhists(goalintlum,bnbin,bxsec,bnfiles,bbinnames,aaname_modB,"SumHistsModelB.root",dooptk,doopta);
 } else if (imode==3) {
   QCDhists(goalintlum,dnbin,dxsec,dnfiles,dbinnames,aaname_d,"SumHistsDebug.root",0,0);
 }
}
