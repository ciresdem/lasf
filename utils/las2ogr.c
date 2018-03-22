/*-------------------------------------------------------------------
 *
 * las2ogr.c 
 *
 * Copyright (C) 2010-2014 Matthew Love <matthew.love@colorado.edu>
 *
 * This file is liscensed under the GPL v.2 or later and 
 * is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details. 
 * <http://www.gnu.org/licenses/>
 *
 *-------------------------------------------------------------------*/

#include<getopt.h>
#include<lasf.h>
#include<gdal/ogr_api.h>

#define LAS2OGR_VERSION "0.1.2"

/* Flag set by `--version'. */
static int version_flag;
static int help_flag;

void 
usage() {
  fprintf(stderr, "\
las2ogr infile(s) [OPTIONS]\n\n\
\
  infile(s)\t\tThe input LAS file(s) \n\
\n\
 Options:\n\
  -o, --output\t\tThe output OGR file. (if not specified,\n\
\t\t\twill use the name of the input LAS file.), \n\
     Note: If more than one input LAS file is specified, it is best\n\
           to omit this flag; by default omiting this flag in such\n\
           cases will output into a folder $(basename $infile .las)\n\n\
  -f, --format\t\tDesired output OGR compatible format\n\
     Note: Default will be 'ESRI Shapefile', check `ogr2ogr --formats` to see valid formats.\n\n\
  -c, --classification\tDesired output classification(s), separate values \n\
\t\t\twith a '/'; e.g. -c 2/9/11 to output classes 2, 9 and 11.\n\
  -r, --records\t\tString of desired attribute field records, \n\
     Note: Possible record values include: 'xyzincsuptXYZ':\n\n\
       x->x (The scaled 'x' value from the point record), \n\
       y->y (The scaled 'y' value from the point record), \n\
       z->z (The scaled 'z' value from the point record), \n\
       i->intensity (The 'Intensity' value from the point record), \n\
       n->returns[5] (The 5 return values from the point record), \n\
       c->classification (The 'Classification' value from the point record), \n\
       s->scan_angle (The 'Scan Angle' value from the point record), \n\
       u->user_data (The 'User Data' value from the point record), \n\
       p->pt_id (The point ID value from the point record), \n\
       t->gps_time (The GPS-Time from the point record (check the 'global' \n\
                    value in the header to determine the format of the GPS Time))\n\n\
  --help\t\tPrint this help menu and exit.\n\
  --version\t\tPrint version information and exit.\n\n\
");
  exit(1);
}

