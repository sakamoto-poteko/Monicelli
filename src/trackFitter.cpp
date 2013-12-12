/****************************************************************************
** Authors: Dario Menasce, Stefano Terzo
**
** I.N.F.N. Milan-Bicocca
** Piazza  della Scienza 3, Edificio U2
** Milano, 20126
**
****************************************************************************/

#include "trackFitter.h"

//===============================================================================
trackFitter::trackFitter(void)
{
    count=0;
    STDLINE("empty constructor",ACWhite);
    debug_ = false;
    tracksFitted_ = false ;
}

//===============================================================================
Event::fittedTracksDef trackFitter::fitTracks(const Event::trackCandidatesDef&  tracks, Geometry *theGeometry, std::string excludedDetector )
{
    fittedTracks_.clear();
    chi2_.clear()        ;
    covMat_.clear()      ;

    for (unsigned int i=0; i < tracks.size(); ++i)
    {
        ss_.str("");
        ss_ << "Track n " << i << " - Excluded detector: " << excludedDetector;
        if(debug_) STDLINE(ss_.str(),ACGreen);
        trackFitter::aFittedTrackDef aFittedTrack = this->fitSingleTrack(tracks[i], theGeometry, excludedDetector);
        fittedTracks_.push_back( aFittedTrack.first.first  );
        covMat_.push_back( aFittedTrack.first.second )      ;
        chi2_.push_back( aFittedTrack.second )              ;

        if( excludedDetectorFound_ ) this->makeDetectorTrackResiduals(aFittedTrack.first.first, aFittedTrack.first.second,
                                                                      clusters_               , tracks                   ,
                                                                      theGeometry             , excludedDetector         , i);
        //assert(0);
        //if ( i+1 == tracks.size() ) count++;
    }

    tracksFitted_ = true ;

    return fittedTracks_;
}

