/****************************************************************************
** Authors: Dario Menasce, Stefano Terzo
**
** I.N.F.N. Milan-Bicocca
** Piazza  della Scienza 3, Edificio U2
** Milano, 20126
**
****************************************************************************/
#ifndef __EventConverter_H__
#define __EventConverter_H__

#include "EventReader.h"
#include <sstream>

//====================================================================
class EventConverter
{
 public:
        EventConverter(EventReader  * reader) ;
       ~EventConverter(void		    ){;}
  
   void convert       (void) ;

 private: 
   EventReader * theEventReader_;
   Event       * theEvent_      ;
   EventHeader * theHeader_     ;
   Geometry    * theGeometry_   ;
   TH2F*         calibHistos2D_[4];
   std::stringstream ss_ ;
} ;

#endif
