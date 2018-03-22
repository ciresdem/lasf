/*-----------------------------------------------------------------
 *
 *  lasfilter.c 
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
 *----------------------------------------------------------------*/

#include<getopt.h>
#include<lasf.h>
#include<gdal/ogr_api.h>

#define LASFILTER_VERSION "0.3.3"

/* Flags set by `--version' and `--help' */
int version_flag;
int help_flag;

typedef struct {
  double x;
  double y;
}point_t;

typedef struct {
  point_t p1, p2;
}line_t;

double
lasf_ccw(point_t* p1, point_t* p2, point_t* p3) {
  return (p2->x - p1->x) * (p3->y - p1->y) - (p2->y - p1->y) * (p3->x - p1->x);
}

int
lasf_intersect(line_t l1, line_t l2, float thresh) {
  double a, b;
  a = lasf_ccw(&l1.p1, &l1.p2, &l2.p1) *lasf_ccw(&l1.p1, &l1.p2, &l2.p2);
  b = lasf_ccw(&l2.p1, &l2.p2, &l1.p1) *lasf_ccw(&l2.p1, &l2.p2, &l1.p2);

  if (a < 0-thresh && b < 0-thresh) return 1;
  else if (a >= 0-thresh && a <= thresh) return 0;
  else if (b >= 0-thresh && b <= thresh) return 0;
  /* if (a < 0 && b < 0) return 1; */
  /* else if (a == 0 || b == 0) return 0; */
  else return -1;
}

int
lasf_ogr_inside(point_t* p1, OGRLayerH hLayer) {
  int k = 0, l, j, jj, j0, j1, i, fCount, gCount;
  line_t lt, lp;
  OGRFeatureH hFeature;
  OGRGeometryH hGeometry, ring;
  
  OGR_L_ResetReading(hLayer);

  lt.p1 = *p1, lt.p2.x = p1->x, lt.p2.y = INT_MAX;
  int t = OGR_L_GetFeatureCount(hLayer, FALSE);
  //while((hFeature = OGR_L_GetNextFeature(hLayer)) != NULL) {
  int h;
  for (h = 0; h < t; h++) { 
    hFeature = OGR_L_GetFeature(hLayer,h);
    k=0;
    hGeometry = OGR_F_GetGeometryRef(hFeature), gCount = OGR_G_GetGeometryCount(hGeometry);
    for (i = 0; i<gCount; i++) {
      ring = OGR_G_GetGeometryRef(hGeometry, i), fCount = OGR_G_GetPointCount(ring);
      for (j = 0; j<fCount-1; j++) {
	j0 = j, j1 = j+1;
	lp.p1.x = OGR_G_GetX(ring,j), lp.p1.y = OGR_G_GetY(ring,j);
	lp.p2.x = OGR_G_GetX(ring,j+1), lp.p2.y = OGR_G_GetY(ring,j+1);
	l = lasf_intersect(lt,lp,0.000000001);
	if (l == 0) {
	  OGR_F_Destroy(hFeature);
	  return 1;
	}
	if (l == 1) k++;
      }
    }
    if (k & 1 == 1) {
      OGR_F_Destroy(hFeature);
      return 1;
    }
    OGR_F_Destroy(hFeature);
  }
  return 0;
}

int
lasf_point_in_region_p(char* las_region, int* lasfhid, lasf_pnts lasfp) {
  int i, status, rflag=0;
  double in_region[4];
  double x, y;

  lasf_header lasf_hbuffer0;

  if (lasfheaderid_array_cnt == 0) {
    lasf_file_status(&status, "There is currently no valid LAS file header open", "");
    return status;
  }
  lasf_get_header(lasfhid, &lasf_hbuffer0);
  
  x = lasfp.x * lasf_hbuffer0.xscale + lasf_hbuffer0.xoffset;
  y = lasfp.y * lasf_hbuffer0.yscale + lasf_hbuffer0.yoffset;
  char* p = strtok(las_region,"/");

  for (i = 0; i < 4; i++) {
    in_region[i] = atof(p);
    p = strtok(NULL, "/");
  }
  
  if (x >= in_region[0] &&			\
      x <= in_region[1] &&			\
      y >= in_region[2] &&			\
      y <= in_region[3]) rflag++;
  
  return rflag;
}

