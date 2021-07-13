/*-----------------------------------------------------------------
 *
 * las2grd.c 
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
//#include<lasf.h>
#include<netcdf.h>

#include "../src/lasf.h"

#define LAS2GRD_VERSION "0.1.6"

/* Flags set by `--version' and `--help' */
static int version_flag;
static int help_flag;

void 
usage() {
  fprintf(stderr, "\
las2grd infile(s) [Options]\n\n\
  Generate a GMT formatted NetCDF file (.grd) from the given las file(s).\n\
\n\
  infile(s)\t\tThe input LAS file(s) \n\
\n\
 Options:\n\
  -o, --output\t\tthe output NetCDF .grd file.\n\
  -i, --increment\tThe desired gridding increment (cellsize).\n\
     Note: The increment value should be in the same units as the input data.\n\n\
  -c, --classification\tGrid according to classification(s); separate values \n\
\t\t\twith a '/'; e.g. -c 2/9/11 to select classes 2, 9 and 11.\n\
  -n, --intensity\tGrid the intensity value of each point rather than the \n\
\t\t\tdefault z value.\n\
  -d, --densitty\tGenerate a point density grid rather than the \n \
\t\t\tdefault elevation value.\n\
\n\
  --help\t\tPrint this help menu and exit.\n\
  --version\t\tPrint version information and exit.\n\n\
" );
  exit(1);
}

