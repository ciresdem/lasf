/*------------------------------------------------------------------
 *
 * header.c - some generic functions for LAS file headers
 *
 * Copyright (C) 2010-2018 Matthew Love <matthew.love@colorado.edu>
 *
 * This file is liscensed under the GPL v.2 or later and
 * is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * <http://www.gnu.org/licenses/>
 *
 *------------------------------------------------------------------*/

#include "lasf.h"

/* Open Header */

int
lasf_open_header(int* lasfid, int* lasfhid) {
  FILE *fp;
  lasf_header lasf_hbuffer;
  lasf_headerid lfhid;
  unsigned char vermajor;
  unsigned char verminor;
  int versum, status;

  if (lasfid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file loaded from which to open a header", "");
    return status;
  }

  vermajor = lasfid_array[*lasfid].vermajor;
  verminor = lasfid_array[*lasfid].verminor;
  versum = vermajor + verminor;

  fp = lasfid_array[*lasfid].las_file;
  if (!fp) {
    lasf_file_status(&status, "Failed to retrieve LAS file stream from:", lasfid_array[*lasfid].lasfn);
    return status;
  }
  rewind(fp);

  status = fread(&lasf_hbuffer.filesig, sizeof(lasf_hbuffer.filesig), 1, fp);

  // 1.0
  if (versum == 1) {
    lasf_hbuffer.filesrcid = 1;
    status = fread(&lasf_hbuffer.globalenc, sizeof(unsigned int), 1, fp);
  }
  // 1.1
  else if (versum == 2) {
    status = fread(&lasf_hbuffer.filesrcid, sizeof(lasf_hbuffer.filesrcid), 1, fp);
    status = fread(&lasf_hbuffer.globalenc, sizeof(unsigned short), 1, fp);
  }
  // 1.2 & 1.3
  else if (versum == 3 || versum == 4) {
    status = fread( &lasf_hbuffer.filesrcid, sizeof(lasf_hbuffer.filesrcid), 1, fp);
    status = fread( &lasf_hbuffer.globalenc, sizeof(lasf_hbuffer.globalenc), 1, fp);
  }
  // 1.4
  else if (versum == 5) {
    status = fread( &lasf_hbuffer.filesrcid, sizeof(lasf_hbuffer.filesrcid), 1, fp);
    status = fread( &lasf_hbuffer.globalenc, sizeof(lasf_hbuffer.globalenc), 1, fp);
  }

  status = fread(&lasf_hbuffer.guid1, sizeof(lasf_hbuffer.guid1), 1, fp);
  status = fread(&lasf_hbuffer.guid2, sizeof(lasf_hbuffer.guid2), 1, fp);
  status = fread(&lasf_hbuffer.guid3, sizeof(lasf_hbuffer.guid3), 1, fp);
  status = fread(&lasf_hbuffer.guid4, sizeof(lasf_hbuffer.guid4), 1, fp);
  status = fread(&lasf_hbuffer.vermajor, sizeof(lasf_hbuffer.vermajor), 1, fp);
  status = fread(&lasf_hbuffer.verminor, sizeof(lasf_hbuffer.verminor), 1, fp);
  status = fread(&lasf_hbuffer.sysid, sizeof(lasf_hbuffer.sysid), 1, fp);
  status = fread(&lasf_hbuffer.gensoftware, sizeof(lasf_hbuffer.gensoftware), 1, fp);
  status = fread(&lasf_hbuffer.fileday, sizeof(lasf_hbuffer.fileday), 1, fp);
  status = fread(&lasf_hbuffer.fileyear, sizeof(lasf_hbuffer.fileyear), 1, fp);
  status = fread(&lasf_hbuffer.headersize, sizeof(lasf_hbuffer.headersize), 1, fp);
  status = fread(&lasf_hbuffer.offset, sizeof(lasf_hbuffer.offset), 1, fp);
  status = fread(&lasf_hbuffer.numvlrecords,sizeof(lasf_hbuffer.numvlrecords), 1, fp);
  status = fread(&lasf_hbuffer.pointformat, sizeof(lasf_hbuffer.pointformat), 1, fp);
  status = fread(&lasf_hbuffer.pointreclen, sizeof(lasf_hbuffer.pointreclen), 1, fp);
  status = fread(&lasf_hbuffer.numptrecords, sizeof(lasf_hbuffer.numptrecords), 1, fp);
  status = fread(&lasf_hbuffer.numptbyreturn, sizeof(lasf_hbuffer.numptbyreturn), 1, fp);
  status = fread(&lasf_hbuffer.xscale, sizeof(lasf_hbuffer.xscale), 1, fp);
  status = fread(&lasf_hbuffer.yscale, sizeof(lasf_hbuffer.yscale), 1, fp);
  status = fread(&lasf_hbuffer.zscale, sizeof(lasf_hbuffer.zscale), 1, fp);
  status = fread(&lasf_hbuffer.xoffset, sizeof(lasf_hbuffer.xoffset), 1, fp);
  status = fread(&lasf_hbuffer.yoffset, sizeof(lasf_hbuffer.yoffset), 1, fp);
  status = fread(&lasf_hbuffer.zoffset, sizeof(lasf_hbuffer.zoffset), 1, fp);
  status = fread(&lasf_hbuffer.xmax, sizeof(lasf_hbuffer.xmax), 1, fp);
  status = fread(&lasf_hbuffer.xmin, sizeof(lasf_hbuffer.xmin), 1, fp);
  status = fread(&lasf_hbuffer.ymax, sizeof(lasf_hbuffer.ymax), 1, fp);
  status = fread(&lasf_hbuffer.ymin, sizeof(lasf_hbuffer.ymin), 1, fp);
  status = fread(&lasf_hbuffer.zmax, sizeof(lasf_hbuffer.zmax), 1, fp);
  status = fread(&lasf_hbuffer.zmin, sizeof(lasf_hbuffer.zmin), 1, fp);
  // 1.3
  if (versum == 4) {
    status = fread( &lasf_hbuffer.waveform, sizeof(lasf_hbuffer.waveform), 1, fp);
    // 1.4
  } else if (versum == 5) {
    status = fread( &lasf_hbuffer.waveform, sizeof(lasf_hbuffer.waveform), 1, fp);
    status = fread( &lasf_hbuffer.extendedvlr, sizeof(lasf_hbuffer.extendedvlr), 1, fp);
    status = fread( &lasf_hbuffer.numextendedvlr, sizeof(lasf_hbuffer.numextendedvlr), 1, fp);
    status = fread( &lasf_hbuffer.numptrecords14, sizeof(lasf_hbuffer.numptrecords14), 1, fp);
    status = fread( &lasf_hbuffer.numptbyreturn14, sizeof(lasf_hbuffer.numptbyreturn14), 1, fp);
    //lasf_hbuffer.numptrecords = lasf_hbuffer.numptrecords14;
  } else {
    lasf_hbuffer.waveform = 0;
  }
  rewind(fp);

  if (!status) {
    lasf_file_status(&status, "Failed to read header.", "");
    return status;
  }

  lfhid.id = lasfheaderid_array_cnt;
  lfhid.lasf_id = *lasfid;
  lfhid.lasfh = lasf_hbuffer;

  // set the lasfheaderid
  *lasfhid = lasfheaderid_array_cnt;
  lasfheaderid_array[lasfheaderid_array_cnt] = lfhid;
  lasfheaderid_array_cnt++;

  return lasf_NOERR;
}

