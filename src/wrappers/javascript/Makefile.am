AM_CPPFLAGS = @DAKOTAINCL@ @PETSCINCL@ @MPIINCL@ @SPOOLESINCL@ @PARMETISINCL@ @METISINCL@ @TRIANGLEINCL@ @CHACOINCL@ @SCOTCHINCL@ @SHAPELIBINCL@ @AMPIINCL@ @ADJOINTMPIINCL@ @CODIPACKINCL@
AUTOMAKE_OPTIONS = subdir-objects

EXEEXT=$(JAVASCRIPTWRAPPEREXT)

# Define prefix (from http://www.gnu.org/software/autoconf/manual/autoconf-2.67/html_node/Defining-Directories.html)
AM_CPPFLAGS += -DISSM_PREFIX='"$(prefix)"'

js_scripts = \
	${ISSM_DIR}/src/wrappers/BamgMesher/BamgMesher.js \
	${ISSM_DIR}/src/wrappers/Triangle/Triangle.js \
	${ISSM_DIR}/src/wrappers/NodeConnectivity/NodeConnectivity.js \
	${ISSM_DIR}/src/wrappers/ContourToMesh/ContourToMesh.js \
	${ISSM_DIR}/src/wrappers/ElementConnectivity/ElementConnectivity.js \
	${ISSM_DIR}/src/wrappers/InterpFromGridToMesh/InterpFromGridToMesh.js \
	${ISSM_DIR}/src/wrappers/InterpFromMeshToMesh2d/InterpFromMeshToMesh2d.js \
	${ISSM_DIR}/src/wrappers/IssmConfig/IssmConfig.js \
	${ISSM_DIR}/src/wrappers/Issm/issm.js

bin_SCRIPTS =  issm-prebin.js

issm-prebin.js: ${js_scripts}
	cat ${js_scripts}  > issm-prebin.js

# JavaScript I/O{{{
lib_LTLIBRARIES = libISSMJavascript.la

io_sources = \
	./io/WriteJavascriptData.cpp \
	./io/FetchJavascriptData.cpp

ALLCXXFLAGS= -fPIC -D_WRAPPERS_ $(CXXFLAGS)

libISSMJavascript_la_SOURCES = $(io_sources)
libISSMJavascript_la_CXXFLAGS= $(ALLCXXFLAGS)
#}}}
# API I/O{{{
lib_LTLIBRARIES += libISSMApi.la

api_sources= ./io/ApiPrintf.cpp

libISSMApi_la_SOURCES = $(api_sources)
libISSMApi_la_CXXFLAGS= $(ALLCXXFLAGS)
#}}}
# Wrappers {{{
bin_PROGRAMS = IssmModule
#}}}

# Dependencies {{{

# Triangle
AM_CXXFLAGS = -DTRILIBRARY -DANSI_DECLARATORS -DNO_TIMER -D_WRAPPERS_
AM_CXXFLAGS += -D_HAVE_JAVASCRIPT_MODULES_ -fPIC

deps = ./libISSMJavascript.la ../../c/libISSMModules.la ../../c/libISSMCore.la ./libISSMApi.la

# Optimization flags
AM_CXXFLAGS += $(CXXFLAGS)
#}}}
# Module sources and dependencies {{{
libISSMJavascript_la_LIBADD = ./../../c/libISSMCore.la ./../../c/libISSMModules.la $(MPILIB) $(PETSCLIB) $(MKLLIB) $(GSLLIB) $(MATHLIB)

if VERSION
libISSMJavascript_la_LDFLAGS =
libISSMApi_la_LDFLAGS =
else
libISSMJavascript_la_LDFLAGS = -avoid-version
libISSMApi_la_LDFLAGS = -avoid-version
endif

if STANDALONE_LIBRARIES
if !MSYS2
libISSMJavascript_la_LDFLAGS += -static
libISSMApi_la_LDFLAGS += -static
endif
deps += $(DAKOTALIB) $(PETSCLIB) $(MUMPSLIB) $(SCALAPACKLIB) $(BLASLAPACKLIB) $(PARMETISLIB) $(METISLIB) $(HDF5LIB) $(TAOLIB) $(M1QN3LIB) $(SEMICLIB) $(PLAPACKLIB) $(SUPERLULIB) $(SPOOLESLIB) $(TRIANGLELIB) $(BLACSLIB) $(HYPRELIB) $(SPAILIB) $(PROMETHEUSLIB) $(PASTIXLIB) $(MLLIB) $(SCOTCHLIB) $(MKLLIB) $(MPILIB) $(NEOPZLIB) $(MATHLIB) $(GRAPHICSLIB) $(MULTITHREADINGLIB) $(GSLLIB) $(ADOLCLIB) $(AMPILIB) $(METEOIOLIB) $(SNOWPACKLIB) $(PROJLIB) $(OSLIBS)
endif

libISSMApi_la_LIBADD = $(PETSCLIB) $(MUMPSLIB) $(SCALAPACKLIB) $(BLASLAPACKLIB) $(PARMETISLIB) $(METISLIB) $(HDF5LIB) $(MPILIB) $(NEOPZLIB) $(GSLLIB) $(PROJLIB) $(MATHLIB)

IssmModule_SOURCES = \
	../BamgMesher/BamgMesher.cpp \
	../Triangle/Triangle.cpp \
	../NodeConnectivity/NodeConnectivity.cpp \
	../ContourToMesh/ContourToMesh.cpp \
	../ElementConnectivity/ElementConnectivity.cpp \
	../InterpFromGridToMesh/InterpFromGridToMesh.cpp \
	../InterpFromMeshToMesh2d/InterpFromMeshToMesh2d.cpp \
	../IssmConfig/IssmConfig.cpp \
	../Issm/issm.cpp

IssmModule_CXXFLAGS= -fPIC -D_DO_NOT_LOAD_GLOBALS_ $(AM_CXXFLAGS) $(CXXFLAGS) -s EXPORTED_FUNCTIONS="['_BamgMesherModule','_TriangleModule','_NodeConnectivityModule','_ContourToMeshModule','_ElementConnectivityModule','_InterpFromGridToMeshModule','_InterpFromMeshToMesh2dModule','_IssmConfigModule','_IssmModule']" -s DISABLE_EXCEPTION_CATCHING=0 -s ALLOW_MEMORY_GROWTH=1 -s INVOKE_RUN=0
IssmModule_LDADD = ${deps} $(TRIANGLELIB) $(GSLLIB)
#}}}
