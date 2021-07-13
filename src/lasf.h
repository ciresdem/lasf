/* ---------------------------------------------------------------
 *
 * LASFile
 *
 * lasf.h
 *
 * Copyright (C) 2010, 2011, 2012, 2013, 2016, 2017, 2018 Matthew Love
 *
 * This file is liscensed under the GPL v.2 or later and
 * is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * <http://www.gnu.org/licenses/>
 *
 * --------------------------------------------------------------*/

#ifndef LASF_H
#define LASF_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<time.h>
#include<math.h>
#include<float.h>
#include<limits.h>

#define lasf_VERMAJOR 1
#define lasf_VERMINOR 4
#define lasf_VERMICRO 3
#define lasf_VERSION "lasf-1.4.3"

/*
 * Max Array Sizes
 * Adjust values here if needed
 */
#define MAXSTRING (100 * 1024 * 1024)
#define MAX_LAS_FILES 5000
#define MAX_HEADERS MAX_LAS_FILES
#define MAX_VL_HEADERS MAX_LAS_FILES
#define MAX_STATUS MAX_LAS_FILES

#define MAX_LEGACY_POINTS UNIT32_MAX

#ifdef NAN
#define lasf_NAN  NAN
#else
#define lasf_NAN -99999
#endif

#define lasf_NOERR 0

/*
  HEADER STRUCTS
*/

// Version struct; holds the vermajor and verminor values
typedef struct {
  unsigned char vermajor;            /* Major LAS File version number */
  unsigned char verminor;            /* Minor LAS File version number */
}lasf_version;

// The LAS Header version 1.0
typedef struct {
  char filesig[4];                   /* LAS File Signature, should be 'lasf' for a valid LAS File. */
  unsigned int reserved;
  unsigned int guid1;
  unsigned short guid2;
  unsigned short guid3;
  unsigned char guid4[8];
  unsigned char vermajor;            /* Major LAS File version number */
  unsigned char verminor;            /* Minor LAS File version number */
  char sysid[32];
  char gensoftware[32];              /* Generating software string. */
  unsigned short fileday;            /* Day the file was created */
  unsigned short fileyear;           /* Year the file was created */ 
  unsigned short headersize;         /* Size in bytes of the header */
  unsigned int offset;              /* Offset in bytes to the point records */
  unsigned int numvlrecords;        /* Number of Variale length records*/
  unsigned char pointformat;         /* Point-record format */
  unsigned short pointreclen;        /* Point-record length in bytes*/
  unsigned int numptrecords;        /* Total number of point-records*/
  unsigned int numptbyreturn[5];    /* Number of points by return */
  double xscale;
  double yscale;
  double zscale;
  double xoffset;
  double yoffset;
  double zoffset;
  double xmax;
  double xmin;
  double ymax;
  double ymin;
  double zmax;
  double zmin;
} lasf_header10;

// The LAS Header version 1.1
typedef struct {
  char filesig[4];                   /* LAS File Signature, should be 'lasf' for a valid LAS File. */
  unsigned short filesrcid;
  unsigned short reserved;          /* Global Encoding - Reserved in 1.1 - same byte count */
  unsigned int guid1;
  unsigned short guid2;
  unsigned short guid3;
  unsigned char guid4[8];
  unsigned char vermajor;            /* Major LAS File version number */
  unsigned char verminor;            /* Minor LAS File version number */
  char sysid[32];
  char gensoftware[32];              /* Generating software string. */
  unsigned short fileday;            /* Day the file was created */
  unsigned short fileyear;           /* Year the file was created */ 
  unsigned short headersize;         /* Size in bytes of the header */
  unsigned int offset;              /* Offset in bytes to the point records */
  unsigned int numvlrecords;        /* Number of Variale length records*/
  unsigned char pointformat;         /* Point-record format */
  unsigned short pointreclen;        /* Point-record length in bytes*/
  unsigned int numptrecords;        /* Total number of point-records*/
  unsigned int numptbyreturn[5];    /* Number of points by return */
  double xscale;
  double yscale;
  double zscale;
  double xoffset;
  double yoffset;
  double zoffset;
  double xmax;
  double xmin;
  double ymax;
  double ymin;
  double zmax;
  double zmin;
} lasf_header11;