//===============================================================================
trackFitter::aFittedTrackDef trackFitter::fitSingleTrack(const Event::alignedHitsCandidateMapDef& alignedHits, Geometry* theGeometry, std::string excludedDetector )
{
    ROOT::Math::SMatrix<double,4,4> AtVA      ;
    ROOT::Math::SMatrix<double,4,4> AtVAInv   ;
    ROOT::Math::SMatrix<double,2,2> sigmaXY   ;
    ROOT::Math::SMatrix<double,2,2> sigmaXYInv;
    ROOT::Math::SVector<double,4>   AtVxy     ;
    ROOT::Math::SVector<double,4>   pars      ;

    double chi2=0;
    excludedDetectorFound_ = false;

    for(Event::alignedHitsCandidateMapDef::const_iterator tr=alignedHits.begin(); tr!=alignedHits.end(); tr++)
    {
        if( theGeometry->getDetector( (*tr).first )->isDUT() ) continue;

        if( (*tr).first == excludedDetector )
        {
            excludedDetectorFound_ = true;
            continue;
        }

        Event::aClusterDef hit = tr->second;
        double x     = hit["x"];
        double y     = hit["y"];
        double z     = hit["z"];
        sigmaXY(0,0) = hit["xErr"] * hit["xErr"];
        sigmaXY(0,1) = hit["xyErr"];
        sigmaXY(1,0) = hit["xyErr"];
        sigmaXY(1,1) = hit["yErr"] * hit["yErr"];

        int ifail;
        sigmaXYInv = sigmaXY.Inverse(ifail);
        // AtVA
        for(int r=0; r<4; r++)
            for(int c=0; c<4; c++)
                AtVA(r,c) += sigmaXYInv(r/2,c/2)*pow(z,(2-(r%2+c%2)));

        // AtVxy
        for(int c=0; c<4; c++)
            AtVxy[c] += pow(z,1-c%2)*(x*sigmaXYInv(0,c/2)+y*sigmaXYInv(c/2,1));
        if(debug_)
        {
            ss_.str("");
            ss_ << "Filling matrix";
            //STDLINE(ss_.str(),ACYellow);

            ss_.str("");
            ss_ << "Detector "  << tr->first;
            ss_ << ": Rotation alpha: " << -(theGeometry->getDetector( (*tr).first )->getXRotation() + theGeometry->getDetector( (*tr).first )->getXRotationCorrection())
                << " - beta: "  << -(theGeometry->getDetector( (*tr).first )->getYRotation() + theGeometry->getDetector( (*tr).first )->getYRotationCorrection())
                << " - gamma: " << -(theGeometry->getDetector( (*tr).first )->getZRotationCorrection());
            STDLINE(ss_.str(),ACRed);


            ss_.str("");
            ss_ << "x prime: "  << clusters_[(*tr).first][(int)hit["num"]]["x"] - theGeometry->getDetector( (*tr).first )->getXPosition()
                << "+-" << clusters_[(*tr).first][(int)hit["num"]]["xErr"]
                << " y prime: " << clusters_[(*tr).first][(int)hit["num"]]["y"] - theGeometry->getDetector( (*tr).first )->getYPosition()
                << "+-" << clusters_[(*tr).first][(int)hit["num"]]["yErr"]
                << " z center: " << theGeometry->getDetector( (*tr).first )->getZPositionTotal();
            STDLINE(ss_.str(),ACPurple);

            ss_.str("");
            ss_ << "x: "   << hit["x"] << "+-" << hit["xErr"]
                << " y: "  << hit["y"] << "+-" << hit["yErr"]
                << " z: "  << hit["z"];
            STDLINE(ss_.str(),ACPurple);
        }
    }

    int fail = 0;
    AtVAInv = AtVA.Inverse(fail);
    if ( fail )
    {
        STDLINE("WARNING: Matrix inversion failed!",ACRed);

        printMatrix("AtVA: ", AtVA);
        double det;

        AtVA.Det2(det);
        ss_.str("");
        ss_ << "det: " << std::setprecision(15) << det
               ;
        STDLINE(ss_.str(),ACRed);

        printMatrix("AtVAInv: ",AtVAInv);

        //assert(0);
        ss_.str("");
        //ss_ << "faild count " << count++;
        //STDLINE(ss_.str(),ACPurple);

        //continue;
    }

    pars = AtVAInv*AtVxy;

    ss_.str("");
    ss_ << "alignedHits.size: " << alignedHits.size();
    //STDLINE(ss_.str(),ACPurple)
    for(Event::alignedHitsCandidateMapDef::const_iterator tr=alignedHits.begin(); tr!=alignedHits.end(); tr++)
    {
        if( theGeometry->getDetector( tr->first )->isDUT() )  continue;
        if( tr->first == excludedDetector )                   continue;

        Event::aClusterDef hit = tr->second;
        chi2 += pow( hit["x"] - pars[0]*hit["z"] - pars[1], 2 )/(hit["xErr"]*hit["xErr"]) +
                pow( hit["y"] - pars[2]*hit["z"] - pars[3], 2 )/(hit["yErr"]*hit["yErr"])  ;

    }

    if(debug_)
    {
        ss_.str("");
        ss_ << " chi2: " << chi2;
        STDLINE(ss_.str(),ACYellow);

        ss_.str("");
        ss_ << "Fit Results sx: "  << pars[0]
            << " qx: " << pars[1]
            << " sy: " << pars[2]
            << " qy: " << pars[3];
        STDLINE(ss_.str(), ACYellow);
    }

    trackFitter::aFittedTrackDef aFittedTrack;
    aFittedTrack.first.first = pars          ;
    aFittedTrack.first.second= AtVAInv         ;
    aFittedTrack.second      = chi2/(alignedHits.size()*2 - 4)          ;

    return aFittedTrack;
}

//===============================================================================
void trackFitter::printMatrix(std::string sm, Event::matrixDef & matrix)
{
    std::cout << sm << std::endl ;
    for(int r=0; r<4; ++r)
    {
        for(int c=0; c<4; ++c)
        {
            std::cout << std::setw(24) << std::setprecision(18) << matrix(r,c) ;
        }
        std::cout << std::endl ;
    }
    std::cout << std::endl ;
}

