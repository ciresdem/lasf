/*----------------------------------------------------------------
 *
 * lasbounds.c 
 *
 * Copyright (C) 2010, 2012, 2015, 2018 Matthew Love <matthew.love@colorado.edu>
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
#include<lasf.h>
#include<gdal/ogr_api.h>

#define LASBOUNDS_VERSION "0.3.1"

#ifndef INFINITY
#define INFINITY (1.0 / 0.0)
#endif 

#ifndef NAN
#define NAN (0.0 / 0.0)
#endif 

/* Flags set by `--version' and `--help' */
static int version_flag;
static int help_flag;

typedef struct {
  double x;
  double y;
}point_t;

typedef struct {
  point_t p1, p2;
}line_t;


/* Get the euclidean distance between p1 and p2 in meters
 */
float
distance_euclid(point_t* p1, point_t* p2) {
  return sqrt( ((p2->y - p1->y) * (p2->y - p1->y)) + ((p2->x - p1->x) * (p2->x - p1->x)) );
}

/* Calculate the angle between two points, from the previous point
 * p1 = current point, p2 = next point, p0 = previous point
 */
float
lasf_atheta(point_t* p1, point_t* p2, point_t* p0) {
  float t, t2, t3, d12x, d12y, d13x, d13y;

  d12x = p1->x - p0->x, d12y = p1->y - p0->y; 
  d13x = p1->x - p2->x, d13y = p1->y - p2->y; 

  t = atan2(d12y, d12x), t2 = atan2(d13y, d13x);

  t3 = (t2 - t);

  if (t3 < 0) t3 = t3 + (2 * M_PI);
  return t3;
}

float
lasf_atheta_degrees(point_t p1, point_t p2, point_t p0) {
  float t, t2, t3, d12x, d12y, d13x, d13y;

  d12x = p1.x - p0.x, d12y = p1.y - p0.y; 
  d13x = p1.x - p2.x, d13y = p1.y - p2.y; 

  t = atan2(d12y, d12x), t2 = atan2(d13y, d13x);

  t3 = (t2 - t) * 180 / M_PI;

  if (t3 < 0) t3 = t3 + 360;
  return t3;
}

/* Return the angle between the two given points 
 */
float
lasf_theta(point_t* p1, point_t* p2) {
  float t = atan2(p2->y - p1->y, p2->x - p1->x);;
  if (t < 0) t = t + (2 * M_PI);
  return t;
}

float
lasf_theta_degrees(point_t* p1, point_t* p2) {
  float t = atan2(p2->y - p1->y, p2->x - p1->x) * 180 / M_PI;
  if (t < 0) t = t + 360;
  return t;
}

/* Three points are a counter-clockwise turn if ccw > 0, clockwise if
 * ccw < 0, and collinear if ccw = 0 because ccw is a determinant that
 * gives the signed area of the triangle formed by p1, p2 and p3.
 */
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
lasf_intersectp(line_t l1, line_t l2) {//point_t* p1, point_t* p2, point_t* p3, point_t* p4) {
  float den = (l1.p2.x - l1.p1.x) * (l2.p2.y - l2.p1.y) - (l1.p2.y - l1.p1.y) * (l2.p2.x - l2.p1.x);
  
  if (fabs(den) == 0) return 0; // parallel lines

  float u = ((l2.p1.x - l1.p1.x) * (l2.p2.y - l2.p1.y) - (l2.p1.y - l1.p1.y) * (l2.p2.x - l2.p1.x)) / den;
  float v = ((l2.p1.x - l1.p1.x) * (l1.p2.y - l1.p1.y) - (l2.p1.y - l1.p1.y) * (l1.p2.x - l1.p1.x)) / den;

  if (u <= 0 || u >= 1) return 0; // intersection point not between p1 and p2
  if (v <= 0 || v >= 1) return 0; // intersection point not between p3 and p4

  return 1;
}

