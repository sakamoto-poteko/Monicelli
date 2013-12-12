/****************************************************************************
** Authors: Dario Menasce, Stefano Terzo
**
** I.N.F.N. Milan-Bicocca
** Piazza  della Scienza 3, Edificio U2
** Milano, 20126
**
****************************************************************************/

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <list>
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <TObject.h>

#include "Detector.h"

#define CONVF 1000

class Geometry : public TObject
{
 public:

  Geometry(void);
 ~Geometry(void);

  Detector                  * addDetector           (std::string  plaqID,
                                                     bool         isDUT = false     );

  void                        clear                 (void                           ) {detectorsMap_.clear()       ;}
  bool                        empty                 (void                           ) {return detectorsMap_.empty();}

  Detector                  * getDetector           (std::string  plaqID            );
  Detector                  * getDetector           (int          station,
                                                     int          plaq              );
  std::vector<Detector*>      getDUTs               (void                           );
  std::string                 getDetectorID         (int          Station, int Plaq );
  unsigned int                getDetectorsNumber    (bool         excludeDUT = false);
  std::string                 getGeometryFileName   (void                           ) {return geometryFileName_    ;}
  double                      getMaxDetectorsLength (void                           );
  unsigned int                getMaxRowsNum         (void                           );
  unsigned int                getMaxColsNum         (void                           );
  bool                        calibrationDone       (void                           ){return calibrationDone_      ;}

  void                        setDUTnumbers         (unsigned int dutNumbers        ) {dutNumbers_ = dutNumbers    ;}
  void                        setGeometryFileName   (std::string  fileName          ) ;
  void                        setCalibrationDone    (bool done                      ) {calibrationDone_ = done     ;}

  void                        dump                  (void                           );

  typedef std::map< std::string , Detector* >::iterator   iterator;
  iterator                                                begin() {return detectorsMap_.begin() ;}
  iterator                                                end()   {return detectorsMap_.end()   ;}

 private:

  typedef std::map< std::string , Detector* >   detectorsMapDef;

  bool compare_zPosition (std::string first, std::string second);

  detectorsMapDef                 detectorsMap_              ;
  std::vector<int>                runNumbers_                ;
  unsigned int                    dutNumbers_                ;
  std::string                     geometryFileName_          ;
  bool                            calibrationDone_           ;

  //std::stringstream               ss_                        ;//! temporary state value

  ClassDef(Geometry,5)

} ;


#endif // GEOMETRY_H
