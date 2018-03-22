/*----------------------------------------------------------------
 *
 *  lasscan.c 
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

#define LASSCAN_VERSION "0.2.9"

/* Flags set by `--version' and `--help' */
static int version_flag;
static int help_flag;

/* Usage */
void
usage() {
  fprintf(stderr, "\
lasscan infile(s) [Options]\n\n\
  Print some information about the given LAS file(s) to stdout.\n\n\
\
  infile(s)\tThe input LAS file(s) \n\
\n\
 Options:\n\
  -v, --variable-headers\tPrint out the headers for the Variable-Length records\n\
  -s, --scan\t\tScan the input LAS file and report findings.\n\
     Note: Default prints out information contained in the header \n\
           of the file, setting this option will scan the point records \n\
           for information.\n\n\
  --help\tPrint this help menu and exit.\n\
  --version\tPrint version information and exit.\n\n\
" );
  exit(1);
}


int
main (int argc, char **argv) {
  char* fn;
  int c, k, lasfid, lasfhid, status;
  int inflag = 0, infosflag = 0, vflag = 0;

  lasf_describe las_mm;
  lasf_globalenc lasf_gbe;
  lasf_header lasf_hbuffer0;

  while (1) {
    static struct option long_options[] =
      {
	/* These options set a flag. */
	{"version", no_argument, &version_flag, 1},
	{"help", no_argument, &help_flag, 1},
	/* These options don't set a flag.
	   We distinguish them by their indices. */
	{"scan", no_argument, 0, 's'},
	{"variable-headers", no_argument, 0, 'v'},
	{0, 0, 0, 0}
      };
    /* getopt_long stores the option index here. */
    int option_index = 0;
    
    c = getopt_long(argc, argv, "sv",
		    long_options, &option_index);
    
    /* Detect the end of the options. */
    if ( c == -1 ) break;
    
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
      case 's':
	infosflag++;
	break;
	
      case 'v':
	vflag++;
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

  if (version_flag) lasf_print_version("lasscan", LASSCAN_VERSION);

  if (help_flag) usage();
  
  if (inflag == 0 && optind >= argc) usage ();

  /* Loop through the input LAS file(s) */
  while (optind < argc) {
    fn = argv[optind];
    inflag++;
    optind++;
    if (inflag > 0) {
      // Open the LAS File
      status = lasf_open(fn, &lasfid, 0);
      if (status != lasf_NOERR) {
	lasf_print_error(status, "lasscan");
	exit(-1);
      }

      // Open the header
      status = lasf_open_header( &lasfid, &lasfhid);
      if (status != lasf_NOERR) {
	lasf_print_error(status, "lasscan");
	exit(-1);
      }

      // Get the header
      status = lasf_get_header(&lasfhid, &lasf_hbuffer0);
      if (status != lasf_NOERR) {
	lasf_print_error(status, "lasscan");
	exit(-1);
      }

      printf("%s: %s Version %d.%d (%d) <%s/%s> <date: %d.%d>\n",	\
	     fn, lasf_hbuffer0.filesig, lasf_hbuffer0.vermajor,		\
	     lasf_hbuffer0.verminor, lasf_hbuffer0.pointformat,		\
	     lasf_hbuffer0.sysid, lasf_hbuffer0.gensoftware,		\
	     lasf_hbuffer0.fileday, lasf_hbuffer0.fileyear);
      printf("%s: x_min: %f x_max: %f x_scale: %f x_offset: %f\n",	\
	     fn, lasf_hbuffer0.xmin, lasf_hbuffer0.xmax, lasf_hbuffer0.xscale, \
	     lasf_hbuffer0.xoffset);
      printf("%s: y_min: %f y_max: %f y_scale: %f y_offset: %f\n",	\
	     fn, lasf_hbuffer0.ymin, lasf_hbuffer0.ymax, lasf_hbuffer0.yscale, \
	     lasf_hbuffer0.yoffset);
      printf("%s: z_min: %f z_max: %f z_scale: %f z_offset: %f\n",	\
	     fn, lasf_hbuffer0.zmin, lasf_hbuffer0.zmax, lasf_hbuffer0.zscale, \
	     lasf_hbuffer0.zoffset);
      printf("%s: Total Number of Points: %d By Return: <%d/%d/%d/%d/%d>\n", \
	     fn, lasf_hbuffer0.numptrecords, lasf_hbuffer0.numptbyreturn[0], \
	     lasf_hbuffer0.numptbyreturn[1], lasf_hbuffer0.numptbyreturn[2], \
	     lasf_hbuffer0.numptbyreturn[3], lasf_hbuffer0.numptbyreturn[4], \
	     lasf_hbuffer0.numptbyreturn[5]);

      if (vflag) {
	int vlhid, i;
	lasf_vlheader lasf_vlhbuff;
	for (i = 0; i < lasf_hbuffer0.numvlrecords; i++) {
	  lasf_open_vlheader(&lasfid, &lasfhid, &vlhid, i);
	  
	  lasf_get_vlheader(&vlhid, &lasf_vlhbuff);
	  
	  printf("%s: Variable Length Header (%d) %s <%d/%d>\n",	\
		 fn, i, lasf_vlhbuff.userid, lasf_vlhbuff.reserved, lasf_vlhbuff.recordid);	      
	  printf("%s: Description: < %s >  Record Length: <%d>\n", fn, lasf_vlhbuff.description, lasf_vlhbuff.recordlen);
	}
      }
	  
      // Print some more info obtained from scanning the las file, if desired.
      if (infosflag > 0) {
	status = lasf_scan(&lasfid, &lasfhid, &las_mm);
	if (status != lasf_NOERR) {
	  lasf_print_error(status, "lasscan");
	  exit(-1);
	}
	      
	//lasf_gbe = lasf_parse_global( lasf_hbuffer0.globalenc );
	printf("%s: X: <%f/%f> Y: <%f/%f> Z: <%f/%f>\n",		\
	       fn, las_mm.xmin, las_mm.xmax, las_mm.ymin,		\
	       las_mm.ymax, las_mm.zmin, las_mm.zmax);
	printf("%s: Intensity: <%d/%d> GPS Time: <%f/%f>\n",		\
		fn, las_mm.intensitymin, las_mm.intensitymax,		\
		las_mm.gpstimemin,las_mm.gpstimemax);
	/* Returns */
	printf("%s: Points: <%d> Returns: <", fn, las_mm.n);
	for (k = 0; k < 5; k++) {
	  printf( "%d( %d ) ", k + 1, las_mm.returns[k]);
	}
	      
	int retsum = las_mm.returns[0] + las_mm.returns[1] +	\
	  las_mm.returns[2] + las_mm.returns[3] + las_mm.returns[4];
	if (retsum != las_mm.n)
	  printf(" N/A( %d ) ", las_mm.n-retsum);

	/* Classifications */
	printf("\b>\n%s: Classifications: <", fn);
	for ( k = 0; k < 32; k++ ) {
	  if ( las_mm.classifications[k] > 0) printf("%d( %d ) ", k, las_mm.classifications[k]);
	}
	printf("\b>\n");
      }

      // Close the LAS File
      status = lasf_close(&lasfid);
	  if (status != lasf_NOERR) {
	    lasf_print_error(status, "lasscan");
	    exit(-1);
	  }
    }
    else {
      usage ();
    }
  }
  exit(1);
}
//END
