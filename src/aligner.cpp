/****************************************************************************
** Authors: Dario Menasce, Stefano Terzo
**
** I.N.F.N. Milan-Bicocca
** Piazza  della Scienza 3, Edificio U2
** Milano, 20126
**
****************************************************************************/

#include "aligner.h"

//=============================================================
aligner::aligner(fileEater* theFileEater, HManager* theHManager) :
    theFileEater_(theFileEater)
  , theHManager_ (theHManager)
{
}

//=============================================================
aligner::~aligner(void)
{
//    STDLINE("aligner::~aligner",ACRed);
}

//=============================================================
bool aligner::align(void)
{
    // Real&Fake Rotation Angles and Shifts
    std::map< std::string, Detector::matrix33Def > fRInv;

    std::map< std::string, double > deltaTx,deltaTy,deltaTz;
    std::map< std::string, double > alpha,beta,gamma;
    std::map< std::string, double > fTz;

    std::vector<double>  parametersCorrection(nAlignPars,0);//0=alpha, 1=beta, 2=gamma, 3=deltaTx, 4=deltaTy, 5=deltaTz

    std::map< std::string, double > rxprime,ryprime;

    std::map<int, std::map<std::string, double> > xmeas, ymeas;
    std::map<int, std::map<std::string, int> >    xsizemeas,ysizemeas;
    std::map<int, std::map<std::string, double> > sigx , sigy ;


    // Control Parameters
    //int phase = strategy_;	// phase=0 does a preliminary XY Trans adjustment and then Trans&Rots for max trial iterations both

    // phase=1 does Trans&Rots from scratch

    //Resetting plots!
    outputPlots_ = theHManager_->clearAlignmentResults();

    Geometry *theGeometry = theFileEater_->getGeometry();

    //Find first and last detectors in case you want to keep them fixed
    std::string firstDetector = theGeometry->begin()->first;
    std::string lastDetector  = theGeometry->begin()->first;
    for(Geometry::iterator git=theGeometry->begin(); git!=theGeometry->end(); git++)
    {
        if ( git->second->getZPosition() < theGeometry->getDetector(firstDetector)->getZPositionTotal() )
            firstDetector = git->first;
        if ( git->second->getZPosition() > theGeometry->getDetector(lastDetector) ->getZPositionTotal() )
            lastDetector  = git->first;
    }

    int goodTracks=0;
    for(int ev=0; ev<nEvents_; ++ev)
    {
        if(ev%1000 == 0)
        {
            ss_.str("");
            ss_ << "Reading event: " << ev ;
            STDLINE(ss_.str(),ACWhite);
        }

        Event *theEvent = theFileEater_->getEvent(ev);

        Event::clustersMapDef     &clusters     = theEvent->getClusters()        ;
        Event::clustersHitsMapDef &clustersHits = theEvent->getClustersHits()    ;
        Event::trackCandidatesDef &tracks       = theEvent->getTrackCandidates() ;
        Event::chi2VectorDef      &tracksChi2   = theEvent->getFittedTracksChi2();

        if(maxTracks_ > 0 && (int)tracks.size() > maxTracks_)
            continue;

        //Tracks selection
        for(unsigned int tr=0; tr < tracks.size(); tr++)
        {
            unsigned int numberOfTelescopeHits     = 0;
            unsigned int numberOfGoodTelescopeHits = 0;
            for(Geometry::iterator git=theGeometry->begin(); git!=theGeometry->end(); git++)
            {
                if(git->second->isDUT()                ) continue;
                if(tracks[tr].count( git->first ) != 0 ) numberOfTelescopeHits++  ;
                if(maxClusterSize_ > 0 && tracks[tr][(*git).first]["size"] <= maxClusterSize_ ) numberOfGoodTelescopeHits++;
            }
            if( numberOfTelescopeHits  < theGeometry->getDetectorsNumber(true) ) continue;//minumum 8 point per track hardcoded!

            if( maxClusterSize_ > 0 && numberOfGoodTelescopeHits < theGeometry->getDetectorsNumber(true) ) continue;

            if( chi2cut_ > 0 && tracksChi2[tr] > chi2cut_        ) continue;//chi2 cut

            if(noDiagonalClusters_)
            {
                bool badCluster = false;
                for(Event::alignedHitsCandidateMapDef::iterator det=tracks[tr].begin(); det!=tracks[tr].end(); ++det)
                {
                    std::list<unsigned int>  nRow;
                    std::list<unsigned int>  nCol;
                    for(Event::hitsDef::iterator hIt=clustersHits[det->first][(int)det->second["num"]].begin(); hIt!=clustersHits[det->first][(int)det->second["num"]].end();hIt++)
                    {
                        nRow.push_back((*hIt)["row"]);
                        nCol.push_back((*hIt)["col"]);
                    }
                    nRow.sort();
                    nCol.sort();
                    nRow.unique();
                    nCol.unique();
                    if(nRow.size() != 1 && nCol.size() != 1)
                    {
                        badCluster = true;
                        break;
                    }
                }
                if(badCluster) continue;
            }

            for(Event::alignedHitsCandidateMapDef::iterator det=tracks[tr].begin(); det!=tracks[tr].end(); ++det)
            {
                if( theGeometry->getDetector(det->first)->isDUT() ) continue;

                xmeas[goodTracks][det->first]    = clusters[det->first][(int)det->second["num"]]["x"];
                ymeas[goodTracks][det->first]    = clusters[det->first][(int)det->second["num"]]["y"];
                if(det->second["size"] == 1)
                {
                    xsizemeas[goodTracks][det->first] = 1;
                    ysizemeas[goodTracks][det->first] = 1;
                }
                else
                {
                    std::list<unsigned int>  nRow;
                    std::list<unsigned int>  nCol;
                    for(Event::hitsDef::iterator hIt=clustersHits[det->first][(int)det->second["num"]].begin(); hIt!=clustersHits[det->first][(int)det->second["num"]].end();hIt++)
                    {
                        nRow.push_back((*hIt)["row"]);
                        nCol.push_back((*hIt)["col"]);
                    }
                    nRow.sort();
                    nCol.sort();
                    nRow.unique();
                    nCol.unique();
                    if(theGeometry->getDetector(det->first)->switchXYFromLocaToGlobal())
                    {
                        xsizemeas[goodTracks][det->first] = nRow.size();
                        ysizemeas[goodTracks][det->first] = nCol.size();
                    }
                    else
                    {
                        xsizemeas[goodTracks][det->first] = nCol.size();
                        ysizemeas[goodTracks][det->first] = nRow.size();
                    }
                }
                sigx[goodTracks][det->first] = clusters[det->first][(int)det->second["num"]]["xErr"];
                sigy[goodTracks][det->first] = clusters[det->first][(int)det->second["num"]]["yErr"];
                theGeometry->getDetector(det->first)->fromLocalToGlobalNoRotation(&xmeas[goodTracks][det->first],&ymeas[goodTracks][det->first],
                                                                                  &sigx [goodTracks][det->first],&sigy [goodTracks][det->first]);

                if(sigx[goodTracks][det->first]<=0 || sigy[goodTracks][det->first]<=0)
                {
                    ss_.str("");
                    ss_ << "WARNING: hit associated error is <=0 for detector: " << det->first << " ; event: " << ev;
                    STDLINE(ss_.str(), ACPurple);
                }

            }
            goodTracks++;
        }
        currentIteration_=ev;
    }

    if(goodTracks == 0)
    {
        ss_.str("");
        ss_ << "WARNING: There are no tracks satisfying your cuts...returning";
        STDLINE(ss_.str(),ACRed) ;
        return false;
    }

    ss_.str("");
    ss_ << "Total tracks found: " << goodTracks;
    STDLINE(ss_.str(), ACYellow);

    for( std::map<std::string, double>::iterator det=xmeas[0].begin(); det!=xmeas[0].end(); det++ )
    {
        Detector *detector = theGeometry->getDetector(det->first);
        if( detector->isDUT() ) continue;//Should be impossible

        // Reset Translations
        alpha  [det->first]=0;
        beta   [det->first]=0;
        gamma  [det->first]=0;
        deltaTx[det->first]=0;
        deltaTy[det->first]=0;
        deltaTz[det->first]=0;

        fTz[det->first] = detector->getZPositionTotal();

        //fake Inverse Rotation
        fRInv[det->first] = detector->getRotationMatrix();

        ss_.str(""); ss_ << "Initial Geometry for detector: " << detector->getID(); STDLINE(ss_.str(),ACRed);
        detector->dump();
    }


    double sumchi2 = 0;
    for(int phase=strategy_; phase<2; phase++)
    {
        for(int trial=0; (phase==0 && trial<maxtrial_) || (phase==1 && trial<=maxtrial_); trial++)
        {
            sumchi2 = 0;
            ss_.str("");
            if(phase == 1 && trial == maxtrial_)
                ss_ << "Making plots!" ;
            else
                ss_ << "Trial number " << trial+1 << "/" << maxtrial_ << " for phase " << phase;
            STDLINE(ss_.str(), ACGreen);

            // Align Matrices
            std::map< std::string, ROOT::Math::SMatrix<double,2,2> >                   AtVAxy    ;
            std::map< std::string, ROOT::Math::SVector<double,2>   >                   AtVInvRxy ;
            std::map< std::string, ROOT::Math::SMatrix<double,nAlignPars,nAlignPars> > AtVAAll   ;
            std::map< std::string, ROOT::Math::SVector<double,nAlignPars>   >          AtVInvRAll;

            // Loop on random tracks
            for(unsigned int j=0; j<xmeas.size(); ++j)
            {
                // Loop to exclude plane ii from the track fit
                for( std::map<std::string, double>::iterator exl=xmeas[j].begin(); exl!=xmeas[j].end(); exl++ )
                {
                    // Fit Matrices
                    ROOT::Math::SMatrix<double,4,4> AtVA   ;
                    ROOT::Math::SMatrix<double,2,2> sigmaXY  ;
                    ROOT::Math::SMatrix<double,2,2> sigmaXYInv;
                    ROOT::Math::SVector<double,4>   AtVxy  ;
                    ROOT::Math::SMatrix<double,4,4> AtVAInv;
                    ROOT::Math::SVector<double,4>   fitpar ;

                    // Loop on all planes but ii for each random track
                    for( std::map<std::string, double>::iterator det=xmeas[j].begin(); det!=xmeas[j].end(); det++ )
                    {
                        rxprime[det->first] = xmeas[j][det->first] - deltaTx[det->first];
                        ryprime[det->first] = ymeas[j][det->first] - deltaTy[det->first];

                        if( det->first == exl->first ) continue;

                        double fxglo,fyglo,fzglo;
                        fxglo = rxprime[det->first]*fRInv[det->first][0][0]+ryprime[det->first]*fRInv[det->first][0][1];
                        fyglo = rxprime[det->first]*fRInv[det->first][1][0]+ryprime[det->first]*fRInv[det->first][1][1];
                        fzglo = fTz[det->first]+rxprime[det->first]*fRInv[det->first][2][0]+ryprime[det->first]*fRInv[det->first][2][1];

                        // Errors
                        sigmaXY(0,0) = pow(fRInv[det->first][0][0]*sigx[j][det->first],2)+pow(fRInv[det->first][0][1]*sigy[j][det->first],2);
                        sigmaXY(0,1) = fRInv[det->first][0][0]*fRInv[det->first][1][0]*pow(sigx[j][det->first],2)+fRInv[det->first][0][1]*fRInv[det->first][1][1]*pow(sigy[j][det->first],2);
                        sigmaXY(1,0) = sigmaXY(0,1);
                        sigmaXY(1,1) = pow(fRInv[det->first][1][0]*sigx[j][det->first],2)+pow(fRInv[det->first][1][1]*sigy[j][det->first],2);

                        int ifail;
                        sigmaXYInv = sigmaXY.Inverse(ifail);

                        // AtVA
                        for(int r=0; r<4; r++)
                            for(int c=0; c<4; c++)
                                AtVA(r,c) += sigmaXYInv(r/2,c/2)*pow(fzglo,(2-(r%2+c%2)));

                        // AtVxy
                        for(int c=0; c<4; c++)
                            AtVxy[c] += pow(fzglo,1-c%2)*(fxglo*sigmaXYInv(0,c/2)+fyglo*sigmaXYInv(c/2,1));

                    }// End Loop on all planes but ii for each random track

                    // Fit Results
                    int ifail;
                    AtVAInv = AtVA.Inverse(ifail) ;
                    fitpar  = AtVAInv*AtVxy;

                    // !!!!!!!!!!ALIGNMENT !!!!!!!!!
                    // Prepare plane ii alignment matrices for each random track
                    double predX,predY;
                    double den = theGeometry->getDetector(exl->first)->getAlignmentPredictedGlobal(fitpar,fRInv[exl->first],fTz[exl->first],predX,predY);

                    double resxprime = rxprime[exl->first]-predX;
                    double resyprime = ryprime[exl->first]-predY;

                    if(phase == 0)//Only XY translations
                    {
                        AtVAxy[exl->first][0][0] += 1/pow(sigx[j][exl->first],2);
                        AtVAxy[exl->first][1][1] += 1/pow(sigy[j][exl->first],2);

                        AtVInvRxy[exl->first][0] += resxprime/pow(sigx[j][exl->first],2);
                        AtVInvRxy[exl->first][1] += resyprime/pow(sigy[j][exl->first],2);
                    }
                    else if(phase == 1)//User defined
                    {
                        if(trial == maxtrial_)
                        {
                            Detector::xyPair predSigmas = theGeometry->getDetector(exl->first)->propagateTrackErrors(fitpar,AtVAInv,fRInv[exl->first],fTz[exl->first]);

                            predSigmas.first  += sigx[j][exl->first]*sigx[j][exl->first];
                            predSigmas.second += sigy[j][exl->first]*sigy[j][exl->first];

                            sumchi2   += resxprime*resxprime/predSigmas.first+resyprime*resyprime/predSigmas.second;


                            theHManager_->fillAlignmentResults(exl->first,
                                                               xsizemeas[j][exl->first], ysizemeas[j][exl->first],
                                                               resxprime , predSigmas.first,
                                                               resyprime , predSigmas.second,
                                                               rxprime[exl->first], ryprime[exl->first]);
                        }
                        else
                            makeAlignMatrices(AtVAAll[exl->first],AtVInvRAll[exl->first],fitpar,fRInv[exl->first],fTz[exl->first],predX,predY,den,sigx[j][exl->first],sigy[j][exl->first],resxprime,resyprime);

                    }
                }   // End Loop to exclude plane from the track fit
            }  // End Loop on random Tracks
            if(phase == 1 && trial == maxtrial_) break;

            // !!!!!!!!!!ALIGNMENT !!!!!!!!!
            // Fit for Alignment xy-Translations and update xy-Trans
            for( std::map<std::string, double>::iterator det=xmeas[0].begin(); det!=xmeas[0].end(); det++ )
            {
                if(phase == 0)
                {
                    int ifail = 0 ;
                    ROOT::Math::SMatrix<double,2,2> xyaTvainv = AtVAxy[det->first].Inverse(ifail) ;
                    ROOT::Math::SVector<double,2>   xyalpar   = xyaTvainv*AtVInvRxy[det->first];

                    //                    if(det->first != "Station: 0 - Plaq: 0" && det->first != "Station: 2 - Plaq: 3")
                    if(det->first != firstDetector && det->first != lastDetector)
                    {
                        // update Trans Offsets
                        deltaTx[det->first] += xyalpar[0];
                        deltaTy[det->first] += xyalpar[1];
                    }
                    char buffer[128];
                    ss_.str(""); ss_ << "Detector: " << det->first; STDLINE(ss_.str(),ACRed);
                    sprintf (buffer, "X     -> Total correction = %7.3f um  Trial correction =  %7.3f um", 10*deltaTx[det->first], 10*xyalpar[0]);STDLINE(buffer,ACCyan);
                    sprintf (buffer, "Y     -> Total correction = %7.3f um  Trial correction =  %7.3f um", 10*deltaTy[det->first], 10*xyalpar[1]);STDLINE(buffer,ACCyan);
                }

                else if(phase == 1)
                {

                    calculateCorrections(det->first, parametersCorrection, AtVAAll[det->first], AtVInvRAll[det->first],fRInv[det->first]);

                    alpha[det->first]   += parametersCorrection[0];
                    beta [det->first]   += parametersCorrection[1];
                    gamma[det->first]   += parametersCorrection[2];
                    deltaTx[det->first] += parametersCorrection[3];
                    deltaTy[det->first] += parametersCorrection[4];
                    deltaTz[det->first] += parametersCorrection[5];

                    fTz[det->first]     += parametersCorrection[5];

                    ss_.str(""); ss_ << "Detector: " << det->first; STDLINE(ss_.str(),ACRed);
                    char buffer[128];
                    sprintf (buffer, "Alpha -> Total correction = %7.3f deg Trial correction =  %7.3f deg", alpha[det->first]*180./M_PI, parametersCorrection[0]*180./M_PI);STDLINE(buffer,ACCyan);
                    sprintf (buffer, "Beta  -> Total correction = %7.3f deg Trial correction =  %7.3f deg", beta [det->first]*180./M_PI, parametersCorrection[1]*180./M_PI);STDLINE(buffer,ACCyan);
                    sprintf (buffer, "Gamma -> Total correction = %7.3f deg Trial correction =  %7.3f deg", gamma[det->first]*180./M_PI, parametersCorrection[2]*180./M_PI);STDLINE(buffer,ACCyan);
                    sprintf (buffer, "X     -> Total correction = %7.3f um  Trial correction =  %7.3f um" , 10*deltaTx[det->first], 10*parametersCorrection[3]);STDLINE(buffer,ACCyan);
                    sprintf (buffer, "Y     -> Total correction = %7.3f um  Trial correction =  %7.3f um" , 10*deltaTy[det->first], 10*parametersCorrection[4]);STDLINE(buffer,ACCyan);
                    sprintf (buffer, "Z     -> Total correction = %7.3f um  Trial correction =  %7.3f um" , 10*deltaTz[det->first], 10*parametersCorrection[5]);STDLINE(buffer,ACCyan);

                    if(trial == maxtrial_-1)
                    {
                        double dalpha = asin(fRInv[det->first][2][1]/sqrt(1-fRInv[det->first][2][0]*fRInv[det->first][2][0]))*180/M_PI
                                -theGeometry->getDetector(det->first)->getXRotation()
                                -theGeometry->getDetector(det->first)->getXRotationCorrection();
                        double dbeta  = asin(-fRInv[det->first][2][0])*180/M_PI
                                -theGeometry->getDetector(det->first)->getYRotation()
                                -theGeometry->getDetector(det->first)->getYRotationCorrection()  ;
                        double dgamma = asin(fRInv[det->first][1][0]/sqrt(1-fRInv[det->first][2][0]*fRInv[det->first][2][0]))*180/M_PI
                                -theGeometry->getDetector(det->first)->getZRotationCorrection();

                        alignmentResults_[det->first].alpha   =  dalpha             ;
                        alignmentResults_[det->first].beta    =  dbeta              ;
                        alignmentResults_[det->first].gamma   =  dgamma             ;
                        alignmentResults_[det->first].deltaTx =  deltaTx[det->first];
                        alignmentResults_[det->first].deltaTy =  deltaTy[det->first];
                        alignmentResults_[det->first].deltaTz =  deltaTz[det->first];
                        //char buffer[128];
                        //sprintf (buffer, "Alpha (deg) -> Total correction = %7.3f", dalpha);STDLINE(buffer,ACCyan);
                        //sprintf (buffer, "Beta  (deg) -> Total correction = %7.3f", dbeta );STDLINE(buffer,ACCyan);
                        //sprintf (buffer, "Gamma (deg) -> Total correction = %7.3f", dgamma);STDLINE(buffer,ACCyan);
                    }
                }
            }
            currentIteration_+= nEvents_;
        }
    }

    //ss_.str("");
    //ss_ << "Sumchi2: " << sumchi2;
    //STDLINE(ss_.str(),ACGreen);
    //ss_.str("");
    //ss_ << "Mean Chi2 per Measure: " << sumchi2/(theGeometry->getDetectorsNumber(true)*xmeas.size()*2);
    //STDLINE(ss_.str(),ACGreen);

    return  true;
}
//=============================================================================================================
bool aligner::alignDUT()
{
    Geometry * theGeometry = theFileEater_->getGeometry();
    Detector * dut         = theGeometry->getDetector(DUT_);

    double  alpha   = 0;
    double  beta    = 0;
    double  gamma   = 0;
    double  deltaTx = 0;
    double  deltaTy = 0;
    double  deltaTz = 0;
    double  fTz     = dut->getZPositionTotal();

    std::vector<double>  parametersCorrection(nAlignPars,0);//0=alpha, 1=beta, 2=gamma, 3=deltaTx, 4=deltaTy, 5=deltaTz

    std::vector<double>              rxprime, ryprime   ;
    std::vector<double>              sigx , sigy        ;
    std::vector<unsigned int>        xClusterSize,yClusterSize;
    unsigned int                     nPoints = 0        ;
    Event::fittedTracksDef           fitpar             ;
    Event::fittedTracksCovarianceDef covMat             ;

    //fake Inverse Rotation
    Detector::matrix33Def fRInv = dut->getRotationMatrix();

    //Resetting plots!
    outputPlots_           = theHManager_->clearAlignmentResults(DUT_);

    ss_.str(""); ss_ << "Initial Geometry for detector Id " <<  DUT_ ; STDLINE(ss_.str(),ACWhite);
    dut->dump();

    for(int ev=0; ev<nEvents_; ++ev)
    {
        if(ev%1000 == 0)
        {
            ss_.str("") ;
            ss_ << "Reading event: " << ev ;
            STDLINE(ss_.str(),ACWhite);
        }

        Event *theEvent = theFileEater_->getEvent(ev);

        Event::clustersMapDef            & clusters      = theEvent->getClusters()              ;
        Event::clustersHitsMapDef        & clustersHits  = theEvent->getClustersHits()          ;
        Event::trackCandidatesDef        & tracks        = theEvent->getTrackCandidates()       ;
        Event::fittedTracksDef           & fittedTracks  = theEvent->getFittedTracks()          ;
        Event::fittedTracksCovarianceDef & covariance    = theEvent->getFittedTracksCovariance();
        Event::chi2VectorDef             & tracksChi2    = theEvent->getFittedTracksChi2()      ;

        if(maxTracks_ > 0 && (int)tracks.size() > maxTracks_)
            continue;

        for(unsigned int tr=0; tr < tracks.size(); tr++)
        {
            if( tracks[tr].count(DUT_) == 0 ) continue;
            int pass = 0;
            unsigned int purePass = 0;

            for(Geometry::iterator git=theGeometry->begin(); git!=theGeometry->end(); git++)
            {
                //Continue if this is not the DUT we are interested in!
                if( git->second->isDUT() && (*git).first != DUT_) continue;

                if( tracks[tr].count( git->first ) != 0 )
                    pass++  ;
                if( maxClusterSize_ > 0 && tracks[tr][(*git).first]["size"] <= maxClusterSize_ )
                    purePass++;
            }
            //minimum points per track (8 telscope + 1 of the DUT that has to be aligned)
            if( minTrackPoints_ > 1 && pass < minTrackPoints_ + 1 ) continue;

            //chi2 cut if present
            if( chi2cut_        > 0 && tracksChi2[tr]    > chi2cut_ ) continue;

            //There must 8+1 pure pass points where pure means that the cluster size is at most 2
            if( maxClusterSize_ > 0 && purePass < theGeometry->getDetectorsNumber(true) + 1 ) continue;

            std::list<unsigned int>  nRow;
            std::list<unsigned int>  nCol;
//            bool inWindow = false;
            for(Event::hitsDef::iterator hIt=clustersHits[DUT_][(int)tracks[tr][DUT_]["num"]].begin(); hIt!=clustersHits[DUT_][(int)tracks[tr][DUT_]["num"]].end();hIt++)
            {
                nRow.push_back((*hIt)["row"]);
                nCol.push_back((*hIt)["col"]);
//                if(!inWindow && (*hIt)["row"]>=42 && (*hIt)["row"]<=77 && (*hIt)["col"]>=19 && (*hIt)["col"]<=33)
//                    inWindow = true;
            }
//            if(!inWindow) continue;
            nRow.sort();
            nCol.sort();
            nRow.unique();
            nCol.unique();
            if(noDiagonalClusters_ && nRow.size() != 1 && nCol.size() != 1) continue;

            if(theGeometry->getDetector(DUT_)->switchXYFromLocaToGlobal())
            {
                xClusterSize.push_back(nRow.size());
                yClusterSize.push_back(nCol.size());
            }
            else
            {
                xClusterSize.push_back(nCol.size());
                yClusterSize.push_back(nRow.size());
            }

            rxprime.push_back( clusters[DUT_][(int)tracks[tr][DUT_]["num"]]["x"] );
            ryprime.push_back( clusters[DUT_][(int)tracks[tr][DUT_]["num"]]["y"] );

            sigx.push_back( clusters[ DUT_ ][ (int)tracks[tr][DUT_]["num"] ]["xErr"] );
            sigy.push_back( clusters[ DUT_ ][ (int)tracks[tr][DUT_]["num"] ]["yErr"] );

            dut->fromLocalToGlobalNoRotation(&rxprime[nPoints],&ryprime[nPoints],&sigx[nPoints],&sigy[nPoints]);
            ++nPoints;

            if(sigx.back()<=0 || sigy.back()<=0)
            {
                ss_.str("");
                ss_ << "WARNING: hit associated error is <=0 for detector: " << DUT_ << " ; event: " << ev;
                STDLINE(ss_.str(), ACPurple);
            }

            fitpar.push_back( fittedTracks[tr] );
            covMat.push_back( covariance[tr]   );
        }
        currentIteration_=ev;
    }

    if(rxprime.size() == 0)
    {
        ss_.str("");
        ss_ << "WARNING: There are no tracks satisfying your cuts...returning";
        STDLINE(ss_.str(),ACRed) ;
        return false;
    }

    ss_.str("");
    ss_ << "Total tracks found: " << rxprime.size();
    STDLINE(ss_.str(), ACYellow);

    for(int ntrial=0; ntrial < maxtrial_; ntrial++)
    {
        ss_.str("");
        ss_ << "Trial number " << ntrial+1 << "/" << maxtrial_;
        STDLINE(ss_.str(), ACGreen);

        ++currentIteration_;

        ROOT::Math::SMatrix<double,nAlignPars,nAlignPars> AtVAAll   ;
        ROOT::Math::SVector<double,nAlignPars>            AtVInvRAll;

        // Loop on random tracks
        for(unsigned int j=0; j < rxprime.size(); ++j)
        {
            double predX,predY;
            double den = dut->getAlignmentPredictedGlobal(fitpar[j],fRInv,fTz,predX,predY);

            double resxprime = rxprime[j] - deltaTx - predX;
            double resyprime = ryprime[j] - deltaTy - predY;

            makeAlignMatrices(AtVAAll,AtVInvRAll,fitpar[j],fRInv,fTz,predX,predY,den,sigx[j],sigy[j],resxprime,resyprime);

            if(ntrial == maxtrial_-1)
            {
                Detector::xyPair predSigmas = dut->propagateTrackErrors(fitpar[j],covMat[j],fRInv,fTz);

                predSigmas.first  += sigx[j]*sigx[j];
                predSigmas.second += sigy[j]*sigy[j];

                theHManager_->fillAlignmentResults(DUT_           ,
                                                   xClusterSize[j], yClusterSize[j],
                                                   resxprime      , predSigmas.first,
                                                   resyprime      , predSigmas.second,
                                                   rxprime[j]     , ryprime[j]      );
            }
        }

        calculateCorrections(DUT_, parametersCorrection, AtVAAll, AtVInvRAll,fRInv);

        alpha   += parametersCorrection[0];
        beta    += parametersCorrection[1];
        gamma   += parametersCorrection[2];
        deltaTx += parametersCorrection[3];
        deltaTy += parametersCorrection[4];
        deltaTz += parametersCorrection[5];

        fTz     += parametersCorrection[5];

        ss_.str(""); ss_ << "Detector Id " << DUT_; STDLINE(ss_.str(),ACRed);
        char buffer[128];
        sprintf (buffer, "Alpha -> Total correction = %7.3f deg Trial correction =  %7.3f deg", alpha*180./M_PI, parametersCorrection[0]*180./M_PI);STDLINE(buffer,ACCyan);
        sprintf (buffer, "Beta  -> Total correction = %7.3f deg Trial correction =  %7.3f deg", beta *180./M_PI, parametersCorrection[1]*180./M_PI);STDLINE(buffer,ACCyan);
        sprintf (buffer, "Gamma -> Total correction = %7.3f deg Trial correction =  %7.3f deg", gamma*180./M_PI, parametersCorrection[2]*180./M_PI);STDLINE(buffer,ACCyan);
        sprintf (buffer, "X     -> Total correction = %7.3f um  Trial correction =  %7.3f um" , 10*deltaTx, 10*parametersCorrection[3]);STDLINE(buffer,ACCyan);
        sprintf (buffer, "Y     -> Total correction = %7.3f um  Trial correction =  %7.3f um" , 10*deltaTy, 10*parametersCorrection[4]);STDLINE(buffer,ACCyan);
        sprintf (buffer, "Z     -> Total correction = %7.3f um  Trial correction =  %7.3f um" , 10*deltaTz, 10*parametersCorrection[5]);STDLINE(buffer,ACCyan);
    }

    alignmentResults_[DUT_].alpha   =  asin(fRInv(2,1)/sqrt(1-fRInv(2,0)*fRInv(2,0)))*180/M_PI - dut->getXRotation() - dut->getXRotationCorrection();
    alignmentResults_[DUT_].beta    =  asin(-fRInv(2,0))*180/M_PI                              - dut->getYRotation() - dut->getYRotationCorrection();
    alignmentResults_[DUT_].gamma   =  asin(fRInv(1,0)/sqrt(1-fRInv(2,0)*fRInv(2,0)))*180/M_PI - dut->getZRotationCorrection();
    alignmentResults_[DUT_].deltaTx =  deltaTx          ;
    alignmentResults_[DUT_].deltaTy =  deltaTy          ;
    alignmentResults_[DUT_].deltaTz =  deltaTz          ;

    return true;
}