int
lasf_create_header(int* lasfid, int* lasfhid) {
  //lasf_header lasf_hbuffer;
  lasf_headerid lfhid;

  lfhid.id = lasfheaderid_array_cnt;
  lfhid.lasf_id = *lasfid;
  
  // set the lasfheaderid
  *lasfhid = lasfheaderid_array_cnt;
  lasfheaderid_array[lasfheaderid_array_cnt] = lfhid;
  lasfheaderid_array_cnt++;

  return lasf_NOERR;
}

int
lasf_copy_header(int* lasfhid0, int* lasfhid1) {
  lasfheaderid_array[*lasfhid1].lasfh = lasfheaderid_array[*lasfhid0].lasfh;
  lasf_header lasfh0;

  lasfh0 = lasfheaderid_array[*lasfhid0].lasfh;
  lasfheaderid_array[*lasfhid1].lasfh = lasfh0;

  return lasf_NOERR;
}

/* 
   lasf_set_header_* - These lasf_set_header_* functions set the given 
   lasf_header value, based on the given las file header id.
*/
int
lasf_set_header_filesig(int* lasfhid) {
  lasf_header lasfh;
  int status;
  char lasffs[5] = "LASF\0";
  
  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header loaded.", "");
    return status;
  }
  lasfh = lasfheaderid_array[*lasfhid].lasfh;
  strcpy(lasfh.filesig, lasffs);
  lasfheaderid_array[*lasfhid].lasfh = lasfh;

  return lasf_NOERR;
}

int
lasf_set_header_today_date(int* lasfhid) {
  lasf_header lasfh;
  int status;
  
  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header loaded.", "" );
    return status;
  }

  /* time */
  struct tm *tmptr;
  time_t ct;

  ct = time(NULL);
  tmptr = localtime(&ct);
  lasfh = lasfheaderid_array[*lasfhid].lasfh;
  lasfh.fileday = tmptr->tm_yday;
  lasfh.fileyear = 1900 + tmptr->tm_year;
  lasfheaderid_array[*lasfhid].lasfh = lasfh;

  return lasf_NOERR;
}

/* 
   lasf_put_header_* - These lasf_put_header_* function put the given 
   value into the associated location withing the lasf_header 
   of the given las file header id.
*/

int
lasf_put_header(int* lasfhid, lasf_header lasfh) {
  int status;
  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header loaded.", "");
    return status;
  }
  lasfheaderid_array[*lasfhid].lasfh = lasfh;

  return lasf_NOERR;
}

int
lasf_put_header_vermajor(int* lasfhid, unsigned char hformat) {
  lasf_header lasfh;
  int status;
  
  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header loaded.", "");
    return status;
  }
  lasfh = lasfheaderid_array[*lasfhid].lasfh;
  lasfh.vermajor = hformat;
  lasfheaderid_array[*lasfhid].lasfh = lasfh;

  return lasf_NOERR;
}

int
lasf_put_header_verminor(int* lasfhid, unsigned char hformat) {
  lasf_header lasfh;
  int status;
  
  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header loaded.", "");
    return status;
  }
  lasfh = lasfheaderid_array[*lasfhid].lasfh;

  if (hformat == 0 || hformat == 1 || hformat == 2) {
    lasfh.headersize = 227;
    lasfh.offset = 229;
  }
  else if (hformat == 3) {
    lasfh.headersize = 235;
    lasfh.offset = 237;
  }
  else if (hformat == 4) {
    lasfh.headersize = 375;
    lasfh.offset = 377;
  }
  lasfh.verminor = hformat;
  lasfheaderid_array[*lasfhid].lasfh = lasfh;

  return lasf_NOERR;
}

int
lasf_put_header_sysid(int* lasfhid, const char *hsysid) {
  lasf_header lasfh;
  int status;
  
  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header loaded.", "");
    return status;
    }
  lasfh = lasfheaderid_array[*lasfhid].lasfh;
  strcpy(lasfh.sysid, hsysid);
  lasfheaderid_array[*lasfhid].lasfh = lasfh;

  return lasf_NOERR;
}

