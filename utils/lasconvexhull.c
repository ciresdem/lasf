/*----------------------------------------------------------------
 *
 * lasconvexhull.c 
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
 *------------------------------------------------------------------*/

#include<getopt.h>
#include "GLT.h"

#define LASCONVEXHULL_VERSION "0.0.5"

typedef point_t* point_ptr_t;

double
theta(point_t p1, point_t p2) {
  
  double dx, dy, ax, ay, t;
  float x1, x2, y1, y2;
  float PI = 3.14159;
  x1 = p1.x * (PI / 180);
  x2 = p2.x * (PI / 180);
  y1 = p1.y * (PI / 180);
  y2 = p2.y * (PI / 180);
  dx = x2 - x1; 
  dy = y2 - y1; 
  t = atan2(dy, dx) * 180 / 3.14159;
  if (t < 0) {
    t = t + 360;
  }
  return t;
}

/*
 *  Compare function for use in qsort
*/

int compare(const void* a, const void* b) {
  const point_t *elem1 = a;    
  const point_t *elem2 = b;

  if (elem1->x < elem2->x) return -1;
  else if (elem1->x > elem2->x) return 1;
  else return 0;
}
 
/* Returns a list of points on the convex hull in counter-clockwise order.
 * Note: the last point in the returned list is the same as the first one.
 */
void
convex_hull(point_t* points, ssize_t npoints, point_ptr_t** out_hull, ssize_t* out_hullsize) {
  point_ptr_t* hull;
  ssize_t i, t, k = 0;

  hull = *out_hull;

  /* lower hull */
  for (i = 0; i < npoints; ++i) {
    while (k >= 2 && GLT_ccw(hull[k - 2], hull[k - 1], &points[i]) <= 0) --k;
    hull[k++] = &points[i];
  }
 
  /* upper hull */
  for (i = npoints - 2, t = k + 1; i >= 0; --i) {
    while (k >= t && GLT_ccw(hull[k - 2], hull[k - 1], &points[i]) <= 0) --k;
    hull[k++] = &points[i];
    
  }

  *out_hull = hull;
  *out_hullsize = k;
}

int
pwrap(point_t* points, int npoints) {

  int i, min, M;
  float th, v;
  point_t t;

  for (min = 0, i = 1; i < npoints; i++)
    if (points[i].y < points[min].y) min = i;

  points[npoints] = points[min]; 
  th = 0.0;

  for (M = 0; M < npoints; M++) {

    t = points[M]; 
    points[M] = points[min]; 
    points[min] = t;
    min = npoints; 

    v = th; 
    th = 360.0;

    for (i = M+1; i <= npoints; i++) {
      if (theta(points[M], points[i]) > v)
	if (theta(points[M], points[i]) < th)
	  min = i, th = theta(points[M], points[min]); 
    if (min == npoints) return M;
    }
  }
}

/* Flags set by `--version' and `--help' */
static int version_flag;
static int help_flag;

void 
usage() {
  fprintf(stderr, "\
lasconvexhull infile(s) [Options]\n\n\
  Generate a GMT formatted vector file depicting the convex hull \n\
  of the given LAS file(s) and print to stdout.\n			   \
\n\
  infile(s)\t\tThe input LAS file(s) \n\
\n\
 Options:\n\
\n\
  --help\t\tPrint this help menu and exit.\n\
  --version\t\tPrint version information and exit.\n\n\
");
  exit(1);
}

int
main (int argc, char **argv) {
  char* fn;
  
  int c, GLTid, GLThid, i, n, status;
  int inflag = 0;
  
  GLT_pnts GLT_ptbuff0;
  GLT_header GLT_hbuffer0; 

  point_t pnt;
  point_ptr_t hull0[MAX_HULLS];
  point_ptr_t* hull = hull0;
  ssize_t hullsize;
  
  while (1) {
    static struct option long_options[] =
      {
	/* These options set a flag. */
	{"version", no_argument, &version_flag, 1},
	{"help", no_argument, &help_flag, 1},
	/* These options don't set a flag.
	   We distinguish them by their indices. */
	{0, 0, 0, 0}
      };
    /* getopt_long stores the option index here. */
    int option_index = 0;
    
    c = getopt_long (argc, argv, "",
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
	
      case '?':
	/* getopt_long already printed an error message. */
	break;
	
      default:
	abort ();
      }
  }
  /* Instead of --
   * -- as they are encountered,
   * we report the final status resulting from them. 
   */

  if (version_flag) GLT_print_version("lasconvexhull", LASCONVEXHULL_VERSION);

  if (help_flag) usage();

  if (inflag == 0 && optind >= argc) usage ();

  /*
    For creating a GMT Vector; gdal-type 'GMT'
   */
  printf("# @VGMT1.0 @GPOLYGON\n");
  printf("# @NName\n");
  printf("# @Tstring\n");
  printf("# FEATURE_DATA\n");

  /* 
   * Loop through the input LAS file(s) 
   */
  while (optind < argc) {
    fn = argv[optind];
    inflag++;
    optind++;
    if (inflag > 0) {
      /*
       * Open input file for reading and get the header.
       */
      status = GLT_open(fn, &GLTid, 0);
      if (status != GLT_NOERR) {
	GLT_print_error(status, "lasconvexhull");
	exit(-1);
      }
      
      GLT_open_header(&GLTid, &GLThid);
      GLT_get_header(&GLTid, &GLT_hbuffer0);
      
      /*
       * Allocate memory for the point array using the total number of points in
       * the input LAS file header.
       */
      point_t* pnts;  
      pnts = (point_t*) malloc(GLT_hbuffer0.numptrecords * sizeof(point_t));  
      
      if (!pnts) {
	printf("lasconvexhull: Failed to allocate memory needed for given array %d\n", __LINE__);
	exit(0);
      }
      
      /* 
       * Loop through the point records 
       */
      i = 0, n = 0;	  
      while (GLT_read_point(GLTid, GLThid, n, &GLT_ptbuff0) == GLT_NOERR) {
	pnts[i].x = GLT_ptbuff0.x * GLT_hbuffer0.xscale + GLT_hbuffer0.xoffset;
	pnts[i].y = GLT_ptbuff0.y * GLT_hbuffer0.yscale + GLT_hbuffer0.yoffset;
	i++; n--;
      }
      
      /* 
       * This is for the GMT compatibility...more can be done here.
       */
      printf(">\n");
      printf("# @D%s\n", fn);
      printf("# @P\n");
            
      /* 
       * Sort the points by x
       */
      qsort(pnts, GLT_hbuffer0.numptrecords, sizeof(point_t), compare);
      
      /*
       * Generate the convex hull
       */
      convex_hull(pnts, GLT_hbuffer0.numptrecords, &hull, &hullsize);
      //hullsize = pwrap(pnts, GLT_hbuffer0.numptrecords);

      fprintf(stderr, "lasconvexhull: Found %d points along the convex hull\n", hullsize);

      /* 
       * Print the bounding coordinates
       */
      for (i = 0; i < hullsize; i++) {
	printf("%f %f\n", hull[i]->x, hull[i]->y);
	//printf("%f %f\n", pnts[i].x, pnts[i].y);
      }
      /*
       * Free the pnts and close the input las file
       */
      free(pnts);
      GLT_close(&GLTid);
    }
    else {
      usage ();
    }
  }
  exit (0);
}




