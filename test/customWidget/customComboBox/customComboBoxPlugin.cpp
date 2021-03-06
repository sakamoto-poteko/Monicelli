#ifndef customComboBoxPlugin_H
#define customComboBoxPlugin_H
#include <assert.h>
#include "customComboBox.h"
#include "customComboBoxPlugin.h"

#include <QtPlugin>

//=============================================================================
customComboBoxPlugin::customComboBoxPlugin(QObject *parent)
    : QObject(parent)
{
    initialized = false;
}
//=============================================================================
void customComboBoxPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (initialized)
        return;

    initialized = true;
}
//=============================================================================
bool customComboBoxPlugin::isInitialized() const
{
    return initialized;
}
//=============================================================================
QWidget *customComboBoxPlugin::createWidget(QWidget *parent)
{
    return new customComboBox(parent);
}
//=============================================================================
QString customComboBoxPlugin::name() const
{
    return "customComboBox";
}
//=============================================================================
QString customComboBoxPlugin::group() const
{
    return "Input Widgets";
}
//=============================================================================
QIcon customComboBoxPlugin::icon() const
{
    return QIcon();
}
//=============================================================================
QString customComboBoxPlugin::toolTip() const
{
    return "This is a custom combobox";
}
//=============================================================================
QString customComboBoxPlugin::whatsThis() const
{
    return "";
}
//=============================================================================
bool customComboBoxPlugin::isContainer() const
{
    return false;
}
//=============================================================================
QString customComboBoxPlugin::domXml() const
{
assert(0) ;
    return "<ui language=\"c++\" displayname=\"MyComboBox\">\n"
           " <widget class=\"customComboBox\" name=\"customComboBox\">\n"
           "  <customwidgets>"
           "   <customwidget>"
           "    <property name=\"geometry\">\n"       
           "     <rect>\n"                            
           "      <x>0</x>\n"                         
           "      <y>0</y>\n"                         
           "      <width>500</width>\n"               
           "      <height>100</height>\n"             
           "     </rect>\n"                           
           "    </property>\n"                        

           "    <property name=\"toolTip\" >\n"       
           "     <string>The current time</string>\n" 
           "    </property>\n"                        
           
           "    <property name=\"whatsThis\" >\n"     
           "     <string>The analog clock widget displays the current time.</string>\n"
           "    </property>\n"
           "   </customwidget>"
           "  </customwidgets>"
           
           " </widget>\n"
           "</ui>\n";
}
//=============================================================================
QString customComboBoxPlugin::includeFile() const
{
    return "customComboBox.h";
}
//=============================================================================
Q_EXPORT_PLUGIN2(customComboBoxPlugin, customComboBoxPlugin)

#endif // customComboBoxPlugin_H