//===================================================================
void aligner::makeAlignMatrices   (ROOT::Math::SMatrix<double,nAlignPars,nAlignPars>& AtVA,
                                   ROOT::Math::SVector<double,nAlignPars>&            AtVAInvR,
                                   ROOT::Math::SVector<double,4>&                     trackPars,
                                   Detector::matrix33Def&                             fRInv,
                                   double z,
                                   double predX,
                                   double predY,
                                   double den,
                                   double sigmaX,
                                   double sigmaY,
                                   double residualX,
                                   double residualY
                                   )
{
    ROOT::Math::SMatrix<double,2,nAlignPars> C;
    C(0,0) = 1/den*(((fRInv[1][2]-trackPars[2]*fRInv[2][2])*(trackPars[0]*z+trackPars[1])-(fRInv[0][2]-trackPars[0]*fRInv[2][2])*(trackPars[2]*z+trackPars[3]))
                    -predX*((fRInv[1][2]-trackPars[2]*fRInv[2][2])*(fRInv[0][0]-trackPars[0]*fRInv[2][0])-(fRInv[0][2]-trackPars[0]*fRInv[2][2])*(fRInv[1][0]-trackPars[2]*fRInv[2][0])));
    C(0,1) = 1/den*predX*((fRInv[0][2]-trackPars[0]*fRInv[2][2])*(fRInv[1][1]-trackPars[2]*fRInv[2][1])-(fRInv[1][2]-trackPars[2]*fRInv[2][2])*(fRInv[0][1]-trackPars[0]*fRInv[2][1]));
    C(0,2) = 1/den*((fRInv[0][0]-trackPars[0]*fRInv[2][0])*(trackPars[2]*z+trackPars[3])-(fRInv[1][0]-trackPars[2]*fRInv[2][0])*(trackPars[0]*z+trackPars[1]));
    C(0,3) = 1;
    C(0,4) = 0;
    C(0,5) = 1/den*((fRInv[0][2]-trackPars[0]*fRInv[2][2])*(fRInv[1][1]-trackPars[2]*fRInv[2][1])-(fRInv[1][2]-trackPars[2]*fRInv[2][2])*(fRInv[0][1]-trackPars[0]*fRInv[2][1]));
    C(1,0) =-1/den*predY*((fRInv[1][2]-trackPars[2]*fRInv[2][2])*(fRInv[0][0]-trackPars[0]*fRInv[2][0])-(fRInv[0][2]-trackPars[0]*fRInv[2][2])*(fRInv[1][0]-trackPars[2]*fRInv[2][0]));
    C(1,1) = 1/den*(((fRInv[1][2]-trackPars[2]*fRInv[2][2])*(trackPars[0]*z+trackPars[1])-(fRInv[0][2]-trackPars[0]*fRInv[2][2])*(trackPars[2]*z+trackPars[3]))
                    +predY*((fRInv[0][2]-trackPars[0]*fRInv[2][2])*(fRInv[1][1]-trackPars[2]*fRInv[2][1])-(fRInv[1][2]-trackPars[2]*fRInv[2][2])*(fRInv[0][1]-trackPars[0]*fRInv[2][1])));
    C(1,2) = 1/den*((fRInv[0][1]-trackPars[0]*fRInv[2][1])*(trackPars[2]*z+trackPars[3])-(fRInv[1][1]-trackPars[2]*fRInv[2][1])*(trackPars[0]*z+trackPars[1]));
    C(1,3) = 0;
    C(1,4) = 1;
    C(1,5) = 1/den*((fRInv[1][2]-trackPars[2]*fRInv[2][2])*(fRInv[0][0]-trackPars[0]*fRInv[2][0])-(fRInv[0][2]-trackPars[0]*fRInv[2][2])*(fRInv[1][0]-trackPars[2]*fRInv[2][0]));

    for(int r=0; r<nAlignPars; r++)
        for(int c=0; c<nAlignPars; c++)
            if(c<r)//To gain time AtVA(r,c) = AtVA(c,r)
                AtVA[r][c] = AtVA[c][r];
            else
                AtVA[r][c] += C(0,r)*C(0,c)/pow(sigmaX,2)+C(1,r)*C(1,c)/pow(sigmaY,2);

    for(int c=0; c<nAlignPars; c++)
        AtVAInvR[c] += C(0,c)*residualX/pow(sigmaX,2)+C(1,c)*residualY/pow(sigmaY,2);
}