//================================================================
void trackFitter::makeDetectorTrackResiduals ( ROOT::Math::SVector<double,4>   &fittedTrack,
                                               Event::matrixDef                &covMat     ,
                                               const Event::clustersMapDef     &clusters   ,
                                               const Event::trackCandidatesDef &tracks     ,
                                               Geometry*                        theGeometry,
                                               std::string                      detector   ,
                                               int                              trackNum   )
{

    int clusterNumber;
    Event::alignedHitsCandidateMapDef::const_iterator trackIt1;
    Event::clusterCoordinateDef::const_iterator       trackIt2;
    if( (trackIt1 = tracks[trackNum].find(detector)) == tracks[trackNum].end())
    {
        residualsMap_[trackNum].erase(detector);
        pullMap_     [trackNum].erase(detector);
        return;
    }
    else if((trackIt2 = trackIt1->second.find("num")) == trackIt1->second.end())
        return;

    clusterNumber = (int)trackIt2->second;

    Detector* det = theGeometry->getDetector( detector );
    Event::clustersMapDef::const_iterator clustersIt;
    Event::aClusterMapDef::const_iterator clusterIt;
    if( (clustersIt = clusters.find(detector)) == clusters.end() || (clusterIt = clustersIt->second.find(clusterNumber)) == clustersIt->second.end())
        return;

    Event::aClusterDef cluster = clusterIt->second;

    double xm    = cluster["x"];
    double ym    = cluster["y"];
    double xmErr = cluster["xErr"];
    double ymErr = cluster["yErr"];
    //double zm    = 0;
    //double zmErr = 0;
    //det->fromDetectorToGlobal(&xm,&ym,&zm,&xmErr,&ymErr,&zmErr);

    double xp     = 0;
    double yp     = 0;
    //double zp     = 0;
    double xpErr  = 0;
    double ypErr  = 0;
    //double zpErr  = 0;

    det->getPredictedLocal(fittedTrack, xp, yp);

    det->flipPositionLocal(&xm, &ym, &xmErr, &ymErr);
    det->flipPositionLocal(&xp, &yp, &xpErr, &ypErr);

    double resX  = ( xm - xp );
    double resY  = ( ym - yp );

    Detector::xyPair predPair = det->getTrackErrorsOnPlane( fittedTrack, covMat );

    double predsigxx = predPair.first  + pow( xmErr , 2 );
    double predsigyy = predPair.second + pow( ymErr , 2 );

    double pullX = resX/sqrt(predsigxx);
    double pullY = resY/sqrt(predsigyy);


    residualsMap_[trackNum][detector] = std::make_pair(resX, resY);
    pullMap_     [trackNum][detector] = std::make_pair(pullX, pullY)   ;

    if(debug_)
    {
        std::cout << "****************************************************************************************" << std::endl;
        ss_.str("");
        ss_ << "Detector: " << detector;
        STDLINE(ss_.str(), ACGreen);
        ss_.str("");
        ss_ << "ResX: " << resX;
        ss_ << " ResY: " << resY;
        STDLINE(ss_.str(), ACGreen);
        std::cout << "****************************************************************************************" << std::endl;
    }
    /*
    xm    = cluster["x"];
    ym    = cluster["y"];
    double zm    = 0;
    xmErr = cluster["xErr"];
    ymErr = cluster["yErr"];
    double zmErr = 0;
    det->test(&xm,&ym,&zm,&xmErr,&ymErr,&zmErr);
*/
    xp = 0;
    yp = 0;
    double zp = 0;
    //std::cout << "[makeDetectorTrackResiduals()]" << "Number of clusters: " << clusters.size() << std::endl;
    det->getPredictedGlobal(fittedTrack, xp, yp, zp);

}

