#############################################################################
# Makefile for building: libcustomLineEdit.so
# Generated by qmake (2.01a) (Qt 4.8.0) on: Fri Dec 6 10:08:14 2013
# Project:  customLineEdit.pro
# Template: lib
# Command: /usr/local/qt/bin/qmake -o Makefile customLineEdit.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_NO_DEBUG -DQT_PLUGIN -DQT_SCRIPT_LIB -DQT_XML_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQDESIGNER_EXPORT_WIDGETS -DQT_SHARED
CFLAGS        = -m64 -pipe -O2 -D_REENTRANT -Wall -W -fPIC $(DEFINES)
CXXFLAGS      = -m64 -pipe -O2 -D_REENTRANT -Wall -W -fPIC $(DEFINES)
INCPATH       = -I/usr/local/Qt-4.8.0/mkspecs/linux-g++-64 -I. -I/usr/local/Qt-4.8.0/include/QtDesigner -I/usr/local/Qt-4.8.0/include/QtCore -I/usr/local/Qt-4.8.0/include/QtGui -I/usr/local/Qt-4.8.0/include/QtXml -I/usr/local/Qt-4.8.0/include/QtScript -I/usr/local/Qt-4.8.0/include -I../../include -I. -I.
LINK          = g++
LFLAGS        = -m64 -Wl,-O1 -Wl,-rpath,/usr/local/Trolltech/Qt-4.8.0/lib -shared
LIBS          = $(SUBLIBS)  -L/usr/local/Qt-4.8.0/lib -lQtScript -L/usr/local/Qt-4.8.0/lib -lQtXml -lQtGui -L/usr/X11R6/lib64 -lQtCore -lQtDesigner -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/local/qt/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = customLineEdit.cpp \
		customLineEditPlugin.cpp moc_customLineEdit.cpp \
		moc_customLineEditPlugin.cpp
OBJECTS       = customLineEdit.o \
		customLineEditPlugin.o \
		moc_customLineEdit.o \
		moc_customLineEditPlugin.o
DIST          = /usr/local/Qt-4.8.0/mkspecs/common/unix.conf \
		/usr/local/Qt-4.8.0/mkspecs/common/linux.conf \
		/usr/local/Qt-4.8.0/mkspecs/common/gcc-base.conf \
		/usr/local/Qt-4.8.0/mkspecs/common/gcc-base-unix.conf \
		/usr/local/Qt-4.8.0/mkspecs/common/g++-base.conf \
		/usr/local/Qt-4.8.0/mkspecs/common/g++-unix.conf \
		/usr/local/Qt-4.8.0/mkspecs/qconfig.pri \
		/usr/local/Qt-4.8.0/mkspecs/modules/qt_webkit_version.pri \
		/usr/local/Qt-4.8.0/mkspecs/features/qt_functions.prf \
		/usr/local/Qt-4.8.0/mkspecs/features/qt_config.prf \
		/usr/local/Qt-4.8.0/mkspecs/features/exclusive_builds.prf \
		/usr/local/Qt-4.8.0/mkspecs/features/default_pre.prf \
		/usr/local/Qt-4.8.0/mkspecs/features/release.prf \
		/usr/local/Qt-4.8.0/mkspecs/features/default_post.prf \
		/usr/local/Qt-4.8.0/mkspecs/features/qt.prf \
		/usr/local/Qt-4.8.0/mkspecs/features/designer.prf \
		/usr/local/Qt-4.8.0/mkspecs/features/unix/thread.prf \
		/usr/local/Qt-4.8.0/mkspecs/features/moc.prf \
		/usr/local/Qt-4.8.0/mkspecs/features/warn_on.prf \
		/usr/local/Qt-4.8.0/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/local/Qt-4.8.0/mkspecs/features/resources.prf \
		/usr/local/Qt-4.8.0/mkspecs/features/uic.prf \
		/usr/local/Qt-4.8.0/mkspecs/features/yacc.prf \
		/usr/local/Qt-4.8.0/mkspecs/features/lex.prf \
		/usr/local/Qt-4.8.0/mkspecs/features/include_source_dir.prf \
		customLineEdit.pro
QMAKE_TARGET  = customLineEdit
DESTDIR       = 
TARGET        = libcustomLineEdit.so
TARGETD       = libcustomLineEdit.so

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile  $(TARGET)