int
lasf_inside(point_t* p1, point_t* poly, ssize_t hullsize) {
  int kross = 0, ip, iHull;
  line_t lt, lp;

  lt.p1 = *p1, lt.p2.x = p1->x, lt.p2.y = INT_MAX;
  
  for (iHull = 0; iHull < hullsize-1; iHull++) {
    lp.p1 = poly[iHull], lp.p2 = poly[iHull+1];
    ip = lasf_intersect(lp,lt,0.000000001);
    //ip = lasf_intersect(lp,lt,FLT_EPSILON);

    if (ip == 0) return 1;
    if (ip == 1) kross++;
  }
  return kross & 1;
}

/* Generate the concave hull using the given distance threshold
 */
ssize_t
lasf_dpw_concave(point_t* points, int npoints, float d) {
  int i, min, M, k, j;
  int preset = 0;
  float th, cth;
  point_t t, t0;
  line_t l1, l2;

  /* Find the minimum y value in the dataset */
  for (min = 0, i = 1; i < npoints - 1; i++)
    if (points[i].y < points[min].y) min = i;

  /* points[npoints - 1] = points[min]; */
  /* points[npoints - 1].x = points[npoints - 1].x - (d * 100); */

  /* Loop through all the points, starting with the point found above. */
  for (M = 0; M < npoints; M++) {
    t = points[M], points[M] = points[min], points[min] = t;
    min = -1, k = 0, th = 2 * M_PI;

    /* set the last point for angle calculations */
    if (M > 0) t0 = points[M - 1];
    /* Re-insert the first point into the dataset */
    /* if (preset == 0 && M > 5) */
    /*   if (lasf_theta(&points[0], &points[M]) > 1) points[npoints - 1] = points[0], preset++; */
    if (M > 10) points[npoints - 1] = points[0];

    /* Loop through the remaining points and find the next concave point; 
       less than distance threshold -> less than working theta -> does not 
       intersect existing boundary */
    for (i = M + 1; i < npoints; i++)
      if (distance_euclid(&points[M], &points[i]) < d) {
	if (M == 0) cth = lasf_theta(&points[M], &points[i]);
	else cth = lasf_atheta(&points[M], &points[i], &t0);
	if (fmaxf(cth, 0.0) == cth && fminf(cth, th) == cth) {
	//if (cth > 0  && cth < th) {
	  /* Make sure the selected point doesn't create a line segment which
	     intersects with the existing hull. */
	  if (M > 0)
	    for (k = 0, j = 0; j < M; j++) {
	      l1.p1 = points[M], l1.p2 = points[i];
	      l2.p1 = points[j], l2.p2 = points[j+1];
	      //if (lasf_intersectp(&points[M], &points[i], &points[j], &points[j + 1]) == 1) k++;
	      if (lasf_intersect(l1,l2,0) > 0) k++;
	    }
	  /* If no segments are crossed, add this point to the hull */
	  if (k == 0)
	    min = i, th = cth; 
	}
      }
    /* No point was found */
    if (min == -1) return min;
    /* The first point was found again, a successful hull was found! end here. */
    if (min == npoints - 1) {
      points[M + 1] = points[0];
      return M + 1;
    }
  }
}

void 
usage() {
  fprintf(stderr, "\
lasbounds infile [Options]\n\n\
  Generate an OGR compatible vector file depicting the boundary \n\
  (concave hull) of the given LAS file.\n\
\n\
  infile\t\tThe input LAS file \n\
\n\
 Options:\n\
  -o, --output\t\tThe output OGR file. (if not specified,\n\
\t\t\twill use the name of the input LAS file.), \n\
  -f, --format\t\tDesired output OGR compatible format\n\
     Note: Default will be 'ESRI Shapefile', check `ogr2ogr --formats` to see valid formats.\n\n\
  -d, --distance\tThe distance threshold. [default is 1]\n\
  -b, --buffer\t\tBuffer the output polygon, give a buffer distance here.\n\
     Note: The distance and buffer parameters should be in the native units of the input data.\n\n\
\n\
  --help\t\tPrint this help menu and exit.\n\
  --version\t\tPrint version information and exit.\n\n\
");
  exit(1);
}

/* Compare function for use in qsort
 */
int
compare(const void* a, const void* b) {
  const point_t *elem1 = a;    
  const point_t *elem2 = b;

  if (elem1->x > elem2->x) return -1;
  else if (elem1->x < elem2->x) return 1;
  else return 0;
}