// The LAS Header version 1.2
typedef struct {
  char filesig[4];                   /* LAS File Signature, should be 'lasf' for a valid LAS File. */
  unsigned short filesrcid;
  unsigned short globalenc;          /* Global Encoding - Reserved in 1.1 - same byte count */
  unsigned int guid1;
  unsigned short guid2;
  unsigned short guid3;
  unsigned char guid4[8];
  unsigned char vermajor;            /* Major LAS File version number */
  unsigned char verminor;            /* Minor LAS File version number */
  char sysid[32];
  char gensoftware[32];              /* Generating software string. */
  unsigned short fileday;            /* Day the file was created */
  unsigned short fileyear;           /* Year the file was created */ 
  unsigned short headersize;         /* Size in bytes of the header */
  unsigned int offset;              /* Offset in bytes to the point records */
  unsigned int numvlrecords;        /* Number of Variale length records*/
  unsigned char pointformat;         /* Point-record format */
  unsigned short pointreclen;        /* Point-record length in bytes*/
  unsigned int numptrecords;        /* Total number of point-records*/
  unsigned int numptbyreturn[5];    /* Number of points by return */
  double xscale;
  double yscale;
  double zscale;
  double xoffset;
  double yoffset;
  double zoffset;
  double xmax;
  double xmin;
  double ymax;
  double ymin;
  double zmax;
  double zmin;
} lasf_header12;

// The lasf Default header (currently version 1.3)
typedef struct {
  char filesig[4];                   /* LAS File Signature, should be 'LASF' for a valid LAS File. */
  unsigned short filesrcid;
  unsigned short globalenc;          /* Global Encoding - Reserved in 1.1 - same byte count */
  unsigned int guid1;
  unsigned short guid2;
  unsigned short guid3;
  unsigned char guid4[8];
  unsigned char vermajor;            /* Major LAS File version number */
  unsigned char verminor;            /* Minor LAS File version number */
  char sysid[32];
  char gensoftware[32];              /* Generating software string. */
  unsigned short fileday;            /* Day the file was created */
  unsigned short fileyear;           /* Year the file was created */ 
  unsigned short headersize;         /* Size in bytes of the header */
  unsigned int offset;              /* Offset in bytes to the point records */
  unsigned int numvlrecords;        /* Number of Variale length records*/
  unsigned char pointformat;         /* Point-record format */
  unsigned short pointreclen;        /* Point-record length in bytes*/
  unsigned int numptrecords;        /* Total number of point-records*/
  unsigned int numptbyreturn[5];    /* Number of points by return */
  double xscale;
  double yscale;
  double zscale;
  double xoffset;
  double yoffset;
  double zoffset;
  double xmax;
  double xmin;
  double ymax;
  double ymin;
  double zmax;
  double zmin;
  unsigned long long waveform;
} lasf_header13;

// The LAS Header version 1.4
typedef struct {
  char filesig[4];                   /* LAS File Signature, should be 'LASF' for a valid LAS File. */
  unsigned short filesrcid;
  unsigned short globalenc;          /* Global Encoding - Reserved in 1.1 - same byte count */
  unsigned int guid1;
  unsigned short guid2;
  unsigned short guid3;
  unsigned char guid4[8];
  unsigned char vermajor;            /* Major LAS File version number */
  unsigned char verminor;            /* Minor LAS File version number */
  char sysid[32];
  char gensoftware[32];              /* Generating software string. */
  unsigned short fileday;            /* Day the file was created */
  unsigned short fileyear;           /* Year the file was created */ 
  unsigned short headersize;         /* Size in bytes of the header */
  unsigned int offset;              /* Offset in bytes to the point records */
  unsigned int numvlrecords;        /* Number of Variale length records*/
  unsigned char pointformat;         /* Point-record format */
  unsigned short pointreclen;        /* Point-record length in bytes*/
  unsigned int numptrecords;        /* Total number of point-records*/
  unsigned int numptbyreturn[5];    /* Number of points by return */
  double xscale;
  double yscale;
  double zscale;
  double xoffset;
  double yoffset;
  double zoffset;
  double xmax;
  double xmin;
  double ymax;
  double ymin;
  double zmax;
  double zmin;
  unsigned long int waveform;
  unsigned long int extendedvlr;
  unsigned int numextendedvlr;
  unsigned long int numptrecords14;
  unsigned long int numptbyreturn14[15];
} lasf_header;