$(TARGET): ui_customLineEdit.h $(OBJECTS) $(SUBLIBS) $(OBJCOMP)  
	-$(DEL_FILE) $(TARGET)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS) $(OBJCOMP)



Makefile: customLineEdit.pro  /usr/local/Qt-4.8.0/mkspecs/linux-g++-64/qmake.conf /usr/local/Qt-4.8.0/mkspecs/common/unix.conf \
		/usr/local/Qt-4.8.0/mkspecs/common/linux.conf \
		/usr/local/Qt-4.8.0/mkspecs/common/gcc-base.conf \
		/usr/local/Qt-4.8.0/mkspecs/common/gcc-base-unix.conf \
		/usr/local/Qt-4.8.0/mkspecs/common/g++-base.conf \
		/usr/local/Qt-4.8.0/mkspecs/common/g++-unix.conf \
		/usr/local/Qt-4.8.0/mkspecs/qconfig.pri \
		/usr/local/Qt-4.8.0/mkspecs/modules/qt_webkit_version.pri \
		/usr/local/Qt-4.8.0/mkspecs/features/qt_functions.prf \
		/usr/local/Qt-4.8.0/mkspecs/features/qt_config.prf \
		/usr/local/Qt-4.8.0/mkspecs/features/exclusive_builds.prf \
		/usr/local/Qt-4.8.0/mkspecs/features/default_pre.prf \
		/usr/local/Qt-4.8.0/mkspecs/features/release.prf \
		/usr/local/Qt-4.8.0/mkspecs/features/default_post.prf \
		/usr/local/Qt-4.8.0/mkspecs/features/qt.prf \
		/usr/local/Qt-4.8.0/mkspecs/features/designer.prf \
		/usr/local/Qt-4.8.0/mkspecs/features/unix/thread.prf \
		/usr/local/Qt-4.8.0/mkspecs/features/moc.prf \
		/usr/local/Qt-4.8.0/mkspecs/features/warn_on.prf \
		/usr/local/Qt-4.8.0/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/local/Qt-4.8.0/mkspecs/features/resources.prf \
		/usr/local/Qt-4.8.0/mkspecs/features/uic.prf \
		/usr/local/Qt-4.8.0/mkspecs/features/yacc.prf \
		/usr/local/Qt-4.8.0/mkspecs/features/lex.prf \
		/usr/local/Qt-4.8.0/mkspecs/features/include_source_dir.prf \
		/usr/local/Qt-4.8.0/lib/libQtScript.prl \
		/usr/local/Qt-4.8.0/lib/libQtCore.prl \
		/usr/local/Qt-4.8.0/lib/libQtXml.prl \
		/usr/local/Qt-4.8.0/lib/libQtGui.prl \
		/usr/local/Qt-4.8.0/lib/libQtDesigner.prl
	$(QMAKE) -o Makefile customLineEdit.pro
