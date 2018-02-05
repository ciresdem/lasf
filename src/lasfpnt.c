/* ---------------------------------------------------------------
 *
 * point.c - some generic functions for LAS files
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
 * --------------------------------------------------------------*/

#include "lasf.h"

int
lasf_get_points(int* lasfid, int* lasfhid, int* lasfpid) {
  return 1;
}

int lasf_read_v0point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff) {
  int status;
  FILE *fp;
  lasf_header lasf_hbuffer;
  
  lasf_pnts0 lasf_ptbuff0;

  if (lasfid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file to write to", "");
    return status;
  }

  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header open", "");
    return status;
  }
  
  lasf_hbuffer = lasfheaderid_array[ lasfhid ].lasfh;
  fp = lasfid_array[ lasfid ].las_file;

  if (seekn >= 0) {
    seekn = (seekn * lasf_hbuffer.pointreclen) + lasf_hbuffer.offset;
    fseek(fp, seekn, SEEK_SET);
  }
  
  status = fread(&lasf_ptbuff0, lasf_hbuffer.pointreclen, 1, fp);

  lasf_ptbuff->x = lasf_ptbuff0.x;
  lasf_ptbuff->y = lasf_ptbuff0.y;
  lasf_ptbuff->z = lasf_ptbuff0.z;
  lasf_ptbuff->intensity = lasf_ptbuff0.intensity;
  lasf_ptbuff->returns = lasf_ptbuff0.returns;
  lasf_ptbuff->classification = lasf_ptbuff0.classification;
  lasf_ptbuff->scanrank = lasf_ptbuff0.scanrank;
  lasf_ptbuff->udata = lasf_ptbuff0.udata;
  lasf_ptbuff->psrcid = lasf_ptbuff0.psrcid;
  lasf_ptbuff->gpstime = -1;
  lasf_ptbuff->red = 0;
  lasf_ptbuff->green = 0;
  lasf_ptbuff->blue = 0;
  lasf_ptbuff->NIR = 0;
  lasf_ptbuff->wavedesc = 0;
  lasf_ptbuff->waveoffset = 0;
  lasf_ptbuff->wavepacksize = 0;
  lasf_ptbuff->wavereturn = 0;
  lasf_ptbuff->x_t = 0;
  lasf_ptbuff->y_t = 0;
  lasf_ptbuff->z_t = 0;
  
  if (status == 1) {
    return lasf_NOERR;
  }
  else {
    lasf_file_status(&status, "Failed to read point", "");
    return status;
  }
}

int lasf_read_v1point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff) {
  int status;
  FILE *fp;
  lasf_header lasf_hbuffer;
  
  lasf_pnts1 lasf_ptbuff0;

  if (lasfid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file to write to", "");
    return status;
  }

  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header open", "");
    return status;
  }
  
  lasf_hbuffer = lasfheaderid_array[ lasfhid ].lasfh;
  fp = lasfid_array[ lasfid ].las_file;

  if (seekn >= 0) {
    seekn = (seekn * lasf_hbuffer.pointreclen) + lasf_hbuffer.offset;
    fseek(fp, seekn, SEEK_SET);
  }
  
  status = fread(&lasf_ptbuff0, lasf_hbuffer.pointreclen, 1, fp);

  lasf_ptbuff->x = lasf_ptbuff0.x;
  lasf_ptbuff->y = lasf_ptbuff0.y;
  lasf_ptbuff->z = lasf_ptbuff0.z;
  lasf_ptbuff->intensity = lasf_ptbuff0.intensity;
  lasf_ptbuff->returns = lasf_ptbuff0.returns;
  lasf_ptbuff->classification = lasf_ptbuff0.classification;
  lasf_ptbuff->scanrank = lasf_ptbuff0.scanrank;
  lasf_ptbuff->udata = lasf_ptbuff0.udata;
  lasf_ptbuff->psrcid = lasf_ptbuff0.psrcid;
  lasf_ptbuff->gpstime = lasf_ptbuff0.gpstime;
  lasf_ptbuff->red = 0;
  lasf_ptbuff->green = 0;
  lasf_ptbuff->blue = 0;
  lasf_ptbuff->NIR = 0;
  lasf_ptbuff->wavedesc = 0;
  lasf_ptbuff->waveoffset = 0;
  lasf_ptbuff->wavepacksize = 0;
  lasf_ptbuff->wavereturn = 0;
  lasf_ptbuff->x_t = 0;
  lasf_ptbuff->y_t = 0;
  lasf_ptbuff->z_t = 0;
  
  if (status == 1) {
    return lasf_NOERR;
  }
  else {
    lasf_file_status(&status, "Failed to read point", "");
    return status;
  }
}

int lasf_read_v2point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff) {
  int status;
  FILE *fp;
  lasf_header lasf_hbuffer;
  
  lasf_pnts2 lasf_ptbuff0;

    if (lasfid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file to write to", "");
    return status;
  }

  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header open", "");
    return status;
  }
  
  lasf_hbuffer = lasfheaderid_array[ lasfhid ].lasfh;
  fp = lasfid_array[ lasfid ].las_file;

  if (seekn >= 0) {
    seekn = (seekn * lasf_hbuffer.pointreclen) + lasf_hbuffer.offset;
    fseek(fp, seekn, SEEK_SET);
  }
  
  status = fread(&lasf_ptbuff0, lasf_hbuffer.pointreclen, 1, fp);

  lasf_ptbuff->x = lasf_ptbuff0.x;
  lasf_ptbuff->y = lasf_ptbuff0.y;
  lasf_ptbuff->z = lasf_ptbuff0.z;
  lasf_ptbuff->intensity = lasf_ptbuff0.intensity;
  lasf_ptbuff->returns = lasf_ptbuff0.returns;
  lasf_ptbuff->classification = lasf_ptbuff0.classification;
  lasf_ptbuff->scanrank = lasf_ptbuff0.scanrank;
  lasf_ptbuff->udata = lasf_ptbuff0.udata;
  lasf_ptbuff->psrcid = lasf_ptbuff0.psrcid;
  lasf_ptbuff->gpstime = -1;
  lasf_ptbuff->red = lasf_ptbuff0.red;
  lasf_ptbuff->green = lasf_ptbuff0.green;
  lasf_ptbuff->blue = lasf_ptbuff0.blue;
  lasf_ptbuff->NIR = 0;
  lasf_ptbuff->wavedesc = 0;
  lasf_ptbuff->waveoffset = 0;
  lasf_ptbuff->wavepacksize = 0;
  lasf_ptbuff->wavereturn = 0;
  lasf_ptbuff->x_t = 0;
  lasf_ptbuff->y_t = 0;
  lasf_ptbuff->z_t = 0;
  
  if (status == 1) {
    return lasf_NOERR;
  }
  else {
    lasf_file_status(&status, "Failed to read point", "");
    return status;
  }
}

int lasf_read_v3point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff) {
  int status;
  FILE *fp;
  lasf_header lasf_hbuffer;
  
  lasf_pnts3 lasf_ptbuff0;

  if (lasfid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file to write to", "");
    return status;
  }

  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header open", "");
    return status;
  }
  
  lasf_hbuffer = lasfheaderid_array[ lasfhid ].lasfh;
  fp = lasfid_array[ lasfid ].las_file;

  if (seekn >= 0) {
    seekn = (seekn * lasf_hbuffer.pointreclen) + lasf_hbuffer.offset;
    fseek(fp, seekn, SEEK_SET);
  }
  
  status = fread(&lasf_ptbuff0, lasf_hbuffer.pointreclen, 1, fp);

  lasf_ptbuff->x = lasf_ptbuff0.x;
  lasf_ptbuff->y = lasf_ptbuff0.y;
  lasf_ptbuff->z = lasf_ptbuff0.z;
  lasf_ptbuff->intensity = lasf_ptbuff0.intensity;
  lasf_ptbuff->returns = lasf_ptbuff0.returns;
  lasf_ptbuff->classification = lasf_ptbuff0.classification;
  lasf_ptbuff->scanrank = lasf_ptbuff0.scanrank;
  lasf_ptbuff->udata = lasf_ptbuff0.udata;
  lasf_ptbuff->psrcid = lasf_ptbuff0.psrcid;
  lasf_ptbuff->gpstime = lasf_ptbuff0.gpstime;
  lasf_ptbuff->red = lasf_ptbuff0.red;
  lasf_ptbuff->green = lasf_ptbuff0.green;
  lasf_ptbuff->blue = lasf_ptbuff0.blue;
  lasf_ptbuff->NIR = 0;
  lasf_ptbuff->wavedesc = 0;
  lasf_ptbuff->waveoffset = 0;
  lasf_ptbuff->wavepacksize = 0;
  lasf_ptbuff->wavereturn = 0;
  lasf_ptbuff->x_t = 0;
  lasf_ptbuff->y_t = 0;
  lasf_ptbuff->z_t = 0;
  
  if (status == 1) {
    return lasf_NOERR;
  }
  else {
    lasf_file_status(&status, "Failed to read point", "");
    return status;
  }
}