int
lasf_put_header_gensoftware(int* lasfhid, const char *hgensoft) {
  lasf_header lasfh;
  int status;

  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header loaded.", "");
    return status;
  }
  lasfh = lasfheaderid_array[*lasfhid].lasfh;
  strcpy(lasfh.gensoftware, hgensoft);
  lasfheaderid_array[*lasfhid].lasfh = lasfh;

  return lasf_NOERR;
}

int
lasf_put_header_fileday(int* lasfhid, unsigned short hday) {
  lasf_header lasfh;
  int status;

  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header loaded.", "");
    return status;
  }
  lasfh = lasfheaderid_array[*lasfhid].lasfh;
  lasfh.fileday = hday;
  lasfheaderid_array[*lasfhid].lasfh = lasfh;

  return lasf_NOERR;
}

int
lasf_put_header_fileyear(int* lasfhid, unsigned short hyear) {
  lasf_header lasfh;
  int status;

  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header loaded.", "");
    return status;
  }
  lasfh = lasfheaderid_array[*lasfhid].lasfh;
  lasfh.fileyear = hyear;
  lasfheaderid_array[*lasfhid].lasfh = lasfh;

  return lasf_NOERR;
}

int
lasf_put_header_offset(int* lasfhid, unsigned int hoffset) {
  lasf_header lasfh;
  int status;

  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header loaded.", "");
    return status;
  }
  lasfh = lasfheaderid_array[*lasfhid].lasfh;
  lasfh.offset = hoffset;
  lasfheaderid_array[*lasfhid].lasfh = lasfh;

  return lasf_NOERR;
}

int
lasf_put_header_pointformat(int* lasfhid, unsigned char pointformat) {
  lasf_header lasfh;
  unsigned short preclen;
  int status;

  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header loaded.", "");
    return status;
  }
  lasfh = lasfheaderid_array[*lasfhid].lasfh;
  lasfh.pointformat = pointformat;

  if (pointformat == 0) {
    preclen = 20;
  }
  else if (pointformat == 1) {
    preclen = 28;
  }
  else if (pointformat == 2) {
    preclen = 26;
  }
  else if (pointformat == 3) {
    preclen = 34;
  }
  else if (pointformat == 4) {
    preclen = 57;
  }
  else if (pointformat == 5) {
    preclen = 63;
  }
  else if (pointformat == 6) {
    preclen = 30;
  }
  else if (pointformat == 7) {
    preclen = 36;
  }
  else if (pointformat == 8) {
    preclen = 38;
  }
  else if (pointformat == 9) {
    preclen = 59;
  }
  else if (pointformat == 10) {
    preclen = 67;
  }
  lasfh.pointreclen = preclen;
  lasfheaderid_array[*lasfhid].lasfh = lasfh;

  return lasf_NOERR;
}

int
lasf_put_header_pointreclen(int* lasfhid, unsigned short pointreclen) {
  lasf_header lasfh;
  int status;

  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header loaded.", "");
    return status;
  }
  lasfh = lasfheaderid_array[*lasfhid].lasfh;
  lasfh.pointreclen = pointreclen;
  lasfheaderid_array[*lasfhid].lasfh = lasfh;

  return lasf_NOERR;
}

int
lasf_put_header_numvlrecords(int* lasfhid, unsigned int hnumvlrecords) {
  lasf_header lasfh;
  int status;

  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header loaded.", "");
    return status;
  }
  lasfh = lasfheaderid_array[*lasfhid].lasfh;
  lasfh.numvlrecords = hnumvlrecords;
  lasfheaderid_array[*lasfhid].lasfh = lasfh;

  return lasf_NOERR;
}

int
lasf_put_header_numptrecords(int* lasfhid, unsigned int hnumptrecords) {
  lasf_header lasfh;
  int status;

  if (lasfheaderid_array_cnt == 0) {
      lasf_file_status(&status, "There is currently no valid LAS file header loaded.", "");
      return status;
    }
  lasfh = lasfheaderid_array[*lasfhid].lasfh;
  lasfh.numptrecords = hnumptrecords;
  lasfheaderid_array[*lasfhid].lasfh = lasfh;

  return lasf_NOERR;
}

int
lasf_put_header_numptrecords14(int* lasfhid, unsigned int hnumptrecords) {
  lasf_header lasfh;
  int status;

  if (lasfheaderid_array_cnt == 0) {
      lasf_file_status(&status, "There is currently no valid LAS file header loaded.", "");
      return status;
    }
  lasfh = lasfheaderid_array[*lasfhid].lasfh;
  lasfh.numptrecords14 = hnumptrecords;
  lasfheaderid_array[*lasfhid].lasfh = lasfh;

  return lasf_NOERR;
}

int
lasf_put_header_numptbyreturn(int* lasfhid, unsigned int npbr[5]) {
  lasf_header lasfh;
  unsigned int i;
  int status;

  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header loaded.", "");
    return status;
  }
  lasfh = lasfheaderid_array[*lasfhid].lasfh;
  	      
  for (i = 0; i < 5 ; i++) {
    lasfh.numptbyreturn[i] = npbr[i];
  }
  lasfheaderid_array[*lasfhid].lasfh = lasfh;

  return lasf_NOERR;
}

int
lasf_put_header_numptbyreturn14(int* lasfhid, unsigned int npbr[15]) {
  lasf_header lasfh;
  unsigned int i;
  int status;

  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header loaded.", "");
    return status;
  }
  lasfh = lasfheaderid_array[*lasfhid].lasfh;

  for (i = 0; i < 15 ; i++) {
    lasfh.numptbyreturn14[i] = npbr[i];
  }
  lasfheaderid_array[*lasfhid].lasfh = lasfh;

  return lasf_NOERR;
}

