/****************************************************************************
** Authors: Dario Menasce, Stefano Terzo
**
** I.N.F.N. Milan-Bicocca
** Piazza  della Scienza 3, Edificio U2
** Milano, 20126
**
****************************************************************************/

#include "EventAnalyzer.h"

int hit_=0, total_=0;

//====================================================================
// This class receives a pointer to the eventReader class. This pointer 
// is used to retrieve individual pointers to the objects stored on
// file (theHeader_, theEvent_ and theGeometry_)
//
EventAnalyzer::EventAnalyzer(EventReader * reader)
{
  theHeader_   = reader->getEventHeaderPointer() ;
  theEvent_    = reader->getEventPointer      () ;
  theGeometry_ = reader->getGeometryPointer   () ;

  // Retrieve from file the number of stored events  
  unsigned int numberOfEvents = reader->getNumberOfEvents() ;
  
  ss_.str(""); ss_ << "Found " << numberOfEvents << " events on file" ;
  STDLINE(ss_.str(),ACYellow) ;
  STDLINE("       ",ACYellow) ;

  std::stringstream ss_ ;
  
  // Retrieve individual events and analyze them individually
  hit_=0  ;
  total_=0;
  for(unsigned int event=0; event<numberOfEvents; ++event)
  {
    reader->readEvent(event) ;

    this->analyzeEvent(event) ;   
  }
  
  double efficency = (1.*hit_)/(1.*total_) * 100.;
  std::cout << "efficency    = " << efficency << std::endl;
  std::cout << "total tracks = " << total_    << std::endl;
  std::cout << "hits         = " << hit_      << std::endl;
}

//====================================================================
// This is the method of the eventAnalyzer class where users are supposed 
// to implement their own private code for analysis. Here you will find 
// an exampl of how to retrieve and manipulate components of the event
// and the geometry.
//
// NOTE: for a detailed description of the Event class, its public access
//       methods and the meaning of invidual containers, please consult
//       ../include/Event.h and comments therein.
//
void EventAnalyzer::analyzeEvent(unsigned int event)
{
  Event::clustersMapDef            & clusters                 = theEvent_->getClusters        ()     ;
  Event::fittedTracksDef           & fittedTracks             = theEvent_->getFittedTracks    ()     ;
  Event::chi2VectorDef             & chi2                     = theEvent_->getFittedTracksChi2()     ;
  Event::fittedTracksCovarianceDef & fittedTrackCovariance    = theEvent_->getFittedTracksCovariance();
  Event::trackCandidatesDef        & trackPoints              = theEvent_->getTrackCandidates ()     ;

  if( fittedTracks.size() == 0 ) return ;

  //STDLINE("",ACWhite) ;
  ss_.str(""); ss_ << "Event "           << event ;               //STDLINE(ss_.str(),ACPurple) ;
  ss_.str(""); ss_ << "Clusters found: " << clusters.size() 
                   << "  Tracks: "       << fittedTracks.size() ; //STDLINE(ss_.str(),ACWhite)  ;
  
  //STDLINE("Track           Slope                           Intercept          Chi2",ACLightBlue) ;
  //STDLINE("          X                Y              X                Y           ",ACRed      ) ;

//             X             Y            X             Y           
//    0   -0.00015444948 -299.69584     0.00023105554  -116.2111      1.1585256
//   0123456789-0123456789-0123456789-0123456789-0123456789-0123456789-0123456789-0123456789-

  for(unsigned int tr=0; tr<fittedTracks.size(); ++tr)
  {
    ROOT::Math::SVector<double,4> tParameters = fittedTracks[tr] ;

    ss_.str("") ;
    ss_.setf(std::ios_base::right,std::ios_base::adjustfield) ;
    ss_ << std::setprecision(8) << " ";
    ss_ << std::setw( 4) << tr  
        << std::setw(15) << tParameters[0] 
        << std::setw(15) << tParameters[2] 
        << std::setw(15) << tParameters[1] 
        << std::setw(15) << tParameters[3] 
        << std::setprecision(3)
        << std::setw( 6) << chi2[tr] ; 
    //STDLINE(ss_.str(),ACWhite) ;
    
    //selection for tracks with chi2/DOF
    if( chi2[tr] > 5 ) continue;

    Detector    *dut         = theGeometry_->getDetector(4,0)  ;
    std::string detectorName = theGeometry_->getDetectorID(4,0);
    double xp, yp, zp, tmp, xm ,ym;
    
    //get the track impact point on dut in the local detector frame
    dut->getPredictedLocal( tParameters, xp, yp );                                                                                               
    
    //check if the impact point is on the dut surface
    if( xp < 0 || xp > dut->getDetectorLengthX(true) || 
        yp < 0 || yp > dut->getDetectorLengthY(true)   ) continue;

    //increse the total tracks number
    total_++;

    //check if there's a hit for this track on dut
    if( trackPoints[tr].find(detectorName) == trackPoints[tr].end() ) continue;
    int clusterNumber = (int)trackPoints[tr][detectorName]["num"];
    
    //get the relative cluster coordinate (that's in the local coordinate frame)
    xm = clusters[detectorName][ clusterNumber ]["x"];
    ym = clusters[detectorName][ clusterNumber ]["y"];
    
    //or compute the local coordinate from the global ones
    //xm = trackPoints[tr][detectorName]["x"] 
    //ym = trackPoints[tr][detectorName]["y"]
    //dut->fromGlobalToDetector(&xm   ,
    //                          &ym   ,
    //                          &trackPoints[tr][detectorName]["xErr"],
    //                          &trackPoints[tr][detectorName]["yErr"],
    //                          &trackPoints[tr][detectorName]["zErr"]  );
    
    //compute the x and y distances between mesured and predicted point (unostrained residuals)
    double distX  = ( xm - xp );
    double distY  = ( ym - yp );
    
    
    //get the track error and sum to the cluster error

    Detector::xyPair predPair = dut->getTrackErrorsOnPlane( fittedTracks[tr], fittedTrackCovariance[tr] );
    double xWindow = predPair.first  + pow( clusters[detectorName][ clusterNumber ]["xErr"] , 2 );
    double yWindow = predPair.second + pow( clusters[detectorName][ clusterNumber ]["yErr"] , 2 );
    //if the point is within n sigma got a hit
    double nSigma = 2;
    if( fabs(distX) > nSigma*sqrt(xWindow) ) continue;
    if( fabs(distY) > nSigma*sqrt(yWindow) ) continue;
    
    hit_++;
  }
                                 
}