/*
Variable Length Headers
*/

// The LAS Variable Length Header version 1.0
typedef struct {
  unsigned short recordsig;
  char userid[16];
  unsigned short recordid;
  unsigned short recordlen;
  char description[32];
}lasf_vlheader10;

// The LAS Variable Length Header version 1.1
typedef struct {
  unsigned short reserved;
  char userid[16];
  unsigned short recordid;
  unsigned short recordlen;
  char description[32];
}lasf_vlheader11;

// The LAS Variable Length Header version 1.2
typedef struct {
  unsigned short reserved;
  char userid[16];
  unsigned short recordid;
  unsigned short recordlen;
  char description[32];
}lasf_vlheader12;

// The LAS Variable Length Header version 1.3
typedef struct {
  unsigned short reserved;
  char userid[16];
  unsigned short recordid;
  unsigned short recordlen;
  char description[32];
}lasf_vlheader;

/*
POINT STRUCTS
*/

// The LAS Point Structure version 0
typedef struct {
  int x;
  int y;
  int z;
  unsigned short intensity;
  unsigned char returns;
  unsigned char classification;
  char scanrank;
  unsigned char udata;
  unsigned short psrcid;
} lasf_pnts0;

// The LAS Point Structure version 1
typedef struct {
  int x;
  int y;
  int z;
  unsigned short intensity;
  unsigned char returns;
  unsigned char classification;
  char scanrank;
  unsigned char udata;
  unsigned short psrcid;
  double gpstime;
} lasf_pnts1;

// The LAS Point Structure version 2
typedef struct {
  int x;
  int y;
  int z;
  unsigned short intensity;
  unsigned char returns;
  unsigned char classification;
  char scanrank;
  unsigned char udata;
  unsigned short psrcid;
  unsigned short red;
  unsigned short green;
  unsigned short blue;
} lasf_pnts2;

// The LAS Point Structure version 3
typedef struct {
  int x;
  int y;
  int z;
  unsigned short intensity;
  unsigned char returns;
  unsigned char classification;
  char scanrank;
  unsigned char udata;
  unsigned short psrcid;
  double gpstime;
  unsigned short red;
  unsigned short green;
  unsigned short blue;
} lasf_pnts3;

// The LAS Point Structure version 4
typedef struct {
  int x;
  int y;
  int z;
  unsigned short intensity;
  unsigned char returns;
  unsigned char classification;
  char scanrank;
  unsigned char udata;
  unsigned short psrcid;
  double gpstime;
  unsigned char wavedesc;
  unsigned long int waveoffset;
  unsigned int wavepacksize;
  float wavereturn;
  float x_t;
  float y_t;
  float z_t;
} lasf_pnts4;

// The LAS Point Structure version 5
typedef struct {
  int x;
  int y;
  int z;
  unsigned short intensity;
  unsigned char returns;
  unsigned char classification;
  char scanrank;
  unsigned char udata;
  unsigned short psrcid;
  double gpstime;
  unsigned short red;
  unsigned short green;
  unsigned short blue;
  unsigned char wavedesc;
  unsigned long int waveoffset;
  unsigned int wavepacksize;
  float wavereturn;
  float x_t;
  float y_t;
  float z_t;
} lasf_pnts5;


// The LAS Point Structure version 6
typedef struct {
  int x;
  int y;
  int z;
  unsigned short intensity;
  unsigned short returns;
  unsigned char classification;
  unsigned char udata;
  short scanrank;
  unsigned short psrcid;
  double gpstime;
} lasf_pnts6;

// The LAS Point Structure version 7
typedef struct {
  int x;
  int y;
  int z;
  unsigned short intensity;
  unsigned short returns;
  unsigned char classification;
  unsigned char udata;
  short scanrank;
  unsigned short psrcid;
  double gpstime;
  unsigned short red;
  unsigned short green;
  unsigned short blue;
} lasf_pnts7;