//===================================================================
bool aligner::calculateCorrections(std::string detectorName,
                                   std::vector<double>&                               deltaPars,
                                   ROOT::Math::SMatrix<double,nAlignPars,nAlignPars>& AtVAAll,
                                   ROOT::Math::SVector<double,nAlignPars>&            AtVInvRAll,
                                   Detector::matrix33Def&                             fRInv)
{
    unsigned int nAlignPars = deltaPars.size();
    deltaPars.assign(nAlignPars,0);

    if( parMap_[detectorName] == 111111 ) return true;//Lock its geometry. //No free parameters!

    bool zLocked = true;
    std::vector<bool>   parStatus(nAlignPars,false);
    int parVal = parMap_[detectorName];
    unsigned int freePars = 0;
    for(unsigned int p=0; p<nAlignPars; p++)
    {
        parStatus[p] = parVal%10;
        parVal /= 10;
        if(parStatus[p] == 0) ++freePars;
    }
    if(parStatus[nAlignPars-1] == 0) zLocked = false;

    TMatrixD aTva(freePars,freePars), aTvainv(freePars,freePars);
    TVectorD aTvinvr(freePars), alpar(freePars);

    int r = 0;
    int c = 0;
    for(unsigned int p=0; p<nAlignPars; p++)
    {
        if(parStatus[p] == 0)
        {
            aTvinvr[r] = AtVInvRAll[p];
            c=0;
            for(unsigned int pp=0; pp<nAlignPars; pp++)
            {
                if(parStatus[pp] == 0)
                {
                    aTva[r][c] = AtVAAll[p][pp];
                    ++c;
                }
            }
            ++r;
        }
    }

//    STDLINE("AtVAAll",ACPurple);
//    std::cout << AtVAAll << std::endl;
//    STDLINE("aTva",ACPurple);
//    aTva.Print();

    double determinant;
    aTvainv = aTva.Invert(&determinant) ;
    alpar   = aTvainv*aTvinvr;

//    STDLINE("aTvainv",ACPurple);
//    aTvainv.Print();
//    STDLINE("aTvinvr",ACPurple);
//    aTvinvr.Print();
//    STDLINE("alpar",ACPurple);
//    alpar.Print();

    int zeroPars = 0;
    for(unsigned int p=0; p<nAlignPars; p++)
    {
        if(p == 3 && (deltaPars[0] != 0 || deltaPars[1] != 0 || deltaPars[2] != 0))
        {
            // update Rot Offsets
//            STDLINE("fRInv",ACPurple);
//            std::cout << fRInv << std::endl;
            fRInv *= Detector::rotationMatrix(deltaPars[0],deltaPars[1],deltaPars[2]);
//            STDLINE("correction",ACPurple);
//            std::cout << Detector::rotationMatrix(deltaPars[0],deltaPars[1],deltaPars[2]) << std::endl;
//            STDLINE("fRInv",ACPurple);
//            std::cout << fRInv << std::endl;
        }
        if(parStatus[p] == 0)
        {
            if(p < 3 || zLocked)//Angles || All parameters when z is locked
                deltaPars[p] = alpar[p-zeroPars];
            else if(p == 3 ||p == 4)   //X Y Translations
                deltaPars[p] = alpar[p-zeroPars]-fRInv[2][p-3]/fRInv[2][2]*alpar[nAlignPars-zeroPars-1];
            else if(p == 5) //Z Translations
                deltaPars[5] = -alpar[nAlignPars-zeroPars-1]/fRInv[2][2];

        }
        else
        {
            deltaPars[p] = 0;
            ++zeroPars;
        }
    }
//    for(unsigned int p=0; p<nAlignPars; p++)
//    {
//        std::cout << deltaPars[p] << " ";
//    }
//    std::cout << std::endl;
    return true;

    /*
        if( parMap_[detectorName] == 111 ) // All angles locked
        {		                             // Free parameter are delta x delta y and delta z
            for(int j=3; j<6; ++j)
            {
                treaTvinvr[j-3] = AtVInvRAll[detectorName][j];
                for(int k=3; k<6; ++k)
                {
                    treaTva[j-3][k-3] = AtVAAll[detectorName][j][k];
                }
            }

            int ifail;
            treaTvainv = treaTva.Inverse(ifail) ;
            trealpar   = treaTvainv*treaTvinvr;

            // update Trans Offsets
            deltaTx[detectorName]+= trealpar[0]-fRInv[detectorName][2][0]/fRInv[detectorName][2][2]*trealpar[2];
            deltaTy[detectorName]+= trealpar[1]-fRInv[detectorName][2][1]/fRInv[detectorName][2][2]*trealpar[2];
            deltaTz[detectorName]-= trealpar[2]/fRInv[detectorName][2][2];

            fTz[detectorName]-=trealpar[2]/fRInv[detectorName][2][2];

            dx=+trealpar[0]-fRInv[detectorName][2][0]/fRInv[detectorName][2][2]*trealpar[2];
            dy=+trealpar[1]-fRInv[detectorName][2][1]/fRInv[detectorName][2][2]*trealpar[2];
            dz=-trealpar[2]/fRInv[detectorName][2][2];

            dalf=0;
            dbet=0;
            dgam=0;

        }

        else if( parMap_[detectorName] == 111010 )       //enforce delta beta=0 & deltaTx=0 & deltaTy=0 & deltaTz=0
        {                                              //free parameters are 0,2 only
            dueaTva(0,0)=AtVAAll[detectorName][0][0];
            dueaTva(0,1)=AtVAAll[detectorName][0][2];
            dueaTva(1,0)=AtVAAll[detectorName][2][0];
            dueaTva(1,1)=AtVAAll[detectorName][2][2];

            dueaTvinvr[0]=AtVInvRAll[detectorName][0];
            dueaTvinvr[1]=AtVInvRAll[detectorName][2];

            int ifail;
            dueaTvainv = dueaTva.Inverse(ifail) ;
            duealpar=dueaTvainv*dueaTvinvr;

            fRInv[detectorName] = fRInv[detectorName] * Detector::rotationMatrix(duealpar[0],0,duealpar[1]);

            dx=0;
            dy=0;
            dz=0;

            dalf=duealpar[0];
            dbet=0;
            dgam=duealpar[1];
        }

        else if( parMap_[detectorName] == 111100 )       //enforce delta gamma=0 & deltaTx=0 & deltaTy=0 & deltaTz=0
        {                                       //free parameters are 0,1 only
            dueaTva(0,0)=AtVAAll[detectorName][0][0];
            dueaTva(0,1)=AtVAAll[detectorName][0][1];
            dueaTva(1,0)=AtVAAll[detectorName][1][0];
            dueaTva(1,1)=AtVAAll[detectorName][1][1];

            dueaTvinvr[0]=AtVInvRAll[detectorName][0];
            dueaTvinvr[1]=AtVInvRAll[detectorName][1];

            int ifail;
            dueaTvainv=dueaTva.Inverse(ifail) ;
            duealpar=dueaTvainv*dueaTvinvr;

            // update Rot Offsets
            fRInv[detectorName] = fRInv[detectorName] * Detector::rotationMatrix(duealpar[0],duealpar[1],0);

            dx = 0;
            dy = 0;
            dz = 0;

            dalf = duealpar[0];
            dbet = duealpar[1];
            dgam = 0;
        }

        else if( parMap_[detectorName] == 111000 )       //enforce deltaTx=0 & deltaTy=0 & deltaTz=0
        {                                         //free parameters are 0,1,2
            for(int j=0; j<3; ++j)
            {
                treaTvinvr[j]=AtVInvRAll[detectorName][j];

                for(int k=0; k<3; ++k)
                {
                    treaTva(j,k)=AtVAAll[detectorName][j][k];
                }
            }
            int ifail;
            treaTvainv=treaTva.Inverse(ifail) ;
            trealpar  =treaTvainv*treaTvinvr;

            // update Rot Offsets
            fRInv[detectorName] = fRInv[detectorName] * Detector::rotationMatrix(trealpar[0],trealpar[1],trealpar[2]);

            dx=0;
            dy=0;
            dz=0;

            dalf=trealpar[0];
            dbet=trealpar[1];
            dgam=trealpar[2];
        }

        else if( parMap_[detectorName] == 100000 )            //enforce deltaTz=0 only
        {                                         //free parameters are 0,1,2,3,4
            for(int j=0; j<5; ++j)
            {
                cinaTvinvr[j] = AtVInvRAll[detectorName][j];

                for(int k=0; k<5; ++k)
                {
                    cinaTva(j,k)=AtVAAll[detectorName][j][k];
                }
            }
            int ifail;
            cinaTvainv= cinaTva.Inverse(ifail) ;
            cinalpar  = cinaTvainv*cinaTvinvr;

            // update Rot Offsets
            fRInv[detectorName] = fRInv[detectorName] * Detector::rotationMatrix(cinalpar[0],cinalpar[1],cinalpar[2]);

            // update Trans Offsets
            deltaTx[detectorName]+=cinalpar[3];
            deltaTy[detectorName]+=cinalpar[4];
            deltaTz[detectorName] = deltaTz[detectorName];

            dx = cinalpar[3];
            dy = cinalpar[4];
            dz = 0;

            dalf = cinalpar[0];
            dbet = cinalpar[1];
            dgam = cinalpar[2];
        }

        else if( parMap_[detectorName] == 1 )       //enforce delta alfa=0 only
        {                                         //free parameters are 1,2,3,4,5
            for(int j=1; j<6; ++j)
            {
                cinaTvinvr[j-1] = AtVInvRAll[detectorName][j];

                for(int k=1; k<6; ++k)
                {
                    cinaTva[j-1][k-1] = AtVAAll[detectorName][j][k];
                }
            }
            int ifail;
            cinaTvainv = cinaTva.Inverse(ifail) ;
            cinalpar   = cinaTvainv*cinaTvinvr  ;

            // update Rot Offsets
            fRInv[detectorName] = fRInv[detectorName] * Detector::rotationMatrix(0,cinalpar[0],cinalpar[1]);

            // update Trans Offsets
            deltaTx[detectorName]+= cinalpar[2]-fRInv[detectorName][2][0]/fRInv[detectorName][2][2]*cinalpar[4];
            deltaTy[detectorName]+= cinalpar[3]-fRInv[detectorName][2][1]/fRInv[detectorName][2][2]*cinalpar[4];
            deltaTz[detectorName]-= cinalpar[4]/fRInv[detectorName][2][2];
            fTz[detectorName]    -= cinalpar[4]/fRInv[detectorName][2][2];

            dx = cinalpar[2]-fRInv[detectorName][2][0]/fRInv[detectorName][2][2]*cinalpar[4];
            dy = cinalpar[3]-fRInv[detectorName][2][1]/fRInv[detectorName][2][2]*cinalpar[4];
            dz =-cinalpar[4]/fRInv[detectorName][2][2];

            dalf = 0;
            dbet = cinalpar[0];
            dgam = cinalpar[1];
        }

        else if( parMap_[detectorName] == 0 )      //all free parameters
        {
            int ifail;
            ROOT::Math::SMatrix<double,6,6> allaTvainv = AtVAAll[detectorName].Inverse(ifail) ;
            ROOT::Math::SVector<double,6>   allalpar   = allaTvainv*AtVInvRAll[detectorName]  ;

            // update Rot Offsets
            fRInv[detectorName] = fRInv[detectorName] * Detector::rotationMatrix(allalpar[0],allalpar[1],allalpar[2]);

            // update Trans Offsets
            deltaTx[detectorName]+= allalpar[3]-fRInv[detectorName][2][0]/fRInv[detectorName][2][2]*allalpar[5];
            deltaTy[detectorName]+= allalpar[4]-fRInv[detectorName][2][1]/fRInv[detectorName][2][2]*allalpar[5];
            deltaTz[detectorName]-= allalpar[5]/fRInv[detectorName][2][2];
            fTz[detectorName]    -= allalpar[5]/fRInv[detectorName][2][2];

            dalf=allalpar[0];
            dbet=allalpar[1];
            dgam=allalpar[2];
            dx=+allalpar[3]-fRInv[detectorName][2][0]/fRInv[detectorName][2][2]*allalpar[5];
            dy=+allalpar[4]-fRInv[detectorName][2][1]/fRInv[detectorName][2][2]*allalpar[5];
            dz=-allalpar[5]/fRInv[detectorName][2][2];

        }
        STDLINE(dalf,ACGreen);
        STDLINE(dbet,ACGreen);
        STDLINE(dgam,ACGreen);
        STDLINE(dx,ACGreen);
        STDLINE(dy,ACGreen);
        STDLINE(dz,ACGreen);
*/
}

