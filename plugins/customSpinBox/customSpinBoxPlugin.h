/****************************************************************************
** Authors: Dario Menasce, Stefano Terzo
**
** I.N.F.N. Milan-Bicocca
** Piazza  della Scienza 3, Edificio U2
** Milano, 20126
**
****************************************************************************/
#ifndef CUSTOMSPINBOXPLUGIN_H
#define CUSTOMSPINBOXPLUGIN_H

#include <QDesignerCustomWidgetInterface>

class customSpinBoxPlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)

public:
    customSpinBoxPlugin(QObject *parent = 0);

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

#endif // CUSTOMSPINBOXPLUGIN_H
