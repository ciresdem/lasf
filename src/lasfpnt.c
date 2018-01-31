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

/*
  Read an lasf_pnts struct from file, based on the las file 
  id and the las file header id. If seekn is -1 or less, 
  will read from current position of the input file associated 
  with the lasfid.  If seekn is 0 or above, will read from that 
  point numbers position, zero is the first point.
 */
size_t
lasf_read_point(int lasfid, int lasfhid, int seekn, lasf_pnts* lasf_ptbuff) {
  FILE *fp;
  lasf_header lasf_hbuffer;
  int status;

  lasf_pnts0 lasf_ptbuff0;
  lasf_pnts1 lasf_ptbuff1;
  lasf_pnts2 lasf_ptbuff2;
  lasf_pnts3 lasf_ptbuff3;
  lasf_pnts4 lasf_ptbuff4;
  lasf_pnts lasf_ptbuff5;
  lasf_pnts6 lasf_ptbuff6;
  lasf_pnts7 lasf_ptbuff7;
  lasf_pnts8 lasf_ptbuff8;
  lasf_pnts9 lasf_ptbuff9;
  lasf_pnts10 lasf_ptbuff10;

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
  
  if (lasf_hbuffer.pointformat == 0) {
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
    lasf_ptbuff->wavedesc = 0;
    lasf_ptbuff->waveoffset = 0;
    lasf_ptbuff->wavereturn = 0;
    lasf_ptbuff->x_t = 0;
    lasf_ptbuff->y_t = 0;
    lasf_ptbuff->z_t = 0;
  }
  else if (lasf_hbuffer.pointformat == 1) {
    status = fread(&lasf_ptbuff1, lasf_hbuffer.pointreclen, 1, fp);
    lasf_ptbuff->x = lasf_ptbuff1.x;
    lasf_ptbuff->y = lasf_ptbuff1.y;
    lasf_ptbuff->z = lasf_ptbuff1.z;
    lasf_ptbuff->intensity = lasf_ptbuff1.intensity;
    lasf_ptbuff->returns = lasf_ptbuff1.returns;
    lasf_ptbuff->classification = lasf_ptbuff1.classification;
    lasf_ptbuff->scanrank = lasf_ptbuff1.scanrank;
    lasf_ptbuff->udata = lasf_ptbuff1.udata;
    lasf_ptbuff->psrcid = lasf_ptbuff1.psrcid;
    lasf_ptbuff->gpstime = lasf_ptbuff1.gpstime;
    lasf_ptbuff->red = 0;
    lasf_ptbuff->green = 0;
    lasf_ptbuff->blue = 0;
    lasf_ptbuff->wavedesc = 0;
    lasf_ptbuff->waveoffset = 0;
    lasf_ptbuff->wavereturn = 0;
    lasf_ptbuff->x_t = 0;
    lasf_ptbuff->y_t = 0;
    lasf_ptbuff->z_t = 0;
  }
  else if (lasf_hbuffer.pointformat == 2) {
    status = fread(&lasf_ptbuff2, lasf_hbuffer.pointreclen, 1, fp);
    lasf_ptbuff->x = lasf_ptbuff2.x;
    lasf_ptbuff->y = lasf_ptbuff2.y;
    lasf_ptbuff->z = lasf_ptbuff2.z;
    lasf_ptbuff->intensity = lasf_ptbuff2.intensity;
    lasf_ptbuff->returns = lasf_ptbuff2.returns;
    lasf_ptbuff->classification = lasf_ptbuff2.classification;
    lasf_ptbuff->scanrank = lasf_ptbuff2.scanrank;
    lasf_ptbuff->udata = lasf_ptbuff2.udata;
    lasf_ptbuff->psrcid = lasf_ptbuff2.psrcid;
    lasf_ptbuff->gpstime = -1;
    lasf_ptbuff->red = lasf_ptbuff2.red;
    lasf_ptbuff->green = lasf_ptbuff2.green;
    lasf_ptbuff->blue = lasf_ptbuff2.blue;
    lasf_ptbuff->wavedesc = 0;
    lasf_ptbuff->waveoffset = 0;
    lasf_ptbuff->wavereturn = 0;
    lasf_ptbuff->x_t = 0;
    lasf_ptbuff->y_t = 0;
    lasf_ptbuff->z_t = 0;
  }
  else if (lasf_hbuffer.pointformat == 3) {
    status = fread(&lasf_ptbuff3, lasf_hbuffer.pointreclen, 1, fp);
    lasf_ptbuff->x = lasf_ptbuff3.x;
    lasf_ptbuff->y = lasf_ptbuff3.y;
    lasf_ptbuff->z = lasf_ptbuff3.z;
    lasf_ptbuff->intensity = lasf_ptbuff3.intensity;
    lasf_ptbuff->returns = lasf_ptbuff3.returns;
    lasf_ptbuff->classification = lasf_ptbuff3.classification;
    lasf_ptbuff->scanrank = lasf_ptbuff3.scanrank;
    lasf_ptbuff->udata = lasf_ptbuff3.udata;
    lasf_ptbuff->psrcid = lasf_ptbuff3.psrcid;
    lasf_ptbuff->gpstime = lasf_ptbuff3.gpstime;
    lasf_ptbuff->red = lasf_ptbuff3.red;
    lasf_ptbuff->green = lasf_ptbuff3.green;
    lasf_ptbuff->blue = lasf_ptbuff3.blue;
    lasf_ptbuff->wavedesc = 0;
    lasf_ptbuff->waveoffset = 0;
    lasf_ptbuff->wavereturn = 0;
    lasf_ptbuff->x_t = 0;
    lasf_ptbuff->y_t = 0;
    lasf_ptbuff->z_t = 0;
  }
  else if (lasf_hbuffer.pointformat == 4) {
    status = fread(&lasf_ptbuff4, lasf_hbuffer.pointreclen, 1, fp);
    lasf_ptbuff->x = lasf_ptbuff4.x;
    lasf_ptbuff->y = lasf_ptbuff4.y;
    lasf_ptbuff->z = lasf_ptbuff4.z;
    lasf_ptbuff->intensity = lasf_ptbuff4.intensity;
    lasf_ptbuff->returns = lasf_ptbuff4.returns;
    lasf_ptbuff->classification = lasf_ptbuff4.classification;
    lasf_ptbuff->scanrank = lasf_ptbuff4.scanrank;
    lasf_ptbuff->udata = lasf_ptbuff4.udata;
    lasf_ptbuff->psrcid = lasf_ptbuff4.psrcid;
    lasf_ptbuff->gpstime = lasf_ptbuff4.gpstime;
    lasf_ptbuff->wavedesc = lasf_ptbuff4.wavedesc;
    lasf_ptbuff->waveoffset = lasf_ptbuff4.waveoffset;
    lasf_ptbuff->wavereturn = lasf_ptbuff4.wavereturn;
    lasf_ptbuff->x_t = lasf_ptbuff4.x_t;
    lasf_ptbuff->y_t = lasf_ptbuff4.y_t;
    lasf_ptbuff->z_t = lasf_ptbuff4.z_t;
  }
  else if (lasf_hbuffer.pointformat == 5) {
    status = fread(&lasf_ptbuff5, lasf_hbuffer.pointreclen, 1, fp);
    lasf_ptbuff->x = lasf_ptbuff5.x;
    lasf_ptbuff->y = lasf_ptbuff5.y;
    lasf_ptbuff->z = lasf_ptbuff5.z;
    lasf_ptbuff->intensity = lasf_ptbuff5.intensity;
    lasf_ptbuff->returns = lasf_ptbuff5.returns;
    lasf_ptbuff->classification = lasf_ptbuff5.classification;
    lasf_ptbuff->scanrank = lasf_ptbuff5.scanrank;
    lasf_ptbuff->udata = lasf_ptbuff5.udata;
    lasf_ptbuff->psrcid = lasf_ptbuff5.psrcid;
    lasf_ptbuff->gpstime = lasf_ptbuff5.gpstime;
    lasf_ptbuff->red = lasf_ptbuff3.red;
    lasf_ptbuff->green = lasf_ptbuff3.green;
    lasf_ptbuff->blue = lasf_ptbuff3.blue;
    lasf_ptbuff->wavedesc = lasf_ptbuff5.wavedesc;
    lasf_ptbuff->waveoffset = lasf_ptbuff5.waveoffset;
    lasf_ptbuff->wavereturn = lasf_ptbuff5.wavereturn;
    lasf_ptbuff->x_t = lasf_ptbuff5.x_t;
    lasf_ptbuff->y_t = lasf_ptbuff5.y_t;
    lasf_ptbuff->z_t = lasf_ptbuff5.z_t;
  }
  else if (lasf_hbuffer.pointformat == 6) {
    status = fread(&lasf_ptbuff6, lasf_hbuffer.pointreclen, 1, fp);
    lasf_ptbuff->x = lasf_ptbuff6.x;
    lasf_ptbuff->y = lasf_ptbuff6.y;
    lasf_ptbuff->z = lasf_ptbuff6.z;
    lasf_ptbuff->intensity = lasf_ptbuff6.intensity;
    lasf_ptbuff->returns = lasf_ptbuff6.returns;
    lasf_ptbuff->classification = lasf_ptbuff6.classification;
    lasf_ptbuff->udata = lasf_ptbuff6.udata;
    lasf_ptbuff->scanrank = lasf_ptbuff6.scanrank;
    lasf_ptbuff->psrcid = lasf_ptbuff6.psrcid;
    lasf_ptbuff->gpstime = lasf_ptbuff6.gpstime;
  }
  /* else if (lasf_hbuffer.pointformat == 168) { */
  /*   //status = fread(&lasf_ptbuff4, lasf_hbuffer.pointreclen, 1, fp); */
  /*   status = lasf_read_xyz_line(&lasf_ptbuff4, fp, " ", "xyz"); */
  /*   lasf_ptbuff->x = lasf_ptbuff4.x; */
  /*   lasf_ptbuff->y = lasf_ptbuff4.y; */
  /*   lasf_ptbuff->z = lasf_ptbuff4.z; */
  /*   lasf_ptbuff->intensity = lasf_ptbuff4.intensity; */
  /*   lasf_ptbuff->returns = lasf_ptbuff4.returns; */
  /*   lasf_ptbuff->classification = lasf_ptbuff4.classification; */
  /*   lasf_ptbuff->scanrank = lasf_ptbuff4.scanrank; */
  /*   lasf_ptbuff->udata = lasf_ptbuff4.udata; */
  /*   lasf_ptbuff->psrcid = lasf_ptbuff4.psrcid; */
  /*   lasf_ptbuff->gpstime = lasf_ptbuff4.gpstime; */
  /*   lasf_ptbuff->wavedesc = lasf_ptbuff4.wavedesc; */
  /*   lasf_ptbuff->waveoffset = lasf_ptbuff4.waveoffset; */
  /*   lasf_ptbuff->wavereturn = lasf_ptbuff4.wavereturn; */
  /*   lasf_ptbuff->x_t = lasf_ptbuff4.x_t; */
  /*   lasf_ptbuff->y_t = lasf_ptbuff4.y_t; */
  /*   lasf_ptbuff->z_t = lasf_ptbuff4.z_t; */
  /* } */
  if (status == 1) {
    return lasf_NOERR;
  }
  else {
    lasf_file_status(&status, "Failed to read point", "");
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
  lasf_pnts0 lasf_ptbuff0;
  lasf_pnts1 lasf_ptbuff1;
  lasf_pnts2 lasf_ptbuff2;
  lasf_pnts3 lasf_ptbuff3;
  lasf_pnts4 lasf_ptbuff4;
  lasf_pnts lasf_ptbuff5;
  lasf_pnts6 lasf_ptbuff6;

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

  if (lasfh.pointformat == 0) {
    lasf_ptbuff0.x = lasf_ptbuff->x;
    lasf_ptbuff0.y = lasf_ptbuff->y;
    lasf_ptbuff0.z = lasf_ptbuff->z;
    lasf_ptbuff0.intensity = lasf_ptbuff->intensity;
    lasf_ptbuff0.returns = lasf_ptbuff->returns;
    lasf_ptbuff0.classification = lasf_ptbuff->classification;
    lasf_ptbuff0.scanrank = lasf_ptbuff->scanrank;
    lasf_ptbuff0.udata = lasf_ptbuff->udata;
    lasf_ptbuff0.psrcid = lasf_ptbuff->psrcid;
    status = fwrite(&lasf_ptbuff0,  sizeof(lasf_ptbuff0), 1, lasout);
  }
  else if (lasfh.pointformat == 1) {
    lasf_ptbuff1.x = lasf_ptbuff->x;
    lasf_ptbuff1.y = lasf_ptbuff->y;
    lasf_ptbuff1.z = lasf_ptbuff->z;
    lasf_ptbuff1.intensity = lasf_ptbuff->intensity;
    lasf_ptbuff1.returns = lasf_ptbuff->returns;
    lasf_ptbuff1.classification = lasf_ptbuff->classification;
    lasf_ptbuff1.scanrank = lasf_ptbuff->scanrank;
    lasf_ptbuff1.udata = lasf_ptbuff->udata;
    lasf_ptbuff1.psrcid = lasf_ptbuff->psrcid;
    lasf_ptbuff1.gpstime = lasf_ptbuff->gpstime;
    status = fwrite(&lasf_ptbuff1,  sizeof(lasf_ptbuff1), 1, lasout);
  }
  else if (lasfh.pointformat == 2) {
    lasf_ptbuff2.x = lasf_ptbuff->x;
    lasf_ptbuff2.y = lasf_ptbuff->y;
    lasf_ptbuff2.z = lasf_ptbuff->z;
    lasf_ptbuff2.intensity = lasf_ptbuff->intensity;
    lasf_ptbuff2.returns = lasf_ptbuff->returns;
    lasf_ptbuff2.classification = lasf_ptbuff->classification;
    lasf_ptbuff2.scanrank = lasf_ptbuff->scanrank;
    lasf_ptbuff2.udata = lasf_ptbuff->udata;
    lasf_ptbuff2.psrcid = lasf_ptbuff->psrcid;
    lasf_ptbuff2.red = lasf_ptbuff->red;
    lasf_ptbuff2.green = lasf_ptbuff->green;
    lasf_ptbuff2.blue = lasf_ptbuff->blue;
    status = fwrite(&lasf_ptbuff2,  sizeof(lasf_ptbuff2), 1, lasout);
  }
  else if (lasfh.pointformat == 3) {
    lasf_ptbuff3.x = lasf_ptbuff->x;
    lasf_ptbuff3.y = lasf_ptbuff->y;
    lasf_ptbuff3.z = lasf_ptbuff->z;
    lasf_ptbuff3.intensity = lasf_ptbuff->intensity;
    lasf_ptbuff3.returns = lasf_ptbuff->returns;
    lasf_ptbuff3.classification = lasf_ptbuff->classification;
    lasf_ptbuff3.scanrank = lasf_ptbuff->scanrank;
    lasf_ptbuff3.udata = lasf_ptbuff->udata;
    lasf_ptbuff3.psrcid = lasf_ptbuff->psrcid;
    lasf_ptbuff3.gpstime = lasf_ptbuff->gpstime;
    lasf_ptbuff3.red = lasf_ptbuff->red;
    lasf_ptbuff3.green = lasf_ptbuff->green;
    lasf_ptbuff3.blue = lasf_ptbuff->blue;
    status = fwrite(&lasf_ptbuff3,  sizeof(lasf_ptbuff3), 1, lasout);
  }
  else if (lasfh.pointformat == 4) {
    lasf_ptbuff4.x = lasf_ptbuff->x;
    lasf_ptbuff4.y = lasf_ptbuff->y;
    lasf_ptbuff4.z = lasf_ptbuff->z;
    lasf_ptbuff4.intensity = lasf_ptbuff->intensity;
    lasf_ptbuff4.returns = lasf_ptbuff->returns;
    lasf_ptbuff4.classification = lasf_ptbuff->classification;
    lasf_ptbuff4.scanrank = lasf_ptbuff->scanrank;
    lasf_ptbuff4.udata = lasf_ptbuff->udata;
    lasf_ptbuff4.psrcid = lasf_ptbuff->psrcid;
    lasf_ptbuff4.gpstime = lasf_ptbuff->gpstime;
    lasf_ptbuff4.wavedesc = lasf_ptbuff->wavedesc;
    lasf_ptbuff4.waveoffset = lasf_ptbuff->waveoffset;
    lasf_ptbuff4.wavereturn = lasf_ptbuff->wavereturn;
    lasf_ptbuff4.x_t = lasf_ptbuff->x_t;
    lasf_ptbuff4.y_t = lasf_ptbuff->y_t;
    lasf_ptbuff4.z_t = lasf_ptbuff->z_t;
    status = fwrite(&lasf_ptbuff0,  sizeof(lasf_ptbuff4), 1, lasout);
  }
  else if (lasfh.pointformat == 5) {
    lasf_ptbuff5.x = lasf_ptbuff->x;
    lasf_ptbuff5.y = lasf_ptbuff->y;
    lasf_ptbuff5.z = lasf_ptbuff->z;
    lasf_ptbuff5.intensity = lasf_ptbuff->intensity;
    lasf_ptbuff5.returns = lasf_ptbuff->returns;
    lasf_ptbuff5.classification = lasf_ptbuff->classification;
    lasf_ptbuff5.scanrank = lasf_ptbuff->scanrank;
    lasf_ptbuff5.udata = lasf_ptbuff->udata;
    lasf_ptbuff5.psrcid = lasf_ptbuff->psrcid;
    lasf_ptbuff5.gpstime = lasf_ptbuff->gpstime;
    lasf_ptbuff5.red = lasf_ptbuff->red;
    lasf_ptbuff5.green = lasf_ptbuff->green;
    lasf_ptbuff5.blue = lasf_ptbuff->blue;
    lasf_ptbuff5.wavedesc = lasf_ptbuff->wavedesc;
    lasf_ptbuff5.waveoffset = lasf_ptbuff->waveoffset;
    lasf_ptbuff5.wavereturn = lasf_ptbuff->wavereturn;
    lasf_ptbuff5.x_t = lasf_ptbuff->x_t;
    lasf_ptbuff5.y_t = lasf_ptbuff->y_t;
    lasf_ptbuff5.z_t = lasf_ptbuff->z_t;
    status = fwrite(&lasf_ptbuff0,  sizeof(lasf_ptbuff5), 1, lasout);
  }
  else if (lasfh.pointformat == 6) {
    lasf_ptbuff6.x = lasf_ptbuff->x;
    lasf_ptbuff6.y = lasf_ptbuff->y;
    lasf_ptbuff6.z = lasf_ptbuff->z;
    lasf_ptbuff6.intensity = lasf_ptbuff->intensity;
    lasf_ptbuff6.returns = lasf_ptbuff->returns;
    lasf_ptbuff6.classification = lasf_ptbuff->classification;
    lasf_ptbuff6.udata = lasf_ptbuff->udata;
    lasf_ptbuff6.scanrank = lasf_ptbuff->scanrank;
    lasf_ptbuff6.psrcid = lasf_ptbuff->psrcid;
    lasf_ptbuff6.gpstime = lasf_ptbuff->gpstime;
    status = fwrite(&lasf_ptbuff0, sizeof(lasf_ptbuff6), 1, lasout);
  }
  
  if (status == 1) {
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

/* int */
/* lasf_insideply(char* poly, lasf_pnts lasf_ptbuff0) { */
/*   // open poly */
/*   GDALAllRegister(); */
/*   GDALDatasetH hDS; */

/*   hDS = GDALOpenEx( poly, GDAL_OF_VECTOR, NULL, NULL, NULL ); */
/*   if( hDS == NULL ) { */
/*     printf( "lasf: OGR Open failed.\n" ); */
/*     exit( 1 ); */
/*   } */

/*   OGRLayerH hLayer; */
  
/*   hLayer = GDALDatasetGetLayerByName( hDS, "polygon" ); */
  
/*   OGRFeatureH hFeature; */
  
/*   OGR_L_ResetReading(hLayer); */
/*   while( (hFeature = OGR_L_GetNextFeature(hLayer)) != NULL ) { */
/*     OGRFeatureDefnH hFDefn = OGR_L_GetLayerDefn(hLayer); */
/*     int iField; */
    
/*     for( iField = 0; iField < OGR_FD_GetFieldCount(hFDefn); iField++ ) { */
/*       OGRFieldDefnH hFieldDefn = OGR_FD_GetFieldDefn( hFDefn, iField ); */
      
/*       OGRGeometryH hGeometry; */
      
/*       hGeometry = OGR_F_GetGeometryRef(hFeature); */
/*       if( hGeometry != NULL ) { */
/* 	//exportToWkt() */
/* 	//printf( "%.3f,%3.f\n", OGR_G_GetX(hGeometry, 0), OGR_G_GetY(hGeometry, 0) ); */
/*       } */
/*       else { */
/* 	printf( "no geometry\n" ); */
/*       } */
/*     } */
/*     OGR_F_Destroy( hFeature ); */
/*   } */
/* } */

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