int
lasf_put_header_xmin(int* lasfhid, double xmin) {
  lasf_header lasfh;
  int status;

  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header loaded.", "");
    return status;
  }
  lasfh = lasfheaderid_array[*lasfhid].lasfh;
  lasfh.xmin = xmin;
  lasfheaderid_array[*lasfhid].lasfh = lasfh;

  return lasf_NOERR;
}

int
lasf_put_header_xmax(int* lasfhid, double xmax) {
  lasf_header lasfh;
  int status;

  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header loaded.", "");
    return status;
  }
  lasfh = lasfheaderid_array[*lasfhid].lasfh;
  lasfh.xmax = xmax;
  lasfheaderid_array[*lasfhid].lasfh = lasfh;

  return lasf_NOERR;
}

int
lasf_put_header_ymin(int* lasfhid, double ymin) {
  lasf_header lasfh;
  int status;

  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header loaded.", "");
    return status;
  }
  lasfh = lasfheaderid_array[*lasfhid].lasfh;
  lasfh.ymin = ymin;
  lasfheaderid_array[*lasfhid].lasfh = lasfh;

  return lasf_NOERR;
}

int
lasf_put_header_ymax(int* lasfhid, double ymax) {
  lasf_header lasfh;
  int status;

  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header loaded.", "");
    return status;
  }
  lasfh = lasfheaderid_array[*lasfhid].lasfh;
  lasfh.ymax = ymax;
  lasfheaderid_array[*lasfhid].lasfh = lasfh;

  return lasf_NOERR;
}

int
lasf_put_header_zmin(int* lasfhid, double zmin) {
  lasf_header lasfh;
  int status;

  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header loaded.", "");
    return status;
  }
  lasfh = lasfheaderid_array[*lasfhid].lasfh;
  lasfh.zmin = zmin;
  lasfheaderid_array[*lasfhid].lasfh = lasfh;

  return lasf_NOERR;
}

int
lasf_put_header_zmax(int* lasfhid, double zmax) {
  lasf_header lasfh;
  int status;

  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header loaded.", "");
    return status;
  }
  lasfh = lasfheaderid_array[*lasfhid].lasfh;
  lasfh.zmax = zmax;
  lasfheaderid_array[*lasfhid].lasfh = lasfh;

  return lasf_NOERR;
}

/* Get Header */

int
lasf_get_header(int* lasfhid, lasf_header* lasfh) {
  int status;
  
  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header loaded.", "");
    return status;
  }
  *lasfh = lasfheaderid_array[*lasfhid].lasfh;

  return lasf_NOERR;
}


/* Write */

int
lasf_write_v0header(int* lasfid, int* lasfhid) {
  FILE *lasout;
  lasf_header lasfh;
  short pads = 2;
  int status;

  lasf_header10 lasfh10;

  if (lasfid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file loaded.", "");
    return status;
  }
  
  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header loaded.", "");
    return status;
  }
  
  lasout = lasfid_array[*lasfid].las_file;
  //rewind( lasout );
  fseek(lasout, 0, SEEK_SET);

  lasfh = lasfheaderid_array[*lasfhid].lasfh;

  fwrite(&lasfh.filesig, sizeof(lasfh10.filesig), 1, lasout);
  fwrite(&lasfh.globalenc, sizeof(lasfh10.reserved), 1, lasout);
  fwrite(&lasfh.guid1, sizeof(lasfh10.guid1), 1, lasout);
  fwrite(&lasfh.guid2, sizeof(lasfh10.guid2), 1, lasout);
  fwrite(&lasfh.guid3, sizeof(lasfh10.guid3), 1, lasout);
  fwrite(&lasfh.guid4, sizeof(lasfh10.guid4), 1, lasout);
  fwrite(&lasfh.vermajor, sizeof(lasfh10.vermajor), 1, lasout);
  fwrite(&lasfh.verminor, sizeof(lasfh10.verminor), 1, lasout);
  fwrite(&lasfh.sysid, sizeof(lasfh10.sysid), 1, lasout);
  fwrite(&lasfh.gensoftware, sizeof(lasfh10.gensoftware), 1, lasout);
  fwrite(&lasfh.fileday, sizeof(lasfh10.fileday), 1, lasout);
  fwrite(&lasfh.fileyear, sizeof(lasfh10.fileyear), 1, lasout);
  fwrite(&lasfh.headersize, sizeof(lasfh10.headersize), 1, lasout);
  fwrite(&lasfh.offset, sizeof(lasfh10.offset), 1, lasout);
  fwrite(&lasfh.numvlrecords,sizeof(lasfh10.numvlrecords), 1, lasout);
  fwrite(&lasfh.pointformat, sizeof(lasfh10.pointformat), 1, lasout);
  fwrite(&lasfh.pointreclen, sizeof(lasfh10.pointreclen), 1, lasout);
  fwrite(&lasfh.numptrecords, sizeof(lasfh10.numptrecords), 1, lasout);
  fwrite(&lasfh.numptbyreturn, sizeof(lasfh10.numptbyreturn), 1, lasout);
  fwrite(&lasfh.xscale, sizeof(lasfh10.xscale), 1, lasout);
  fwrite(&lasfh.yscale, sizeof(lasfh10.yscale), 1, lasout);
  fwrite(&lasfh.zscale, sizeof(lasfh10.zscale), 1, lasout);
  fwrite(&lasfh.xoffset, sizeof(lasfh10.xoffset), 1, lasout);
  fwrite(&lasfh.yoffset, sizeof(lasfh10.yoffset), 1, lasout);
  fwrite(&lasfh.zoffset, sizeof(lasfh10.zoffset), 1, lasout);
  fwrite(&lasfh.xmax, sizeof(lasfh10.xmax), 1, lasout);
  fwrite(&lasfh.xmin, sizeof(lasfh10.xmin), 1, lasout);
  fwrite(&lasfh.ymax, sizeof(lasfh10.ymax), 1, lasout);
  fwrite(&lasfh.ymin, sizeof(lasfh10.ymin), 1, lasout);
  fwrite(&lasfh.zmax, sizeof(lasfh10.zmax), 1, lasout);
  fwrite(&lasfh.zmin, sizeof(lasfh10.zmin), 1, lasout);   

  fwrite(&pads, sizeof(pads), 1, lasout);

  return lasf_NOERR;
}

