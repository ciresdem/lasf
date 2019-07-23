/*----------------------------------------------------------------
 *
 *  lasdump.c 
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
#include "../src/lasf.h"

#define LASDUMP_VERSION "0.2.1"

/* Flags set by `--version' and `--help' */
static int version_flag;
static int help_flag;

void 
usage() {
  fprintf(stderr, "\
lasdump infile [Options]\n\n\
  Dump the contents of an LAS file to stdout.\n\
\n\
  infile\t\tThe input LAS file \n\
\n\
 Options:\n\
  -d, --delimiter\tThe output ascii delimeter, default is comma ','.\n\
  -m, --comment_char\tThe output ascii comment character for commenting \n\
\t\t\tthe header, default is pound '#'.\n\
  -h, --header_only\tDump only the header.\n\
  -v, --no_vheader\tOmit dumping the variable-lenght headers.\n\
     Note: Default dumps the variable length headers along with\n\
           the main header.\n\n\
  -p, --points_only\tDump only the points.\n\
     Note: Setting both -p and -h will enact the default behavior, \n\
           which is to dump them both.\n\n\
 Point Options:\n\
  -x, --xyz_only\tDump only the xyz values of the point record when dumping points. \n\
     Note: This is a shortcut for `-r xyz` \n\
  -c, --classification\tSelect by classification(s); separate values \n\
\t\t\twith a '/'; e.g. -c 2/9/11 to select classes 2, 9 and 11.\n\
  -r, --records\t\tString of desired output records, default is 'xyz', \n\
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
                    value in the header to determine the format of the GPS Time))\n\
       R->red (The `Red` value from the point RGB)\n\
       G->green (The `Green` value from the point RGB)\n\
       B->blue (The `Blue` value from the point RGB)\n\
       X->ix (The raw unscaled 'x' value from the point record), \n\
       Y->iy (The raw unscaled 'y' value from the point record), \n\
       Z->iz (The raw unscaled 'z' value from the point record), \n\n\
  -l, --line_num\tInclude line numbers in the point output. [numbers start at 0 (zero)]\n\n\
  --help\t\tPrint this help menu and exit.\n\
  --version\t\tPrint version information and exit.\n\n\
" );
  exit(1);
}