int lasf_read_v4point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff) {
  int status;
  FILE *fp;
  lasf_header lasf_hbuffer;
  
  lasf_pnts4 lasf_ptbuff0;

    if (lasfid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file to write to", "");
    return status;
  }

  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header open", "");
    return status;
  }
  
  lasf_hbuffer = lasfheaderid_array[ lasfhid ].lasfh;
  fp = lasfid_array[ lasfid ].las_file;

  if (seekn >= 0) {
    seekn = (seekn * lasf_hbuffer.pointreclen) + lasf_hbuffer.offset;
    fseek(fp, seekn, SEEK_SET);
  }
  
  status = fread(&lasf_ptbuff0, lasf_hbuffer.pointreclen, 1, fp);

  lasf_ptbuff->x = lasf_ptbuff0.x;
  lasf_ptbuff->y = lasf_ptbuff0.y;
  lasf_ptbuff->z = lasf_ptbuff0.z;
  lasf_ptbuff->intensity = lasf_ptbuff0.intensity;
  lasf_ptbuff->returns = lasf_ptbuff0.returns;
  lasf_ptbuff->classification = lasf_ptbuff0.classification;
  lasf_ptbuff->scanrank = lasf_ptbuff0.scanrank;
  lasf_ptbuff->udata = lasf_ptbuff0.udata;
  lasf_ptbuff->psrcid = lasf_ptbuff0.psrcid;
  lasf_ptbuff->gpstime = lasf_ptbuff0.gpstime;
  lasf_ptbuff->red = 0;
  lasf_ptbuff->green = 0;
  lasf_ptbuff->blue = 0;
  lasf_ptbuff->NIR = 0;
  lasf_ptbuff->wavedesc = lasf_ptbuff0.wavedesc;
  lasf_ptbuff->waveoffset = lasf_ptbuff0.waveoffset;
  lasf_ptbuff->wavepacksize = lasf_ptbuff0.wavepacksize;
  lasf_ptbuff->wavereturn = lasf_ptbuff0.wavereturn;
  lasf_ptbuff->x_t = lasf_ptbuff0.x_t;
  lasf_ptbuff->y_t = lasf_ptbuff0.y_t;
  lasf_ptbuff->z_t = lasf_ptbuff0.z_t;
  
  if (status == 1) {
    return lasf_NOERR;
  }
  else {
    lasf_file_status(&status, "Failed to read point", "");
    return status;
  }
}

int lasf_read_v5point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff) {
  int status;
  FILE *fp;
  lasf_header lasf_hbuffer;
  
  lasf_pnts5 lasf_ptbuff0;

  if (lasfid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file to write to", "");
    return status;
  }

  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header open", "");
    return status;
  }
  
  lasf_hbuffer = lasfheaderid_array[ lasfhid ].lasfh;
  fp = lasfid_array[ lasfid ].las_file;

  if (seekn >= 0) {
    seekn = (seekn * lasf_hbuffer.pointreclen) + lasf_hbuffer.offset;
    fseek(fp, seekn, SEEK_SET);
  }
  
  status = fread(&lasf_ptbuff0, lasf_hbuffer.pointreclen, 1, fp);

  lasf_ptbuff->x = lasf_ptbuff0.x;
  lasf_ptbuff->y = lasf_ptbuff0.y;
  lasf_ptbuff->z = lasf_ptbuff0.z;
  lasf_ptbuff->intensity = lasf_ptbuff0.intensity;
  lasf_ptbuff->returns = lasf_ptbuff0.returns;
  lasf_ptbuff->classification = lasf_ptbuff0.classification;
  lasf_ptbuff->scanrank = lasf_ptbuff0.scanrank;
  lasf_ptbuff->udata = lasf_ptbuff0.udata;
  lasf_ptbuff->psrcid = lasf_ptbuff0.psrcid;
  lasf_ptbuff->gpstime = lasf_ptbuff0.gpstime;
  lasf_ptbuff->red = lasf_ptbuff0.red;
  lasf_ptbuff->green = lasf_ptbuff0.green;
  lasf_ptbuff->blue = lasf_ptbuff0.blue;
  lasf_ptbuff->NIR = 0;
  lasf_ptbuff->wavedesc = lasf_ptbuff0.wavedesc;
  lasf_ptbuff->waveoffset = lasf_ptbuff0.waveoffset;
  lasf_ptbuff->wavepacksize = lasf_ptbuff0.wavepacksize;
  lasf_ptbuff->wavereturn = lasf_ptbuff0.wavereturn;
  lasf_ptbuff->x_t = lasf_ptbuff0.x_t;
  lasf_ptbuff->y_t = lasf_ptbuff0.y_t;
  lasf_ptbuff->z_t = lasf_ptbuff0.z_t;
  
  if (status == 1) {
    return lasf_NOERR;
  }
  else {
    lasf_file_status(&status, "Failed to read point", "");
    return status;
  }
}

int lasf_read_v6point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff) {
  int status;
  FILE *fp;
  lasf_header lasf_hbuffer;
  
  lasf_pnts6 lasf_ptbuff0;

  if (lasfid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file to write to", "");
    return status;
  }

  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header open", "");
    return status;
  }
  
  lasf_hbuffer = lasfheaderid_array[ lasfhid ].lasfh;
  fp = lasfid_array[ lasfid ].las_file;

  if (seekn >= 0) {
    seekn = (seekn * lasf_hbuffer.pointreclen) + lasf_hbuffer.offset;
    fseek(fp, seekn, SEEK_SET);
  }
  
  status = fread(&lasf_ptbuff0, lasf_hbuffer.pointreclen, 1, fp);

  lasf_ptbuff->x = lasf_ptbuff0.x;
  lasf_ptbuff->y = lasf_ptbuff0.y;
  lasf_ptbuff->z = lasf_ptbuff0.z;
  lasf_ptbuff->intensity = lasf_ptbuff0.intensity;
  lasf_ptbuff->returns = lasf_ptbuff0.returns;
  lasf_ptbuff->classification = lasf_ptbuff0.classification;
  lasf_ptbuff->scanrank = lasf_ptbuff0.scanrank;
  lasf_ptbuff->udata = lasf_ptbuff0.udata;
  lasf_ptbuff->psrcid = lasf_ptbuff0.psrcid;
  lasf_ptbuff->gpstime = lasf_ptbuff0.gpstime;
  lasf_ptbuff->red = 0;
  lasf_ptbuff->green = 0;
  lasf_ptbuff->blue = 0;
  lasf_ptbuff->NIR = 0;
  lasf_ptbuff->wavedesc = 0;
  lasf_ptbuff->waveoffset = 0;
  lasf_ptbuff->wavepacksize = 0;
  lasf_ptbuff->wavereturn = 0;
  lasf_ptbuff->x_t = 0;
  lasf_ptbuff->y_t = 0;
  lasf_ptbuff->z_t = 0;
  
  if (status == 1) {
    return lasf_NOERR;
  }
  else {
    lasf_file_status(&status, "Failed to read point", "");
    return status;
  }
}