int
lasf_write_v1header(int* lasfid, int* lasfhid) {
  FILE *lasout;
  lasf_header lasfh;
  short pads = 2;
  int status;

  lasf_header11 lasfh11;

  if (lasfid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file loaded.", "");
    return status;
  }
  
  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header loaded.", "");
    return status;
  }
  
  lasout = lasfid_array[*lasfid].las_file;
  //rewind( lasout );
  fseek(lasout, 0, SEEK_SET);

  lasfh = lasfheaderid_array[*lasfhid].lasfh;

  fwrite(&lasfh.filesig, sizeof(lasfh11.filesig), 1, lasout);
  fwrite(&lasfh.filesrcid, sizeof(lasfh11.filesrcid), 1, lasout);
  fwrite(&lasfh.globalenc, sizeof(lasfh11.reserved), 1, lasout);
  fwrite(&lasfh.guid1, sizeof(lasfh11.guid1), 1, lasout);
  fwrite(&lasfh.guid2, sizeof(lasfh11.guid2), 1, lasout);
  fwrite(&lasfh.guid3, sizeof(lasfh11.guid3), 1, lasout);
  fwrite(&lasfh.guid4, sizeof(lasfh11.guid4), 1, lasout);
  fwrite(&lasfh.vermajor, sizeof(lasfh11.vermajor), 1, lasout);
  fwrite(&lasfh.verminor, sizeof(lasfh11.verminor), 1, lasout);
  fwrite(&lasfh.sysid, sizeof(lasfh11.sysid), 1, lasout);
  fwrite(&lasfh.gensoftware, sizeof(lasfh11.gensoftware), 1, lasout);
  fwrite(&lasfh.fileday, sizeof(lasfh11.fileday), 1, lasout);
  fwrite(&lasfh.fileyear, sizeof(lasfh11.fileyear), 1, lasout);
  fwrite(&lasfh.headersize, sizeof(lasfh11.headersize), 1, lasout);
  fwrite(&lasfh.offset, sizeof(lasfh11.offset), 1, lasout);
  fwrite(&lasfh.numvlrecords,sizeof(lasfh11.numvlrecords), 1, lasout);
  fwrite(&lasfh.pointformat, sizeof(lasfh11.pointformat), 1, lasout);
  fwrite(&lasfh.pointreclen, sizeof(lasfh11.pointreclen), 1, lasout);
  fwrite(&lasfh.numptrecords, sizeof(lasfh11.numptrecords), 1, lasout);
  fwrite(&lasfh.numptbyreturn, sizeof(lasfh11.numptbyreturn), 1, lasout);
  fwrite(&lasfh.xscale, sizeof(lasfh11.xscale), 1, lasout);
  fwrite(&lasfh.yscale, sizeof(lasfh11.yscale), 1, lasout);
  fwrite(&lasfh.zscale, sizeof(lasfh11.zscale), 1, lasout);
  fwrite(&lasfh.xoffset, sizeof(lasfh11.xoffset), 1, lasout);
  fwrite(&lasfh.yoffset, sizeof(lasfh11.yoffset), 1, lasout);
  fwrite(&lasfh.zoffset, sizeof(lasfh11.zoffset), 1, lasout);
  fwrite(&lasfh.xmax, sizeof(lasfh11.xmax), 1, lasout);
  fwrite(&lasfh.xmin, sizeof(lasfh11.xmin), 1, lasout);
  fwrite(&lasfh.ymax, sizeof(lasfh11.ymax), 1, lasout);
  fwrite(&lasfh.ymin, sizeof(lasfh11.ymin), 1, lasout);
  fwrite(&lasfh.zmax, sizeof(lasfh11.zmax), 1, lasout);
  fwrite(&lasfh.zmin, sizeof(lasfh11.zmin), 1, lasout);   
  
  fwrite(&pads, sizeof(pads), 1, lasout);

  return lasf_NOERR;
}