//================================================================
void trackFitter::makeFittedTracksResiduals(Event *theEvent, Geometry *theGeometry)
{

    //    residualsMap_ = theEvent->getFittedTrackResiduals();
    //    pullMap_      = theEvent->getFittedTrackPulls()    ;

    residualsMap_.clear() ;
    pullMap_     .clear() ;

    Event::clustersMapDef                       & clusters                            = theEvent->getClusters()       ;
    Event::trackCandidatesDef                   & tracks                              = theEvent->getTrackCandidates();
    for (unsigned int i=0; i < tracks.size(); ++i)
    {
        for(Event::alignedHitsCandidateMapDef::iterator det=tracks[i].begin(); det!=tracks[i].end(); det++)
        {
            if(!selectedDetectors_.empty())
            {
                if( selectedDetectors_.find(det->first) ==  selectedDetectors_.end() )
                    continue;
            }

            if(debug_)
            {
                ss_.str("");
                ss_ << "Track n " << i << " - Excluded detector: " << det->first;
                STDLINE(ss_.str(),ACGreen);
            }
            trackFitter::aFittedTrackDef aFittedTrack = this->fitSingleTrack(tracks[i], theGeometry, det->first);
            theEvent->addUnconstrainedFittedTrack(i,det->first,aFittedTrack.first.first,aFittedTrack.first.second,aFittedTrack.second);

            this->makeDetectorTrackResiduals(aFittedTrack.first.first, aFittedTrack.first.second,
                                             clusters                , tracks                   ,
                                             theGeometry             , det->first               , i);
        }
    }
    theEvent->setFittedTrackResiduals(residualsMap_);
    theEvent->setFittedTrackPulls(pullMap_         );
}
//================================================================
void  trackFitter::clear(void )
{
    residualsMap_     .clear();
    pullMap_          .clear();
    fittedTracks_     .clear();
    covMat_           .clear();
    chi2_             .clear();
    selectedDetectors_.clear();
}
//=================================================================
void trackFitter::execute(Event * theEvent, Geometry * theGeometry)
{
    (this->*subProcessOperation_)(theEvent,theGeometry);
}
//==================================================================
std::string trackFitter::getLabel(void )
{
    if(subProcessOperation_ == &trackFitter::makeFittedTracksResiduals)  return "Computing unconstrained residuals";
    if(subProcessOperation_ == &trackFitter::makeFittedTrackDeviations)  return "Computing constrained residuals"  ;
    else                                                                 return  NO_PROCESS_LABEL_                 ;
}
//===============================================================================
void trackFitter::makeFittedTrackDeviations(Event * theEvent, Geometry*)
{
    //residualsMap_ = theEvent->getFittedTrackDeviations();
    residualsMap_.clear();
    Event::trackCandidatesDef &trackCandidates = theEvent->getTrackCandidates();
    Event::fittedTracksDef    &fittedTracks    = theEvent->getFittedTracks()   ;

    if ( trackCandidates.size() != fittedTracks.size() )
    {
        ss_.str("");
        ss_ << "track candidates size: " << trackCandidates.size()
            << " fitted tracks   size: " << fittedTracks.size()    ;
        STDLINE(ss_.str(),ACYellow)
    }

    for( unsigned int i=0; i < trackCandidates.size(); i++ )
    {
        for(Event::alignedHitsCandidateMapDef::iterator pit = trackCandidates[i].begin(); pit != trackCandidates[i].end()  ; pit++)
        {
            double resX = ( (*pit).second["x"] - fittedTracks[i][0]*(*pit).second["z"] - fittedTracks[i][1] ) ;
            double resY = ( (*pit).second["y"] - fittedTracks[i][2]*(*pit).second["z"] - fittedTracks[i][3] ) ;
            if(debug_)
            {
                ss_.str("");
                ss_ << "track: " << i << " detector: " << (*pit).first << " ResX: " << resX << " ResY: " << resY;
                STDLINE(ss_.str(),ACYellow);
            }
            residualsMap_[i][(*pit).first] = std::make_pair(resX, resY);
        }
    }
    //STDLINE("",ACYellow)
    theEvent->setFittedTrackDeviations(residualsMap_);
}
//=======================================================================================

