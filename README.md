## LASF

### LASFile C Library and Utilities

LASFile provides a C library and set of command-line utilities for utilizing the LAS 1.x file format.

C Library:

- Public Header: lasf.h

Utilities:

- Command-Line Program: `lasdump`

```
lasdump infile [Options]

  Dump the contents of an LAS file to stdout.

  infile		The input LAS file 

 Options:
  -d, --delimiter	The output ascii delimeter, default is comma ','.
  -m, --comment_char	The output ascii comment character for commenting 
			the header, default is pound '#'.
  -h, --header_only	Dump only the header.
  -v, --no_vheader	Omit dumping the variable-lenght headers.
     Note: Default dumps the variable length headers along with
           the main header.

  -p, --points_only	Dump only the points.
     Note: Setting both -p and -h will enact the default behavior, 
           which is to dump them both.

 Point Options:
  -x, --xyz_only	Dump only the xyz values of the point record when dumping points. 
     Note: This is a shortcut for `-r xyz` 
  -c, --classification	Select by classification(s); separate values 
			with a '/'; e.g. -c 2/9/11 to select classes 2, 9 and 11.
  -r, --records		String of desired output records, default is 'xyz', 
     Note: Possible record values include: 'xyzincsuptXYZ':

       x->x (The scaled 'x' value from the point record), 
       y->y (The scaled 'y' value from the point record), 
       z->z (The scaled 'z' value from the point record), 
       i->intensity (The 'Intensity' value from the point record), 
       n->returns[5] (The 5 return values from the point record), 
       c->classification (The 'Classification' value from the point record), 
       s->scan_angle (The 'Scan Angle' value from the point record), 
       u->user_data (The 'User Data' value from the point record), 
       p->pt_id (The point ID value from the point record), 
       t->gps_time (The GPS-Time from the point record (check the 'global' 
                    value in the header to determine the format of the GPS Time))
       R->red (The `Red` value from the point RGB)
       G->green (The `Green` value from the point RGB)
       B->blue (The `Blue` value from the point RGB)
       X->ix (The raw unscaled 'x' value from the point record), 
       Y->iy (The raw unscaled 'y' value from the point record), 
       Z->iz (The raw unscaled 'z' value from the point record), 

  -l, --line_num	Include line numbers in the point output. [numbers start at 0 (zero)]

  --help		Print this help menu and exit.
  --version		Print version information and exit.
```

- Command-Line Program: `lasscan`

```
  Print some information about the given LAS file(s) to stdout.

  infile(s)	The input LAS file(s) 

 Options:
  -v, --variable-headers	Print out the headers for the Variable-Length records
  -s, --scan		Scan the input LAS file and report findings.
     Note: Default prints out information contained in the header 
           of the file, setting this option will scan the point records 
           for information.

  --help	Print this help menu and exit.
  --version	Print version information and exit.
```

- Command-Line Program: `lascopy`

```
  Copy the given LAS file to an LAS file.  The default will output a 
  file version 1.4 with point record version 10.

  infile(s)	The input LAS file(s) 

 Options:
  -f, --format		The output LAS file format, (0-4)
  -p, --pointformat	The output LAS file point format, (0-10)

     Note: Converting between various point formats may result in the loss of data. 
 
  --help	Print this help menu and exit.
  --version	Print version information and exit.
```