int lasf_read_v7point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff) {
  int status;
  FILE *fp;
  lasf_header lasf_hbuffer;
  
  lasf_pnts7 lasf_ptbuff0;

  if (lasfid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file to write to", "");
    return status;
  }

  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header open", "");
    return status;
  }
  
  lasf_hbuffer = lasfheaderid_array[ lasfhid ].lasfh;
  fp = lasfid_array[ lasfid ].las_file;

  if (seekn >= 0) {
    seekn = (seekn * lasf_hbuffer.pointreclen) + lasf_hbuffer.offset;
    fseek(fp, seekn, SEEK_SET);
  }
  
  status = fread(&lasf_ptbuff0, lasf_hbuffer.pointreclen, 1, fp);

  lasf_ptbuff->x = lasf_ptbuff0.x;
  lasf_ptbuff->y = lasf_ptbuff0.y;
  lasf_ptbuff->z = lasf_ptbuff0.z;
  lasf_ptbuff->intensity = lasf_ptbuff0.intensity;
  lasf_ptbuff->returns = lasf_ptbuff0.returns;
  lasf_ptbuff->classification = lasf_ptbuff0.classification;
  lasf_ptbuff->scanrank = lasf_ptbuff0.scanrank;
  lasf_ptbuff->udata = lasf_ptbuff0.udata;
  lasf_ptbuff->psrcid = lasf_ptbuff0.psrcid;
  lasf_ptbuff->gpstime = lasf_ptbuff0.gpstime;
  lasf_ptbuff->red = lasf_ptbuff0.red;
  lasf_ptbuff->green = lasf_ptbuff0.green;
  lasf_ptbuff->blue = lasf_ptbuff0.blue;
  lasf_ptbuff->NIR = 0;
  lasf_ptbuff->wavedesc = 0;
  lasf_ptbuff->waveoffset = 0;
  lasf_ptbuff->wavepacksize = 0;
  lasf_ptbuff->wavereturn = 0;
  lasf_ptbuff->x_t = 0;
  lasf_ptbuff->y_t = 0;
  lasf_ptbuff->z_t = 0;
  
  if (status == 1) {
    return lasf_NOERR;
  }
  else {
    lasf_file_status(&status, "Failed to read point", "");
    return status;
  }
}

int lasf_read_v8point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff) {
  int status;
  FILE *fp;
  lasf_header lasf_hbuffer;
  
  lasf_pnts8 lasf_ptbuff0;

  if (lasfid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file to write to", "");
    return status;
  }

  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header open", "");
    return status;
  }
  
  lasf_hbuffer = lasfheaderid_array[ lasfhid ].lasfh;
  fp = lasfid_array[ lasfid ].las_file;

  if (seekn >= 0) {
    seekn = (seekn * lasf_hbuffer.pointreclen) + lasf_hbuffer.offset;
    fseek(fp, seekn, SEEK_SET);
  }
  
  status = fread(&lasf_ptbuff0, lasf_hbuffer.pointreclen, 1, fp);

  lasf_ptbuff->x = lasf_ptbuff0.x;
  lasf_ptbuff->y = lasf_ptbuff0.y;
  lasf_ptbuff->z = lasf_ptbuff0.z;
  lasf_ptbuff->intensity = lasf_ptbuff0.intensity;
  lasf_ptbuff->returns = lasf_ptbuff0.returns;
  lasf_ptbuff->classification = lasf_ptbuff0.classification;
  lasf_ptbuff->scanrank = lasf_ptbuff0.scanrank;
  lasf_ptbuff->udata = lasf_ptbuff0.udata;
  lasf_ptbuff->psrcid = lasf_ptbuff0.psrcid;
  lasf_ptbuff->gpstime = lasf_ptbuff0.gpstime;
  lasf_ptbuff->red = lasf_ptbuff0.red;
  lasf_ptbuff->green = lasf_ptbuff0.green;
  lasf_ptbuff->blue = lasf_ptbuff0.blue;
  lasf_ptbuff->NIR = lasf_ptbuff0.NIR;
  lasf_ptbuff->wavedesc = 0;
  lasf_ptbuff->waveoffset = 0;
  lasf_ptbuff->wavepacksize = 0;
  lasf_ptbuff->wavereturn = 0;
  lasf_ptbuff->x_t = 0;
  lasf_ptbuff->y_t = 0;
  lasf_ptbuff->z_t = 0;
  
  if (status == 1) {
    return lasf_NOERR;
  }
  else {
    lasf_file_status(&status, "Failed to read point", "");
    return status;
  }
}

int lasf_read_v9point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff) {
  int status;
  FILE *fp;
  lasf_header lasf_hbuffer;
  
  lasf_pnts9 lasf_ptbuff0;

  if (lasfid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file to write to", "");
    return status;
  }

  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header open", "");
    return status;
  }
  
  lasf_hbuffer = lasfheaderid_array[ lasfhid ].lasfh;
  fp = lasfid_array[ lasfid ].las_file;

  if (seekn >= 0) {
    seekn = (seekn * lasf_hbuffer.pointreclen) + lasf_hbuffer.offset;
    fseek(fp, seekn, SEEK_SET);
  }
  
  status = fread(&lasf_ptbuff0, lasf_hbuffer.pointreclen, 1, fp);

  lasf_ptbuff->x = lasf_ptbuff0.x;
  lasf_ptbuff->y = lasf_ptbuff0.y;
  lasf_ptbuff->z = lasf_ptbuff0.z;
  lasf_ptbuff->intensity = lasf_ptbuff0.intensity;
  lasf_ptbuff->returns = lasf_ptbuff0.returns;
  lasf_ptbuff->classification = lasf_ptbuff0.classification;
  lasf_ptbuff->scanrank = lasf_ptbuff0.scanrank;
  lasf_ptbuff->udata = lasf_ptbuff0.udata;
  lasf_ptbuff->psrcid = lasf_ptbuff0.psrcid;
  lasf_ptbuff->gpstime = lasf_ptbuff0.gpstime;
  lasf_ptbuff->red = 0;
  lasf_ptbuff->green = 0;
  lasf_ptbuff->blue = 0;
  lasf_ptbuff->NIR = 0;
  lasf_ptbuff->wavedesc = lasf_ptbuff0.wavedesc;
  lasf_ptbuff->waveoffset = lasf_ptbuff0.waveoffset;
  lasf_ptbuff->wavepacksize = lasf_ptbuff0.wavepacksize;
  lasf_ptbuff->wavereturn = lasf_ptbuff0.wavereturn;
  lasf_ptbuff->x_t = lasf_ptbuff0.x_t;
  lasf_ptbuff->y_t = lasf_ptbuff0.y_t;
  lasf_ptbuff->z_t = lasf_ptbuff0.z_t;
  
  if (status == 1) {
    return lasf_NOERR;
  }
  else {
    lasf_file_status(&status, "Failed to read point", "");
    return status;
  }
}

int lasf_read_v10point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff) {
  int status;
  FILE *fp;
  lasf_header lasf_hbuffer;
  
  lasf_pnts lasf_ptbuff0;
  
  if (lasfid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file to write to", "");
    return status;
  }

  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header open", "");
    return status;
  }
  
  lasf_hbuffer = lasfheaderid_array[ lasfhid ].lasfh;
  fp = lasfid_array[ lasfid ].las_file;

  if (seekn >= 0) {
    seekn = (seekn * lasf_hbuffer.pointreclen) + lasf_hbuffer.offset;
    fseek(fp, seekn, SEEK_SET);
  }
  
  status = fread(&lasf_ptbuff0, lasf_hbuffer.pointreclen, 1, fp);

  lasf_ptbuff->x = lasf_ptbuff0.x;
  lasf_ptbuff->y = lasf_ptbuff0.y;
  lasf_ptbuff->z = lasf_ptbuff0.z;
  lasf_ptbuff->intensity = lasf_ptbuff0.intensity;
  lasf_ptbuff->returns = lasf_ptbuff0.returns;
  lasf_ptbuff->classification = lasf_ptbuff0.classification;
  lasf_ptbuff->scanrank = lasf_ptbuff0.scanrank;
  lasf_ptbuff->udata = lasf_ptbuff0.udata;
  lasf_ptbuff->psrcid = lasf_ptbuff0.psrcid;
  lasf_ptbuff->gpstime = lasf_ptbuff0.gpstime;
  lasf_ptbuff->red = lasf_ptbuff0.red;
  lasf_ptbuff->green = lasf_ptbuff0.green;
  lasf_ptbuff->blue = lasf_ptbuff0.blue;
  lasf_ptbuff->NIR = lasf_ptbuff0.NIR;
  lasf_ptbuff->wavedesc = lasf_ptbuff0.wavedesc;
  lasf_ptbuff->waveoffset = lasf_ptbuff0.waveoffset;
  lasf_ptbuff->wavepacksize = lasf_ptbuff0.wavepacksize;
  lasf_ptbuff->wavereturn = lasf_ptbuff0.wavereturn;
  lasf_ptbuff->x_t = lasf_ptbuff0.x_t;
  lasf_ptbuff->y_t = lasf_ptbuff0.y_t;
  lasf_ptbuff->z_t = lasf_ptbuff0.z_t;
  
  if (status == 1) {
    return lasf_NOERR;
  }
  else {
    lasf_file_status(&status, "Failed to read point", "");
    return status;
  }
}