int
lasf_write_v2header(int* lasfid, int* lasfhid) {
  FILE *lasout;
  lasf_header lasfh;
  short pads = 2;
  int status;

  lasf_header12 lasfh12;

  if (lasfid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file loaded.", "");
    return status;
  }
  
  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header loaded.", "");
    return status;
  }
  
  lasout = lasfid_array[*lasfid].las_file;
  //rewind( lasout );
  fseek(lasout, 0, SEEK_SET);

  lasfh = lasfheaderid_array[*lasfhid].lasfh;

  fwrite(&lasfh.filesig, sizeof(lasfh12.filesig), 1, lasout);
  fwrite(&lasfh.filesrcid, sizeof(lasfh12.filesrcid), 1, lasout);
  fwrite(&lasfh.globalenc, sizeof(lasfh12.globalenc), 1, lasout);
  fwrite(&lasfh.guid1, sizeof(lasfh12.guid1), 1, lasout);
  fwrite(&lasfh.guid2, sizeof(lasfh12.guid2), 1, lasout);
  fwrite(&lasfh.guid3, sizeof(lasfh12.guid3), 1, lasout);
  fwrite(&lasfh.guid4, sizeof(lasfh12.guid4), 1, lasout);
  fwrite(&lasfh.vermajor, sizeof(lasfh12.vermajor), 1, lasout);
  fwrite(&lasfh.verminor, sizeof(lasfh12.verminor), 1, lasout);
  fwrite(&lasfh.sysid, sizeof(lasfh12.sysid), 1, lasout);
  fwrite(&lasfh.gensoftware, sizeof(lasfh12.gensoftware), 1, lasout);
  fwrite(&lasfh.fileday, sizeof(lasfh12.fileday), 1, lasout);
  fwrite(&lasfh.fileyear, sizeof(lasfh12.fileyear), 1, lasout);
  fwrite(&lasfh.headersize, sizeof(lasfh12.headersize), 1, lasout);
  fwrite(&lasfh.offset, sizeof(lasfh12.offset), 1, lasout);
  fwrite(&lasfh.numvlrecords,sizeof(lasfh12.numvlrecords), 1, lasout);
  fwrite(&lasfh.pointformat, sizeof(lasfh12.pointformat), 1, lasout);
  fwrite(&lasfh.pointreclen, sizeof(lasfh12.pointreclen), 1, lasout);
  fwrite(&lasfh.numptrecords, sizeof(lasfh12.numptrecords), 1, lasout);
  fwrite(&lasfh.numptbyreturn, sizeof(lasfh12.numptbyreturn), 1, lasout);
  fwrite(&lasfh.xscale, sizeof(lasfh12.xscale), 1, lasout);
  fwrite(&lasfh.yscale, sizeof(lasfh12.yscale), 1, lasout);
  fwrite(&lasfh.zscale, sizeof(lasfh12.zscale), 1, lasout);
  fwrite(&lasfh.xoffset, sizeof(lasfh12.xoffset), 1, lasout);
  fwrite(&lasfh.yoffset, sizeof(lasfh12.yoffset), 1, lasout);
  fwrite(&lasfh.zoffset, sizeof(lasfh12.zoffset), 1, lasout);
  fwrite(&lasfh.xmax, sizeof(lasfh12.xmax), 1, lasout);
  fwrite(&lasfh.xmin, sizeof(lasfh12.xmin), 1, lasout);
  fwrite(&lasfh.ymax, sizeof(lasfh12.ymax), 1, lasout);
  fwrite(&lasfh.ymin, sizeof(lasfh12.ymin), 1, lasout);
  fwrite(&lasfh.zmax, sizeof(lasfh12.zmax), 1, lasout);
  fwrite(&lasfh.zmin, sizeof(lasfh12.zmin), 1, lasout);
  //fwrite(&lasfh12, sizeof(lasfh12), 1, lasout);      
  
  fwrite(&pads, sizeof(pads), 1, lasout);

  return lasf_NOERR;
}

int
lasf_write_v3header(int* lasfid, int* lasfhid) {
  FILE *lasout;
  lasf_header lasfh;
  short pads = 2;
  int status;

  lasf_header13 lasfh13;

  if (lasfid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file loaded.", "");
    return status;
  }
  
  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header loaded.", "");
    return status;
  }
  
  lasout = lasfid_array[*lasfid].las_file;
  //rewind( lasout );
  fseek(lasout, 0, SEEK_SET);

  lasfh = lasfheaderid_array[*lasfhid].lasfh;

  fwrite(&lasfh.filesig, sizeof(lasfh13.filesig), 1, lasout);
  fwrite(&lasfh.filesrcid, sizeof(lasfh13.filesrcid), 1, lasout);
  fwrite(&lasfh.globalenc, sizeof(lasfh13.globalenc), 1, lasout);
  fwrite(&lasfh.guid1, sizeof(lasfh13.guid1), 1, lasout);
  fwrite(&lasfh.guid2, sizeof(lasfh13.guid2), 1, lasout);
  fwrite(&lasfh.guid3, sizeof(lasfh13.guid3), 1, lasout);
  fwrite(&lasfh.guid4, sizeof(lasfh13.guid4), 1, lasout);
  fwrite(&lasfh.vermajor, sizeof(lasfh13.vermajor), 1, lasout);
  fwrite(&lasfh.verminor, sizeof(lasfh13.verminor), 1, lasout);
  fwrite(&lasfh.sysid, sizeof(lasfh13.sysid), 1, lasout);
  fwrite(&lasfh.gensoftware, sizeof(lasfh13.gensoftware), 1, lasout);
  fwrite(&lasfh.fileday, sizeof(lasfh13.fileday), 1, lasout);
  fwrite(&lasfh.fileyear, sizeof(lasfh13.fileyear), 1, lasout);
  fwrite(&lasfh.headersize, sizeof(lasfh13.headersize), 1, lasout);
  fwrite(&lasfh.offset, sizeof(lasfh13.offset), 1, lasout);
  fwrite(&lasfh.numvlrecords,sizeof(lasfh13.numvlrecords), 1, lasout);
  fwrite(&lasfh.pointformat, sizeof(lasfh13.pointformat), 1, lasout);
  fwrite(&lasfh.pointreclen, sizeof(lasfh13.pointreclen), 1, lasout);
  fwrite(&lasfh.numptrecords, sizeof(lasfh13.numptrecords), 1, lasout);
  fwrite(&lasfh.numptbyreturn, sizeof(lasfh13.numptbyreturn), 1, lasout);
  fwrite(&lasfh.xscale, sizeof(lasfh13.xscale), 1, lasout);
  fwrite(&lasfh.yscale, sizeof(lasfh13.yscale), 1, lasout);
  fwrite(&lasfh.zscale, sizeof(lasfh13.zscale), 1, lasout);
  fwrite(&lasfh.xoffset, sizeof(lasfh13.xoffset), 1, lasout);
  fwrite(&lasfh.yoffset, sizeof(lasfh13.yoffset), 1, lasout);
  fwrite(&lasfh.zoffset, sizeof(lasfh13.zoffset), 1, lasout);
  fwrite(&lasfh.xmax, sizeof(lasfh13.xmax), 1, lasout);
  fwrite(&lasfh.xmin, sizeof(lasfh13.xmin), 1, lasout);
  fwrite(&lasfh.ymax, sizeof(lasfh13.ymax), 1, lasout);
  fwrite(&lasfh.ymin, sizeof(lasfh13.ymin), 1, lasout);
  fwrite(&lasfh.zmax, sizeof(lasfh13.zmax), 1, lasout);
  fwrite(&lasfh.zmin, sizeof(lasfh13.zmin), 1, lasout);   
  fwrite(&lasfh.waveform, sizeof(lasfh13.waveform), 1, lasout);   
  
  fwrite(&pads, sizeof(pads), 1, lasout);

  return lasf_NOERR;
}

