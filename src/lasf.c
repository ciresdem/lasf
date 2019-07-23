/*-------------------------------------------------------------------
 *
 * lasf.c - some generic functions for LAS files
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
 *-------------------------------------------------------------------*/

#include "lasf.h"

/* 
 *LAS-file-p -- returns 0 if the input file 
 * appears to be an LAS file 
 */
int
lasf_lasf_p(const char *fn) {
  FILE *fp;
  int status;
  fp = fopen(fn, "rb");
  char lasf[4];

  if (fp) {
    fseek(fp, 0, SEEK_SET);
    fread(&lasf, sizeof(lasf), 1, fp);
    fclose(fp);
    if (lasf[0] == 'L' && lasf[1] == 'A' && lasf[2] == 'S' && lasf[3] == 'F') {
      return lasf_NOERR;
    }
    else {
      lasf_file_status(&status, "Input file does not appear to be a valid LAS file: ", fn);
      return status;
    }
  }
  else {
    lasf_file_status(&status, "Failed to open file:", fn);
    return status;
  }
}

int
lasf_open(const char *fn, int* lasfid, int omode) {
  lasf_id lfid;
  FILE *fp;
  int status;
  unsigned char vermajor;
  unsigned char verminor;
  
  // READ
  if (omode == 0) {
    fp = fopen(fn, "rb");
  }
  else if (omode == 1) {
    fp = fopen(fn, "wb");
  }
  else if (omode == 2) {
    fp = fopen(fn, "r+b");
  }

  if (fp) {
    // Read some info from the LAS file
    fseek(fp, 24, SEEK_SET);
    fread(&vermajor, sizeof(verminor), 1, fp);
    fread(&verminor, sizeof(verminor), 1, fp);
    rewind(fp);
    
    // Set the lasf-id values
    lfid.id = lasfid_array_cnt;
    lfid.lasfn = (char *)fn;
    lfid.vermajor = vermajor;
    lfid.verminor = verminor;
    lfid.status = "open";
    lfid.las_file = fp;
    
    // Set the lasfid
    *lasfid = lasfid_array_cnt;
    lasfid_array[lasfid_array_cnt] = lfid;
    lasfid_array_cnt++;
    
    return lasf_NOERR;
  }
  else {
    lasf_file_status(&status, "Failed to open file:", fn);
    return status;
  }
}

int
lasf_close(int* lasfid) {
  FILE *fp;
  int status;

  fp = lasfid_array[*lasfid].las_file;
  if (fclose(fp) == 0) {
    lasfid_array[*lasfid].status = "closed";
    return lasf_NOERR;
  }
  else {
    lasf_file_status(&status, "Failed to close file:", lasfid_array[*lasfid].lasfn);
    return status;
  }
}

int
lasf_scan(int* lasfid, int* lasfhid, lasf_describe* lasf_stats) {
  lasf_pnts lasf_ptbuff0;
  int i, n, status;
  lasf_describe lasf_stats0;

  i = 0, n = 0;  
  /* Loop through the point records */
  while (lasf_read_point(*lasfid, *lasfhid, n, &lasf_ptbuff0) == lasf_NOERR) {
      lasf_add_to_stats(lasfhid, lasf_ptbuff0, &lasf_stats0, i); 
      i++, n--;
  }
  *lasf_stats = lasf_stats0;
  return lasf_NOERR;
}

void
lasf_file_status(int* status, const char* status_string, const char* lasfn) {
  lasf_status lfstat;
  
  if (lasfstatus_array_cnt == 0) {
    lfstat.id = 0;
    lfstat.status_string = "No Error.";
    lfstat.lasfn = "";
    lasfstatus_array[0] = lfstat;
    lasfstatus_array_cnt++;
  }

  // Set the lasf-status values
  lfstat.id = lasfid_array_cnt;
  lfstat.status_string = (char *)status_string;
  lfstat.lasfn = (char *)lasfn;
  
  // Set the lasfid
  *status = lasfstatus_array_cnt;
  lasfstatus_array[lasfstatus_array_cnt] = lfstat;
  lasfstatus_array_cnt++;  
}

const char*
lasf_strerror(int status) {
  lasf_status lfstat;

  if (lasfstatus_array_cnt == 0) {
    lfstat.id = 0;
    lfstat.status_string = "No Error.";
    lfstat.lasfn = "";
    lasfstatus_array[0] = lfstat;
    lasfstatus_array_cnt++;
  }

  return lasfstatus_array[status].status_string;
}

void
lasf_print_version(const char* command_name, const char* command_version) {
  fprintf(stderr, "%s version %s (%s)\n\
Copyright (C) 2010, 2011, 2012, 2013, 2016, 2017, 2018 Matthew Love \n\
LASFile (lasf) is liscensed under the GPL v.2 or later and is \n\
distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;\n\
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A\n\
PARTICULAR PURPOSE.  See the GNU General Public License for more details.\n\
<http://www.gnu.org/licenses/>\n", command_name, command_version, lasf_VERSION);
  exit (1);
}

int
lasf_print_error(int status, const char* bn) {
  lasf_status lfstat;

  if (lasfstatus_array_cnt == 0) {
    lfstat.id = 0;
    lfstat.status_string = "No Error.";
    lfstat.lasfn = "";
    lasfstatus_array[0] = lfstat;
    lasfstatus_array_cnt++;
  }

  fprintf(stderr, "%s: %s (%s)\n", bn, lasfstatus_array[status].status_string, lasfstatus_array[status].lasfn);

  return lasf_NOERR;
}

void get_string( char *buffer )
{
  char *ptr = buffer;
  while( (*ptr = getchar( )) )
    if( *ptr == '\n' )
      break;
    else if( ptr < buffer + MAXSTRING - 1 )
      ptr++;
  *ptr = '\0';
}

inline void
lasf_printf_progress( int i, int i_total, const char* bn) {
  float ifl, iflt;
  ifl = i;
  iflt = i_total;
  float idiff = ifl/iflt;
  //if (fmod(idiff*100, 1) == 0) {
  fprintf(stderr, "%s: %.0f\%\r", bn, idiff * 100);
  fflush(stderr);
  //}
}