/*
  Read an lasf_pnts struct from file, based on the las file 
  id and the las file header id. If seekn is -1 or less, 
  will read from current position of the input file associated 
  with the lasfid.  If seekn is 0 or above, will read from that 
  point numbers position, zero is the first point.
 */
size_t
lasf_read_point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff) {
  lasf_header lasf_hbuffer;
  int status;

  lasf_pnts lasf_ptbuff1;

  if (lasfid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file to write to", "");
    return status;
  }

  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header open", "");
    return status;
  }
  
  lasf_hbuffer = lasfheaderid_array[ lasfhid ].lasfh;

  switch(lasf_hbuffer.pointformat) {
  case 0:
    status = lasf_read_v0point(lasfid, lasfhid, seekn, &lasf_ptbuff1);
    break;
  case 1:
    status = lasf_read_v1point(lasfid, lasfhid, seekn, &lasf_ptbuff1);
    break;
  case 2:
    status = lasf_read_v2point(lasfid, lasfhid, seekn, &lasf_ptbuff1);
    break;
  case 3:
    status = lasf_read_v3point(lasfid, lasfhid, seekn, &lasf_ptbuff1);
    break;
  case 4:
    status = lasf_read_v4point(lasfid, lasfhid, seekn, &lasf_ptbuff1);
    break;
  case 5:
    status = lasf_read_v5point(lasfid, lasfhid, seekn, &lasf_ptbuff1);
    break;
  case 6:
    status = lasf_read_v6point(lasfid, lasfhid, seekn, &lasf_ptbuff1);
    if (status != lasf_NOERR) {
      lasf_print_error(status, "laspnt.c");
    }
    break;
  case 7:
    status = lasf_read_v7point(lasfid, lasfhid, seekn, &lasf_ptbuff1);
    break;
  case 8:
    status = lasf_read_v8point(lasfid, lasfhid, seekn, &lasf_ptbuff1);
    break;
  case 9:
    status = lasf_read_v9point(lasfid, lasfhid, seekn, &lasf_ptbuff1);
    break;
  case 10:
    status = lasf_read_v10point(lasfid, lasfhid, seekn, &lasf_ptbuff1);
    break;
  }
  if (status == lasf_NOERR) {
    *lasf_ptbuff = lasf_ptbuff1;
    return lasf_NOERR;
  }
  else {
    lasf_file_status(&status, "Failed to read point", "");
    return status;
  }
}

int
lasf_write_v0point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff) {
  FILE *lasout;
  lasf_header lasfh;
  int status;

  lasf_pnts0 lasf_ptbuff0;

  if (lasfid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file to write to", "");
    return status;
  }
  
  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header open", "");
    return status;
  }
  
  lasout = lasfid_array[ lasfid ].las_file;
  lasfh = lasfheaderid_array[ lasfhid ].lasfh;

  if (seekn >= 0) {
    seekn = (seekn * lasfh.pointreclen) + lasfh.offset;
    fseek(lasout, seekn, SEEK_SET);
  }

  status = fwrite(&lasf_ptbuff->x, sizeof(lasf_ptbuff0.x), 1, lasout);
  status = fwrite(&lasf_ptbuff->y, sizeof(lasf_ptbuff0.y), 1, lasout);
  status = fwrite(&lasf_ptbuff->z,sizeof(lasf_ptbuff0.z), 1, lasout);
  status = fwrite(&lasf_ptbuff->intensity, sizeof(lasf_ptbuff0.intensity), 1, lasout);
  status = fwrite(&lasf_ptbuff->returns, sizeof(lasf_ptbuff0.returns), 1, lasout);
  status = fwrite(&lasf_ptbuff->classification,sizeof(lasf_ptbuff0.classification), 1, lasout);
  status = fwrite(&lasf_ptbuff->scanrank, sizeof(lasf_ptbuff0.scanrank), 1, lasout);
  status = fwrite(&lasf_ptbuff->udata, sizeof(lasf_ptbuff0.udata), 1, lasout);
  status = fwrite(&lasf_ptbuff->psrcid, sizeof(lasf_ptbuff0.psrcid), 1, lasout);

  if (status == 1) {
    return  lasf_NOERR;
  }
  else {
    lasf_file_status(&status, "Failed to write point", "");
    return status;
  }
}

int
lasf_write_v1point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff) {
  FILE *lasout;
  lasf_header lasfh;
  int status;

  lasf_pnts1 lasf_ptbuff0;

  if (lasfid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file to write to", "");
    return status;
  }
  
  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header open", "");
    return status;
  }
  
  lasout = lasfid_array[ lasfid ].las_file;
  lasfh = lasfheaderid_array[ lasfhid ].lasfh;

  if (seekn >= 0) {
    seekn = (seekn * lasfh.pointreclen) + lasfh.offset;
    fseek(lasout, seekn, SEEK_SET);
  }

  status = fwrite(&lasf_ptbuff->x, sizeof(lasf_ptbuff0.x), 1, lasout);
  status = fwrite(&lasf_ptbuff->y, sizeof(lasf_ptbuff0.y), 1, lasout);
  status = fwrite(&lasf_ptbuff->z,sizeof(lasf_ptbuff0.z), 1, lasout);
  status = fwrite(&lasf_ptbuff->intensity, sizeof(lasf_ptbuff0.intensity), 1, lasout);
  status = fwrite(&lasf_ptbuff->returns, sizeof(lasf_ptbuff0.returns), 1, lasout);
  status = fwrite(&lasf_ptbuff->classification,sizeof(lasf_ptbuff0.classification), 1, lasout);
  status = fwrite(&lasf_ptbuff->scanrank, sizeof(lasf_ptbuff0.scanrank), 1, lasout);
  status = fwrite(&lasf_ptbuff->udata, sizeof(lasf_ptbuff0.udata), 1, lasout);
  status = fwrite(&lasf_ptbuff->psrcid, sizeof(lasf_ptbuff0.psrcid), 1, lasout);
  status = fwrite(&lasf_ptbuff->gpstime, sizeof(lasf_ptbuff0.gpstime), 1, lasout);

  if (status == 1) {
    return lasf_NOERR;
  }
  else {
    lasf_file_status(&status, "Failed to write point", "");
    return status;
  }
}

int
lasf_write_v2point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff) {
  FILE *lasout;
  lasf_header lasfh;
  int status;

  lasf_pnts2 lasf_ptbuff0;

  if (lasfid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file to write to", "");
    return status;
  }
  
  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header open", "");
    return status;
  }
  
  lasout = lasfid_array[ lasfid ].las_file;
  lasfh = lasfheaderid_array[ lasfhid ].lasfh;

  if (seekn >= 0) {
    seekn = (seekn * lasfh.pointreclen) + lasfh.offset;
    fseek(lasout, seekn, SEEK_SET);
  }

  status = fwrite(&lasf_ptbuff->x, sizeof(lasf_ptbuff0.x), 1, lasout);
  status = fwrite(&lasf_ptbuff->y, sizeof(lasf_ptbuff0.y), 1, lasout);
  status = fwrite(&lasf_ptbuff->z,sizeof(lasf_ptbuff0.z), 1, lasout);
  status = fwrite(&lasf_ptbuff->intensity, sizeof(lasf_ptbuff0.intensity), 1, lasout);
  status = fwrite(&lasf_ptbuff->returns, sizeof(lasf_ptbuff0.returns), 1, lasout);
  status = fwrite(&lasf_ptbuff->classification,sizeof(lasf_ptbuff0.classification), 1, lasout);
  status = fwrite(&lasf_ptbuff->scanrank, sizeof(lasf_ptbuff0.scanrank), 1, lasout);
  status = fwrite(&lasf_ptbuff->udata, sizeof(lasf_ptbuff0.udata), 1, lasout);
  status = fwrite(&lasf_ptbuff->psrcid, sizeof(lasf_ptbuff0.psrcid), 1, lasout);
  status = fwrite(&lasf_ptbuff->red, sizeof(lasf_ptbuff0.red), 1, lasout);
  status = fwrite(&lasf_ptbuff->green, sizeof(lasf_ptbuff0.green), 1, lasout);
  status = fwrite(&lasf_ptbuff->blue, sizeof(lasf_ptbuff0.blue), 1, lasout);

  if (status == 1) {
    return  lasf_NOERR;
  }
  else {
    lasf_file_status(&status, "Failed to write point", "");
    return status;
  }
}

int
lasf_write_v3point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff) {
  FILE *lasout;
  lasf_header lasfh;
  int status;

  lasf_pnts3 lasf_ptbuff0;

  if (lasfid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file to write to", "");
    return status;
  }
  
  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header open", "");
    return status;
  }
  
  lasout = lasfid_array[ lasfid ].las_file;
  lasfh = lasfheaderid_array[ lasfhid ].lasfh;

  if (seekn >= 0) {
    seekn = (seekn * lasfh.pointreclen) + lasfh.offset;
    fseek(lasout, seekn, SEEK_SET);
  }

  status = fwrite(&lasf_ptbuff->x, sizeof(lasf_ptbuff0.x), 1, lasout);
  status = fwrite(&lasf_ptbuff->y, sizeof(lasf_ptbuff0.y), 1, lasout);
  status = fwrite(&lasf_ptbuff->z, sizeof(lasf_ptbuff0.z), 1, lasout);
  status = fwrite(&lasf_ptbuff->intensity, sizeof(lasf_ptbuff0.intensity), 1, lasout);
  status = fwrite(&lasf_ptbuff->returns, sizeof(lasf_ptbuff0.returns), 1, lasout);
  status = fwrite(&lasf_ptbuff->classification, sizeof(lasf_ptbuff0.classification), 1, lasout);
  status = fwrite(&lasf_ptbuff->scanrank, sizeof(lasf_ptbuff0.scanrank), 1, lasout);
  status = fwrite(&lasf_ptbuff->udata, sizeof(lasf_ptbuff0.udata), 1, lasout);
  status = fwrite(&lasf_ptbuff->psrcid, sizeof(lasf_ptbuff0.psrcid), 1, lasout);
  status = fwrite(&lasf_ptbuff->gpstime, sizeof(lasf_ptbuff0.gpstime), 1, lasout);
  status = fwrite(&lasf_ptbuff->red, sizeof(lasf_ptbuff0.red), 1, lasout);
  status = fwrite(&lasf_ptbuff->green, sizeof(lasf_ptbuff0.green), 1, lasout);
  status = fwrite(&lasf_ptbuff->blue, sizeof(lasf_ptbuff0.blue), 1, lasout);

  if (status == 1) {
    return  lasf_NOERR;
  }
  else {
    lasf_file_status(&status, "Failed to write point", "");
    return status;
  }
}

int
lasf_write_v4point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff) {
  FILE *lasout;
  lasf_header lasfh;
  int status;

  lasf_pnts4 lasf_ptbuff0;

  if (lasfid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file to write to", "");
    return status;
  }
  
  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header open", "");
    return status;
  }
  
  lasout = lasfid_array[ lasfid ].las_file;
  lasfh = lasfheaderid_array[ lasfhid ].lasfh;

  if (seekn >= 0) {
    seekn = (seekn * lasfh.pointreclen) + lasfh.offset;
    fseek(lasout, seekn, SEEK_SET);
  }

  status = fwrite(&lasf_ptbuff->x, sizeof(lasf_ptbuff0.x), 1, lasout);
  status = fwrite(&lasf_ptbuff->y, sizeof(lasf_ptbuff0.y), 1, lasout);
  status = fwrite(&lasf_ptbuff->z, sizeof(lasf_ptbuff0.z), 1, lasout);
  status = fwrite(&lasf_ptbuff->intensity, sizeof(lasf_ptbuff0.intensity), 1, lasout);
  status = fwrite(&lasf_ptbuff->returns, sizeof(lasf_ptbuff0.returns), 1, lasout);
  status = fwrite(&lasf_ptbuff->classification, sizeof(lasf_ptbuff0.classification), 1, lasout);
  status = fwrite(&lasf_ptbuff->scanrank, sizeof(lasf_ptbuff0.scanrank), 1, lasout);
  status = fwrite(&lasf_ptbuff->udata, sizeof(lasf_ptbuff0.udata), 1, lasout);
  status = fwrite(&lasf_ptbuff->psrcid, sizeof(lasf_ptbuff0.psrcid), 1, lasout);
  status = fwrite(&lasf_ptbuff->gpstime, sizeof(lasf_ptbuff0.gpstime), 1, lasout);
  status = fwrite(&lasf_ptbuff->wavedesc, sizeof(lasf_ptbuff0.wavedesc), 1, lasout);
  status = fwrite(&lasf_ptbuff->waveoffset, sizeof(lasf_ptbuff0.waveoffset), 1, lasout);
  status = fwrite(&lasf_ptbuff->wavepacksize, sizeof(lasf_ptbuff0.wavepacksize), 1, lasout);
  status = fwrite(&lasf_ptbuff->wavereturn, sizeof(lasf_ptbuff0.wavereturn), 1, lasout);
  status = fwrite(&lasf_ptbuff->x_t, sizeof(lasf_ptbuff0.x_t), 1, lasout);
  status = fwrite(&lasf_ptbuff->y_t, sizeof(lasf_ptbuff0.y_t), 1, lasout);
  status = fwrite(&lasf_ptbuff->z_t, sizeof(lasf_ptbuff0.z_t), 1, lasout);

  if (status == 1) {
    return lasf_NOERR;
  }
  else {
    lasf_file_status(&status, "Failed to write point", "");
    return status;
  }
}

int
lasf_write_v5point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff) {
  FILE *lasout;
  lasf_header lasfh;
  int status;

  lasf_pnts5 lasf_ptbuff0;

  if (lasfid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file to write to", "");
    return status;
  }
  
  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header open", "");
    return status;
  }
  
  lasout = lasfid_array[ lasfid ].las_file;
  lasfh = lasfheaderid_array[ lasfhid ].lasfh;

  if (seekn >= 0) {
    seekn = (seekn * lasfh.pointreclen) + lasfh.offset;
    fseek(lasout, seekn, SEEK_SET);
  }

  status = fwrite(&lasf_ptbuff->x, sizeof(lasf_ptbuff0.x), 1, lasout);
  status = fwrite(&lasf_ptbuff->y, sizeof(lasf_ptbuff0.y), 1, lasout);
  status = fwrite(&lasf_ptbuff->z, sizeof(lasf_ptbuff0.z), 1, lasout);
  status = fwrite(&lasf_ptbuff->intensity, sizeof(lasf_ptbuff0.intensity), 1, lasout);
  status = fwrite(&lasf_ptbuff->returns, sizeof(lasf_ptbuff0.returns), 1, lasout);
  status = fwrite(&lasf_ptbuff->classification, sizeof(lasf_ptbuff0.classification), 1, lasout);
  status = fwrite(&lasf_ptbuff->scanrank, sizeof(lasf_ptbuff0.scanrank), 1, lasout);
  status = fwrite(&lasf_ptbuff->udata, sizeof(lasf_ptbuff0.udata), 1, lasout);
  status = fwrite(&lasf_ptbuff->psrcid, sizeof(lasf_ptbuff0.psrcid), 1, lasout);
  status = fwrite(&lasf_ptbuff->gpstime, sizeof(lasf_ptbuff0.gpstime), 1, lasout);
  status = fwrite(&lasf_ptbuff->red, sizeof(lasf_ptbuff0.red), 1, lasout);
  status = fwrite(&lasf_ptbuff->green, sizeof(lasf_ptbuff0.green), 1, lasout);
  status = fwrite(&lasf_ptbuff->blue, sizeof(lasf_ptbuff0.blue), 1, lasout);
  status = fwrite(&lasf_ptbuff->wavedesc, sizeof(lasf_ptbuff0.wavedesc), 1, lasout);
  status = fwrite(&lasf_ptbuff->waveoffset, sizeof(lasf_ptbuff0.waveoffset), 1, lasout);
  status = fwrite(&lasf_ptbuff->wavepacksize, sizeof(lasf_ptbuff0.wavepacksize), 1, lasout);
  status = fwrite(&lasf_ptbuff->wavereturn, sizeof(lasf_ptbuff0.wavereturn), 1, lasout);
  status = fwrite(&lasf_ptbuff->x_t, sizeof(lasf_ptbuff0.x_t), 1, lasout);
  status = fwrite(&lasf_ptbuff->y_t, sizeof(lasf_ptbuff0.y_t), 1, lasout);
  status = fwrite(&lasf_ptbuff->z_t, sizeof(lasf_ptbuff0.z_t), 1, lasout);

  if (status == 1) {
    return  lasf_NOERR;
  }
  else {
    lasf_file_status(&status, "Failed to write point", "");
    return status;
  }
}

int
lasf_write_v6point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff) {
  FILE *lasout;
  lasf_header lasfh;
  int status;

  lasf_pnts6 lasf_ptbuff0;

  if (lasfid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file to write to", "");
    return status;
  }
  
  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header open", "");
    return status;
  }
  
  lasout = lasfid_array[ lasfid ].las_file;
  lasfh = lasfheaderid_array[ lasfhid ].lasfh;

  if (seekn >= 0) {
    seekn = (seekn * lasfh.pointreclen) + lasfh.offset;
    fseek(lasout, seekn, SEEK_SET);
  }

  status = fwrite(&lasf_ptbuff->x, sizeof(lasf_ptbuff0.x), 1, lasout);
  status = fwrite(&lasf_ptbuff->y, sizeof(lasf_ptbuff0.y), 1, lasout);
  status = fwrite(&lasf_ptbuff->z, sizeof(lasf_ptbuff0.z), 1, lasout);
  status = fwrite(&lasf_ptbuff->intensity, sizeof(lasf_ptbuff0.intensity), 1, lasout);
  status = fwrite(&lasf_ptbuff->returns, sizeof(lasf_ptbuff0.returns), 1, lasout);
  status = fwrite(&lasf_ptbuff->classification, sizeof(lasf_ptbuff0.classification), 1, lasout);
  status = fwrite(&lasf_ptbuff->udata, sizeof(lasf_ptbuff0.udata), 1, lasout);
  status = fwrite(&lasf_ptbuff->scanrank, sizeof(lasf_ptbuff0.scanrank), 1, lasout);
  status = fwrite(&lasf_ptbuff->psrcid, sizeof(lasf_ptbuff0.psrcid), 1, lasout);
  status = fwrite(&lasf_ptbuff->gpstime, sizeof(lasf_ptbuff0.gpstime), 1, lasout);
  
  if (status == 1) {
    return  lasf_NOERR;
  }
  else {
    lasf_file_status(&status, "Failed to write point", "");
    return status;
  }
}

int
lasf_write_v7point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff) {
  FILE *lasout;
  lasf_header lasfh;
  int status;

  lasf_pnts7 lasf_ptbuff0;

  if (lasfid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file to write to", "");
    return status;
  }
  
  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header open", "");
    return status;
  }
  
  lasout = lasfid_array[ lasfid ].las_file;
  lasfh = lasfheaderid_array[ lasfhid ].lasfh;

  if (seekn >= 0) {
    seekn = (seekn * lasfh.pointreclen) + lasfh.offset;
    fseek(lasout, seekn, SEEK_SET);
  }

  status = fwrite(&lasf_ptbuff->x, sizeof(lasf_ptbuff0.x), 1, lasout);
  status = fwrite(&lasf_ptbuff->y, sizeof(lasf_ptbuff0.y), 1, lasout);
  status = fwrite(&lasf_ptbuff->z, sizeof(lasf_ptbuff0.z), 1, lasout);
  status = fwrite(&lasf_ptbuff->intensity, sizeof(lasf_ptbuff0.intensity), 1, lasout);
  status = fwrite(&lasf_ptbuff->returns, sizeof(lasf_ptbuff0.returns), 1, lasout);
  status = fwrite(&lasf_ptbuff->classification, sizeof(lasf_ptbuff0.classification), 1, lasout);
  status = fwrite(&lasf_ptbuff->udata, sizeof(lasf_ptbuff0.udata), 1, lasout);
  status = fwrite(&lasf_ptbuff->scanrank, sizeof(lasf_ptbuff0.scanrank), 1, lasout);
  status = fwrite(&lasf_ptbuff->psrcid, sizeof(lasf_ptbuff0.psrcid), 1, lasout);
  status = fwrite(&lasf_ptbuff->gpstime, sizeof(lasf_ptbuff0.gpstime), 1, lasout);
  status = fwrite(&lasf_ptbuff->red, sizeof(lasf_ptbuff0.red), 1, lasout);
  status = fwrite(&lasf_ptbuff->green, sizeof(lasf_ptbuff0.green), 1, lasout);
  status = fwrite(&lasf_ptbuff->blue, sizeof(lasf_ptbuff0.blue), 1, lasout);

  if (status == 1) {
    return  lasf_NOERR;
  }
  else {
    lasf_file_status(&status, "Failed to write point", "");
    return status;
  }
}

int
lasf_write_v8point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff) {
  FILE *lasout;
  lasf_header lasfh;
  int status;

  lasf_pnts8 lasf_ptbuff0;

  if (lasfid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file to write to", "");
    return status;
  }
  
  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header open", "");
    return status;
  }
  
  lasout = lasfid_array[ lasfid ].las_file;
  lasfh = lasfheaderid_array[ lasfhid ].lasfh;

  if (seekn >= 0) {
    seekn = (seekn * lasfh.pointreclen) + lasfh.offset;
    fseek(lasout, seekn, SEEK_SET);
  }

  status = fwrite(&lasf_ptbuff->x, sizeof(lasf_ptbuff0.x), 1, lasout);
  status = fwrite(&lasf_ptbuff->y, sizeof(lasf_ptbuff0.y), 1, lasout);
  status = fwrite(&lasf_ptbuff->z, sizeof(lasf_ptbuff0.z), 1, lasout);
  status = fwrite(&lasf_ptbuff->intensity, sizeof(lasf_ptbuff0.intensity), 1, lasout);
  status = fwrite(&lasf_ptbuff->returns, sizeof(lasf_ptbuff0.returns), 1, lasout);
  status = fwrite(&lasf_ptbuff->classification, sizeof(lasf_ptbuff0.classification), 1, lasout);
  status = fwrite(&lasf_ptbuff->udata, sizeof(lasf_ptbuff0.udata), 1, lasout);
  status = fwrite(&lasf_ptbuff->scanrank, sizeof(lasf_ptbuff0.scanrank), 1, lasout);
  status = fwrite(&lasf_ptbuff->psrcid, sizeof(lasf_ptbuff0.psrcid), 1, lasout);
  status = fwrite(&lasf_ptbuff->gpstime, sizeof(lasf_ptbuff0.gpstime), 1, lasout);
  status = fwrite(&lasf_ptbuff->red, sizeof(lasf_ptbuff0.red), 1, lasout);
  status = fwrite(&lasf_ptbuff->green, sizeof(lasf_ptbuff0.green), 1, lasout);
  status = fwrite(&lasf_ptbuff->blue, sizeof(lasf_ptbuff0.blue), 1, lasout);
  status = fwrite(&lasf_ptbuff->NIR, sizeof(lasf_ptbuff0.NIR), 1, lasout);

  if (status == 1) {
    return  lasf_NOERR;
  }
  else {
    lasf_file_status(&status, "Failed to write point", "");
    return status;
  }
}