int
lasf_write_v4header(int* lasfid, int* lasfhid) {
  FILE *lasout;
  lasf_header lasfh;
  short pads = 2;
  int status;

  lasf_header lasfh14;

  if (lasfid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file loaded.", "");
    return status;
  }
  
  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header loaded.", "");
    return status;
  }
  
  lasout = lasfid_array[*lasfid].las_file;
  //rewind( lasout );
  fseek(lasout, 0, SEEK_SET);

  lasfh = lasfheaderid_array[*lasfhid].lasfh;

  fwrite(&lasfh.filesig, sizeof(lasfh14.filesig), 1, lasout);
  fwrite(&lasfh.filesrcid, sizeof(lasfh14.filesrcid), 1, lasout);
  fwrite(&lasfh.globalenc, sizeof(lasfh14.globalenc), 1, lasout);
  fwrite(&lasfh.guid1, sizeof(lasfh14.guid1), 1, lasout);
  fwrite(&lasfh.guid2, sizeof(lasfh14.guid2), 1, lasout);
  fwrite(&lasfh.guid3, sizeof(lasfh14.guid3), 1, lasout);
  fwrite(&lasfh.guid4, sizeof(lasfh14.guid4), 1, lasout);
  fwrite(&lasfh.vermajor, sizeof(lasfh14.vermajor), 1, lasout);
  fwrite(&lasfh.verminor, sizeof(lasfh14.verminor), 1, lasout);
  fwrite(&lasfh.sysid, sizeof(lasfh14.sysid), 1, lasout);
  fwrite(&lasfh.gensoftware, sizeof(lasfh14.gensoftware), 1, lasout);
  fwrite(&lasfh.fileday, sizeof(lasfh14.fileday), 1, lasout);
  fwrite(&lasfh.fileyear, sizeof(lasfh14.fileyear), 1, lasout);
  fwrite(&lasfh.headersize, sizeof(lasfh14.headersize), 1, lasout);
  fwrite(&lasfh.offset, sizeof(lasfh14.offset), 1, lasout);
  fwrite(&lasfh.numvlrecords,sizeof(lasfh14.numvlrecords), 1, lasout);
  fwrite(&lasfh.pointformat, sizeof(lasfh14.pointformat), 1, lasout);
  fwrite(&lasfh.pointreclen, sizeof(lasfh14.pointreclen), 1, lasout);
  fwrite(&lasfh.numptrecords, sizeof(lasfh14.numptrecords), 1, lasout);
  fwrite(&lasfh.numptbyreturn, sizeof(lasfh14.numptbyreturn), 1, lasout);
  fwrite(&lasfh.xscale, sizeof(lasfh14.xscale), 1, lasout);
  fwrite(&lasfh.yscale, sizeof(lasfh14.yscale), 1, lasout);
  fwrite(&lasfh.zscale, sizeof(lasfh14.zscale), 1, lasout);
  fwrite(&lasfh.xoffset, sizeof(lasfh14.xoffset), 1, lasout);
  fwrite(&lasfh.yoffset, sizeof(lasfh14.yoffset), 1, lasout);
  fwrite(&lasfh.zoffset, sizeof(lasfh14.zoffset), 1, lasout);
  fwrite(&lasfh.xmax, sizeof(lasfh14.xmax), 1, lasout);
  fwrite(&lasfh.xmin, sizeof(lasfh14.xmin), 1, lasout);
  fwrite(&lasfh.ymax, sizeof(lasfh14.ymax), 1, lasout);
  fwrite(&lasfh.ymin, sizeof(lasfh14.ymin), 1, lasout);
  fwrite(&lasfh.zmax, sizeof(lasfh14.zmax), 1, lasout);
  fwrite(&lasfh.zmin, sizeof(lasfh14.zmin), 1, lasout);   
  fwrite(&lasfh.waveform, sizeof(lasfh14.waveform), 1, lasout);
  fwrite(&lasfh.extendedvlr, sizeof(lasfh14.extendedvlr), 1, lasout);
  fwrite(&lasfh.numextendedvlr, sizeof(lasfh14.numextendedvlr), 1, lasout);
  fwrite(&lasfh.numptrecords14, sizeof(lasfh14.numptrecords14), 1, lasout);
  fwrite(&lasfh.numptbyreturn14, sizeof(lasfh14.numptbyreturn14), 1, lasout);
  
  fwrite(&pads, sizeof(pads), 1, lasout);

  return lasf_NOERR;
}