// The LAS Point Structure version 8
typedef struct {
  int x;
  int y;
  int z;
  unsigned short intensity;
  unsigned short returns;
  unsigned char classification;
  unsigned char udata;
  short scanrank;
  unsigned short psrcid;
  double gpstime;
  unsigned short red;
  unsigned short green;
  unsigned short blue;
  unsigned short NIR;
} lasf_pnts8;

// The LAS Point Structure version 9
typedef struct {
  int x;
  int y;
  int z;
  unsigned short intensity;
  unsigned short returns;
  unsigned char classification;
  unsigned char udata;
  short scanrank;
  unsigned short psrcid;
  double gpstime;
  unsigned char wavedesc;
  unsigned long int waveoffset;
  unsigned int wavepacksize;
  float wavereturn;
  float x_t;
  float y_t;
  float z_t;
} lasf_pnts9;

// The LAS Point Structure version 10
typedef struct {
  int x;
  int y;
  int z;
  unsigned short intensity;
  unsigned short returns;
  unsigned char classification;
  unsigned char udata;
  short scanrank;
  unsigned short psrcid;
  double gpstime;
  unsigned short red;
  unsigned short green;
  unsigned short blue;
  unsigned short NIR;
  unsigned char wavedesc;
  unsigned long int waveoffset;
  unsigned int wavepacksize;
  float wavereturn;
  float x_t;
  float y_t;
  float z_t;
} lasf_pnts;

// lasf Structs

typedef struct {
  int return_num;
  int num_return;
  int class_flags;
  int scan_chan;
  int scan_dir;
  int flight_edge;
} lasf_pntreturns;

typedef struct {
  int classification;
  int synthetic;
  int keypoint;
  int withheld;
}lasf_pntclass;

typedef struct {
  unsigned int gpstime_flag;
}lasf_globalenc;

// minmax struct
typedef struct {
  int n;
  double xmin;
  double xmax;
  double ymin;
  double ymax;
  double zmin;
  double zmax;
  unsigned short intensitymin;
  unsigned short intensitymax;
  double gpstimemin;
  double gpstimemax;
  unsigned int classifications[33];
  unsigned int returns[5];
  unsigned int returns6[15];
} lasf_describe;

typedef struct {
  int id; // ID to locate struct.
  char *lasfn; // file name
  int vermajor; // LAS Major Version
  int verminor; // LAS Minor Version
  char *status;
  FILE *las_file;
}lasf_id;

extern lasf_id lasfid_array[MAX_LAS_FILES];
extern int lasfid_array_cnt;

typedef struct {
  int id; // ID to locate struct.
  int lasf_id; // ID to the associated LAS File.
  lasf_header lasfh;
}lasf_headerid;

extern lasf_headerid lasfheaderid_array[MAX_HEADERS];
extern int lasfheaderid_array_cnt;

typedef struct {
  int id; // ID to locate struct.
  int lasf_id; // ID to the associated LAS File.
  int lasf_hid; // ID to the associated LAS File header.
  int data_offset; // Offset to associated data
  lasf_vlheader lasfvlh;
}lasf_vlheaderid;

extern lasf_vlheaderid lasfvlheaderid_array[MAX_VL_HEADERS];
extern int lasfvlheaderid_array_cnt;

typedef struct {
  int laspid; // ID to locate struct.
}lasf_pointid;

typedef struct {
  size_t id;
  const char* status_string;
  const char* lasfn;
}lasf_status;

extern lasf_status lasfstatus_array[MAX_STATUS];
extern int lasfstatus_array_cnt;

/* 
   Functions
*/

/* LAS-file-p -- returns 0 if the input file 
   appears to be an LAS file */
int
lasf_lasf_p(const char *fn);

// omode = 1 -> read
// omode = 2 -> write
int
lasf_open(const char *fn, int* lasfid, int omode);

int
lasf_close(int* lasfid);

int
lasf_open_header(int* lasfid, int* lasfhid);

int
lasf_create_header(int* lasfid, int* lasfhid);