/usr/local/Qt-4.8.0/mkspecs/common/unix.conf:
/usr/local/Qt-4.8.0/mkspecs/common/linux.conf:
/usr/local/Qt-4.8.0/mkspecs/common/gcc-base.conf:
/usr/local/Qt-4.8.0/mkspecs/common/gcc-base-unix.conf:
/usr/local/Qt-4.8.0/mkspecs/common/g++-base.conf:
/usr/local/Qt-4.8.0/mkspecs/common/g++-unix.conf:
/usr/local/Qt-4.8.0/mkspecs/qconfig.pri:
/usr/local/Qt-4.8.0/mkspecs/modules/qt_webkit_version.pri:
/usr/local/Qt-4.8.0/mkspecs/features/qt_functions.prf:
/usr/local/Qt-4.8.0/mkspecs/features/qt_config.prf:
/usr/local/Qt-4.8.0/mkspecs/features/exclusive_builds.prf:
/usr/local/Qt-4.8.0/mkspecs/features/default_pre.prf:
/usr/local/Qt-4.8.0/mkspecs/features/release.prf:
/usr/local/Qt-4.8.0/mkspecs/features/default_post.prf:
/usr/local/Qt-4.8.0/mkspecs/features/qt.prf:
/usr/local/Qt-4.8.0/mkspecs/features/designer.prf:
/usr/local/Qt-4.8.0/mkspecs/features/unix/thread.prf:
/usr/local/Qt-4.8.0/mkspecs/features/moc.prf:
/usr/local/Qt-4.8.0/mkspecs/features/warn_on.prf:
/usr/local/Qt-4.8.0/mkspecs/features/unix/gdb_dwarf_index.prf:
/usr/local/Qt-4.8.0/mkspecs/features/resources.prf:
/usr/local/Qt-4.8.0/mkspecs/features/uic.prf:
/usr/local/Qt-4.8.0/mkspecs/features/yacc.prf:
/usr/local/Qt-4.8.0/mkspecs/features/lex.prf:
/usr/local/Qt-4.8.0/mkspecs/features/include_source_dir.prf:
/usr/local/Qt-4.8.0/lib/libQtScript.prl:
/usr/local/Qt-4.8.0/lib/libQtCore.prl:
/usr/local/Qt-4.8.0/lib/libQtXml.prl:
/usr/local/Qt-4.8.0/lib/libQtGui.prl:
/usr/local/Qt-4.8.0/lib/libQtDesigner.prl:
qmake:  FORCE
	@$(QMAKE) -o Makefile customLineEdit.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/customLineEdit1.0.0 || $(MKDIR) .tmp/customLineEdit1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/customLineEdit1.0.0/ && $(COPY_FILE) --parents customLineEdit.h customLineEditPlugin.h .tmp/customLineEdit1.0.0/ && $(COPY_FILE) --parents customLineEdit.cpp customLineEditPlugin.cpp .tmp/customLineEdit1.0.0/ && $(COPY_FILE) --parents customLineEdit.ui .tmp/customLineEdit1.0.0/ && (cd `dirname .tmp/customLineEdit1.0.0` && $(TAR) customLineEdit1.0.0.tar customLineEdit1.0.0 && $(COMPRESS) customLineEdit1.0.0.tar) && $(MOVE) `dirname .tmp/customLineEdit1.0.0`/customLineEdit1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/customLineEdit1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_customLineEdit.cpp moc_customLineEditPlugin.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_customLineEdit.cpp moc_customLineEditPlugin.cpp
moc_customLineEdit.cpp: customLineEdit.h
	/usr/local/Qt-4.8.0/bin/moc $(DEFINES) $(INCPATH) customLineEdit.h -o moc_customLineEdit.cpp

moc_customLineEditPlugin.cpp: customLineEditPlugin.h
	/usr/local/Qt-4.8.0/bin/moc $(DEFINES) $(INCPATH) customLineEditPlugin.h -o moc_customLineEditPlugin.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: ui_customLineEdit.h
compiler_uic_clean:
	-$(DEL_FILE) ui_customLineEdit.h
ui_customLineEdit.h: customLineEdit.ui
	/usr/local/Qt-4.8.0/bin/uic customLineEdit.ui -o ui_customLineEdit.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_uic_clean 

####### Compile

customLineEdit.o: customLineEdit.cpp customLineEdit.h \
		ui_customLineEdit.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o customLineEdit.o customLineEdit.cpp

customLineEditPlugin.o: customLineEditPlugin.cpp customLineEdit.h \
		customLineEditPlugin.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o customLineEditPlugin.o customLineEditPlugin.cpp

moc_customLineEdit.o: moc_customLineEdit.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_customLineEdit.o moc_customLineEdit.cpp

moc_customLineEditPlugin.o: moc_customLineEditPlugin.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_customLineEditPlugin.o moc_customLineEditPlugin.cpp

####### Install

install_target: first FORCE
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)/home/akrzywda/Programming/Monicelli/plugins/customLineEdit/../libs/ || $(MKDIR) $(INSTALL_ROOT)/home/akrzywda/Programming/Monicelli/plugins/customLineEdit/../libs/ 
	-$(INSTALL_PROGRAM) "$(TARGET)" "$(INSTALL_ROOT)/home/akrzywda/Programming/Monicelli/plugins/libs/$(TARGET)"
	-$(STRIP) --strip-unneeded "$(INSTALL_ROOT)/home/akrzywda/Programming/Monicelli/plugins/libs/$(TARGET)"

uninstall_target:  FORCE
	-$(DEL_FILE) "$(INSTALL_ROOT)/home/akrzywda/Programming/Monicelli/plugins/libs/$(TARGET)"
	-$(DEL_DIR) $(INSTALL_ROOT)/home/akrzywda/Programming/Monicelli/plugins/customLineEdit/../libs/ 


install:  install_target  FORCE

uninstall: uninstall_target   FORCE

FORCE:

