alias do='make -f MakefileAll '
export BASEDATADIR=/home/akrzywda/Programming/2013_11_November
export STORAGEDIR=/media/96c057c5-5e1d-400c-b99f-a1aaab8d6ff8

  #===== Local directories
  export MonicelliDir=`pwd`
  export Monicelli_RawData_Dir=${BASEDATADIR}/Merged
  export Monicelli_DataSample_Dir=${BASEDATADIR}/Merged
  export Monicelli_CalSample_Dir=${BASEDATADIR}/Calibrations
  export MonicelliOutputDir=${STORAGEDIR}/MonicelliOutput
  export Monicelli_XML_Dir=${BASEDATADIR}/Geometries
  
  #===== Location of the ROOT components
  export ROOTSYS=/opt/local/root

  #===== Location of the Qt components
  export QTDIR=/usr/local/qt
  export QTCREATORDIR=/usr/local/qt-creator
  export QTROOTSYSDIR=`pwd`/qtrootsys
  export QTROOT_INCLUDE_PATH=$ROOTSYS/include

  #===== Location of the BOOST components
  export BOOSTINC=/usr/include
  export BOOSTLIB=/usr/lib64

  #===== Location of the XERCES components
  export XERCESCINC=/opt/local/xerces/include
  export XERCESCLIB=/opt/local/xerces/lib
  


#===== Final PATH definitions
export PATH=$ROOTSYS/bin:$QTDIR/bin:$QTCREATORDIR/bin:$PATH
export LD_LIBRARY_PATH=$ROOTSYS/lib:$QTDIR/lib:$QTCREATORDIR/lib:$XERCESCLIB/:$MonicelliDir/plugins/libs/:$MonicelliDir/test/customWidgets/libs