int
lasf_write_v9point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff) {
  FILE *lasout;
  lasf_header lasfh;
  int status;

  lasf_pnts9 lasf_ptbuff0;

  if (lasfid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file to write to", "");
    return status;
  }
  
  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header open", "");
    return status;
  }
  
  lasout = lasfid_array[ lasfid ].las_file;
  lasfh = lasfheaderid_array[ lasfhid ].lasfh;

  if (seekn >= 0) {
    seekn = (seekn * lasfh.pointreclen) + lasfh.offset;
    fseek(lasout, seekn, SEEK_SET);
  }

  status = fwrite(&lasf_ptbuff->x, sizeof(lasf_ptbuff0.x), 1, lasout);
  status = fwrite(&lasf_ptbuff->y, sizeof(lasf_ptbuff0.y), 1, lasout);
  status = fwrite(&lasf_ptbuff->z, sizeof(lasf_ptbuff0.z), 1, lasout);
  status = fwrite(&lasf_ptbuff->intensity, sizeof(lasf_ptbuff0.intensity), 1, lasout);
  status = fwrite(&lasf_ptbuff->returns, sizeof(lasf_ptbuff0.returns), 1, lasout);
  status = fwrite(&lasf_ptbuff->classification, sizeof(lasf_ptbuff0.classification), 1, lasout);
  status = fwrite(&lasf_ptbuff->udata, sizeof(lasf_ptbuff0.udata), 1, lasout);
  status = fwrite(&lasf_ptbuff->scanrank, sizeof(lasf_ptbuff0.scanrank), 1, lasout);
  status = fwrite(&lasf_ptbuff->psrcid, sizeof(lasf_ptbuff0.psrcid), 1, lasout);
  status = fwrite(&lasf_ptbuff->gpstime, sizeof(lasf_ptbuff0.gpstime), 1, lasout);
  status = fwrite(&lasf_ptbuff->wavedesc, sizeof(lasf_ptbuff0.wavedesc), 1, lasout);
  status = fwrite(&lasf_ptbuff->waveoffset, sizeof(lasf_ptbuff0.waveoffset), 1, lasout);
  status = fwrite(&lasf_ptbuff->wavepacksize, sizeof(lasf_ptbuff0.wavepacksize), 1, lasout);
  status = fwrite(&lasf_ptbuff->wavereturn, sizeof(lasf_ptbuff0.wavereturn), 1, lasout);
  status = fwrite(&lasf_ptbuff->x_t, sizeof(lasf_ptbuff0.x_t), 1, lasout);
  status = fwrite(&lasf_ptbuff->y_t, sizeof(lasf_ptbuff0.y_t), 1, lasout);
  status = fwrite(&lasf_ptbuff->z_t, sizeof(lasf_ptbuff0.z_t), 1, lasout);

  if (status == 1) {
    return  lasf_NOERR;
  }
  else {
    lasf_file_status(&status, "Failed to write point", "");
    return status;
  }
}

int
lasf_write_v10point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff) {
  FILE *lasout;
  lasf_header lasfh;
  int status;

  lasf_pnts lasf_ptbuff0;

  if (lasfid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file to write to", "");
    return status;
  }
  
  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header open", "");
    return status;
  }
  
  lasout = lasfid_array[ lasfid ].las_file;
  lasfh = lasfheaderid_array[ lasfhid ].lasfh;

  if (seekn >= 0) {
    seekn = (seekn * lasfh.pointreclen) + lasfh.offset;
    fseek(lasout, seekn, SEEK_SET);
  }

  status = fwrite(&lasf_ptbuff->x, sizeof(lasf_ptbuff0.x), 1, lasout);
  status = fwrite(&lasf_ptbuff->y, sizeof(lasf_ptbuff0.y), 1, lasout);
  status = fwrite(&lasf_ptbuff->z, sizeof(lasf_ptbuff0.z), 1, lasout);
  status = fwrite(&lasf_ptbuff->intensity, sizeof(lasf_ptbuff0.intensity), 1, lasout);
  status = fwrite(&lasf_ptbuff->returns, sizeof(lasf_ptbuff0.returns), 1, lasout);
  status = fwrite(&lasf_ptbuff->classification, sizeof(lasf_ptbuff0.classification), 1, lasout);
  status = fwrite(&lasf_ptbuff->udata, sizeof(lasf_ptbuff0.udata), 1, lasout);
  status = fwrite(&lasf_ptbuff->scanrank, sizeof(lasf_ptbuff0.scanrank), 1, lasout);
  status = fwrite(&lasf_ptbuff->psrcid, sizeof(lasf_ptbuff0.psrcid), 1, lasout);
  status = fwrite(&lasf_ptbuff->gpstime, sizeof(lasf_ptbuff0.gpstime), 1, lasout);
  status = fwrite(&lasf_ptbuff->red, sizeof(lasf_ptbuff0.red), 1, lasout);
  status = fwrite(&lasf_ptbuff->green, sizeof(lasf_ptbuff0.green), 1, lasout);
  status = fwrite(&lasf_ptbuff->blue, sizeof(lasf_ptbuff0.blue), 1, lasout);
  status = fwrite(&lasf_ptbuff->NIR, sizeof(lasf_ptbuff0.NIR), 1, lasout);
  status = fwrite(&lasf_ptbuff->wavedesc, sizeof(lasf_ptbuff0.wavedesc), 1, lasout);
  status = fwrite(&lasf_ptbuff->waveoffset, sizeof(lasf_ptbuff0.waveoffset), 1, lasout);
  status = fwrite(&lasf_ptbuff->wavepacksize, sizeof(lasf_ptbuff0.wavepacksize), 1, lasout);
  status = fwrite(&lasf_ptbuff->wavereturn, sizeof(lasf_ptbuff0.wavereturn), 1, lasout);
  status = fwrite(&lasf_ptbuff->x_t, sizeof(lasf_ptbuff0.x_t), 1, lasout);
  status = fwrite(&lasf_ptbuff->y_t, sizeof(lasf_ptbuff0.y_t), 1, lasout);
  status = fwrite(&lasf_ptbuff->z_t, sizeof(lasf_ptbuff0.z_t), 1, lasout);

  if (status == 1) {
    return lasf_NOERR;
  }
  else {
    lasf_file_status(&status, "Failed to write point", "");
    return status;
  }
}

/*
  Write an lasf_pnts struct to file, based on the las file 
  id and the las file header id. If seekn is -1 or less, 
  will write to current position of the input file associated 
  with the lasfid.  If seekn is 0 or above, will write to that 
  point numbers position, zero is the first point.
 */
int
lasf_write_point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff) {
  FILE *lasout;
  lasf_header lasfh;
  int status;

  if (lasfid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file to write to", "");
    return status;
  }

  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header open", "");
    return status;
  }
  lasout = lasfid_array[ lasfid ].las_file;
  lasfh = lasfheaderid_array[ lasfhid ].lasfh;

  lasf_pnts lasf_ptbuff0;
  lasf_ptbuff0 = *lasf_ptbuff;
  
  switch(lasfh.pointformat) {
  case 0:
    status = lasf_write_v0point(lasfid, lasfhid, seekn, &lasf_ptbuff0);
    break;
  case 1:
    status = lasf_write_v1point(lasfid, lasfhid, seekn, &lasf_ptbuff0);
    break;
  case 2:
    status = lasf_write_v2point(lasfid, lasfhid, seekn, &lasf_ptbuff0);
    break;
  case 3:
    status = lasf_write_v3point(lasfid, lasfhid, seekn, &lasf_ptbuff0);
    break;
  case 4:
    status = lasf_write_v4point(lasfid, lasfhid, seekn, &lasf_ptbuff0);
    break;
  case 5:
    status = lasf_write_v5point(lasfid, lasfhid, seekn, &lasf_ptbuff0);
    break;
  case 6:
    status = lasf_write_v6point(lasfid, lasfhid, seekn, &lasf_ptbuff0);
    break;
  case 7:
    status = lasf_write_v7point(lasfid, lasfhid, seekn, &lasf_ptbuff0);
    break;
  case 8:
    status = lasf_write_v8point(lasfid, lasfhid, seekn, &lasf_ptbuff0);
    break;
  case 9:
    status = lasf_write_v9point(lasfid, lasfhid, seekn, &lasf_ptbuff0);
    break;
  case 10:
    status = lasf_write_v10point(lasfid, lasfhid, seekn, &lasf_ptbuff0);
    break;
  }  
  if (status == lasf_NOERR) {
    return lasf_NOERR;
  }
  else {
    lasf_file_status(&status, "Failed to write point", "");
    return status;
  }
}

