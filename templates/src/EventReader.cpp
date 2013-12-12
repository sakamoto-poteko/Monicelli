/****************************************************************************
** Authors: Dario Menasce, Stefano Terzo
**
** I.N.F.N. Milan-Bicocca
** Piazza  della Scienza 3, Edificio U2
** Milano, 20126
**
** This class is provided as a service to deal with the details of how to 
** extract events with hits and recnstructed tracks from a file produced by 
** Monicelli.
**
** NOTE:
**     casual users are not supposed nor required to modify or even understand 
**     implementation details of this class, see instead the eventAnalyzer.ccp 
**     files for a template of your own analysis algorithm                     
**
****************************************************************************/

#include "EventReader.h"

//====================================================================
EventReader::EventReader(void)
{
  STDLINE("                                   ", ACCyan) ;
  STDLINE("+=================================+", ACCyan) ;
  STDLINE("|                                 |", ACCyan) ;
  STDLINE("|    Welcome to readerTemplate    |", ACCyan) ;
  STDLINE("|                                 |", ACCyan) ;
  STDLINE("| Template analyzer of data files |", ACCyan) ;
  STDLINE("|  aligned and reconstructed by   |", ACCyan) ;
  STDLINE("|        M o n i c e l l i        |", ACCyan) ;
  STDLINE("|                                 |", ACCyan) ;
  STDLINE("+=================================+", ACCyan) ;
  STDLINE("                                   ", ACCyan) ;
   
}

//====================================================================
 bool EventReader::openFile(std::string inputFileName)
 {
  if(inputFileName.find(".root") != -1)
    fileName_ = inputFileName;
  std::string fullPath = path_ + inputFileName ;
 STDLINE(path_        ,ACPurple) ;
 STDLINE(inputFileName,ACPurple) ;
  TFile * inputFile = new TFile( fullPath.c_str(), "read" );

  if( !inputFile->IsOpen() )
  {
     STDLINE(std::string("Could not open file ") + fullPath, ACRed) ;
     exit(EXIT_FAILURE) ;
  }
  
  ss_.str(""); ss_ << "File " << ACYellow << fullPath << ACGreen << " successfully opened" ;
  STDLINE(ss_.str(), ACGreen) ;
  STDLINE("       ", ACGreen) ;
  
  std::stringstream geometryTreeName;
  std::stringstream eventsTreeName;
  std::stringstream eventsHeaderName;
  geometryTreeName << "Geometry";
  eventsTreeName   << inputFileName << "Events";
  eventsHeaderName << inputFileName << "Header";

  theEvent_       = new Event() ;
  theGeometry_    = new Geometry() ;
  theEventHeader_ = new EventHeader() ; 
  
  STDLINE("",ACWhite) ; STDLINE("----- File content ------", ACYellow) ;
  inputFile->ls() ;
  STDLINE("--------------------------", ACYellow) ;STDLINE("",ACWhite) ;

  STDLINE(eventsTreeName.str(),ACPurple) ;

  bool atLeastOneBranchFound = false ;

  STDLINE("Checking for GeometryBranch...",ACWhite) ;
  if ( (TTree*)inputFile->Get(geometryTreeName.str().c_str()) )
  {
     inputGeometryTree_ = (TTree*)inputFile->Get(geometryTreeName.str().c_str());

     inputGeometryTree_->SetBranchAddress("GeometryBranch", &theGeometry_   );

     inputGeometryTree_->GetEntry(0);

     ss_.str("");
     ss_ << "Found " << inputGeometryTree_->GetEntries() << " geometry entries";
     STDLINE(ss_.str(),ACGreen) ;
     
     atLeastOneBranchFound = true ;
  }
  
  STDLINE("Checking for EventBranch...",ACWhite) ;
  if ( (TTree*)inputFile->Get(eventsTreeName.str().c_str()) )
  {
     inputEventTree_   = (TTree*)inputFile->Get(eventsTreeName.str().c_str()) ;

     inputEventTree_    ->SetBranchAddress("EventBranch",    &theEvent_      );

     ss_.str("");
     ss_ << "Found " << inputEventTree_->GetEntries() << " events on file";
     STDLINE(ss_.str(),ACGreen) ;

     inputEventHeader_ = (TTree*)inputFile->Get(eventsHeaderName.str().c_str()) ;

     if( inputEventHeader_ )
     {
       inputEventHeader_->SetBranchAddress("EventHeader",    &theEventHeader_);

       inputEventHeader_->GetEntry(0);
 
       ss_.str("");
       ss_ << "Found " << inputEventHeader_->GetEntries() << " headers";
       STDLINE(ss_.str(),ACGreen) ;
     }
     
     atLeastOneBranchFound = true ;
  }
  
  if( !atLeastOneBranchFound )
  {
    STDLINE("FATAL: no branches found in file. Aborting",ACRed) ;
    exit(0) ;
  }
  
  return true;
 }
