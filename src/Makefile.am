## Directories
bindir=${prefix}/bin

## Libraries
lib_LTLIBRARIES= liblasf.la
liblasf_la_SOURCES = lasf.c lasfhdr.c lasfpnt.c lasf.h

## C Programs
bin_PROGRAMS = lasdump lascopy
#las2grd las2nc lasproject
lasdump_SOURCES = lasdump.c
lasdump_LDADD = liblasf.la
lascopy_SOURCES = lascopy.c
lascopy_LDADD = liblasf.la