void 
usage() {
  fprintf(stderr, "\
lasfilter infile(s) [Options]\n\n\
  Perform a various reclassification filters on the given LAS file(s).\n\
\n\
  infile(s)\tThe input LAS file(s) \n\
\n\
 Options:\n\
  -c, --classification\tInput classification(s); separate values \n\
\t\t\twith a '/'; e.g. -c 2/9/11 to select classes 2, 9 and 11.\n\
\t\t\tLeave blank to specify all classificatins as input\n\
  -p, --polygon\t\tSpecify an input OGR supported vector file. Only points within the\n\
\t\t\tgiven polygon will be filtered.\n\
  -r, --region\t\tSelect by region; The region should be formated as:\n\
\t\t\tminX/maxX/minY/maxY, using a '/' as a separater.\n\
  -f, --filter\t\tThe desired filter to use. Separate filter option\n\
              \t\tvalues with a \":\"(e.g. 1:10:1 to use the \"Min-Block\"\n\
              \t\tfilter with a block size of 10 and a diffval of 1)\n\
     Current Filters:\n\n\
   0: Reclassify Filter - Reclassify the specified points to the given classification.\n\
     Options:\n\
      (int) The desired output classification value (between 0 and 32).\n\n\
   1: \"Min-Block\" Ground Filter - Classify the specified points as ground (2) based on their relative\n\
                                  height compared to the minimum value in each respective block\n\
     Options:\n\
      (double) The desired blocking increment. [should be in the same \n\
               units and the input data]\n\
      (double) The maximum difference value.\n\n\
   2: \"Mean-Block\" Ground Filter - Classify the specified points as ground (2) based on their relative\n\
                                   height compared to the mean value in each respective block.\n\
     Options:\n\
      (double) The desired blocking increment. [should be in the same \n\
               units as the input data]\n\
      (double) The maximum difference value.\n\n\
   3: Height Filter - Reclassify points to the given classification which are above or below the \n\
                      specified height value.\n\
     Options:\n\
      (int)    Lower/Higher (specify 0 if you want to reclassify points below the the given height \n\
               value and 1 to reclassify points above the given height value.)\n\
      (double) The desired height value.\n\
      (int)    The desired output classification value (between 0 and 32).\n\n\
   4: Intensity Filter - Reclassify points to the given classification which are within the range of \n\
                         the specified intensity values \n\
     Options:\n\
      (int)    The lower range intensity value. \n\
      (int)    The upper range intensity value. \n\
      (int)    The desired output classification value (between 0 and 32).\n\n\
   5: Density Filter - Reclassify points to the given classification based on the density of the \n\
                       given block size.\n\
     Options:\n\
      (double) The desired blocking increment. [should be in the same \n\
               units as the input data]\n\
      (int)    Lower/Higher (specify 0 if you want to reclassify points below the the given density \n\
               value and 1 to reclassify points above the given density value.)\n\
      (double) The desired density value.\n\
      (int)    The desired output classification value (between 0 and 32).\n\n\
     Note: Use -c to specify which points to consider in filtering, default considers all points.\n\n\
\n\
  --help\tPrint this help menu and exit.\n\
  --version\tPrint version information and exit.\n\n\
");
  exit(1);
}

/*
 * Reclassify Filter - Classify all points to a given classification.
 */
int
lasf_reclassify_fltr(int* lasfid, int* lasfhid, char* lasf_clr, char* lasf_region, OGRDataSourceH hDS, char* lasf_filter_args) {
  int i, n, status;
  lasf_pnts lasf_ptbuff;
  lasf_header lasf_hbuffer;
  int out_class = 0, printflag = 0;
  char cbuff[64], rbuff[64];
  point_t p1;
  
  /* 
   * Parse the lasf_filter_args
   */
  char* p = strtok(lasf_filter_args, ":");
  if (p != NULL) out_class = atoi(p);

  /*
   * Make sure the user supplied classification value is valid
   */
  if (out_class < 0 || out_class > 32) {
    lasf_file_status(&status, "The given output class is not valid", p);
    return status;
  }
  
  lasf_get_header(lasfid, &lasf_hbuffer);

  /* 
   * clfag tells whether to check for classifications, 
   * if 0 classification selection is skipped, if 1 check 
   * classifications against 'lasf_clr. printflag tells
   * whether to print the record, 0 will not print it, 
   * but 1 will
   */
  int cflag = 0, pflag = 0, rflag = 0;
  OGRLayerH hLayer;

  if (!(strlen(lasf_clr) == 0)) cflag++;
  if (hDS != NULL) {
    pflag++;
    hLayer = OGR_DS_GetLayer(hDS, 0);
  }
  if (!(strlen(lasf_region) == 0)) rflag++;

  fprintf(stderr,"cflag: %d\n", cflag);
  fprintf(stderr,"pflag: %d\n", pflag);
  fprintf(stderr,"rflag: %d\n", rflag);

  /* 
   * Loop through the point records and reclassify accordingly 
   */
  i = 0, n = 0;
  while (lasf_read_point(*lasfid, *lasfhid, n, &lasf_ptbuff) == lasf_NOERR) {
    //lasf_printf_progress(abs(n), lasf_hbuffer.numptrecords);

    /* 
     * If the select-by-classification flag is set, check if to print 
     */
    if (cflag == 1) {
      strcpy(cbuff, lasf_clr);
      if (lasf_cmpclass(cbuff, lasf_ptbuff.classification) == 1) printflag++;
    }

    /* 
     * If the select-by-polygon flag is set, check if to print 
     */
    if (pflag == 1 && printflag == cflag) {
      p1.x = lasf_ptbuff.x * lasf_hbuffer.xscale + lasf_hbuffer.xoffset;
      p1.y = lasf_ptbuff.y * lasf_hbuffer.yscale + lasf_hbuffer.yoffset;
      if (lasf_ogr_inside(&p1, hLayer)) printflag++;
      fprintf(stderr,"printflag: %d\n", printflag);
    }

    /* 
     * If the select-by-bounding-box flag is set, check if to print 
     */
    if (rflag == 1 && printflag == cflag + pflag) {
      strcpy(rbuff, lasf_region);
      if (lasf_point_in_region_p(rbuff, lasfhid, lasf_ptbuff)) printflag++;
    }

    /* 
     * If the printflag is set, print and gather. 
     */
    if (printflag == (cflag + pflag + rflag)) {
      if (lasf_ptbuff.classification != out_class) {
	lasf_ptbuff.classification = out_class;
	status = lasf_write_point(*lasfid, *lasfhid, i, &lasf_ptbuff);	
      }
    }
    printflag = 0;
    i++, n--;
  }
  return(0);
}