/* 
   Parse the point record 'returns' byte and return a 
   structure holding the 4 integer values 
*/
lasf_pntreturns
lasf_parse_point_returns(unsigned char pt_rt) {
  lasf_pntreturns lasf_ptrt;

  /* Return Number: The Return Number is the pulse return number for a given output pulse. A
     given output laser pulse can have many returns, and they must be marked in sequence of 
     returThe first return will have a Return Number of one, the second a Return Number of two, 
     and so on up to five returns. */
  lasf_ptrt.return_num = (pt_rt >> 0) & ((1 << 3) - 1);
  /* Number of Returns (for this emitted pulse): The Number of Returns is the total number of
     returns for a given pulse. For example, a laser data point may be return two (Return Number)
     within a total number of five returns. */
  lasf_ptrt.num_return = (pt_rt >> 3) & ((1 << 3) - 1);
  /* Scan Direction Flag: The Scan Direction Flag denotes the direction at which the scanner mirror
     was traveling at the time of the output pulse. A bit value of 1 is a positive scan direction, and a
     bit value of 0 is a negative scan direction (where positive scan direction is a scan moving
     from the left side of the in-track direction to the right side and negative the opposite). */
  lasf_ptrt.scan_dir = (pt_rt >> 6) & ((1 << 1) - 1);
  /* Edge of Flight Line: The Edge of Flight Line data bit has a value of 1 only when the point is at
     the end of a scan. It is the last point on a given scan line before it changes direction. */
  lasf_ptrt.flight_edge = (pt_rt >> 7) & ((1 << 1) - 1);

  return lasf_ptrt;
}

/* 
   Parse the point record 'returns' byte and return a 
   structure holding the 4 integer values 
*/
lasf_pntreturns
lasf_parse_point_returns6(unsigned short pt_rt) {
  lasf_pntreturns lasf_ptrt;

  /* Return Number: The Return Number is the pulse return number for a given output pulse. A
     given output laser pulse can have many returns, and they must be marked in sequence of 
     returThe first return will have a Return Number of one, the second a Return Number of two, 
     and so on up to five returns. */
  lasf_ptrt.return_num = (pt_rt >> 0) & ((1 << 4) - 1);
  /* Number of Returns (for this emitted pulse): The Number of Returns is the total number of
     returns for a given pulse. For example, a laser data point may be return two (Return Number)
     within a total number of five returns. */
  lasf_ptrt.num_return = (pt_rt >> 4) & ((1 << 4) - 1);
  /* Scan Direction Flag: The Scan Direction Flag denotes the direction at which the scanner mirror
     was traveling at the time of the output pulse. A bit value of 1 is a positive scan direction, and a
     bit value of 0 is a negative scan direction (where positive scan direction is a scan moving
     from the left side of the in-track direction to the right side and negative the opposite). */
  lasf_ptrt.class_flags = (pt_rt >> 0) & ((1 << 4) - 1);
  /* Edge of Flight Line: The Edge of Flight Line data bit has a value of 1 only when the point is at
     the end of a scan. It is the last point on a given scan line before it changes direction. */
  lasf_ptrt.scan_chan = (pt_rt >> 4) & ((1 << 2) - 1);
  lasf_ptrt.scan_dir = (pt_rt >> 6) & ((1 << 1) - 1);
  lasf_ptrt.flight_edge = (pt_rt >> 7) & ((1 << 1) - 1);

  return lasf_ptrt;
}

/* 
   Compare the / separated values in 'lasl_clr against 
   the int 'classification value from the LAS point 
   record. Will return 1 if 'classification matches one 
   of the values in 'lasl_clr 
*/
int
lasf_cmpclass(char* lasl_clr, int classification) {
  int pflag = 0;

  char* p = strtok(lasl_clr, "/");
  while(p != NULL) {
    if ((atoi(p) == classification))    
      pflag++;
    p = strtok(NULL, "/");
  }
  return pflag; 
}

/* Given an LAS point record's returns value 'lasf_ptrt', 
   add to it's corresponding array loaction.*/
void
lasf_fill_by_return(unsigned int (*lasf_ptbr)[5], lasf_pntreturns lasf_ptrt) {
  (*lasf_ptbr)[lasf_ptrt.return_num - 1]++;
}

void
lasf_fill_by_class(unsigned int (*lasf_clsr)[32], int classification) {
  (*lasf_clsr)[classification]++;
}

/* 
   Given a header id, a lasf_pnts struct and a lasf_describe struct, 
   add to the lasf_describe struct based on the contents of the lasf_pnts struct 
*/
int
lasf_add_to_stats(int* lasfhid, lasf_pnts lasf_ptbuff0, lasf_describe* llmm, int init) {
  lasf_pntreturns lasf_ptrt;
  double x, y, z, gpstime;
  unsigned short intensity;
  int j, l, status;

  lasf_header lasf_hbuffer0;

  if (lasfheaderid_array_cnt == 0) {
      lasf_file_status(&status, "There is currently no valid LAS file header open", "");
      return status;
    }

  lasf_get_header(lasfhid, &lasf_hbuffer0);

  if (init == 0) {
    for (l = 0; l < 32; l++) {
      llmm->classifications[l] = 0;
    }
      
    for (j = 0; j < 5; j++) {
      llmm->returns[j] = 0;
    }

    for (j = 0; j < 15; j++) {
      llmm->returns6[j] = 0;
    }

    if (lasf_hbuffer0.pointformat == 6) {
      lasf_ptrt = lasf_parse_point_returns6(lasf_ptbuff0.returns);
    } else {
      lasf_ptrt = lasf_parse_point_returns(lasf_ptbuff0.returns);
    }
    llmm->n = 1;
    llmm->xmax = lasf_ptbuff0.x * lasf_hbuffer0.xscale + lasf_hbuffer0.xoffset;
    llmm->ymax = lasf_ptbuff0.y * lasf_hbuffer0.yscale + lasf_hbuffer0.yoffset;
    llmm->zmax = lasf_ptbuff0.z * lasf_hbuffer0.zscale + lasf_hbuffer0.zoffset;
    llmm->xmin = lasf_ptbuff0.x * lasf_hbuffer0.xscale + lasf_hbuffer0.xoffset;
    llmm->ymin = lasf_ptbuff0.y * lasf_hbuffer0.yscale + lasf_hbuffer0.yoffset;
    llmm->zmin = lasf_ptbuff0.z * lasf_hbuffer0.zscale + lasf_hbuffer0.zoffset;
    llmm->intensitymax = lasf_ptbuff0.intensity;
    llmm->intensitymin = lasf_ptbuff0.intensity;
    llmm->gpstimemax = lasf_ptbuff0.gpstime;
    llmm->gpstimemin = lasf_ptbuff0.gpstime;
    llmm->classifications[ lasf_ptbuff0.classification ]++;
    //if (lasf_hbuffer0.pointformat == 6) {
    llmm->returns6[ lasf_ptrt.return_num - 1 ]++;
    //} else {
    llmm->returns[ lasf_ptrt.return_num - 1 ]++;
    //}
  }
  else {
    llmm->n = init + 1;
    if (lasf_hbuffer0.pointformat == 6) {
      lasf_ptrt = lasf_parse_point_returns6(lasf_ptbuff0.returns);
    }  else {
      lasf_ptrt = lasf_parse_point_returns(lasf_ptbuff0.returns);
    }
    x = lasf_ptbuff0.x * lasf_hbuffer0.xscale + lasf_hbuffer0.xoffset;
    y = lasf_ptbuff0.y * lasf_hbuffer0.yscale + lasf_hbuffer0.yoffset;
    z = lasf_ptbuff0.z * lasf_hbuffer0.zscale + lasf_hbuffer0.zoffset;
    intensity = lasf_ptbuff0.intensity;
    gpstime = lasf_ptbuff0.gpstime;
      
    if ((x >= llmm->xmax))
      llmm->xmax = x;
    if ((y >= llmm->ymax))
      llmm->ymax = y;
    if ((z >= llmm->zmax))
      llmm->zmax = z;
    if ((x <= llmm->xmin))
      llmm->xmin = x;
    if ((y <= llmm->ymin))
      llmm->ymin = y;
    if ((z <= llmm->zmin))
      llmm->zmin = z;
    if ((intensity <= llmm->intensitymin))
      llmm->intensitymin = intensity;
    if ((intensity >= llmm->intensitymax))
      llmm->intensitymax = intensity;
    if ((gpstime <= llmm->gpstimemin))
      llmm->gpstimemin = gpstime;
    if ((gpstime >= llmm->gpstimemax))
      llmm->gpstimemax = gpstime;

    llmm->classifications[ lasf_ptbuff0.classification ]++;
    //if (lasf_hbuffer0.pointformat == 6) {
    llmm->returns6[ lasf_ptrt.return_num - 1 ]++;
    //} else {
    llmm->returns[ lasf_ptrt.return_num - 1 ]++;
    //}
  }
  
  return lasf_NOERR;
}