int
main (int argc, char **argv) {
  char* fn;
  char *lasf_od, *lasf_cc;
  char *lasf_clr, *lasf_recr;
  char lasfp;
  char* od0;
  int c, i, j, n, status, lasfid, lasfhid, pf_length;
  int dflag = 0, inflag = 0, ccflag = 0, hflag = 0, pflag = 0,  \
    xflag = 0, lflag = 0, vflag = 0, clflag = 0, printflag = 0, rflag = 0;
  int xdecnum = 1, ydecnum = 1, zdecnum = 1;
  char cbuff[64];
  
  lasf_header lasf_hbuffer0;
  lasf_pnts lasf_ptbuff0;
  lasf_pntreturns lasf_ptrt;
	      
  while (1) {
    static struct option long_options[] =
      {
	/* These options set a flag. */
	{"version", no_argument, &version_flag, 1 },
	{"help", no_argument, &help_flag, 1 },
	/* These options don't set a flag.
	   We distinguish them by their indices. */
	{"delimiter", required_argument, 0, 'd'},
	{"comment_char", required_argument, 0, 'm'},
	{"header_only", no_argument, 0, 'h'},
	{"no_vheader", no_argument, 0, 'v'},
	{"points_only", no_argument, 0, 'p'},
	{"xyz_only", no_argument, 0, 'x'},
	{"classification", required_argument, 0, 'c'},
	{"records", required_argument, 0, 'r'},
	{"line_num", no_argument, 0, 'l'},
	{0, 0, 0, 0}
      };
    /* getopt_long stores the option index here. */
    int option_index = 0;
    
    c = getopt_long(argc, argv, "d:m:hvpxc:r:l",
		    long_options, &option_index);

    /* Detect the end of the options. */
    if (c == -1) break;
    
    switch (c)
      {
      case 0:
	/* If this option set a flag, do nothing else now. */
	if (long_options[option_index].flag != 0)
	  break;
	printf("option %s", long_options[option_index].name);
	if (optarg) printf(" with arg %s", optarg);
	printf("\n");
	break;
	
      case 'd':
	lasf_od = optarg;
	dflag++;
	break;
      case 'm':
	lasf_cc = optarg;
	ccflag++;
	break;
	
      case 'c':
	lasf_clr = optarg;
	clflag++;
	break;
	
      case 'h':
	hflag++;
	  break;
      case 'v':
	vflag++;
	break;
      case 'p':
	pflag++;
	break;
      case 'r':
	lasf_recr = optarg;
	rflag++;
	break;
      case 'x':
	xflag++;
	break;
      case 'l':
	lflag++;
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

  if (version_flag) lasf_print_version("lasdump", LASDUMP_VERSION);

  if (help_flag) usage();

  if (inflag == 0 && optind >= argc) usage ();

  /* Loop through the input LAS file(s) */
  fn = argv[optind];
  inflag++;
  
  if (inflag > 0) {
    if (dflag == 0) lasf_od = ",";
    if (ccflag == 0) lasf_cc = "#";
    if (clflag==0) lasf_clr = "";
    if (rflag == 0) lasf_recr = "xyzincsupgRGBw";
    if (xflag) lasf_recr = "xyz";

    od0 = lasf_od;
    
    /* 
     * clfag tells whether to check for classifications, 
     * if 0 classification selection is skipped, if 1 check 
     * classifications against 'lasf_clr. printflag tells
     * whether to print the record, 0 will not print it, 
     * but 1 will
     */
    int cflag = 0;
    
    if (!(strlen(lasf_clr) == 0))
      cflag++;
    
    // Open the LAS File
    status = lasf_open(fn, &lasfid, 0);	  
    if (status != lasf_NOERR) {
      lasf_print_error(status, "lasdump");
      exit(-1);
    }
    
    // Open the header
    status = lasf_open_header(&lasfid, &lasfhid);
    if (status != lasf_NOERR) {
      lasf_print_error(status, "lasdump");
      exit(-1);
    }
    
    // Get the header
    status = lasf_get_header(&lasfid, &lasf_hbuffer0);
    if (status != lasf_NOERR) {
      lasf_print_error(status, "lasdump");
      exit(-1);
    }
    
    /* dump the header */
    if ((hflag > 0 && pflag == 0) || (hflag == 0 && pflag == 0) || (hflag > 0 && pflag > 0)) {
      printf("%s File Signature: %s\n", lasf_cc, lasf_hbuffer0.filesig);
      printf("%s Global: %d\n", lasf_cc, lasf_hbuffer0.globalenc);
      printf("%s GUID1: %d\n", lasf_cc, lasf_hbuffer0.guid1);
      printf("%s GUID2: %d\n", lasf_cc, lasf_hbuffer0.guid2);
      printf("%s GUID3: %d\n", lasf_cc, lasf_hbuffer0.guid3);
      printf("%s GUID4: %d,%d,%d,%d,%d,%d,%d,%d\n", lasf_cc, lasf_hbuffer0.guid4[0], \
	     lasf_hbuffer0.guid4[1], lasf_hbuffer0.guid4[2], lasf_hbuffer0.guid4[3], \
	     lasf_hbuffer0.guid4[4], lasf_hbuffer0.guid4[5], lasf_hbuffer0.guid4[6], \
	     lasf_hbuffer0.guid4[7]);
      printf("%s Version Major: %d\n", lasf_cc, lasf_hbuffer0.vermajor);
      printf("%s Version Minor: %d\n", lasf_cc, lasf_hbuffer0.verminor);
      printf("%s System ID: %s\n", lasf_cc, lasf_hbuffer0.sysid);
      printf("%s Generating Software: %s\n", lasf_cc, lasf_hbuffer0.gensoftware);
      printf("%s File Day: %d\n", lasf_cc, lasf_hbuffer0.fileday);
      printf("%s File Year: %d\n", lasf_cc, lasf_hbuffer0.fileyear);
      printf("%s Header Size (bytes): %d\n", lasf_cc, lasf_hbuffer0.headersize);
      printf("%s Offset to Point Records (bytes): %d\n", lasf_cc, lasf_hbuffer0.offset);
      printf("%s Number of Variable Length Records: %d\n", lasf_cc, lasf_hbuffer0.numvlrecords);
      printf("%s Point Record Format: %u\n", lasf_cc, lasf_hbuffer0.pointformat);
      printf("%s Point Record Length (bytes): %d\n", lasf_cc, lasf_hbuffer0.pointreclen);
      printf("%s Number of Point Records: %d\n", lasf_cc, lasf_hbuffer0.numptrecords);
      printf("%s Number of Points by Return: %d,%d,%d,%d,%d\n",		\
	     lasf_cc, lasf_hbuffer0.numptbyreturn[0], lasf_hbuffer0.numptbyreturn[1], \
	     lasf_hbuffer0.numptbyreturn[2], lasf_hbuffer0.numptbyreturn[3], \
	     lasf_hbuffer0.numptbyreturn[4]);
      printf("%s X-Scale: %.10f\n", lasf_cc, lasf_hbuffer0.xscale);
      printf("%s Y-Scale: %.10f\n", lasf_cc, lasf_hbuffer0.yscale);
      printf("%s Z-Scale: %.10f\n", lasf_cc, lasf_hbuffer0.zscale);
      printf("%s X-Offset: %.10f\n", lasf_cc, lasf_hbuffer0.xoffset);
      printf("%s Y-Offset: %.10f\n", lasf_cc, lasf_hbuffer0.yoffset);
      printf("%s Z-Offset: %.10f\n", lasf_cc, lasf_hbuffer0.zoffset);
      printf("%s Maximum X Value: %.6f\n", lasf_cc, lasf_hbuffer0.xmax);
      printf("%s Minimum X Value: %.6f\n", lasf_cc, lasf_hbuffer0.xmin);
      printf("%s Maximum Y Value: %.6f\n", lasf_cc, lasf_hbuffer0.ymax);
      printf("%s Minimum Y Value: %.6f\n", lasf_cc, lasf_hbuffer0.ymin);
      printf("%s Maximum Z Value: %.2f\n", lasf_cc, lasf_hbuffer0.zmax);
      printf("%s Minimum Z Value: %.2f\n", lasf_cc, lasf_hbuffer0.zmin);
      if (lasf_hbuffer0.verminor > 2) {
	printf("%s Waveform: %d\n", lasf_cc, lasf_hbuffer0.waveform);
      }
      if (lasf_hbuffer0.verminor > 3) {
	printf("%s Offset to Extended Variable Length Records: %d\n", lasf_cc, lasf_hbuffer0.extendedvlr);
	printf("%s Number of Extended Variable Length Records: %d\n", lasf_cc, lasf_hbuffer0.numextendedvlr);
	printf("%s Number of Point Records: %d\n", lasf_cc, lasf_hbuffer0.numptrecords14);
	printf("%s Number of Points by Return: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", \
	       lasf_cc, lasf_hbuffer0.numptbyreturn14[0],			\
	       lasf_hbuffer0.numptbyreturn14[1],				\
	       lasf_hbuffer0.numptbyreturn14[2],				\
	       lasf_hbuffer0.numptbyreturn14[3],				\
	       lasf_hbuffer0.numptbyreturn14[4],				\
	       lasf_hbuffer0.numptbyreturn14[5],				\
	       lasf_hbuffer0.numptbyreturn14[6],				\
	       lasf_hbuffer0.numptbyreturn14[7],				\
	       lasf_hbuffer0.numptbyreturn14[8],				\
	       lasf_hbuffer0.numptbyreturn14[9],				\
	       lasf_hbuffer0.numptbyreturn14[10],			\
	       lasf_hbuffer0.numptbyreturn14[11],			\
	       lasf_hbuffer0.numptbyreturn14[12],			\
	       lasf_hbuffer0.numptbyreturn14[13],			\
	       lasf_hbuffer0.numptbyreturn14[14]);
      }
      
      /* Dump the Variable Length Headers */
      if (vflag == 0) {
	int vlhid;
	lasf_vlheader lasf_vlhbuff;
	for (i = 0; i < lasf_hbuffer0.numvlrecords; i++) {
	  
	  status = lasf_open_vlheader(&lasfid, &lasfhid, &vlhid, i);
	  if (status != lasf_NOERR) {
	    lasf_print_error(status, "lasdump");
	    exit(-1);
	  }
	  
	  status = lasf_get_vlheader(&vlhid, &lasf_vlhbuff);
	  if (status != lasf_NOERR) {
	    lasf_print_error(status, "lasdump");
	    exit(-1);
	  }
	  
	  printf("%s Reserved: %d\n", lasf_cc, lasf_vlhbuff.reserved);
	  printf("%s User ID: %s\n", lasf_cc, lasf_vlhbuff.userid);
	  printf("%s Record ID: %d\n", lasf_cc, lasf_vlhbuff.recordid);
	  printf("%s Record Length: %d\n", lasf_cc, lasf_vlhbuff.recordlen);
	  printf("%s Description: %s\n", lasf_cc, lasf_vlhbuff.description);

	  status = lasf_print_vldata(&vlhid);

	}
      }
    }
    
    /* Loop through the point records and dump them out*/
    if ((hflag == 0 && pflag > 0) || (hflag == 0 && pflag == 0) || (hflag > 0 && pflag > 0)) {	      
      /* Get the decimal value to print */
      n = 10;
      while (lasf_hbuffer0.xscale * n != floor(lasf_hbuffer0.xscale * n) && xdecnum != 7) {
      	n = n * 10;
      	xdecnum++;
      }
      n = 10;
      while (lasf_hbuffer0.yscale * n != floor(lasf_hbuffer0.yscale * n) && ydecnum != 7 ) {
      	n = n * 10;
      	ydecnum++;
      }
      n = 10;
      while (lasf_hbuffer0.zscale * n != floor(lasf_hbuffer0.zscale * n) && zdecnum != 7) {
      	n = n * 10;
      	zdecnum++;
      }

      /* Read the points from file and print them if desired*/
      n = 0, i = 0;
      pf_length = strlen(lasf_recr);
      
      while (lasf_read_point(lasfid, lasfhid, n, &lasf_ptbuff0) == lasf_NOERR) {

	if (cflag == 1) {
	  strcpy(cbuff, lasf_clr);
	  if (lasf_cmpclass(cbuff, lasf_ptbuff0.classification) == 1) printflag++;
	}
	if ((lflag != 0))
	  printf("%d%s", i, od0);
	
	if (printflag == cflag) {
	  if (rflag == 0) {
	    printf("%.*f%s%.*f%s%.*f\n",				\
		   xdecnum, lasf_ptbuff0.x * lasf_hbuffer0.xscale + lasf_hbuffer0.xoffset, lasf_od, \
		   ydecnum, lasf_ptbuff0.y * lasf_hbuffer0.yscale + lasf_hbuffer0.yoffset, lasf_od, \
		   zdecnum, lasf_ptbuff0.z * lasf_hbuffer0.zscale + lasf_hbuffer0.zoffset);
	  }
	  else {
	    for (j = 0; j < pf_length; j++) {
	      lasfp = lasf_recr[j];
	      if (j == pf_length - 1) {
		lasf_od = "\n";
	      }
	      else {
		lasf_od = od0;
	      }
	      if (lasfp == 'x') {
		printf("%.*f%s",			   \
		       xdecnum, lasf_ptbuff0.x * lasf_hbuffer0.xscale +	\
		       lasf_hbuffer0.xoffset, lasf_od);
	      }
	      else if (lasfp == 'y') {
		printf("%.*f%s",					\
		       ydecnum, lasf_ptbuff0.y * lasf_hbuffer0.yscale +	\
		       lasf_hbuffer0.yoffset, lasf_od);
	      }
	      else if (lasfp == 'z') {
		printf("%.*f%s",					\
		       zdecnum, lasf_ptbuff0.z * lasf_hbuffer0.zscale +	\
		       lasf_hbuffer0.zoffset, lasf_od);
	      }
	      else if (lasfp == 'i') {
		printf("%d%s", lasf_ptbuff0.intensity, lasf_od);
	      }
	      else if (lasfp == 'n') {
		lasf_ptrt = lasf_parse_point_returns(lasf_ptbuff0.returns);
		
		printf("%d%s%d%s%d%s%d%s",				\
		       lasf_ptrt.return_num, od0, lasf_ptrt.num_return,	\
		       od0, lasf_ptrt.scan_dir, od0, lasf_ptrt.flight_edge, lasf_od);
	      }
	      else if (lasfp == 'c') {
		printf("%d%s", lasf_ptbuff0.classification, lasf_od);
	      }
	      else if (lasfp == 's') {
		printf("%d%s", lasf_ptbuff0.scanrank, lasf_od);
	      }
	      else if (lasfp == 'u') {
		printf("%d%s", lasf_ptbuff0.udata, lasf_od);
	      }
	      else if (lasfp == 'p') {
		printf("%d%s", lasf_ptbuff0.psrcid, lasf_od);
	      }
	      else if (lasfp == 't') {
		printf("%f%s", lasf_ptbuff0.gpstime, lasf_od);
	      }
	      else if (lasfp == 'X') {
		printf("%d%s", lasf_ptbuff0.x, lasf_od);
	      }
	      else if (lasfp == 'Y') {
		printf("%d%s", lasf_ptbuff0.y, lasf_od);
	      }
	      else if (lasfp == 'Z') {
		printf("%d%s", lasf_ptbuff0.z, lasf_od);
	      }
	      else {
		printf("%s", lasf_od);
	      }
	    }
	  }
	}
	printflag = 0, n--, i++;
      }
    }
    /* close shop */
    status = lasf_close(&lasfid);
    if (status != lasf_NOERR) {
      lasf_print_error(status, "lasdump");
      exit(-1);
    }
  }
  else {
    usage ();
  }
  exit(1);
}