/*
 * Height filter - Classify all points to a given classification based on z value
 */
int
lasf_height_fltr(int* lasfid, int* lasfhid, char* lasf_clr, OGRDataSourceH hDS, char* lasf_filter_args) {
  int i, n, status, highlow;
  lasf_pnts lasf_ptbuff;
  lasf_header lasf_hbuffer;
  int out_class = 0, printflag = 0;
  char cbuff[64];
  double hval;
  point_t p1;

  /*
   * Parse the lasf_filter_args
   */
  char* p = strtok(lasf_filter_args, ":");
  if (p != NULL) highlow = atoi(p);
  else highlow = 0; // default is low
  p = strtok(NULL, ":");
  if (p != NULL) hval = atof(p);
  else hval = 1;
  p = strtok(NULL, ":");
  if (p != NULL) out_class = atoi(p);
  else out_class = 2;

  /*
   * Make sure the user supplied classification value is valid
   */
  if (out_class < 0 || out_class > 32) {
    lasf_file_status(&status, "The given output class is not valid", p);
    return status;
  }
  
  lasf_get_header(lasfid, &lasf_hbuffer);

  /* 
   * clfag tells whether to check for classifications, 
   * if 0 classification selection is skipped, if 1 check 
   * classifications against 'lasf_clr. printflag tells
   * whether to print the record, 0 will not print it, 
   * but 1 will
   */
  int cflag = 0, pflag = 0;
  OGRLayerH hLayer;
  
  if (!(strlen(lasf_clr) == 0)) cflag++;
  if (hDS != NULL) {
    pflag++;
    hLayer = OGR_DS_GetLayer(hDS, 0);
  }

  /* 
   * Loop through the point records and reclassify accordingly 
   */
  i = 0, n = 0;
  while (lasf_read_point(*lasfid, *lasfhid, n, &lasf_ptbuff) == lasf_NOERR) {
    //lasf_printf_progress(abs(n), lasf_hbuffer.numptrecords);

    /* 
     * If the select-by-classification flag is set, check if to print 
     */
    if (cflag == 1) {
      strcpy(cbuff, lasf_clr);
      if (lasf_cmpclass(cbuff, lasf_ptbuff.classification) == 1) printflag++;
    }

    /* 
     * If the select-by-polygon flag is set, check if to print 
     */
    if (pflag == 1 && printflag == cflag) {
      p1.x = lasf_ptbuff.x * lasf_hbuffer.xscale + lasf_hbuffer.xoffset;
      p1.y = lasf_ptbuff.y * lasf_hbuffer.yscale + lasf_hbuffer.yoffset;
      if (lasf_ogr_inside(&p1, hLayer)) printflag++;
    }
    
    /* 
     * If the printflag is set, print and gather. 
     */
    if (printflag == (cflag + pflag)) {
      if (highlow == 0) { //low
	if ((lasf_ptbuff.z * lasf_hbuffer.zscale + lasf_hbuffer.zoffset) <= hval) {
	  lasf_ptbuff.classification = out_class;
	  status = lasf_write_point(*lasfid, *lasfhid, i, &lasf_ptbuff);	
	}
      }
      else if (highlow == 1) { //high
	//fprintf(stderr,"pnt: %f\n",lasf_ptbuff.z * lasf_hbuffer.zscale + lasf_hbuffer.zoffset);
	//fprintf(stderr,"hval: %f\n",hval);
	if ((lasf_ptbuff.z * lasf_hbuffer.zscale + lasf_hbuffer.zoffset) >= hval) {
	  lasf_ptbuff.classification = out_class;
	  status = lasf_write_point(*lasfid, *lasfhid, i, &lasf_ptbuff);	
	}
      }
    }
    printflag = 0;
    i++, n--;
  }
  return(0);
}

/*
 * Intensity Filter - Classify points to a given classification based on the given intensity range.
 */
int
lasf_intensity_fltr(int* lasfid, int* lasfhid, char* lasf_clr, OGRDataSourceH hDS, char* lasf_filter_args) {
  int i, n, status, lowint, highint;
  lasf_pnts lasf_ptbuff;
  lasf_header lasf_hbuffer;
  int out_class = 0, printflag = 0;
  char cbuff[64];
  point_t p1;

  /*
   * Parse the lasf_filter_args
   */
  char* p = strtok(lasf_filter_args, ":");
  if (p != NULL) lowint = atoi(p);
  else lowint = 0; // default is low
  p = strtok(NULL, ":");
  if (p != NULL) highint = atof(p);
  else highint = 10;
  p = strtok(NULL, ":");
  if (p != NULL) out_class = atoi(p);
  else out_class = 9;

  /*
   * Make sure the user supplied classification value is valid
   */
  if (out_class < 0 || out_class > 32) {
    lasf_file_status(&status, "The given output class is not valid", p);
    return status;
  }
  
  lasf_get_header(lasfid, &lasf_hbuffer);

  /* 
   * clfag tells whether to check for classifications, 
   * if 0 classification selection is skipped, if 1 check 
   * classifications against 'lasf_clr. printflag tells
   * whether to print the record, 0 will not print it, 
   * but 1 will
   */
  int cflag = 0, pflag = 0;
  OGRLayerH hLayer;
  
  if (!(strlen(lasf_clr) == 0)) cflag++;
  if (hDS != NULL) {
    pflag++;
    hLayer = OGR_DS_GetLayer(hDS, 0);
  }

  /* 
   * Loop through the point records and reclassify accordingly 
   */
  i = 0, n = 0;
  while (lasf_read_point(*lasfid, *lasfhid, n, &lasf_ptbuff) == lasf_NOERR) {
    //lasf_printf_progress(abs(n), lasf_hbuffer.numptrecords);

    /* 
     * If the select-by-classification flag is set, check if to print 
     */
    if (cflag == 1) {
      strcpy(cbuff, lasf_clr);
      if (lasf_cmpclass(cbuff, lasf_ptbuff.classification) == 1) printflag++;
    }

    /* 
     * If the select-by-polygon flag is set, check if to print 
     */
    if (pflag == 1 && printflag == cflag) {
      p1.x = lasf_ptbuff.x * lasf_hbuffer.xscale + lasf_hbuffer.xoffset;
      p1.y = lasf_ptbuff.y * lasf_hbuffer.yscale + lasf_hbuffer.yoffset;
      if (lasf_ogr_inside(&p1, hLayer)) printflag++;
    }

    /* 
     * If the printflag is set, print and gather. 
     */
    if (printflag == (cflag + pflag)) {
      if (lasf_ptbuff.intensity >= lowint && lasf_ptbuff.intensity <= highint) {
	lasf_ptbuff.classification = out_class;
	status = lasf_write_point(*lasfid, *lasfhid, i, &lasf_ptbuff);	
      }
    }
    printflag = 0;
    i++, n--;
  }
  return(0);
}

