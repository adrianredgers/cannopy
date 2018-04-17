# Microsoft Visual C++ generated build script - Do not modify

PROJ = BP
DEBUG = 1
PROGTYPE = 3
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = /d_DEBUG 
R_RCDEFINES = /dNDEBUG 
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = C:\QTM\CURRENT\BP\
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
CFLAGS_R_WTTY = /nologo /Gs /G2 /Mq /W3 /AH /O2 /D "NDEBUG" /FR 
LFLAGS_D_WTTY = /NOLOGO /NOD /NOI /PACKC:57334 /STACK:1000 /ALIGN:16 /ONERROR:NOEXE /CO 
LFLAGS_R_WTTY = /NOLOGO /NOD /NOI /PACKC:57344 /STACK:1024 /ALIGN:16 /ONERROR:NOEXE 
LIBS_D_WTTY = lafxcwd oldnames libw llibcewq 
LIBS_R_WTTY = lafxcw oldnames libw llibcewq 
RCFLAGS = /nologo 
RESFLAGS = /nologo /k
RUNFLAGS = 
DEFFILE = BP.DEF
OBJS_EXT = 
LIBS_EXT = 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_WTTY)
LFLAGS = $(LFLAGS_D_WTTY)
LIBS = $(LIBS_D_WTTY)
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
DEFFILE=C:\QTM\CURRENT\BP\BP.DEF
!else
CFLAGS = $(CFLAGS_R_WTTY)
LFLAGS = $(LFLAGS_R_WTTY)
LIBS = $(LIBS_R_WTTY)
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
DEFFILE=C:\QTM\CURRENT\BP\BP.DEF
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = ALLOCATE.SBR \
		BP.SBR \
		BP_CONF.SBR \
		BP_STRIP.SBR \
		CALCPERF.SBR \
		GET_CONF.SBR \
		MAIN.SBR \
		PANDL.SBR \
		PREPARE2.SBR \
		SETUPNN.SBR \
		STATUTIL.SBR \
		UTILS.SBR


ALLOCATE_DEP = c:\qtm\current\bp\header.h \
	c:\qtm\current\bp\struct.h \
	c:\qtm\current\bp\prtype.h


BP_DEP = c:\qtm\current\bp\header.h \
	c:\qtm\current\bp\struct.h \
	c:\qtm\current\bp\prtype.h


BP_CONF_DEP = c:\qtm\current\bp\header.h \
	c:\qtm\current\bp\struct.h \
	c:\qtm\current\bp\prtype.h


BP_STRIP_DEP = c:\qtm\current\bp\header.h \
	c:\qtm\current\bp\struct.h \
	c:\qtm\current\bp\prtype.h


CALCPERF_DEP = c:\qtm\current\bp\header.h \
	c:\qtm\current\bp\struct.h \
	c:\qtm\current\bp\prtype.h


GET_CONF_DEP = c:\qtm\current\bp\header.h \
	c:\qtm\current\bp\struct.h \
	c:\qtm\current\bp\prtype.h


MAIN_DEP = c:\qtm\current\bp\header.h \
	c:\qtm\current\bp\struct.h \
	c:\qtm\current\bp\prtype.h


PANDL_DEP = c:\qtm\current\bp\header.h \
	c:\qtm\current\bp\struct.h \
	c:\qtm\current\bp\prtype.h


PREPARE2_DEP = c:\qtm\current\bp\header.h \
	c:\qtm\current\bp\struct.h \
	c:\qtm\current\bp\prtype.h


SETUPNN_DEP = c:\qtm\current\bp\header.h \
	c:\qtm\current\bp\struct.h \
	c:\qtm\current\bp\prtype.h


STATUTIL_DEP = c:\qtm\current\bp\header.h \
	c:\qtm\current\bp\struct.h \
	c:\qtm\current\bp\prtype.h


UTILS_DEP = c:\qtm\current\bp\header.h \
	c:\qtm\current\bp\struct.h \
	c:\qtm\current\bp\prtype.h


all:	$(PROJ).EXE $(PROJ).BSC

ALLOCATE.OBJ:	ALLOCATE.C $(ALLOCATE_DEP)
	$(CC) $(CFLAGS) $(CCREATEPCHFLAG) /c ALLOCATE.C

BP.OBJ:	BP.C $(BP_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c BP.C

BP_CONF.OBJ:	BP_CONF.C $(BP_CONF_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c BP_CONF.C

BP_STRIP.OBJ:	BP_STRIP.C $(BP_STRIP_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c BP_STRIP.C

CALCPERF.OBJ:	CALCPERF.C $(CALCPERF_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c CALCPERF.C

GET_CONF.OBJ:	GET_CONF.C $(GET_CONF_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c GET_CONF.C

MAIN.OBJ:	MAIN.C $(MAIN_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c MAIN.C

PANDL.OBJ:	PANDL.C $(PANDL_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c PANDL.C

PREPARE2.OBJ:	PREPARE2.C $(PREPARE2_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c PREPARE2.C

SETUPNN.OBJ:	SETUPNN.C $(SETUPNN_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c SETUPNN.C

STATUTIL.OBJ:	STATUTIL.C $(STATUTIL_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c STATUTIL.C

UTILS.OBJ:	UTILS.C $(UTILS_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c UTILS.C


$(PROJ).EXE::	ALLOCATE.OBJ BP.OBJ BP_CONF.OBJ BP_STRIP.OBJ CALCPERF.OBJ GET_CONF.OBJ \
	MAIN.OBJ PANDL.OBJ PREPARE2.OBJ SETUPNN.OBJ STATUTIL.OBJ UTILS.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
ALLOCATE.OBJ +
BP.OBJ +
BP_CONF.OBJ +
BP_STRIP.OBJ +
CALCPERF.OBJ +
GET_CONF.OBJ +
MAIN.OBJ +
PANDL.OBJ +
PREPARE2.OBJ +
SETUPNN.OBJ +
STATUTIL.OBJ +
UTILS.OBJ +
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