int
main (argc, argv)
     int argc;
     char **argv;
{
  char* fn;
  char* fo;
  char* bn;
  char *lasf_recr, *lasf_clr, *OGRformat;
  char lasfp;  
  int c, i, j, n, pf_length, lasfid, lasfhid, status;
  int rflag = 0, inflag = 0, outflag = 0, clflag = 0, plfag = 0, fflag = 0;
  double x, y, z;

  lasf_header lasf_hbuffer0;
  lasf_pnts lasf_ptbuff0;
  lasf_pntreturns lasf_ptrt;

  OGRSFDriverH hDriver;
  OGRDataSourceH hDS;
  OGRLayerH hLayer;
  OGRFieldDefnH hFieldDefn;
  OGRFeatureH hFeature;

  while (1) {
    static struct option long_options[] =
      {
	/* These options set a flag. */
	{"version", no_argument, &version_flag, 1},
	{"help", no_argument, &help_flag, 1},
	/* These options don't set a flag.
	   We distinguish them by their indices. */
	{"output", required_argument, 0, 'o'},
	{"format", required_argument, 0, 'f'},
	{"classification", required_argument, 0, 'c'},
	{"records", required_argument, 0, 'r'},
	{0, 0, 0, 0}
      };
    /* getopt_long stores the option index here. */
    int option_index = 0;
    
    c = getopt_long (argc, argv, "o:r:c:f:",
		     long_options, &option_index);
    
    /* Detect the end of the options. */
    if (c == -1)
      break;
    
    switch (c)
      {
      case 0:
	/* If this option set a flag, do nothing else now. */
	if (long_options[option_index].flag != 0)
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
      case 'f':
	OGRformat = optarg;
	fflag++;
	break;
      case 'c':
	lasf_clr = optarg;
	clflag++;
	break;
      case 'r':
	lasf_recr = optarg;
	rflag++;
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
  
  if (version_flag) lasf_print_version("las2ogr", LAS2OGR_VERSION);
  
  if (help_flag) usage();
  
  if (inflag == 0 && optind >= argc) usage ();
  
  /* Loop through the input LAS file(s) */
  while (optind < argc) {
    fn = argv[optind];
    inflag++;
    optind++;
    if (inflag > 0) {
      //if (outflag == 0) usage();
      bn = malloc(strlen(fn) - 4);
      strncpy(bn, fn, strlen(fn) - 4);
      *(bn + strlen(fn) - 4) = '\0';
      if (outflag == 0) fo = bn;
      //if (rflag == 0) lasf_recr = "xyz";
      if (clflag == 0) lasf_clr = "";

      if (!fflag) OGRformat = "ESRI Shapefile";
 
      if (rflag == 0 && strcmp(OGRformat,"CSV") == 0) {
	rflag++;
	lasf_recr = "xyz";
      }

      /*
       * Open input file for reading and get the header.
       */
      status = lasf_open(fn, &lasfid, 0);
      if (status != lasf_NOERR) {
	lasf_print_error(status, "las2ogr");
	exit(-1);
      }

      lasf_open_header(&lasfid, &lasfhid);
      lasf_get_header(&lasfid, &lasf_hbuffer0);

      OGRRegisterAll();

      hDriver = OGRGetDriverByName(OGRformat);
      if(hDriver == NULL) {
	fprintf(stderr, "%s driver not available.\n", OGRformat);
	exit(1);
      }

      hDS = OGR_Dr_CreateDataSource(hDriver, fo, NULL);
      if(hDS == NULL) {
	fprintf(stderr, "las2ogr: Creation of output file failed.\n");
	exit(1);
      }
      
      hLayer = OGR_DS_CreateLayer(hDS, bn, NULL, wkbPoint25D, NULL);
      if(hLayer == NULL) {
	fprintf(stderr, "las2ogr: Layer creation failed.\n");
	exit(1);
      }

      if (rflag > 0) {
	pf_length = strlen(lasf_recr);
	for (j = 0; j < pf_length; j++) {
	  lasfp = lasf_recr[j];
	  if (lasfp == 'x') {
	    hFieldDefn = OGR_Fld_Create("x", OFTReal);
	    
	    if (OGR_L_CreateField(hLayer, hFieldDefn, TRUE) != OGRERR_NONE) {
	      fprintf(stderr, "las2ogr: Creating Name field failed.\n");
	      exit(1);
	    }
	    OGR_Fld_Destroy(hFieldDefn);
	  }
	  else if (lasfp == 'y') {
	    hFieldDefn = OGR_Fld_Create("y", OFTReal);
	    
	    if (OGR_L_CreateField(hLayer, hFieldDefn, TRUE) != OGRERR_NONE) {
	      fprintf(stderr, "las2ogr: Creating Name field failed.\n");
	      exit(1);
	    }
	    OGR_Fld_Destroy(hFieldDefn);
	  }
	  else if (lasfp == 'z') {
	    hFieldDefn = OGR_Fld_Create("z", OFTReal);
      
	    if (OGR_L_CreateField(hLayer, hFieldDefn, TRUE) != OGRERR_NONE) {
	      fprintf(stderr, "las2ogr: Creating Height field failed.\n");
	      exit(1);
	    }
	    OGR_Fld_Destroy(hFieldDefn);
	  }
	  else if (lasfp == 'i') {
	    hFieldDefn = OGR_Fld_Create("Intensity", OFTReal);
	    
	    if (OGR_L_CreateField(hLayer, hFieldDefn, TRUE) != OGRERR_NONE) {
	      fprintf(stderr, "las2ogr: Creating Name field failed.\n");
	      exit(1);
	    }
	    OGR_Fld_Destroy(hFieldDefn);
	  }
	  else if (lasfp == 'n') {
	    hFieldDefn = OGR_Fld_Create("ReturnNum", OFTReal);
	    
	    if (OGR_L_CreateField(hLayer, hFieldDefn, TRUE) != OGRERR_NONE) {
	      fprintf(stderr, "las2ogr: Creating Name field failed.\n");
	      exit(1);
	    }
	    OGR_Fld_Destroy(hFieldDefn);
	    hFieldDefn = OGR_Fld_Create("NumReturn", OFTReal);
	    
	    if (OGR_L_CreateField(hLayer, hFieldDefn, TRUE) != OGRERR_NONE) {
	      fprintf(stderr, "las2ogr: Creating Name field failed.\n");
	      exit(1);
	    }
	    OGR_Fld_Destroy(hFieldDefn);
	    hFieldDefn = OGR_Fld_Create("ScanDir", OFTReal);
	    
	    if (OGR_L_CreateField(hLayer, hFieldDefn, TRUE) != OGRERR_NONE) {
	      fprintf(stderr, "las2ogr: Creating Name field failed.\n");
	      exit(1);
	    }
	    OGR_Fld_Destroy(hFieldDefn);
	    hFieldDefn = OGR_Fld_Create("FlightEdge", OFTReal);
	    
	    if (OGR_L_CreateField(hLayer, hFieldDefn, TRUE) != OGRERR_NONE) {
	      fprintf(stderr, "las2ogr: Creating Name field failed.\n");
	      exit(1);
	    }
	    OGR_Fld_Destroy(hFieldDefn);
	  }
	  else if (lasfp == 'c') {
	    hFieldDefn = OGR_Fld_Create("Class", OFTReal);
	    if (OGR_L_CreateField(hLayer, hFieldDefn, TRUE) != OGRERR_NONE) {
	      fprintf(stderr, "las2ogr: Creating Name field failed.\n");
	      exit(1);
	    }
	    OGR_Fld_Destroy(hFieldDefn);
	  }
	  else if (lasfp == 's') {
	    hFieldDefn = OGR_Fld_Create("ScanRank", OFTReal);
	    
	    if (OGR_L_CreateField(hLayer, hFieldDefn, TRUE) != OGRERR_NONE) {
	      fprintf(stderr, "las2ogr: Creating Name field failed.\n");
	      exit(1);
	    }
	    OGR_Fld_Destroy(hFieldDefn);
	  }
	  else if (lasfp == 'u') {
	    hFieldDefn = OGR_Fld_Create("UserData", OFTReal);
	    
	    if (OGR_L_CreateField( hLayer, hFieldDefn, TRUE) != OGRERR_NONE) {
	      fprintf(stderr, "las2ogr: Creating Name field failed.\n");
	      exit(1);
	    }
	    OGR_Fld_Destroy(hFieldDefn);
	  }
	  else if (lasfp == 'p') {
	    hFieldDefn = OGR_Fld_Create("PointSrcID", OFTReal);
	    
	    if (OGR_L_CreateField( hLayer, hFieldDefn, TRUE) != OGRERR_NONE) {
	      fprintf(stderr, "las2ogr: Creating Name field failed.\n");
	      exit(1);
	    }
	    OGR_Fld_Destroy(hFieldDefn);
	  }
	  else if (lasfp == 't') {
	    hFieldDefn = OGR_Fld_Create("GPSTime", OFTReal);
	    
	    if (OGR_L_CreateField(hLayer, hFieldDefn, TRUE) != OGRERR_NONE) {
	      fprintf(stderr,"las2ogr: Creating Name field failed.\n");
	      exit(1);
	    }
	    OGR_Fld_Destroy(hFieldDefn);
	  }
	}
      }
      
      /* clfag tells whether to check for classifications, 
	 if 0 classification selection is skipped, if 1 check 
	 classifications against 'lasf_clr. printflag tells
	 whether to print the record, 0 will not print it, 
	 but 1 will*/
      int cflag = 0, printflag = 0;
      char buff[64];
      
      if (!(strlen(lasf_clr) == 0)) cflag++;

      i = 0, n = 0;	  
      while (lasf_read_point(lasfid, lasfhid, n, &lasf_ptbuff0) == lasf_NOERR) {
	//lasf_printf_progress(abs(n), lasf_hbuffer0.numptrecords);
	if (cflag == 1) {
	  strcpy(buff, lasf_clr);
	  if ((lasf_cmpclass(buff, lasf_ptbuff0.classification) == 1)) printflag++;
	}
	
	if ((printflag == cflag)) {
	  /* Print the record if flagged for printing. */
	  x = lasf_ptbuff0.x * lasf_hbuffer0.xscale + lasf_hbuffer0.xoffset;
	  y = lasf_ptbuff0.y * lasf_hbuffer0.yscale + lasf_hbuffer0.yoffset;
	  z = lasf_ptbuff0.z * lasf_hbuffer0.zscale + lasf_hbuffer0.zoffset;

	  hFeature = OGR_F_Create(OGR_L_GetLayerDefn(hLayer));

	  if (rflag !=0) {
	    pf_length = strlen(lasf_recr);
	    for (j = 0; j < pf_length; j++) {
	      lasfp = lasf_recr[j];
	      if (lasfp == 'x') 	OGR_F_SetFieldDouble(hFeature, OGR_F_GetFieldIndex(hFeature, "x"), x);
	      else if (lasfp == 'y') OGR_F_SetFieldDouble(hFeature, OGR_F_GetFieldIndex(hFeature, "y"), y);
	      else if (lasfp == 'z') OGR_F_SetFieldDouble(hFeature, OGR_F_GetFieldIndex(hFeature, "z"), z);
	      else if (lasfp == 'i') OGR_F_SetFieldInteger(hFeature, OGR_F_GetFieldIndex(hFeature, "Intensity"), i);
	      else if (lasfp == 'n') {
	  	lasf_ptrt = lasf_parse_point_returns(lasf_ptbuff0.returns);
		OGR_F_SetFieldInteger( hFeature, OGR_F_GetFieldIndex(hFeature, "ReturnNum"), lasf_ptrt.return_num);
		OGR_F_SetFieldInteger( hFeature, OGR_F_GetFieldIndex(hFeature, "NumReturn"), lasf_ptrt.num_return);
		OGR_F_SetFieldInteger( hFeature, OGR_F_GetFieldIndex(hFeature, "ScanDir"), lasf_ptrt.scan_dir);
		OGR_F_SetFieldInteger( hFeature, OGR_F_GetFieldIndex(hFeature, "FlightEdge"), lasf_ptrt.flight_edge);
	      }
	      else if (lasfp == 'c') OGR_F_SetFieldInteger(hFeature, OGR_F_GetFieldIndex(hFeature, "Class"), lasf_ptbuff0.classification);
	      else if (lasfp == 's') OGR_F_SetFieldInteger(hFeature, OGR_F_GetFieldIndex(hFeature, "ScanRank"), lasf_ptbuff0.scanrank);
	      else if (lasfp == 'u') OGR_F_SetFieldInteger(hFeature, OGR_F_GetFieldIndex(hFeature, "UserData"), lasf_ptbuff0.udata);
	      else if (lasfp == 'p') OGR_F_SetFieldInteger(hFeature, OGR_F_GetFieldIndex(hFeature, "PointSrcID"), lasf_ptbuff0.psrcid);
	      else if (lasfp == 't') OGR_F_SetFieldDouble(hFeature, OGR_F_GetFieldIndex(hFeature, "GPSTime"), lasf_ptbuff0.gpstime);
	    }
	  }
	  OGRGeometryH hPt;
	  hPt = OGR_G_CreateGeometry(wkbPoint25D);
	  OGR_G_SetPoint(hPt, 0, x, y, z);
	  
	  OGR_F_SetGeometry(hFeature, hPt); 
	  OGR_G_DestroyGeometry(hPt);
	  
	  if (OGR_L_CreateFeature(hLayer, hFeature) != OGRERR_NONE) {
	    fprintf(stderr,"las2ogr: Failed to create feature in shapefile.\n");
	    exit(1);
	  }
	  OGR_F_Destroy(hFeature);
	}
	// Reset the print flag
	printflag = 0;
	i++; n--;
      }
      /* close shop */
      OGR_DS_Destroy(hDS);
      lasf_close(&lasfid);
    }
    else usage ();
  }
  exit (0);
}