int
main (int argc, char **argv) {
  char* fn;
  char* fo;
  char *lasf_clr; 
  char cbuff[64];
  
  int c, lasfid, lasfhid, i, j, n, status, xpos, ypos;
  int inflag = 0, outflag = 0, incflag = 0, clflag = 0, intensflag = 0, printflag = 0, dflag = 0;
  
  float lasf_inc, x, y, z;
  float nanval = lasf_NAN;
  
  lasf_pnts lasf_ptbuff0;
  lasf_describe lasf_stats;
  lasf_header lasf_hbuffer0; 

  //const char *hist_c = argv;

  while (1) {
    static struct option long_options[] =
      {
	/* These options set a flag. */
	{"version", no_argument, &version_flag, 1},
	{"help", no_argument, &help_flag, 1},
	/* These options don't set a flag.
	   We distinguish them by their indices. */
	{"output", required_argument, 0, 'o'},
	{"increment", required_argument, 0, 'i'},
	{"classification", required_argument, 0, 'c'},
	{"intensity", no_argument, 0, 'n'},
	{"density", no_argument, 0, 'd'},
	{0, 0, 0, 0}
      };
    /* getopt_long stores the option index here. */
    int option_index = 0;
    
    c = getopt_long (argc, argv, "o:i:c:nd",
		     long_options, &option_index);
    
    /* Detect the end of the options. */
    if (c == -1)
      break;
    
    switch (c) 
      {
      case 0:
	/* If this option set a flag, do nothing else now. */
	if (long_options[ option_index ].flag != 0)
	  break;
	printf ("option %s", long_options[option_index].name);
	if (optarg)
	  printf (" with arg %s", optarg);
	printf ("\n");
	break;
	
      case 'o':
	fo = optarg;
	outflag++;
	break;
      case 'i':
	lasf_inc = atof( optarg );
	incflag++;
	break;
      case 'c':
	lasf_clr = optarg;
	clflag++;
	break;
      case 'n':
	intensflag++;
	break;
      case 'd':
	dflag++;
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

  if (version_flag) lasf_print_version("las2grd", LAS2GRD_VERSION);

  if (help_flag) usage();

  if (inflag == 0 && optind >= argc) usage ();

  /* Loop through the input LAS file(s) */
  while (optind < argc) {
    fn = argv[optind];
    inflag++;
    optind++;
    if (inflag > 0) {
      if (outflag == 0) {
	char *ret = malloc(strlen(fn) + 4);
	strncpy(ret, fn, strlen(fn) - 4);
	strcpy(&ret[strlen(fn) - 4], ".grd");
	*(ret + strlen(fn) + 3) = '\0';
	fo = ret;
      }

      if (incflag == 0) lasf_inc = 1;
      if (clflag == 0) lasf_clr = "";
  
      // open input for reading
      status = lasf_open(fn, &lasfid, 0);
      if (status != lasf_NOERR) {
	lasf_print_error(status, "las2grd");
	exit(-1);
      }
      fprintf(stderr, "las2grd: Working on file %s\n", fn);
	  
      lasf_open_header(&lasfid, &lasfhid);
      lasf_get_header(&lasfid, &lasf_hbuffer0);
	  
      /* Create the netcdf instance */
      int ncid;
      status = nc_create(fo, NC_CLASSIC_MODEL, &ncid);
	  
      // Set the rows and columns of the internal grid
      int ysize = fabs((lasf_hbuffer0.ymax - lasf_hbuffer0.ymin) / lasf_inc) + 1;
      int xsize = fabs((lasf_hbuffer0.xmax - lasf_hbuffer0.xmin) / lasf_inc) + 1;
	  
      /* 
       * Allocate memory for a 2d array of size ysize/xsize
       */
      float** zarray;  
      zarray = (float**) malloc(ysize * sizeof(float*));  
      for (i = 0; i < ysize; i++) {
	zarray[i] = (float*) malloc(xsize * sizeof(float));  
      }
      if (!zarray) {
	printf( "las2grd: Failed to allocate memory needed for given array %d\n", __LINE__ );
	exit(EXIT_FAILURE);
      }
	  
      int** cntarray;  
      cntarray = (int**) malloc(ysize * sizeof(int*));  
      for (i = 0; i < ysize; i++) {
	cntarray[i] = (int*) malloc(xsize * sizeof(int));  
      }
      if (!cntarray) {
	printf( "las2grd: Failed to allocate memory needed for given array %d\n", __LINE__ );
	exit(EXIT_FAILURE);
      }
	  
	  
      /* 
       * Initialize the output NetCDF file 
       */

      status = nc_redef(ncid);
      int xdim, ydim, xid, yid, zid;
      status = nc_def_dim(ncid, "x", xsize, &xdim);
      status = nc_def_dim(ncid, "y", ysize, &ydim);
	  
      int xydimids[2];
      xydimids[0] = ydim;
      xydimids[1] = xdim;
	  
      status = nc_def_var(ncid, "x", NC_DOUBLE, 1, &xdim, &xid);
      status = nc_def_var(ncid, "y", NC_DOUBLE, 1, &ydim, &yid);
      status = nc_def_var(ncid, "z", NC_FLOAT, 2, xydimids, &zid);
      
      status = nc_put_att_text(ncid, xid, "long_name", strlen("longitude"), "longitude");
      status = nc_put_att_text(ncid, yid, "long_name", strlen("latitude"), "latitude");
	  
      if (intensflag > 0) {
	status = nc_put_att_text(ncid, zid, "long_name", strlen("intensity"), "intensity");
      }
      else {
	status = nc_put_att_text(ncid, zid, "long_name", strlen("topography"), "topography");
      }
	  
      status = nc_put_att_float(ncid, zid, "_FillValue", NC_FLOAT, 1, &nanval);

      //int noff = 0;
      status = nc_put_att_text(ncid, NC_GLOBAL, "Conventions", strlen("COARDS/CF-1.0"), "COARDS/CF-1.0");
      status = nc_put_att_text(ncid, NC_GLOBAL, "title", strlen(fn), fn);     
      status = nc_put_att_text(ncid, NC_GLOBAL, "lasf_version", strlen(lasf_VERSION), lasf_VERSION);     
      //status = nc_put_att_int(ncid, NC_GLOBAL, "node_offset", NC_INT, 1, &noff);
      /* 
       * clfag tells whether to check for classifications, 
       * if 0 classification selection is skipped, if 1 check 
       * classifications against 'lasf_clr. printflag tells
       * whether to print the record, 0 will not print it, 
       * but 1 will
       */
      int cflag = 0;
      
      if (!(strlen(lasf_clr) == 0)) cflag++;
      
      /* 
       * Loop through the point records 
       */
      i = 0, n = 0;
      while (lasf_read_point(lasfid, lasfhid, n, &lasf_ptbuff0) == lasf_NOERR) {
	//lasf_printf_progress(abs(n), lasf_hbuffer0.numptrecords);
	/* 
	 * If the select-by-classification flag is set, check if to print 
	 */
	if (cflag == 1) {
	  strcpy(cbuff, lasf_clr);
	  if (lasf_cmpclass(cbuff, lasf_ptbuff0.classification) == 1) printflag++;
	}
	/* 
	 * If the printflag is set, print and gather. 
	 */
	if (printflag == (cflag)) {
	  x = lasf_ptbuff0.x * lasf_hbuffer0.xscale + lasf_hbuffer0.xoffset;
	  y = lasf_ptbuff0.y * lasf_hbuffer0.yscale + lasf_hbuffer0.yoffset;
	  z = lasf_ptbuff0.z * lasf_hbuffer0.zscale + lasf_hbuffer0.zoffset;
	  /*
	   *  Determine which cell to assign this point
	   */
	  if (x <= lasf_hbuffer0.xmax && x >= lasf_hbuffer0.xmin && y <= lasf_hbuffer0.ymax && y >= lasf_hbuffer0.ymin) {
	    lasf_add_to_stats(&lasfhid, lasf_ptbuff0, &lasf_stats, i); 
	    i++;
	    xpos = fabs((x - lasf_hbuffer0.xmin) / lasf_inc);
	    ypos = fabs((y - lasf_hbuffer0.ymin) / lasf_inc);
	    cntarray[ypos][xpos]++;

	    if (intensflag > 0) {
	      zarray[ypos][xpos] = zarray[ypos][xpos] + (lasf_ptbuff0.intensity * 1.0);
	    }
	    else if (dflag > 0) {
	      zarray[ypos][xpos] = zarray[ypos][xpos] + cntarray[ypos][xpos];
	    }
	    else {
	      zarray[ypos][xpos] = zarray[ypos][xpos] + z;
	    }
	  }
	}
	printflag = 0; 
	n--;
      }
      /* 
       * Insert the actual range values as an 
       * attribute to the variable 
       */

      double ncxrange[2] = {lasf_stats.xmin - (lasf_inc / 2), lasf_stats.xmax + (lasf_inc / 2)};
      status = nc_put_att_double(ncid, xid, "actual_range", NC_DOUBLE, 2, ncxrange);
	  
      double ncyrange[2] = {lasf_stats.ymin - (lasf_inc / 2), lasf_stats.ymax + (lasf_inc / 2)};
      status = nc_put_att_double(ncid, yid, "actual_range", NC_DOUBLE, 2, ncyrange);
	  
      float nczrange[2] = {lasf_stats.zmax, lasf_stats.zmin};

      status = nc_enddef(ncid);

      n = 0;
      /* Insert the values into the grid */
      for (i = 0; i < ysize; i++) {
	for (j = 0; j < xsize; j++) {
	  //lasf_printf_progress(n, (ysize-1)*(xsize-1));
	  double xval = ((j * lasf_inc) + lasf_hbuffer0.xmin);
	  double yval = ((i * lasf_inc) + lasf_hbuffer0.ymin);
	  size_t y_index[] = {i};
	  size_t x_index[] = {j};
	  size_t z_index[] = {i, j};

	  float zval = zarray[i][j];
	  int cval = cntarray[i][j];
	  if (cval == 0) {
	    cval = 1;
	    zval = lasf_NAN;
	  }
	  
	  float czval = zval / cval;
	  if (czval <= nczrange[0]) nczrange[0] = czval;
	  if (czval >= nczrange[1]) nczrange[1] = czval;
	  
	  status = nc_put_var1_double(ncid, yid, y_index, &yval);
	  if (status != NC_NOERR) {
	    fprintf(stderr, "y(%d): %s\n", (int)y_index[0], nc_strerror(status));
	    exit(-1);
	  }
	  status = nc_put_var1_double(ncid, xid, x_index, &xval);
	  if (status != NC_NOERR) {
	    fprintf(stderr, "x(%d): %s\n", (int)x_index[0], nc_strerror(status));
	    exit(-1);
	  }
	  status = nc_put_var1_float(ncid, zid, z_index, &czval);
	  if (status != NC_NOERR) {
	    fprintf(stderr, "z: %s\n", nc_strerror(status));
	    exit(-1);
	  }
	  n++;
	}
      }
      //int noff = 1;
      //status = nc_put_att_int(ncid, NC_GLOBAL, "node_offset", NC_INT, sizeof(noff), &noff);
      //status = nc_put_att_text( ncid, NC_GLOBAL, "history", strlen( *hist_c ),  *hist_c );
      
      status = nc_redef(ncid);

      status = nc_put_att_float(ncid, zid, "actual_range", NC_FLOAT, 2, nczrange);
      if (status != NC_NOERR) fprintf(stderr, "%s\n", nc_strerror(status));

      status = nc_enddef(ncid);

      for (i = 0; i < ysize; i++) free(zarray[i]);
      free(zarray);
      for (i = 0; i < ysize; i++) free(cntarray[i]);
      free(cntarray);

      status = nc_close(ncid);
      lasf_close(&lasfid);
      fprintf(stderr, "\n");
    }
    else {
      usage ();
    }
  }
  exit(1);
}