/*
 * "Min-Block" Filter -
 */
int
lasf_minblock_fltr(int* lasfid, int* lasfhid, char* lasf_clr, OGRDataSourceH hDS, char* lasf_filter_args) {
  int i, n, status, xpos, ypos;
  lasf_header lasf_hbuffer0;
  lasf_pnts lasf_ptbuff0;
  lasf_pnts lasf_ptbuff1;
  double lasf_inc, diffval;
  int printflag = 0;
  char cbuff[64];
  point_t p1;

  /*
   * Parse the lasf_filter_args
   */
  char* p = strtok(lasf_filter_args, ":");
  if (p != NULL) lasf_inc = atof(p);
  else lasf_inc = 10;
  p = strtok(NULL, ":");
  if (p != NULL) diffval = atof(p);
  else diffval = 1;

  lasf_get_header(lasfid, &lasf_hbuffer0);

  /*
   * Set the rows and columns of the internal grid
   */
  int ysize = fabs((lasf_hbuffer0.ymax - lasf_hbuffer0.ymin) / lasf_inc) + 1;
  int xsize = fabs((lasf_hbuffer0.xmax - lasf_hbuffer0.xmin) / lasf_inc) + 1;
  
  /* 
   * Allocate memory for the internal grids 
   */
  lasf_pnts** zarray;  
  zarray = (lasf_pnts**) malloc(ysize*sizeof( lasf_pnts*));  
  for (i = 0; i < ysize; i++) zarray[i] = (lasf_pnts*) malloc(xsize*sizeof(lasf_pnts));  
  
  if (!zarray){
    printf("lasfilter: Failed to allocate memory needed for given array %d\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  int** cntarray;  
  cntarray = (int**) malloc(ysize * sizeof(int*));  
  for (i = 0; i < ysize; i++) cntarray[i] = (int*) malloc(xsize * sizeof(int));  
  
  if (!cntarray){
    printf("lasfilter: Failed to allocate memory needed for given array %d\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  /* 
   * clfag tells whether to check for classifications, 
   * if 0 classification selection is skipped, if 1 check 
   * classifications against 'lasf_clr. printflag tells
   * whether to print the record, 0 will not print it, 
   * but 1 will
   */
  int cflag = 0, pflag = 0;
  OGRLayerH hLayer;
  
  if (!(strlen(lasf_clr) == 0)) cflag++;
  if (hDS != NULL) {
    pflag++;
    hLayer = OGR_DS_GetLayer(hDS, 0);
  }
  
  /* 
   * Loop through the point records and grid them
   */
  i = 0, n = 0;
  while (lasf_read_point(*lasfid, *lasfhid, n, &lasf_ptbuff0) == lasf_NOERR) {
    //lasf_printf_progress(abs(n), lasf_hbuffer0.numptrecords);
    i++;

    /* 
     * If the select-by-classification flag is set, check if to print 
     */
    if (cflag == 1) {
      strcpy(cbuff, lasf_clr);
      if (lasf_cmpclass(cbuff, lasf_ptbuff0.classification) == 1) printflag++;
    }

    /* 
     * If the select-by-polygon flag is set, check if to print 
     */
    if (pflag == 1 && printflag == cflag) {
      p1.x = lasf_ptbuff0.x * lasf_hbuffer0.xscale + lasf_hbuffer0.xoffset;
      p1.y = lasf_ptbuff0.y * lasf_hbuffer0.yscale + lasf_hbuffer0.yoffset;
      if (lasf_ogr_inside(&p1, hLayer)) printflag++;
    }

    /* 
     * If the printflag is set, print and gather. 
     */
    if (printflag == (cflag + pflag)) {
      xpos = fabs((lasf_ptbuff0.x * lasf_hbuffer0.xscale + lasf_hbuffer0.xoffset - lasf_hbuffer0.xmin) / lasf_inc);
      ypos = fabs((lasf_ptbuff0.y * lasf_hbuffer0.yscale + lasf_hbuffer0.yoffset - lasf_hbuffer0.ymin) / lasf_inc);
      
      cntarray[ypos][xpos]++;
      lasf_ptbuff1 = zarray[ypos][xpos];
    
      if (cntarray[ypos][xpos] > 1) {
	if (lasf_ptbuff0.z * lasf_hbuffer0.zscale + lasf_hbuffer0.zoffset < \
	    lasf_ptbuff1.z * lasf_hbuffer0.zscale + lasf_hbuffer0.zoffset) {
	  zarray[ypos][xpos] = lasf_ptbuff0;
	}
      }
      else {
	zarray[ypos][xpos] = lasf_ptbuff0;
      }
    }
    printflag = 0;
    n--;
  }
  
  /* 
   * Check points against min grid
   */
  i = 0, n = 0;
  double z, z1;
  while (lasf_read_point(*lasfid, *lasfhid, n, &lasf_ptbuff0) == lasf_NOERR) {
    //lasf_printf_progress(abs(n), lasf_hbuffer0.numptrecords);
    
    /* 
     * If the select-by-classification flag is set, check if to print 
     */
    if (cflag == 1) {
      strcpy(cbuff, lasf_clr);
      if (lasf_cmpclass(cbuff, lasf_ptbuff0.classification) == 1) printflag++;
    }

    /* 
     * If the select-by-polygon flag is set, check if to print 
     */
    if (pflag == 1 && printflag == cflag) {
      p1.x = lasf_ptbuff0.x * lasf_hbuffer0.xscale + lasf_hbuffer0.xoffset;
      p1.y = lasf_ptbuff0.y * lasf_hbuffer0.yscale + lasf_hbuffer0.yoffset;
      if (lasf_ogr_inside(&p1, hLayer)) printflag++;
    }

    /* 
     * If the printflag is set, print and gather. 
     */
    if (printflag == (cflag + pflag)) {

      xpos = fabs((lasf_ptbuff0.x * lasf_hbuffer0.xscale + lasf_hbuffer0.xoffset - lasf_hbuffer0.xmin) / lasf_inc);
      ypos = fabs((lasf_ptbuff0.y * lasf_hbuffer0.yscale + lasf_hbuffer0.yoffset - lasf_hbuffer0.ymin) / lasf_inc);
      
      lasf_ptbuff1 = zarray[ypos][xpos];
      
      z = lasf_ptbuff0.z * lasf_hbuffer0.zscale + lasf_hbuffer0.zoffset;
      z1 = lasf_ptbuff1.z * lasf_hbuffer0.zscale + lasf_hbuffer0.zoffset;
      
      if ((z - z1) <= diffval) lasf_ptbuff0.classification = 2;
      else lasf_ptbuff0.classification = 3;
      status = lasf_write_point(*lasfid, *lasfhid, i, &lasf_ptbuff0);	
    }
    i++, printflag = 0, n--;
  }

  /* 
   * Free allocated grids from memory
   */
  for (i = 0; i < ysize; i++) free(zarray[i]);
  free( zarray);
  for (i = 0; i < ysize; i++) free(cntarray[i]);
  free(cntarray);

  return(0);
}

/*
 * "Mean-Block" Filter -
 */
int
lasf_meanblock_fltr(int* lasfid, int* lasfhid, char* lasf_clr, OGRDataSourceH hDS, char* lasf_filter_args) {

  int i, n, status, xpos, ypos;
  lasf_header lasf_hbuffer0;
  lasf_pnts lasf_ptbuff0;
  lasf_pnts lasf_ptbuff1;
  double lasf_inc, diffval;
  int printflag = 0;
  char cbuff[64];
  point_t p1;

  /*
   * Parse the lasf_filter_args
   */
  char* p = strtok(lasf_filter_args, ":");
  if (p != NULL) lasf_inc = atof(p);
  else lasf_inc = 10;
  p = strtok(NULL, ":");
  if (p != NULL) diffval = atof(p);
  else diffval = 1;

  lasf_get_header(lasfid, &lasf_hbuffer0);

  /*
   * Set the rows and columns of the internal grid
   */
  int ysize = fabs((lasf_hbuffer0.ymax - lasf_hbuffer0.ymin) / lasf_inc) + 1;
  int xsize = fabs((lasf_hbuffer0.xmax - lasf_hbuffer0.xmin) / lasf_inc) + 1;
  
  /* 
   * Allocate memory for the internal grids 
   */
  /* lasf_pnts** zarray;   */
  /* zarray = (lasf_pnts**) malloc(ysize*sizeof( lasf_pnts*));   */
  /* for (i = 0; i < ysize; i++) zarray[i] = (lasf_pnts*) malloc(xsize*sizeof(lasf_pnts));   */

  float** zarray;  
  zarray = (float**) malloc(ysize * sizeof(float*));  
  for (i = 0; i < ysize; i++) {
    zarray[i] = (float*) malloc(xsize * sizeof(float));  
  }  
  
  if (!zarray){
    printf("lasfilter: Failed to allocate memory needed for given array %d\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  int** cntarray;  
  cntarray = (int**) malloc(ysize * sizeof(int*));  
  for (i = 0; i < ysize; i++) cntarray[i] = (int*) malloc(xsize * sizeof(int));  
  
  if (!cntarray){
    printf("lasfilter: Failed to allocate memory needed for given array %d\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  /* 
   * clfag tells whether to check for classifications, 
   * if 0 classification selection is skipped, if 1 check 
   * classifications against 'lasf_clr. printflag tells
   * whether to print the record, 0 will not print it, 
   * but 1 will
   */
  int cflag = 0, pflag = 0;
  OGRLayerH hLayer;

  if (!(strlen(lasf_clr) == 0)) cflag++;
  if (hDS != NULL) {
    pflag++;
    hLayer = OGR_DS_GetLayer(hDS, 0);
  }
  
  /* 
   * Loop through the point records and grid them
   */
  i = 0, n = 0;
  double x,y,z;
  while (lasf_read_point(*lasfid, *lasfhid, n, &lasf_ptbuff0) == lasf_NOERR) {
    //lasf_printf_progress(abs(n), lasf_hbuffer0.numptrecords);
    i++;

    /* 
     * If the select-by-classification flag is set, check if to print 
     */
    if (cflag == 1) {
      strcpy(cbuff, lasf_clr);
      if (lasf_cmpclass(cbuff, lasf_ptbuff0.classification) == 1) printflag++;
    }

    /* 
     * If the select-by-polygon flag is set, check if to print 
     */
    if (pflag == 1 && printflag == cflag) {
      p1.x = lasf_ptbuff0.x * lasf_hbuffer0.xscale + lasf_hbuffer0.xoffset;
      p1.y = lasf_ptbuff0.y * lasf_hbuffer0.yscale + lasf_hbuffer0.yoffset;
      if (lasf_ogr_inside(&p1, hLayer)) printflag++;
    }

    /* 
     * If the printflag is set, print and gather. 
     */
    if (printflag == (cflag + pflag)) {

      x = lasf_ptbuff0.x * lasf_hbuffer0.xscale + lasf_hbuffer0.xoffset;
      y = lasf_ptbuff0.y * lasf_hbuffer0.yscale + lasf_hbuffer0.yoffset;
      z = lasf_ptbuff0.z * lasf_hbuffer0.zscale + lasf_hbuffer0.zoffset;
      
      xpos = fabs((x - lasf_hbuffer0.xmin) / lasf_inc);
      ypos = fabs((y - lasf_hbuffer0.ymin) / lasf_inc);
      
      cntarray[ypos][xpos]++;

      if (cntarray[ypos][xpos] > 1) {    
	zarray[ypos][xpos] = zarray[ypos][xpos] + z;
      }
      else {
	zarray[ypos][xpos] = z;
      }
    }
    printflag = 0;
    n--;
  }
  
  /* 
   * Check points against meann grid
   */
  i = 0, n = 0;
  double z1, zval;
  while (lasf_read_point(*lasfid, *lasfhid, n, &lasf_ptbuff0) == lasf_NOERR) {
    //lasf_printf_progress(abs(n), lasf_hbuffer0.numptrecords);

    /* 
     * If the select-by-classification flag is set, check if to print 
     */
    if (cflag == 1) {
      strcpy(cbuff, lasf_clr);
      if (lasf_cmpclass(cbuff, lasf_ptbuff0.classification) == 1) printflag++;
    }

    /* 
     * If the select-by-polygon flag is set, check if to print 
     */
    if (pflag == 1 && printflag == cflag) {
      p1.x = lasf_ptbuff0.x * lasf_hbuffer0.xscale + lasf_hbuffer0.xoffset;
      p1.y = lasf_ptbuff0.y * lasf_hbuffer0.yscale + lasf_hbuffer0.yoffset;
      if (lasf_ogr_inside(&p1, hLayer)) printflag++;
    }

    /* 
     * If the printflag is set, print and gather. 
     */
    if (printflag == (cflag + pflag)) {
    
      xpos = fabs((lasf_ptbuff0.x * lasf_hbuffer0.xscale + lasf_hbuffer0.xoffset - lasf_hbuffer0.xmin) / lasf_inc);
      ypos = fabs((lasf_ptbuff0.y * lasf_hbuffer0.yscale + lasf_hbuffer0.yoffset - lasf_hbuffer0.ymin) / lasf_inc);
      
      zval = lasf_ptbuff0.z * lasf_hbuffer0.zscale + lasf_hbuffer0.zoffset;
      z1 = zarray[ypos][xpos];
      int cval = cntarray[ypos][xpos];
      z = z1/cval;
      
      if (fabs(z - zval) <= diffval) lasf_ptbuff0.classification = 2;
      else lasf_ptbuff0.classification = 3;
      status = lasf_write_point(*lasfid, *lasfhid, i, &lasf_ptbuff0);	
    }
    printflag = 0, i++, n--;
  }

  /* 
   * Free allocated grids from memory
   */
  for (i = 0; i < ysize; i++) free(zarray[i]);
  free(zarray);
  for (i = 0; i < ysize; i++) free(cntarray[i]);
  free(cntarray);

  return(0);
}

/*
 * "Density-Block" Filter -
 */
int
lasf_density_fltr(int* lasfid, int* lasfhid, char* lasf_clr, OGRDataSourceH hDS, char* lasf_filter_args) {
  int i, n, status, xpos, ypos, highlow, diffval;
  lasf_header lasf_hbuffer0;
  lasf_pnts lasf_ptbuff0;
  lasf_pnts lasf_ptbuff1;
  double lasf_inc;
  int printflag = 0, out_class = 0;
  char cbuff[64];
  point_t p1;

  /*
   * Parse the lasf_filter_args
   */
  char* p = strtok(lasf_filter_args, ":");
  if (p != NULL) lasf_inc = atof(p);
  else lasf_inc = 10;
  p = strtok(NULL, ":");
  if (p != NULL) highlow = atoi(p);
  else highlow = 0; // default is low
  p = strtok(NULL, ":");
  if (p != NULL) diffval = atoi(p);
  else diffval = 10;
  p = strtok(NULL, ":");
  if (p != NULL) out_class = atoi(p);
  else out_class = 2;

  //  fprintf(stderr,"inc: %f\nhighlow: %d\ndiff: %d\noutclass: %d\n",lasf_inc, highlow, diffval, out_class);

  lasf_get_header(lasfid, &lasf_hbuffer0);

  /*
   * Set the rows and columns of the internal grid
   */
  int ysize = fabs((lasf_hbuffer0.ymax - lasf_hbuffer0.ymin) / lasf_inc) + 1;
  int xsize = fabs((lasf_hbuffer0.xmax - lasf_hbuffer0.xmin) / lasf_inc) + 1;
  
  /* 
   * Allocate memory for the internal grids 
   */

  int** cntarray;  
  cntarray = (int**) malloc(ysize * sizeof(int*));  
  for (i = 0; i < ysize; i++) cntarray[i] = (int*) malloc(xsize * sizeof(int));  
  
  if (!cntarray){
    printf("lasfilter: Failed to allocate memory needed for given array %d\n", __LINE__);
    exit(EXIT_FAILURE);
  }

  /* 
   * clfag tells whether to check for classifications, 
   * if 0 classification selection is skipped, if 1 check 
   * classifications against 'lasf_clr. printflag tells
   * whether to print the record, 0 will not print it, 
   * but 1 will
   */
  int cflag = 0, pflag = 0;
  OGRLayerH hLayer;
  
  if (!(strlen(lasf_clr) == 0)) cflag++;
  if (hDS != NULL) {
    pflag++;
    hLayer = OGR_DS_GetLayer(hDS, 0);
  }
  
  /* 
   * Loop through the point records and grid them
   */
  i = 0, n = 0;
  while (lasf_read_point(*lasfid, *lasfhid, n, &lasf_ptbuff0) == lasf_NOERR) {
    //lasf_printf_progress(abs(n), lasf_hbuffer0.numptrecords);
    i++;

    /* 
     * If the select-by-classification flag is set, check if to print 
     */
    if (cflag == 1) {
      strcpy(cbuff, lasf_clr);
      if (lasf_cmpclass(cbuff, lasf_ptbuff0.classification) == 1) printflag++;
    }

    /* 
     * If the select-by-polygon flag is set, check if to print 
     */
    if (pflag == 1 && printflag == cflag) {
      p1.x = lasf_ptbuff0.x * lasf_hbuffer0.xscale + lasf_hbuffer0.xoffset;
      p1.y = lasf_ptbuff0.y * lasf_hbuffer0.yscale + lasf_hbuffer0.yoffset;
      if (lasf_ogr_inside(&p1, hLayer)) printflag++;
    }

    /* 
     * If the printflag is set, print and gather. 
     */
    if (printflag == (cflag + pflag)) {
      xpos = fabs((lasf_ptbuff0.x * lasf_hbuffer0.xscale + lasf_hbuffer0.xoffset - lasf_hbuffer0.xmin) / lasf_inc);
      ypos = fabs((lasf_ptbuff0.y * lasf_hbuffer0.yscale + lasf_hbuffer0.yoffset - lasf_hbuffer0.ymin) / lasf_inc);
      
      cntarray[ypos][xpos]++;
    }
    printflag = 0;
    n--;
  }
  
  /* 
   * Check points against density grid
   */
  i = 0, n = 0;
  while (lasf_read_point(*lasfid, *lasfhid, n, &lasf_ptbuff0) == lasf_NOERR) {
    //lasf_printf_progress(abs(n), lasf_hbuffer0.numptrecords);
    
    /* 
     * If the select-by-classification flag is set, check if to print 
     */
    if (cflag == 1) {
      strcpy(cbuff, lasf_clr);
      if (lasf_cmpclass(cbuff, lasf_ptbuff0.classification) == 1) printflag++;
    }

    /* 
     * If the select-by-polygon flag is set, check if to print 
     */
    if (pflag == 1 && printflag == cflag) {
      p1.x = lasf_ptbuff0.x * lasf_hbuffer0.xscale + lasf_hbuffer0.xoffset;
      p1.y = lasf_ptbuff0.y * lasf_hbuffer0.yscale + lasf_hbuffer0.yoffset;
      if (lasf_ogr_inside(&p1, hLayer)) printflag++;
    }

    /* 
     * If the printflag is set, print and gather. 
     */
    if (printflag == (cflag + pflag)) {

      xpos = fabs((lasf_ptbuff0.x * lasf_hbuffer0.xscale + lasf_hbuffer0.xoffset - lasf_hbuffer0.xmin) / lasf_inc);
      ypos = fabs((lasf_ptbuff0.y * lasf_hbuffer0.yscale + lasf_hbuffer0.yoffset - lasf_hbuffer0.ymin) / lasf_inc);
      //fprintf(stderr,"c: %d d: %d\n",cntarray[ypos][xpos],diffval);
      //fprintf(stderr,"%d\n",highlow);
      if (highlow == 0) {//low
	if (cntarray[ypos][xpos] <= diffval) {
	  //fprintf(stderr,"lwo\n");
	  lasf_ptbuff0.classification = out_class;
	  status = lasf_write_point(*lasfid, *lasfhid, i, &lasf_ptbuff0);	
	}
      }
      else if (highlow == 1) {//high
	if (cntarray[ypos][xpos] >= diffval) {
	  //fprintf(stderr,"hi\n");
	  lasf_ptbuff0.classification = out_class;
	  status = lasf_write_point(*lasfid, *lasfhid, i, &lasf_ptbuff0);	
	}
      }
    }
    i++, printflag = 0, n--;
  }

  /* 
   * Free allocated grids from memory
   */
  for (i = 0; i < ysize; i++) free(cntarray[i]);
  free(cntarray);

  return(0);
}


int
main (int argc, char **argv) {
  char* fn;
  
  int c, lasfid, lasfhid, i, status;
  int inflag = 0, outflag = 0, filterflag = 0, cflag = 0, pflag = 0, bflag = 0;
  char* filter_buff;
  char *lasf_clr, *lasf_ply, *lasf_bb; 
	    
  while (1) {
    static struct option long_options[] =
      {
	/* These options set a flag. */
	{"version", no_argument, &version_flag, 1},
	{"help", no_argument, &help_flag, 1},
	/* These options don't set a flag.
	   We distinguish them by their indices. */
	{"filter", required_argument, 0, 'f'},
	{"polygon", required_argument, 0, 'p'},
	{"bounding-box", required_argument, 0, 'b'},
	{"classification", required_argument, 0, 'c'},
	{0, 0, 0, 0}
      };
    /* getopt_long stores the option index here. */
    int option_index = 0;
    
    c = getopt_long (argc, argv, "c:f:p:b:",
		     long_options, &option_index);
    
    /* Detect the end of the options. */
    if (c == -1) break;
    
    switch (c)
      {
      case 0:
	/* If this option set a flag, do nothing else now. */
	if (long_options[option_index].flag != 0)
	  break;
	printf ("option %s", long_options[option_index].name);
	if (optarg) printf (" with arg %s", optarg);
	printf ("\n");
	break;
	
      case 'f':
	filter_buff = optarg;
	filterflag++;
	break;
      case 'c':
	lasf_clr = optarg;
	cflag++;
	break;
      case 'p':
	lasf_ply = optarg;
	pflag++;
	break;
      case 'b':
	lasf_bb = optarg;
	bflag++;
	break;
	
      case '?':
	/* getopt_long already printed an error message. */
	break;
	
      default:
	abort ();
      }
  }
  /* Instead of --
     -- as they are encountered,
     we report the final status resulting from them. */

  if (version_flag) lasf_print_version("lasfilter", LASFILTER_VERSION);

  if (help_flag) usage();

  if (inflag == 0 && optind >= argc) usage ();

  /* Loop through the input LAS file(s) */
  while (optind < argc)
    {
      fn = argv[optind];
      inflag++;
      optind++;
      if (inflag > 0)
	{
	  status = lasf_open(fn, &lasfid, 2);
	  if (status != lasf_NOERR) {
	    lasf_print_error(status, "lasfilter");
	    exit(-1);
	  }

	  lasf_open_header( &lasfid, &lasfhid );

	  if (cflag == 0) lasf_clr = "";
	  if (pflag == 0) lasf_ply = "";
	  if (bflag == 0) lasf_bb = "";
	  
	  if (filterflag > 0) {
	    fprintf(stderr, "lasfilter: Working on file %s\n", fn);
	    fflush(stderr);
	    int lasf_filter;
	    char* lasf_filter_args;
	    
	    char* p = strtok(filter_buff, ":");
	    lasf_filter = atoi(p);
	    p = strtok(NULL, "");
	    if (p != NULL) lasf_filter_args = p;
	    else lasf_filter_args = "";

	    // OGR Polygon support
	    OGRDataSourceH hDS;
	    OGRRegisterAll();
	    hDS = OGROpen(lasf_ply, FALSE, NULL);

	    // Run a Filter
	    if (lasf_filter == 0) { // Reclassify
	      status = lasf_reclassify_fltr(&lasfid, &lasfhid, lasf_clr, lasf_bb, hDS, lasf_filter_args);
	      if (status != lasf_NOERR) {
		lasf_print_error(status, "lasfilter");
	      }
	    }
	    else if (lasf_filter == 1) { // Min-Block
	      lasf_minblock_fltr(&lasfid, &lasfhid, lasf_clr, hDS, lasf_filter_args);
	    }
	    else if (lasf_filter == 2) { // Mean-Block
	      lasf_meanblock_fltr(&lasfid, &lasfhid, lasf_clr, hDS, lasf_filter_args);
	    }
	    else if (lasf_filter == 3) { // Height
	      lasf_height_fltr(&lasfid, &lasfhid, lasf_clr, hDS, lasf_filter_args);
	    }
	    else if (lasf_filter == 4) { // Intensity
	      lasf_intensity_fltr(&lasfid, &lasfhid, lasf_clr, hDS, lasf_filter_args);
	    }
	    else if (lasf_filter == 5) { // Density
	      lasf_density_fltr(&lasfid, &lasfhid, lasf_clr, hDS, lasf_filter_args);
	    }
	    if (hDS != NULL) OGR_DS_Destroy(hDS);
	  }
	  lasf_close(&lasfid);
	  fprintf(stderr, "\n");
	}
      else {
	usage ();
      }
    }
  exit(0);
}
