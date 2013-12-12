#include "DataClass.h"
#include <TTree.h>
#include <TBranch.h>
#include <iostream>


//Index
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////
DataClass::DataClass() : MaxCol_(0), MaxRow_(0)
{ 
}

///////////////////////////////////////////////////////////////////////////////////////
DataClass::~DataClass()
{
}

///////////////////////////////////////////////////////////////////////////////////////
void DataClass::setBranchAddress(TTree* tree)
{
  tree->SetBranchAddress("evtId"   , &evtId_); 	 //0
  tree->SetBranchAddress("ntrack"  , &ntrack_);
  tree->SetBranchAddress("itrack"  , &itrack_);
  tree->SetBranchAddress("nhits"   , &nhits_);
  tree->SetBranchAddress("ngoodx"  , &ngoodx_);     
  tree->SetBranchAddress("ngoody"  , &ngoody_);     //5
  tree->SetBranchAddress("ndof"    , &ndof_);
  tree->SetBranchAddress("chi2"    , &chi2_);
  tree->SetBranchAddress("prob"    , &prob_);	   
  tree->SetBranchAddress("xproj0"  , &xproj0_);     
  tree->SetBranchAddress("yproj0"  , &yproj0_);     //10
  tree->SetBranchAddress("xprojErr", &xprojErr_); 
  tree->SetBranchAddress("yprojErr", &yprojErr_);
  tree->SetBranchAddress("xslope"  , &xslope_);  	   
  tree->SetBranchAddress("yslope"  , &yslope_);     
  tree->SetBranchAddress("hasHit"  , &hasHit_);  	  //15
  tree->SetBranchAddress("measureX", &measureX_);	
  tree->SetBranchAddress("npix"    , &npix_);
  tree->SetBranchAddress("ncol"    , &ncol_);	   
  tree->SetBranchAddress("nrow"    , &nrow_);	  
  tree->SetBranchAddress("charge"  , &charge_);    //20
  tree->SetBranchAddress("col"     , &col_); 			
  tree->SetBranchAddress("row"     , &row_);
  tree->SetBranchAddress("xp"      , &xp_);
  tree->SetBranchAddress("yp"      , &yp_);  	  
  tree->SetBranchAddress("xl"      , &xl_);  	  //25
  tree->SetBranchAddress("yl"      , &yl_);     
  tree->SetBranchAddress("xpErr"   , &xpErr_);
  tree->SetBranchAddress("ypErr"   , &ypErr_);	  
  tree->SetBranchAddress("ndofExcl", &ndofExcl_);   
  tree->SetBranchAddress("chi2Excl", &chi2Excl_);   //30
  tree->SetBranchAddress("probExcl", &probExcl_);   
  tree->SetBranchAddress("xresid"  , &xresid_);    
  tree->SetBranchAddress("yresid"  , &yresid_);
  tree->SetBranchAddress("xtErr"   , &xtErr_);	  
  tree->SetBranchAddress("ytErr"   , &ytErr_);		  //35
  
  tree->SetBranchAddress("belongsToTrack", &belongsToTrack_);
  //tree->SetBranchAddress("xresidCalc"    , &xresidCalc_    );    
  //tree->SetBranchAddress("yresidCalc"    , &yresidCalc_    );
};
///////////////////////////////////////////////////////////////////////////////////////
const float& DataClass::getMaxCol( int p, TTree* tree )
{
  setBranchAddress(tree) ;
  for( int i = 0; i < 100000; ++i)
  {
    tree->GetEntry(i) ;
    if( getCol(p) > MaxCol_ ) MaxCol_ = getCol(p) ;
  }

  return MaxCol_ ;
}
///////////////////////////////////////////////////////////////////////////////////////
const float& DataClass::getMaxRow( int p, TTree* tree )
{
  setBranchAddress(tree) ;
  for( int i = 0; i < 100000; ++i)
  {
    tree->GetEntry(i) ;
    if( getRow(p) > MaxRow_ ) MaxRow_ = getRow(p) ;
  }
  
  return MaxRow_ ;

}
