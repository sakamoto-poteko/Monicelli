#ifndef customTableViewPLUGIN_H
#define customTableViewPLUGIN_H
/****************************************************************************
** Authors: Dario Menasce, Stefano Terzo
**
** I.N.F.N. Milan-Bicocca
** Piazza  della Scienza 3, Edificio U2
** Milano, 20126
**
****************************************************************************/

#include <QDesignerCustomWidgetInterface>

class customTableViewPlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    customTableViewPlugin(QObject *parent = 0);

    bool      isContainer  (void                                  ) const ;
    bool      isInitialized(void                                  ) const ;
    QIcon     icon         (void                                  ) const ;
    QString   domXml       (void                                  ) const ;
    QString   group        (void                                  ) const ;
    QString   includeFile  (void                                  ) const ;
    QString   name         (void                                  ) const ;
    QString   toolTip      (void                                  ) const ;
    QString   whatsThis    (void                                  ) const ;
    QWidget * createWidget (QWidget                      * parent )       ;
    void      initialize   (QDesignerFormEditorInterface * core   )       ;

private:
    bool      initialized;
};

#endif // customTableViewPLUGIN_H
