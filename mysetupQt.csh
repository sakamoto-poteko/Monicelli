alias do 'make -f MakefileAll '
setenv BASEDATADIR /storage/2012_04_April

  #===== Local directories
  setenv MonicelliDir		   `pwd`
  setenv Monicelli_RawData_Dir     ${BASEDATADIR}/Merged
  setenv Monicelli_DataSample_Dir  ${BASEDATADIR}/Merged
  setenv Monicelli_CalSample_Dir   ${BASEDATADIR}/Calibrations
  setenv MonicelliOutputDir	   ${BASEDATADIR}/MonicelliOutput
  setenv Monicelli_XML_Dir         ${BASEDATADIR}/Geometries
  
  #===== Location of the ROOT components
  setenv ROOTSYS		   /usr/local/root

  #===== Location of the Qt components
  setenv QTDIR  		   /usr/local/Trolltech/Qt-4.8.1
  setenv QTCREATORDIR		   /opt/QtSDK/QtCreator
  setenv QTROOTSYSDIR		   `pwd`/qtrootsys
  setenv QTROOT_INCLUDE_PATH	   $ROOTSYS/include

  #===== Location of the BOOST components
  setenv BOOSTINC		   /usr/local
  setenv BOOSTLIB		   /usr/local/lib

  #===== Location of the XERCES components
  setenv XERCESCINC		   /usr/local/include
  setenv XERCESCLIB		   /usr/local/lib
  
  setenv LD_LIBRARY_PATH           /usr/lib64

#===== Final PATH definitions
setenv PATH			$ROOTSYS/bin:$QTDIR/bin:$QTCREATORDIR/bin:$PATH
setenv LD_LIBRARY_PATH          $ROOTSYS/lib:$QTDIR/lib:$QTCREATORDIR/lib:$XERCESCLIB/:$MonicelliDir/plugins/libs/:$MonicelliDir/test/customWidgets/libs:$LD_LIBRARY_PATH

#----------------------------------------------------------------------------------------------------
