/*---------------------------------------------------------------
 *
 * lascopy.c
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
#include "lasf.h"

#define LASCOPY_VERSION "0.2.3"

/* Flags set by `--version' and `--help' */
static int version_flag;
static int help_flag;

/* Usage */
void
usage() {
  fprintf(stderr, "\
lascopy infile(s) [Options]\n\n\
  Copy the given LAS file to an LAS file.  The default will output a \n\
  file version 1.4 with point record version 10.\n\n\
\
  infile(s)\tThe input LAS file(s) \n\
\n\
 Options:\n\
  -f, --format\t\tThe output LAS file format, (0-4)\n\
  -p, --pointformat\tThe output LAS file point format, (0-10)\n\
\n\
     Note: Converting between various point formats may result in the loss of data. \n \
\n\
  --help\tPrint this help menu and exit.\n\
  --version\tPrint version information and exit.\n\n\
");
  exit(0);
}

int
main (argc, argv)
     int argc;
     char **argv;
{
  char* fn;
  char* fo;
  int c, k, lasfid, lasfhid, lasfid_out, lasfhid_out, status;
  int inflag = 0, i = 0, n = 0;
  unsigned char hformat = 3, pformat = 3;

  lasf_header lasf_hbuffer0;
  lasf_describe lasf_stats;
  lasf_pnts lasf_ptbuff0;

  while (1) {
    static struct option long_options[] =
      {
	/* These options set a flag. */
	{"version", no_argument, &version_flag, 1},
	{"help", no_argument, &help_flag, 1},
	/* These options don't set a flag.
	   We distinguish them by their indices. */
	{"format", required_argument, 0, 'f'},
	{"pointformat", required_argument, 0, 'p'},
	{0, 0, 0, 0}
      };
    /* getopt_long stores the option index here. */
    int option_index = 0;
    
    c = getopt_long(argc, argv, "f:p:",
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
	
      case 'f':
	hformat = atoi(optarg);
	break;
	
      case 'p':
	pformat = atoi(optarg);
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

  if (version_flag) lasf_print_version("lascopy", LASCOPY_VERSION);

  if (help_flag) usage();
  
  if (inflag == 0 && optind >= argc) usage ();

  /* Loop through the input LAS file(s) */
  while (optind < argc) {
    fn = argv[optind];
    inflag++;
    optind++;
    if (inflag > 0) {
      
      // output filename
      char *ret = malloc(strlen(fn) + 4);
      strncpy(ret, fn, strlen(fn) - 4);
      strcpy(&ret[strlen(fn) - 4], "_1.las");
      *(ret + strlen(fn) + 3) = '\0';
      fo = ret;
      
      // open input for reading
      status = lasf_open(fn, &lasfid, 0);
      if (status != lasf_NOERR) {
	lasf_print_error(status, "lascopy");
	exit(-1);
      }
      
      // open output for writing
      status = lasf_open(fo, &lasfid_out, 1);
      if (status != lasf_NOERR) {
	lasf_print_error(status, "lascopy");
	exit(-1);
      }

      fprintf(stderr, "lascopy: Working on file %s\n", fn);
      
      status = lasf_open_header(&lasfid, &lasfhid);
      if (status != lasf_NOERR) {
	lasf_print_error(status, "lascopy");
	exit(-1);
      }
      
      lasf_get_header(&lasfid, &lasf_hbuffer0);
      
      status = lasf_create_header(&lasfid_out, &lasfhid_out);
      if (status != lasf_NOERR) {
	lasf_print_error(status, "lascopy");
	exit(-1);
      }
      
      status = lasf_copy_header (&lasfhid, &lasfhid_out);
      if (status != lasf_NOERR) {
	lasf_print_error(status, "lascopy");
	exit(-1);
      }
      
      status = lasf_put_header_vermajor (&lasfhid_out, 1);
      if (status != lasf_NOERR) {
	lasf_print_error(status, "lascopy");
	exit(-1);
      }
      
      status = lasf_put_header_verminor (&lasfhid_out, hformat);
      if (status != lasf_NOERR) {
	lasf_print_error(status, "lascopy");
	exit(-1);
      }
      
      status = lasf_put_header_pointformat (&lasfhid_out, pformat);
      if (status != lasf_NOERR) {
	lasf_print_error(status, "lascopy");
	exit(-1);
      }
      
      status = lasf_put_header_numvlrecords (&lasfhid_out, 0);
      if (status != lasf_NOERR) {
	lasf_print_error(status, "lascopy");
	exit(-1);
      }
      
      status = lasf_put_header_sysid(&lasfhid_out, lasf_VERSION);
      if (status != lasf_NOERR) {
	lasf_print_error(status, "lascopy");
	exit(-1);
      }
      
      status = lasf_put_header_gensoftware(&lasfhid_out, "lascopy");
      if (status != lasf_NOERR) {
	lasf_print_error(status, "lascopy");
	exit(-1);
      }
      
      status = lasf_set_header_today_date(&lasfhid_out);
      if (status != lasf_NOERR) {
	lasf_print_error(status, "lascopy");
	exit(-1);
      }
      
      status = lasf_set_header_filesig(&lasfhid_out);
      if (status != lasf_NOERR) {
	lasf_print_error(status, "lascopy");
	exit(-1);
      }
      
      status = lasf_write_header(&lasfid_out, &lasfhid_out);
      if (status != lasf_NOERR) {
	lasf_print_error(status, "lascopy");
	exit(-1);
      }
      
      while (lasf_read_point(lasfid, lasfhid, n, &lasf_ptbuff0) == lasf_NOERR) {
	//lasf_printf_progress(abs(n), lasf_hbuffer0.numptrecords);
	status = lasf_add_to_stats(&lasfhid, lasf_ptbuff0, &lasf_stats, i); 
	if (status != lasf_NOERR) {
	  lasf_print_error(status, "lascopy");
	  exit(-1);
	}
	status = lasf_write_point(lasfid_out, lasfhid_out, n, &lasf_ptbuff0);
	if (status != lasf_NOERR) {
	  lasf_print_error(status, "lascopy");
	  exit(-1);
	}
	i++, n--;
      }

      status = lasf_put_header_numptrecords(&lasfhid_out, lasf_stats.n);
      if (status != lasf_NOERR) {
	lasf_print_error(status, "lascopy");
	exit(-1);
      }

      status = lasf_put_header_numptrecords14(&lasfhid_out, lasf_stats.n);
      if (status != lasf_NOERR) {
	lasf_print_error(status, "lascopy");
	exit(-1);
      }
      
      status = lasf_put_header_xmin(&lasfhid_out, lasf_stats.xmin);
      if (status != lasf_NOERR) {
	lasf_print_error(status, "lascopy");
	exit(-1);
      }
      
      status = lasf_put_header_xmax(&lasfhid_out, lasf_stats.xmax);
      if (status != lasf_NOERR) {
	lasf_print_error(status, "lascopy");
	exit(-1);
      }
      
      status = lasf_put_header_ymin(&lasfhid_out, lasf_stats.ymin);
      if (status != lasf_NOERR) {
	lasf_print_error(status, "lascopy");
	exit(-1);
      }
      
      status = lasf_put_header_ymax(&lasfhid_out, lasf_stats.ymax);
      if (status != lasf_NOERR) {
	lasf_print_error(status, "lascopy");
	exit(-1);
      }
      
      status = lasf_put_header_zmin(&lasfhid_out, lasf_stats.zmin);
      if (status != lasf_NOERR) {
	lasf_print_error(status, "lascopy");
	exit(-1);
      }
      
      status = lasf_put_header_zmax(&lasfhid_out, lasf_stats.zmax);
      if (status != lasf_NOERR) {
	lasf_print_error(status, "lascopy");
	exit(-1);
      }
      
      status = lasf_put_header_numptbyreturn(&lasfhid_out, lasf_stats.returns);
      if (status != lasf_NOERR) {
	lasf_print_error(status, "lascopy");
	exit(-1);
      }

      status = lasf_put_header_numptbyreturn14(&lasfhid_out, lasf_stats.returns6);
      if (status != lasf_NOERR) {
	lasf_print_error(status, "lascopy");
	exit(-1);
      }
      
      status = lasf_write_header(&lasfid_out, &lasfhid_out);
      if (status != lasf_NOERR) {
	lasf_print_error(status, "lascopy");
	exit(-1);
      }
      
      status = lasf_close(&lasfid);
      if (status != lasf_NOERR) {
	lasf_print_error(status, "lascopy");
	exit(-1);
      }
      
      status = lasf_close(&lasfid_out);
      if (status != lasf_NOERR) {
	lasf_print_error(status, "lascopy");
	exit(-1);
      }
      fprintf(stderr, "\n");
    }
    else {
      usage ();
    }
  }
  exit (1);
}
//END