int
lasf_write_header(int* lasfid, int* lasfhid) {
  FILE *lasout;
  lasf_header lasfh;
  short pads = 2;
  int verminor, status;

  lasf_header10 lasfh10;
  lasf_header11 lasfh11;
  lasf_header12 lasfh12;
  lasf_header13 lasfh13;
  lasf_header lasfh14;

  if (lasfid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file loaded.", "");
    return status;
  }

  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header loaded.", "");
    return status;
  }
  lasout = lasfid_array[*lasfid].las_file;
  //rewind( lasout );
  fseek(lasout, 0, SEEK_SET);

  lasfh = lasfheaderid_array[*lasfhid].lasfh;
  verminor = lasfh.verminor;

  if (verminor == 0) {
    lasf_write_v0header(lasfid, lasfhid);
  }
  else if (verminor == 1) {
    lasf_write_v1header(lasfid, lasfhid);
  }
  else if (verminor == 2) {
    lasf_write_v2header(lasfid, lasfhid);
  }
  else if (verminor == 3) {
    lasf_write_v3header(lasfid, lasfhid);
  }
  else if (verminor == 4) {
    lasf_write_v4header(lasfid, lasfhid);
  }
  
  fwrite(&pads, sizeof(pads), 1, lasout);

  return lasf_NOERR;
}

/*
  Variable Length Headers 
*/
int
lasf_open_vlheader(int* lasfid, int* lasfhid, int* lasfvlhid, int seekn) {
  FILE *fp;
  lasf_header lasf_hbuffer;
  lasf_vlheader lasf_vlhbuffer;
  lasf_vlheaderid lfvlhid;
  unsigned char vermajor;
  unsigned char verminor;
  int versum, status, cseekn, i;
  
  if (lasfid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file loaded from which to open a header", "");
    return status;
  }

  unsigned short vlreclen;

  lasf_get_header(lasfhid,  &lasf_hbuffer);
  
  vermajor = lasfid_array[*lasfid].vermajor;
  verminor = lasfid_array[*lasfid].verminor;
  versum = vermajor + verminor;
  fp = lasfid_array[*lasfid].las_file;

  if (! fp) {
    lasf_file_status(&status, "Failed to retrieve LAS file stream from:", lasfid_array[*lasfid].lasfn);
    return status;
  }
  
  if (lasf_hbuffer.numvlrecords == 0 || seekn > lasf_hbuffer.numvlrecords) {
    lasf_file_status(&status, "There dont appear to be any variable length headers here.", lasfid_array[*lasfid].lasfn);
    return status;
  }
  
  vlreclen = 0;
  cseekn = lasf_hbuffer.headersize;
  
  for (i = 0; i < seekn + 1; i++) {
    if (i != 0) {
      cseekn = cseekn + vlreclen + 54;
    }
    fseek(fp, cseekn + 20, SEEK_SET);
    fread(&vlreclen, sizeof(vlreclen), 1, fp);
    rewind(fp);
  }
  
  fseek(fp, cseekn , SEEK_SET);

  status = fread(&lasf_vlhbuffer.reserved, sizeof(lasf_vlhbuffer.reserved), 1, fp);
  status = fread(&lasf_vlhbuffer.userid, sizeof(lasf_vlhbuffer.userid), 1, fp);
  status = fread(&lasf_vlhbuffer.recordid, sizeof(lasf_vlhbuffer.recordid), 1, fp);
  status = fread(&lasf_vlhbuffer.recordlen, sizeof(lasf_vlhbuffer.recordlen), 1, fp);
  status = fread(&lasf_vlhbuffer.description, sizeof(lasf_vlhbuffer.description), 1, fp);

  // TODO don't print this here, send to variable.
  char vlrec[vlreclen];
  status = fread(&vlrec, sizeof(vlrec), 1, fp);
  printf("----\n");
  printf("%s\n",vlrec);
  printf("----\n");
  
  rewind(fp);

  if (!status) {
    lasf_file_status(&status, "Failed to read vl header.", "");
    return status;
  }

  lfvlhid.id = lasfvlheaderid_array_cnt;
  lfvlhid.lasf_id = *lasfid;
  lfvlhid.lasf_hid = *lasfhid;
  lfvlhid.lasfvlh = lasf_vlhbuffer;

  // set the lasfvlheaderid
  *lasfvlhid = lasfvlheaderid_array_cnt;
  lasfvlheaderid_array[lasfvlheaderid_array_cnt] = lfvlhid;
  lasfvlheaderid_array_cnt++;

  return lasf_NOERR;
}

int
lasf_get_vlheader(int* lasfvlhid, lasf_vlheader* lasfvlh)
{
  int status;
  
  if (lasfvlheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header loaded.", "");
    return status;
  }
  *lasfvlh = lasfvlheaderid_array[*lasfvlhid].lasfvlh;

  return lasf_NOERR;
}

/* int */
/* lasf_print_vldata(int* lasfvlhid) */
/* { */
/*   int status; */
/*   lasf_vlheader lasfvlh; */

/*   if (lasfvlheaderid_array_cnt == 0) { */
/*     lasf_file_status(&status, "There is currently no valid LAS file header loaded.", ""); */
/*     return status; */
/*   } */
/*   lasfvlh = lasfvlheaderid_array[*lasfvlhid].lasfvlh; */
/*   lasfvlh.recordlen; */
/*   lasfh = lasfheaderid_array[lasfvlheaderid_array[*lasfvlhid].lasf_hid].lasf_header; */
/*   lasffp = lasfid_array[lasfvlheaderid_array[*lasfvlhid].lasf_id].las_file; */
/* } */

int
lasf_read_byte_write_byte(int* lasfid, int* lasfid_out, int bn) {
  FILE *fp;
  FILE *fo;
  int i;
  char inb;

  fp = lasfid_array[*lasfid].las_file;
  fo = lasfid_array[*lasfid_out].las_file;

  for (i = 0; i < bn; i++) {
    fread(&bn, sizeof(bn), 1, fp);
    fwrite(&bn, sizeof(bn), 1, fo);
  }
}