int
main (int argc, char **argv) {
  char* fn;
  char* fo;
  char* bn;
  char *OGRformat, *lasf_clr;
  char cbuff[64];
  
  int c, lasfid, lasfhid, i, n, status, npr;
  int inflag = 0, distflag = 0, fflag = 0, clflag = 0, printflag = 0; 
  int outflag = 0, buffflag = 0, pc = 0;
  double dist = 1.0, buffer = 0;
  ssize_t hullsize;

  lasf_pnts lasf_ptbuff0;
  lasf_header lasf_hbuffer0; 

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
	{"distance", required_argument, 0, 'd'},
	{"buffer", required_argument, 0, 'b'},
	{"classification", required_argument, 0, 'c'},
	{0, 0, 0, 0}
      };
    /* getopt_long stores the option index here. */
    int option_index = 0;
    
    c = getopt_long (argc, argv, "d:o:f:b:c:",
		     long_options, &option_index);
    
    /* Detect the end of the options. */
    if (c == -1)
      break;
    
    switch (c) {
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
      case 'd':
	dist = atof( optarg );
	distflag++;
	break;
      case 'b':
	buffer = atof( optarg );
	buffflag++;
	break;
      case 'f':
	OGRformat = optarg;
	fflag++;
	break;
      case 'c':
	lasf_clr = optarg;
	clflag++;
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

  if (version_flag) lasf_print_version("lasbounds", LASBOUNDS_VERSION);

  if (help_flag) usage();

  if (inflag == 0 && optind >= argc) usage ();

  fn = argv[optind];
  inflag++;
  if (inflag > 0) {
    
    /* Determine the basename of the input file for use as an OGR layer name
     */
    if (outflag == 0) fo = fn;
    bn = malloc(strlen(fo) - 4);
    strncpy(bn, fo, strlen(fo) - 4);
    *(bn + strlen(fo) - 4) = '\0';
    if (outflag == 0) fo = bn;
    
    if (clflag==0) lasf_clr = "";

    /* Open input file for reading and get the header.
     */
    status = lasf_open(fn, &lasfid, 0);
    if (status != lasf_NOERR) {
      lasf_print_error(status, "lasbounds");
      exit(-1);
    }
    
    fprintf(stderr,"lasbounds: Working on file: %s\n", fn);
    
    if (!fflag) OGRformat = "ESRI Shapefile";
    
    lasf_open_header(&lasfid, &lasfhid);
    lasf_get_header(&lasfid, &lasf_hbuffer0);

    /* 
     * clfag tells whether to check for classifications, 
     * if 0 classification selection is skipped, if 1 check 
     * classifications against 'lasf_clr. printflag tells
     * whether to print the record, 0 will not print it, 
     * but 1 will
     */
    int cflag = 0;

    if (!(strlen(lasf_clr) == 0)) cflag++;

    OGRRegisterAll();
    
    hDriver = OGRGetDriverByName(OGRformat);
    if(hDriver == NULL) {
      fprintf(stderr, "%s driver not available.\n", OGRformat);
      exit(1);
    }
    
    hDS = OGR_Dr_CreateDataSource(hDriver, fo, NULL);
    if(hDS == NULL) {
      fprintf(stderr, "lasbounds: Creation of output file failed.\n");
      exit(1);
    }
    
    hLayer = OGR_DS_CreateLayer(hDS, bn, NULL, wkbPolygon, NULL);
    if(hLayer == NULL) {
      fprintf(stderr, "lasbounds: Layer creation failed.\n");
      exit(1);
    }
    
    hFeature = OGR_F_Create(OGR_L_GetLayerDefn(hLayer));
    
    /* Allocate memory for the point array using the total number of points in
     * the input LAS file header.
     */
    point_t* pnts;  
    pnts = (point_t*) malloc((lasf_hbuffer0.numptrecords+1) * sizeof(point_t));  
    
    if (!pnts) {
      printf("lasbounds: Failed to allocate memory needed for given array %d\n", __LINE__);
      exit(0);
    }
    
    point_t* pnts2;  
    pnts2 = (point_t*) malloc((lasf_hbuffer0.numptrecords+1) * sizeof(point_t));  
    
    if (!pnts2) {
      printf("lasbounds: Failed to allocate memory needed for given array %d\n", __LINE__);
      exit(0);
    }
    
    /* Loop through the point records */
    i = 0, n = 0;
    //int j = 0;
    while (lasf_read_point(lasfid, lasfhid, n, &lasf_ptbuff0) == lasf_NOERR) {

      /* 
       * If the select-by-classification flag is set, check if to print 
       */
      if (cflag == 1) {
	strcpy(cbuff, lasf_clr);
	if (lasf_cmpclass(cbuff, lasf_ptbuff0.classification) == 1) printflag++;
      }
      if (printflag == cflag) {
	pnts[i].x = lasf_ptbuff0.x * lasf_hbuffer0.xscale + lasf_hbuffer0.xoffset;
	pnts[i].y = lasf_ptbuff0.y * lasf_hbuffer0.yscale + lasf_hbuffer0.yoffset;
	pnts2[i].x = pnts[i].x,	pnts2[i].y = pnts[i].y;
	i++;
      }
      n--;
      printflag = 0;
    }

    //fprintf(stderr,"%d\n",i);
    npr = i;
    
    /* The distance parameter can't be less than zero 
     */
    if (dist > 0) hullsize = -1;
    else hullsize = 0;
    
    /* Generate the concave hull 
     */
    while (hullsize == -1) {
      qsort(pnts, npr, sizeof(point_t), compare);	
      hullsize = lasf_dpw_concave(pnts, npr, dist);
      /* If a hull was found, check that it gathered all the points.
       * If there are points still outside the boundary, increase the
       * distance and retry.
       */
      if (hullsize > 0)
	for (i = hullsize; i < npr; i++)
	  if (!lasf_inside(&pnts[i], pnts, hullsize)) {
	    hullsize=-1;
	    break;
	  }
      
      if (hullsize == -1) {
	fprintf(stderr,"\rlasbounds: Increasing distance ( %f )",dist);
	fflush(stderr);
	dist = dist + (dist * 0.5), pc++;
	memcpy(pnts, pnts2, sizeof(point_t)*npr);
      }
      
      /* In case something funky happens. 
       */
      if (dist == INFINITY || dist == NAN || dist <= FLT_EPSILON) hullsize = 0;
    }
    
    if (pc > 0) fprintf(stderr,"\n");

    /* Print out the hull 
     */
    OGRGeometryH hRing;
    hRing = OGR_G_CreateGeometry(wkbLinearRing);
    
    for (i = 0; i < hullsize; i++)
      OGR_G_AddPoint_2D(hRing, pnts[i].x, pnts[i].y);
    
    OGRGeometryH hPoly;
    hPoly = OGR_G_CreateGeometry(wkbPolygon);

    OGR_G_CloseRings(hRing);

    OGR_G_AddGeometry(hPoly,hRing);

    OGRGeometryH hBPoly;
    if (buffflag == 1) {
      hBPoly = OGR_G_Buffer(hPoly, buffer, 30);
      if (hBPoly == NULL) {
	fprintf(stderr, "lasbounds: Buffering Failed.\n");
	buffflag=0;
      }
    }

    if (buffflag == 1) {
      OGR_F_SetGeometry(hFeature, hBPoly);
    }
    else {
      OGR_F_SetGeometry(hFeature, hPoly);
    }

    OGR_G_DestroyGeometry(hPoly);
    OGR_G_DestroyGeometry(hRing);
    if (buffflag == 1) {
      OGR_G_DestroyGeometry(hBPoly);
    }
    
    if (OGR_L_CreateFeature(hLayer, hFeature) != OGRERR_NONE) {
      fprintf(stderr,"lasbounds: Failed to create feature in shapefile.\n");
      exit(1);
    }
    OGR_F_Destroy(hFeature);
    
    fprintf(stderr, "lasbounds: Found %d concave boundary points using a distance threshold of %.6f.\n", hullsize, dist);
    
    /* Free the pnts and close the input las file 
     */
    free(pnts);
    free(pnts2);
    free(bn);
    OGR_DS_Destroy(hDS);
    lasf_close(&lasfid);
  }
  else usage ();
  exit(0);
}




