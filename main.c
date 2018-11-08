#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include "bmplib.h"

/*
 * This method enlarges a 24-bit, uncompressed .bmp file
 * that has been read in using readFile()
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the original number of rows
 * cols     - the original number of columns
 *
 * scale    - the multiplier applied to EACH OF the rows and columns, e.g.
 *           if scale=2, then 2* rows and 2*cols
 *
 * new      - the new array containing the PIXELs, allocated within
 * newrows  - the new number of rows (scale*rows)
 * newcols  - the new number of cols (scale*cols)
 */
int enlarge(PIXEL* original, int rows, int cols, int scale,
	    PIXEL** new, int* newrows, int* newcols)
{
     int i, j;

    if ((rows <= 0) || (cols <= 0)) return -1;

    int r, c;
    *newrows = rows * scale;
    *newcols = cols * scale;
    *new = (PIXEL *) malloc((*newrows) * (*newcols) * sizeof (PIXEL));

    for (r = 0; r < rows; r++) {
        for (c = 0; c < cols; c++) {
            PIXEL *o = original + (r * cols) + c;
            for (i = 0; i < scale; i++) {
                for (j = 0; j < scale; j++) {
                    PIXEL *n = (*new) + scale * r * (*newcols) + c * scale + j + i * (*newcols);
                    (*n) = (*o);
                }
            }
        }
    }
  return 0;
}

/*
 * This method rotates a 24-bit, uncompressed .bmp file that has been read
 * in using readFile(). The rotation is expressed in degrees and can be
 * positive, negative, or 0 -- but it must be a multiple of 90 degrees
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 * rotation - a positive or negative rotation,
 *
 * new      - the new array containing the PIXELs, allocated within
 * newrows  - the new number of rows
 * newcols  - the new number of cols
 */
int rotate(PIXEL* original, int rows, int cols, int rotation,
	   PIXEL** new, int* newrows, int* newcols)
{
    int row, col,i;
    *newrows=rows;
    *newcols=cols;   

  if ((rows <= 0) || (cols <= 0)) return -1;
  *new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));

  if(rotation <0)
  {
    rotation = rotation / (-90);
    for(i=0;i<rotation;i++)
    {
      *new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));
      for (row=0; row < rows; row++)
      {
        for (col=0; col < cols; col++) 
        {
          PIXEL* o = original + row*cols + col;
          PIXEL* n = (*new) + rows*col + (cols-row-1);
          *n = *o;
        }
      }
      original=*new;
      *newcols = rows;
      *newrows = cols;
    }  

  }
  else
  {

    rotation /= 90;
    while(rotation >=4)
    {
      rotation =  rotation % 4;
    }
    if(rotation == 1)
    {
      rotation=3;
    }
    else if(rotation ==3)
    {
      rotation =1;
    }
    
    if(rotation == 0 || rotation == 4)
    {
      *new = original;
    }
    else
    {
      for(i=0;i<rotation;i++)
      {
        *new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));

        for (row=0; row < rows; row++)
        {
          for (col=0; col < cols; col++) 
          {
            PIXEL* o = original + row*cols + col;
            PIXEL* n = (*new) + rows*col + (cols-row-1);
            *n = *o;
          }
        }

        original=*new;
        *newcols = rows;
        *newrows = cols;
      }
    }
  }
  

  return 0;
}

/*
 * This method Vertically flips a 24-bit, uncompressed bmp file
 * that has been read in using readFile().
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 *
 * new      - the new array containing the PIXELs, allocated within
 */

int verticalflip(PIXEL *original, PIXEL **new, int rows, int cols)
{
    int row, col;

  if ((rows <= 0) || (cols <= 0)) return -1;

  *new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));

    for (row = 0; row < rows; row++)
     {
        for (col = 0; col < cols; col++) 
        {
          PIXEL *o = original + row * cols + col;
          PIXEL *n = (*new) + (rows - row - 1) * cols + col;
          *n = *o;
        }
      }
      
  return 0;
}


/*
 * This method horizontally flips a 24-bit, uncompressed bmp file
 * that has been read in using readFile().
 *
 * THIS IS GIVEN TO YOU SOLELY TO LOOK AT AS AN EXAMPLE
 * TRY TO UNDERSTAND HOW IT WORKS
 *
 * original - an array containing the original PIXELs, 3 bytes per each
 * rows     - the number of rows
 * cols     - the number of columns
 *
 * new      - the new array containing the PIXELs, allocated within
 */
int flip (PIXEL *original, PIXEL **new, int rows, int cols)
{
  int row, col;

  if ((rows <= 0) || (cols <= 0)) return -1;

  *new = (PIXEL*)malloc(rows*cols*sizeof(PIXEL));

  for (row=0; row < rows; row++)
    for (col=0; col < cols; col++) {
      PIXEL* o = original + row*cols + col;
      PIXEL* n = (*new) + row*cols + (cols-1-col);
      *n = *o;
    }

  return 0;
}

int main(int argc, char *argv[])
{
  int r, c,r1,c1,r2,c2;
  PIXEL *b, *nb,*nnb;
  int scale, degree,o;
  char* iFile = NULL;
  char* oFile = NULL;
  char* usage="Usage: ./bmptool [-s scale | -r degree | -f | -v] [-o output_file] [input_file]";

  int sflag =0 ,rflag = 0, fflag = 0, vflag = 0, oflag =0;
  if(argc <1)
  {
    fprintf(stderr,usage,argv[0]);
  }
  while((o = getopt(argc,argv,"s:r:fvo:"))!=-1)
  {
    switch(o)
    {
      case 's':
          sflag = 1;
          scale = atoi(optarg);
          break;
      case 'r':
          rflag = 1;
          degree = atoi(optarg);
          break;
      case 'f':
          fflag = 1;
          break;
      case 'v':
          vflag = 1;
          break;
      case 'o':
          oflag = 1;
          oFile = optarg;
          break;
    }
  } 
  char str[100];  
  if(access(argv[argc-1],F_OK) != -1)
  {
    iFile= argv[argc-1];
  }
 if(iFile == NULL)
 {
      printf("Enter the name of the input_file\n");
    scanf("%s",str);
 }

  

  readFile(iFile, &r, &c, &b);
  r1=r;
  c1=c;
  
  
  if(oflag == 0)
  {
    //puts("missing output_file");
    return 1;
  }
  if(sflag)
  {
    if(scale < 0)
    {
      printf("Error: Scale must be an Integer greater than zero\n");
      return 1;
    }
    enlarge(b,r, c, scale,&nb, &r1, &c1);

  }

  if(rflag)
  {
    if(degree % 90 != 0)
    {
      printf("Error: degree must be a number multiple of 90\n");
      return 1;
    }
    if(sflag)
    {
      rotate(nb, r1, c1, degree,&nnb, &r2, &c2);
      free(nb);
      nb = nnb;     
      r1=r2;
      c1=c2;      
    }
    else
    {
      rotate(b,r,c,degree,&nb,&r1,&r2);
    }
    
  }

  if(fflag)
  {
    if(sflag || rflag)
    {
      //free(nnb);
      flip(nb,&nnb,r1, c1);
      free(nb);
      nb=nnb;
      
    }
    else
    {
      flip(b,&nb,r,c);
    }
    
  }
  if(vflag)
  {
    if(sflag || rflag)
    {
      //free(nnb);
      verticalflip(nb, &nnb, r1, c1);
      free(nb);
      nb=nnb;
      
    }
    else
    {
      verticalflip(b,&nb,r,c);
    }
  }

  writeFile(oFile, r1, c1, nb);

  free(b);
  free(nb);
  return 0;
}
