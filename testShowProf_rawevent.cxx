//
// take raw data and plot profile
//


#include <TTree.h> //represents a columnar data set
#include <TGraph.h> //Object made of two arrays X and Y with npoints each
#include <TFile.h> //It is a file directory, it may contain directories and objects
#include <TSystem.h> // To define a generic interface to the underlying Operation System
#include <TCanvas.h> // Needed to make plots
#include <TProfile2D.h> // Used to plot profile 2D histograms, with X, Y, and Z but better precission than scatter-plot
#include <TStyle.h> //To define special styles (special canvas style)



void testShowProf_rawevent(char* rootfile, int nevn)
{

  using std::count;//returns number of ocurrences of an element
  using std::endl;//new line
  gStyle ->SetCanvasDefH(900);//To define canvas height
  gStyle ->SetCanvasDefW(1500);//To define canvas width

  gSystem->Load("../libData.so");//Loads shared library

  TFile *fr = new TFile(rootfile);//Redefining TFile
  //
  TTree *tr = (TTree*)fr->Get("rtree");//Getting data from "rtree"

  RawEvent *revent = new RawEvent();//Defining RawEvent
//Set up number of elements in arrays
  TGraph *gr[10000];
  Int_t x[10000];
  Int_t y[10000];
  Int_t peakhigh[10000];
  Int_t vpeakhigh[10000];//vpeakhigh refers to high part of the wave voltage
  Int_t vpeaksum[10000];//vpeaksum refers to the total high of the wave voltage from bottom to top
  Int_t peaktime[10000];
  Int_t peaksum[10000];
  Int_t peaksumtime1[10000];
  Int_t peaksumtime2[10000];


  Int_t ch_vpeakhigh[10000][10000];
  Int_t ch_vpeakhigh_time_us[10000][10000];
  Int_t ch_vpeakhigh_time_ns[10000][10000];

  Long_t ch_vpeaksum0[10000][10000];
  Int_t ch_vpeaksum0_peak[10000][10000];
  Long_t ch_vpeaksum_tail[10000][10000];
  Long_t ch_vpeaksum_peak[10000][10000];


  Int_t ch_vpeaksum[10000][10000];
  Int_t ch_vpeaksum_time0_us[10000][10000];
  UInt_t ch_vpeaksum_time0_ns[10000][10000];

  Double_t ch_vpeaksum_time1_ms[10000][10000];
  Double_t ch_vpeaksum_time1_us[10000][10000];
  UInt_t ch_vpeaksum_time1_ns[10000][10000];//Uint_t refers to unsigned integers 4 bytes

  Double_t ch_vpeaksum_time2_us[10000][10000];
  UInt_t ch_vpeaksum_time2_ns[10000][10000];

  Int_t npeak_ch0[10000];
  Int_t npeak_ch1[10000];
  Int_t npeak_ch2[10000];
  Int_t npeak_ch3[10000];

  Int_t nsum_ch0[10000];
  Int_t nsum_ch1[10000];
  Int_t nsum_ch2[10000];
  Int_t nsum_ch3[10000];

  TGraph *gr[10000];
  Int_t nraw[10000];
  Int_t raw_x[10000];
  Int_t raw_y[10000];

  int nevents = tr->GetEntries();



  hprof2d_vpeakheight_ms = new TProfile2D("hprof2d_vpeakheight_ms","vprofile 2D peakheight ms  ",600,-50,550,5000,0,5000,-2500,0);//File,Title, x-axis(-50),x-axis(550), number events,y-axis(0), y-axis(5000),z-axis(-2500),z-axis(0)

  hprof2d_vpeaksum_ms = new TProfile2D("hprof2d_vpeaksum_ms","vprofile 2D peaksum ms  ",600,-50,550,5000,0,5000,-30000,0);
  hprof2d_vpeaksum_160ms = new TProfile2D("hprof2d_vpeaksum_160ms","vprofile 2D peaksum 160ms  ",210,-50,160,5000,0,5000,-30000,0);
  hprof2d_vpeaksum_us_batch1 = new TProfile2D("hprof2d_vpeaksum_us_batch1","vprofile 2D peaksum us batch 1 ",3000,0,3000,5000,0,5000,-30000,0);
  hprof2d_vpeaksum_us_batch2 = new TProfile2D("hprof2d_vpeaksum_us_batch2","vprofile 2D peaksum us batch 2 ",3000,66000,69000,5000,0,5000,-30000,0);

  tr->SetBranchAddress("ch3.",&revent);
  printf("CH3 CH3 CH3 \n");
  for(int i= 0; i< nevn ;i++)
  //for(int i= 0; i< nevents ;i++)
  //for(int i= 0; i< 10 ;i++)
   { 
        tr->GetEntry(i);
	nraw[i] = revent -> GetVAmpSize(); //Get values for variables
        npeak_ch3[i] = revent->GetVPeakHighSize();
        nsum_ch3[i] = revent->GetVPeakSumSize();
	printf("event %d \t", i);
        printf("nraw_ch3 = %d\t",nraw[i]);//Print every column for previous variables
        printf("npeak_ch3 = %d\t",npeak_ch3[i]);
        printf("nsum_ch3 = %d\n",nsum_ch3[i]);


        for(int j =0; j< npeak_ch3[i] ;j++)
        {
        //Going column by column and row by row and pick the different values for the peak and put them together
           ch_vpeakhigh[i][j]=revent->GetVPeakHigh()[j];
           ch_vpeakhigh_time_us[i][j]=revent->GetVPeakHighTime()[j]*0.001; //convert to us
           ch_vpeakhigh_time_ns[i][j]=revent->GetVPeakHighTime()[j]; //ns
           hprof2d_vpeakheight_ms -> Fill(ch_vpeakhigh_time_us[i][j]*0.001,i,ch_vpeakhigh[i][j],1);//Fill canvas with information that we are interested on
       }               

       for(int j =0; j< nsum_ch3[i] ;j++)
       {
           //Going column by column and row by row and pick the different values for the sum and put them together
          ch_vpeaksum0[i][j]=revent->GetVPeakSum0()[j];
          ch_vpeaksum[i][j]=revent->GetVPeakSum()[j];
          ch_vpeaksum0_peak[i][j]=revent->GetVPeakSum0_peak()[j];

          ch_vpeaksum_time0_us[i][j]=revent->GetVPeakSumTime0()[j]*0.001; //convert to us
          ch_vpeaksum_time0_ns[i][j]=revent->GetVPeakSumTime0()[j]; //ns

          ch_vpeaksum_time1_ms[i][j]=revent->GetVPeakSumTime1()[j]*0.000001; //convert to ms
          ch_vpeaksum_time1_us[i][j]=revent->GetVPeakSumTime1()[j]*0.001; //convert to us
          ch_vpeaksum_time1_ns[i][j]=revent->GetVPeakSumTime1()[j]; //ns
           
           //----------Good way would be to print the times and then ry to work from there---------
           
          //printf("time sum1 = %d \t", ch_vpeaksum_time1_ns[i][j]);
          //printf("time sum0 = %d \t", ch_vpeaksum_time0_ns[i][j]);

          ch_vpeaksum_time2_us[i][j]=revent->GetVPeakSumTime2()[j]*0.001; //convert to us
          ch_vpeaksum_time2_ns[i][j]=revent->GetVPeakSumTime2()[j]; //ns
          //printf("time sum2 = %d \n", ch_vpeaksum_time2_ns[i][j]);


          hprof2d_vpeaksum_ms -> Fill(ch_vpeaksum_time1_ms[i][j],i,ch_vpeaksum[i][j],1);

             if(ch_vpeaksum_time1_us[i][j] < 3000)
               {
                hprof2d_vpeaksum_us_batch1 -> Fill(ch_vpeaksum_time1_us[i][j],i,ch_vpeaksum[i][j],1);
               } else if(ch_vpeaksum_time1_us[i][j] > 66000 && ch_vpeaksum_time1_us[i][j] < 69000)
               {
                hprof2d_vpeaksum_us_batch2 -> Fill(ch_vpeaksum_time1_us[i][j],i,ch_vpeaksum[i][j],1);
               } 
       }
      

  }
        
    
  

   TCanvas *c_prof_ms = new TCanvas("c_prof_ms","profile sum ms ",200,10,700,500);
   TCanvas *c_prof_us_batch1 = new TCanvas("c_prof_us_batch1","profile sum us batch1",200,10,700,500);
   TCanvas *c_prof_us_batch2 = new TCanvas("c_prof_us_batch2","profile sum us batch2",200,10,700,500);

  c_prof_ms->cd();
  hprof2d_vpeaksum_ms->Draw("colz");

  c_prof_us_batch1->cd();
  hprof2d_vpeaksum_us_batch1->Draw("colz");

  c_prof_us_batch2->cd();
  hprof2d_vpeaksum_us_batch2->Draw("colz");

}
