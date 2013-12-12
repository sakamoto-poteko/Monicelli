/****************************************************************************
** Authors: Dario Menasce, Stefano Terzo
**
** I.N.F.N. Milan-Bicocca
** Piazza  della Scienza 3, Edificio U2
** Milano, 20126
**
****************************************************************************/
#ifndef __EventAnalyzer_H__
#define __EventAnalyzer_H__

#include "EventReader.h"

//====================================================================
class EventAnalyzer
{
 public:
        EventAnalyzer(EventReader  * reader) ;
       ~EventAnalyzer(void                 ){;}
  
   void analyzeEvent (unsigned int   event ) ;

 private: 
   Event       * theEvent_    ;
   EventHeader * theHeader_   ;
   Geometry    * theGeometry_ ;
   
   std::stringstream ss_ ;
} ;

#endif
