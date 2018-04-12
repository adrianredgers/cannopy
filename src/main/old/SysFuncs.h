/*      SysFuncs.h         agcr        12/5/91                             *//*        Functions header file. Designed for use with the gcc ansi compiler.Contents:        System library functions *//* *** System Library Functions *** *//* UNIX */void system(char *message);/* I/O */void printf(const char *fmt, ...);void sprintf(char *toStr, const char *fmt, ...);void fprintf(FILE *fPtr, const char *fmt, ...);void fflush(FILE *);int scanf(const char *fmt, ...);int sscanf(char *fromStr, const char *fmt, ...);int fscanf(FILE *fPtr, const char *fmt, ...);void puts(char *letter);int _filbuf(FILE *fPtr);                /* for putchar(char letter) macro */int _flsbuf(char letter, FILE *fPtr);   /* for getchar(char letter) macro *//*int getc(FILE *fPtr);*//*int putc(int c, FILE *fPtr);*/int fgetc(FILE *fPtr);/* Memory Allocation */void free(void *ptr);void *malloc(int numBytes);/* File */FILE *fopen(char *fName, const char *how);void fclose(FILE *fPtr);/*int feof(FILE *fPtr);*/void exit(int message);/* Maths */void time(int *ptr); double pow(double base, double indx);double sin(double x);double sqrt(double x);double erf(double x);double log10(double x);int abs(int x);int atoi(char *numStr);double atof(char *numStr);/* string *//*char *strcpy(char *txtPtr, char *mess);*//*int strlen(char *message);*/char *strcpy();int strlen();/* DIG Library *//* best to look in dig documentation for info. */void cdcinit(void);                     /* "set term = kyocera" in csh for  */                                        /* laser printer output */void digstart(char digDevice, FILE *defaultMetaFile, int callDigendOnInterrupt);  void digorigin(float xNewOrigin, float yNewOrigin);void digclearscreen(void);void digscale(float xRange, float yRange, int forceScalesEqual);void persp(float *contents, int xPoints, int yPoints, float *param, int mode, char *xTitle, char *yTitle, char *zTitle, char *graphTitle); void digend(void);