int
lasf_get_header(int* lasfhid, lasf_header* lasfh);

int
lasf_write_header(int* lasfid, int* lasfhid);

int
lasf_copy_header(int* lasfhid0, int* lasfhid1);

int
lasf_put_header(int* lasfhid, lasf_header lasfh);

int
lasf_put_header_vermajor(int* lasfhid, unsigned char hformat);

int
lasf_put_header_verminor(int* lasfhid, unsigned char hformat);

int
lasf_put_header_sysid(int* lasfhid, const char *hsysid);

int
lasf_put_header_gensoftware(int* lasfhid, const char *hgensoft);

int
lasf_put_header_fileday(int* lasfhid, unsigned short hday);

int
lasf_put_header_fileyear(int* lasfhid, unsigned short hyear);

int
lasf_put_header_offset(int* lasfhid, unsigned int hoffset);

int
lasf_put_header_pointformat(int* lasfhid, unsigned char pointformat);

int
lasf_put_header_pointreclen(int* lasfhid, unsigned short pointreclen);

int
lasf_put_header_numvlrecords(int* lasfhid, unsigned int hnumvlrecords);

int
lasf_put_header_numptrecords(int* lasfhid, unsigned int hnumptrecords);

int
lasf_put_header_numptrecords14(int* lasfhid, unsigned int hnumptrecords);

int
lasf_put_header_numptbyreturn(int* lasfhid, unsigned int npbr[5]);

int
lasf_put_header_numptbyreturn14(int* lasfhid, unsigned int npbr[15]);

int
lasf_put_header_xmin(int* lasfhid, double xmin);

int
lasf_put_header_xmax(int* lasfhid, double xmax);

int
lasf_put_header_ymin(int* lasfhid, double ymin);

int
lasf_put_header_ymax(int* lasfhid, double ymax);

int
lasf_put_header_zmin(int* lasfhid, double zmin);

int
lasf_put_header_zmax(int* lasfhid, double zmax);

int
lasf_set_header_filesig(int* lasfhid);

int
lasf_set_header_today_date(int* lasfhid);

int
lasf_open_vlheader(int* lasfid, int* lasfhid, int* lasfvlhid, int seekn);

int
lasf_get_vlheader(int* lasfvlhid, lasf_vlheader* lasfvlh);

int
lasf_print_vldata(int* lasfvlhid);

int
lasf_read_v0point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff);

int
lasf_read_v1point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff);

int
lasf_read_v2point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff);

int
lasf_read_v3point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff);

int
lasf_read_v4point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff);

int
lasf_read_v5point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff);

int
lasf_read_v6point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff);

int
lasf_read_v7point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff);

int
lasf_read_v8point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff);

int
lasf_read_v9point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff);

int
lasf_read_v10point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff);

size_t
lasf_read_point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff);

int
lasf_write_v0point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff);
  
int
lasf_write_v1point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff);

int
lasf_write_v2point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff);

int
lasf_write_v3point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff);

int
lasf_write_v4point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff);

int
lasf_write_v5point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff);

int
lasf_write_v6point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff);

int
lasf_write_v7point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff);

int
lasf_write_v8point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff);

int
lasf_write_v9point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff);

int
lasf_write_v10point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff);

int
lasf_write_point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff);

lasf_pntreturns
lasf_parse_point_returns(unsigned char pt_rt);

int
lasf_cmpclass(char* lasl_clr, int classification);

int
lasf_add_to_stats(int* lasfhid, lasf_pnts lasf_ptbuff0, lasf_describe* llmm, int init);

int
lasf_add_to_stats(int* lasfhid, lasf_pnts lasf_ptbuff0, lasf_describe* llmm, int init);

int
lasf_scan(int* lasfid, int* lasfhid, lasf_describe* lasf_stats);

void
lasf_file_status(int* status, const char* status_string, const char* lasfn);

int
lasf_print_error(int status, const char* bn);

const char*
lasf_strerror(int status);

void 
lasf_printf_progress(int x, int n, const char* bn);

void
lasf_print_version(const char* command_name, const char* command_version);
#endif
