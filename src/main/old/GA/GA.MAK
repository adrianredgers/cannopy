# Microsoft Visual C++ generated build script - Do not modify

PROJ = GA
DEBUG = 1
PROGTYPE = 3
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = /d_DEBUG 
R_RCDEFINES = /dNDEBUG 
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = C:\QTM\CURRENT\GA\
USEMFC = 1
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC = ALLOCATE.C  
FIRSTCPP =             
RC = rc
CFLAGS_D_WTTY = /nologo /G2 /Mq /W4 /Zi /AH /Gx- /Od /D "_DEBUG" /FR /Fd"GENNET.PDB"
CFLAGS_R_WTTY = /nologo /Gs /G2 /Mq /W3 /AH /Ox /D "NDEBUG" /FR 
LFLAGS_D_WTTY = /NOLOGO /NOD /NOI /PACKC:57334 /STACK:1000 /ALIGN:16 /ONERROR:NOEXE /CO 
LFLAGS_R_WTTY = /NOLOGO /NOD /NOI /PACKC:57344 /STACK:8096 /ALIGN:16 /ONERROR:NOEXE 
LIBS_D_WTTY = lafxcwd oldnames libw llibcewq 
LIBS_R_WTTY = lafxcw oldnames libw llibcewq 
RCFLAGS = /nologo 
RESFLAGS = /nologo /k
RUNFLAGS = 
DEFFILE = GA.DEF
OBJS_EXT = 
LIBS_EXT = 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_WTTY)
LFLAGS = $(LFLAGS_D_WTTY)
LIBS = $(LIBS_D_WTTY)
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
DEFFILE=C:\QTM\CURRENT\GA\GA.DEF
!else
CFLAGS = $(CFLAGS_R_WTTY)
LFLAGS = $(LFLAGS_R_WTTY)
LIBS = $(LIBS_R_WTTY)
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
DEFFILE=C:\QTM\CURRENT\GA\GA.DEF
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = ALLOCATE.SBR \
		GA_STRIP.SBR \
		UTILS.SBR \
		GA_CONF.SBR \
		GA_MAIN.SBR \
		GA.SBR \
		SETUPNN.SBR \
		GET_CONF.SBR \
		PREPARE.SBR \
		RND_SORT.SBR \
		CALCPERF.SBR \
		STATUTIL.SBR \
		PANDL.SBR


ALLOCATE_DEP = c:\qtm\current\ga\g_type.h \
	c:\qtm\current\ga\proto.h


GA_STRIP_DEP = c:\qtm\current\ga\g_type.h \
	c:\qtm\current\ga\proto.h


UTILS_DEP = c:\qtm\current\ga\g_type.h \
	c:\qtm\current\ga\proto.h


GA_CONF_DEP = c:\qtm\current\ga\g_type.h \
	c:\qtm\current\ga\proto.h


GA_MAIN_DEP = c:\qtm\current\ga\g_type.h \
	c:\qtm\current\ga\proto.h


GA_DEP = c:\qtm\current\ga\g_type.h \
	c:\qtm\current\ga\proto.h


SETUPNN_DEP = c:\qtm\current\ga\g_type.h \
	c:\qtm\current\ga\proto.h


GET_CONF_DEP = c:\qtm\current\ga\g_type.h \
	c:\qtm\current\ga\proto.h


PREPARE_DEP = c:\qtm\current\ga\g_type.h \
	c:\qtm\current\ga\proto.h


RND_SORT_DEP = c:\qtm\current\ga\g_type.h \
	c:\qtm\current\ga\proto.h


CALCPERF_DEP = c:\qtm\current\ga\g_type.h \
	c:\qtm\current\ga\proto.h


STATUTIL_DEP = c:\qtm\current\ga\g_type.h \
	c:\qtm\current\ga\proto.h


all:	$(PROJ).EXE $(PROJ).BSC

ALLOCATE.OBJ:	ALLOCATE.C $(ALLOCATE_DEP)
	$(CC) $(CFLAGS) $(CCREATEPCHFLAG) /c ALLOCATE.C

GA_STRIP.OBJ:	GA_STRIP.C $(GA_STRIP_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c GA_STRIP.C

UTILS.OBJ:	UTILS.C $(UTILS_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c UTILS.C

GA_CONF.OBJ:	GA_CONF.C $(GA_CONF_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c GA_CONF.C

GA_MAIN.OBJ:	GA_MAIN.C $(GA_MAIN_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c GA_MAIN.C

GA.OBJ:	GA.C $(GA_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c GA.C

SETUPNN.OBJ:	SETUPNN.C $(SETUPNN_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c SETUPNN.C

GET_CONF.OBJ:	GET_CONF.C $(GET_CONF_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c GET_CONF.C

PREPARE.OBJ:	PREPARE.C $(PREPARE_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c PREPARE.C

RND_SORT.OBJ:	RND_SORT.C $(RND_SORT_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c RND_SORT.C

CALCPERF.OBJ:	CALCPERF.C $(CALCPERF_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c CALCPERF.C

STATUTIL.OBJ:	STATUTIL.C $(STATUTIL_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c STATUTIL.C

PANDL.OBJ:	PANDL.C $(PANDL_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c PANDL.C


$(PROJ).EXE::	ALLOCATE.OBJ GA_STRIP.OBJ UTILS.OBJ GA_CONF.OBJ GA_MAIN.OBJ GA.OBJ \
	SETUPNN.OBJ GET_CONF.OBJ PREPARE.OBJ RND_SORT.OBJ CALCPERF.OBJ STATUTIL.OBJ PANDL.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
ALLOCATE.OBJ +
GA_STRIP.OBJ +
UTILS.OBJ +
GA_CONF.OBJ +
GA_MAIN.OBJ +
GA.OBJ +
SETUPNN.OBJ +
GET_CONF.OBJ +
PREPARE.OBJ +
RND_SORT.OBJ +
CALCPERF.OBJ +
STATUTIL.OBJ +
PANDL.OBJ +
$(OBJS_EXT)
$(PROJ).EXE
$(MAPFILE)
c:\msvc\lib\+
c:\msvc\mfc\lib\+
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF
	$(RC) $(RESFLAGS) $@


run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