//===================================================================
bool aligner::execute()
{
    if(processOperation_) return (this->*processOperation_)();
    else                  return false   ;
}

//================================================================
void aligner::setAlignmentPreferences(int    maxtrial,
                                      int    strategy,
                                      double chi2cut,
                                      int    maxClusterSize,
                                      int    minTrackPoints,
                                      int    maxTracks,
                                      bool   noDiagonalClusters,
                                      std::string DUT,
                                      int    nEvents)
{
    maxtrial_           = maxtrial          ;
    strategy_           = strategy          ;
    chi2cut_            = chi2cut           ;
    maxClusterSize_     = maxClusterSize    ;
    minTrackPoints_     = minTrackPoints    ;
    maxTracks_          = maxTracks         ;
    noDiagonalClusters_ = noDiagonalClusters;
    DUT_                = DUT               ;
    if(nEvents == -1)
        nEvents_ = theFileEater_->getEventsNumber();
    else
        nEvents_ = nEvents;
}

//=======================================================================
int aligner::getMaxIterations (void)
{
    if(processOperation_ == &aligner::align   )
    {
        if(strategy_==0) return nEvents_+maxtrial_*2*nEvents_;
        if(strategy_==1) return nEvents_+maxtrial_*nEvents_  ;
        else return 0;
    }
    if(processOperation_ == &aligner::alignDUT)
    {
        return nEvents_+maxtrial_;
    }
    else return 0;
}

//=======================================================================
std::string  aligner::getLabel(void  )
{
    if(processOperation_ == &aligner::alignDUT) return "DUT alignment";
    if(processOperation_ == &aligner::align   ) return "Telescope alignment";
    else                                        return NO_PROCESS_LABEL_;
}

//=======================================================================
