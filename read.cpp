/*Program to emulate image processing part of
 * E101 library on PC
 * Either Linux or mingw
 * Use at your own risk.
 * Feel free to modify */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <iostream>


#define CAMERA_WIDTH 320 //Control Resolution from Camera
#define CAMERA_HEIGHT 240 //Control Resolution from Camera
unsigned char pixels_buf[CAMERA_WIDTH*CAMERA_HEIGHT*4];

// the file that we are reading from:
const char* read_filename = "file5.ppm";

// the file that we are writing to
char save_filename[] = "test.ppm";

// declaring them at the start, so that our code can be at the top
unsigned char get_pixel( int,int, int);
int set_pixel(int, int, char, char,char);
int ReadPPM(const char *);
int SavePPM(char [5]);

bool isRed(int r, int g, int b) {
	float threshold = 2.1;
	
	float redness = (r / g) + (r / b);
	return (redness >= threshold);
	//return (r / b > threshold && r / g > threshold);
}

void processImageCore() {
	int left = 319;
	int top = 239;
	int right = 0;
	int bottom = 0;	
	
	for (int i=0; i<240; i++) {
		for (int j=0; j<320; j++) {
			int red = get_pixel(i,j,0);
			int green = get_pixel(i,j,1);
			int blue = get_pixel(i,j,2);
			
			if (isRed(red,green,blue)) {
				set_pixel(i,j,255,0,0); // draw red where the program sees red
					
				if (j < left) {
					left = j;
				} else if (j > right) {
					right = j;
				} else if (i < top) {
					top = i;
				} else if (i > bottom) {
					bottom = i;
				}
			} else {
				set_pixel(i,j,255,255,255);  // draw white where the program doesn't see red
			}
		}
		
	}
	/*
	// draw green bounding box	
	for (int i=0; i<320; i++) {
		set_pixel(top,i,0,255,0);
		set_pixel(bottom,i,0,255,0);
	}
	
	for (int i=0; i<240; i++) {
		set_pixel(i,left,0,255,0);
		set_pixel(i,right,0,255,0);
	}
	
	int centre_x = left + (right-left) / 2;
	int centre_y = top + (bottom-top) / 2;
	
	// draw cyan lines through centre
	for (int i=0; i<320; i++) {
		set_pixel(centre_y,i,0,255,255);
	}
	
	for (int i=0; i<240; i++) {
		set_pixel(i,centre_x,0,255,255);
	}

	// only if the yaw was in the middle though...
	float yaw_offset = 48 + (160-centre_x)/10;
	//pitch_offset
	*/
}

/**
 * Detects sun, but not bird
*/
void processImageCompletion() {
	
	int redInRows[240] = {0};
	int redInCols[320] = {0};
	bool foundSun = false;
	
	for (int i=0; i<240; i++) {
		for (int j=0; j<320; j++) {
			int red = get_pixel(i,j,0);
			int green = get_pixel(i,j,1);
			int blue = get_pixel(i,j,2);
			
			if (isRed(red,green,blue)) {
				foundSun = true;
				redInRows[i] ++;
				redInCols[j] ++;	
			}
		}
		
	}
	
	// dont try and do anything else if you didn't find any red pixels
	if (!foundSun) {
		std::cout<<"nothing detected"<<std::endl;
		return;	
	}
	
	int mostRedRowIndex = -1;
	int mostRedColIndex = -1;
	int mostRedRow = 0;
	int mostRedCol = 0;
	
	// go through and find the row and column with most red
	// the ones with the most should be the centre of the circle
	for (int i=0; i<320; i++) {
		if (redInCols[i] > mostRedCol) {
			mostRedCol = redInCols[i];
			mostRedColIndex = i;
		}	
	}
	
	for (int i=0; i<240; i++) {
		if (redInRows[i] > mostRedRow) {
			mostRedRow = redInRows[i];
			mostRedRowIndex = i;
		}
	}
	
	// draw cyan lines through centre
	for (int i=0; i<320; i++) {
		set_pixel(mostRedRowIndex,i,0,255,255);
	}
	
	for (int i=0; i<240; i++) {
		set_pixel(i,mostRedColIndex,0,255,255);
	}
}

void processImageChallenge() {
	
	int thresholdImage[320][240] = {0};
	bool foundSun = false;
	
	for (int i=0; i<240; i++) {
		for (int j=0; j<320; j++) {
			int red = get_pixel(i,j,0);
			int green = get_pixel(i,j,1);
			int blue = get_pixel(i,j,2);
			
			if (isRed(red,green,blue)) {
				thresholdImage[j][i] = 1;
			}
		}	
	}
	
	// for all pixels in the image (except boundaries):
	for (int x=1; x<319; x++) {
		for (int y=1; y<239; y++) {
			
			int toplft = thresholdImage[x-1][y-1];
			int topmid = thresholdImage[x][y-1];
			int toprgt = thresholdImage[x+1][y-1];
			
			int midlft = thresholdImage[x-1][y];
			int midrgt = thresholdImage[x+1][y];
			
			int btmlft = thresholdImage[x-1][y+1];
			int btmmid = thresholdImage[x][y+1];
			int btmrgt = thresholdImage[x+1][y+1];
			
			int verticalTotal = -toplft + toprgt - 2*midlft + 2*midrgt - btmlft + btmrgt;
			//int horizontalTotal = toplft + 2*topmid + toprgt - btmlft + 2*btmmid + btmrgt;
			
			if (verticalTotal > 2) {
				set_pixel(y, x, 255, 255, 255);
			} else {
				set_pixel(y, x, 33, 33, 33);
			}
		}	
	}
	/*
	
	// dont try and do anything else if you didn't find any red pixels
	if (!foundSun) {
		std::cout<<"nothing detected"<<std::endl;
		return;	
	}
	
	// check each pixel
	
	int mostRedRowIndex = -1;
	int mostRedColIndex = -1;
	int mostRedRow = 0;
	int mostRedCol = 0;
	
	// go through and find the row and column with most red
	// the ones with the most should be the centre of the circle
	for (int i=0; i<320; i++) {
		if (redInCols[i] > mostRedCol) {
			mostRedCol = redInCols[i];
			mostRedColIndex = i;
		}	
	}
	
	for (int i=0; i<240; i++) {
		if (redInRows[i] > mostRedRow) {
			mostRedRow = redInRows[i];
			mostRedRowIndex = i;
		}
	}
	
	// draw cyan lines through centre
	for (int i=0; i<320; i++) {
		set_pixel(mostRedRowIndex,i,0,255,255);
	}
	
	for (int i=0; i<240; i++) {
		set_pixel(i,mostRedColIndex,0,255,255);
	}*/
}

// returns color component (color==0 -red,color==1-green,color==2-blue
// color == 3 - luminocity
// for pixel located at (row,column)
unsigned char get_pixel( int row,int col, int color)
{
    // calculate address in 1D array of pixels
    int address = CAMERA_WIDTH*row*3 + col*3;
    if ((row < 0 ) || (row > CAMERA_HEIGHT) )
    {
        printf("row is out of range\n");
        return -1;
    }
    if ( (col< 0) || (col > CAMERA_WIDTH))
    {
        printf("column is out of range\n");
        return -1;
    }

    if (color==0)
    {
        return (pixels_buf[address]);
    }
    if (color==1)
    {
        return (pixels_buf[address + 1]);
    }
    if (color==2)
    {
        return (pixels_buf[address + 2]);
    }
    if (color==3)
    {
        unsigned char y = (pixels_buf[address] + pixels_buf[address+1] +pixels_buf[address+2])/3;
        return y;
    }
    printf("Color encoding wrong: 0-red, 1-green,2-blue,3 - luminosity\n");
    return -2; //error
}

int set_pixel(int row, int col, char red, char green,char blue)
{
    int address = CAMERA_WIDTH*row*3 + col*3;
    if ((address < 0) || (address>CAMERA_WIDTH*CAMERA_HEIGHT*4))
    {
        printf("SetPixel(): wrong x,y coordinates\n");
        return -1;
    }
    pixels_buf[address] = red;
    pixels_buf[address+1] = green;
    pixels_buf[address+2] = blue;
    return 0;
}


int ReadPPM(const char *filename)
{
  //char buff[16];
  FILE *fp=fopen(filename, "rb");
   if (!fp) {
     printf("Unable to open file '%s'\n", filename);
     return -1;
  }
  // read the header
  char ch;
  if ( fscanf(fp,"P%c\n",&ch) != 1 || ch != '6')
  {
	  printf("file is wrong format\n");
	  return -2;
  }
  // skip comments
  ch = getc(fp);
  while(ch == '#')
  {
	  do {
		  ch = getc(fp);
	  } while (ch != '\n');
	  ch = getc(fp);
  }
 
   if (!isdigit(ch))  printf("Wrong header\n");
   ungetc(ch,fp);  
  //read width,height and max color value
  int width, height, maxval;
  int res = fscanf(fp,"%d%d%d\n",&width,&height,&maxval);
  printf("Open file: width=%d height=%d\n",width,height);
  

  int size = width*height*3;
  
  int num =fread((void*) pixels_buf, 1,size,fp);
  if (num!=size) {
	printf("can not allocate image data memory: file=%s num=%d size=%d\n",
	filename,num,size);
	return -3;
   }
  fclose(fp);
  return 0;

} 


int SavePPM(char fn[5]){
  //save image into ppm file
  FILE *fp;
  char fname[9];
  sprintf(fname,"%s",fn);
  fp = fopen(fname,"wb");
  if ( !fp){
           printf("Unable to open the file\n");
           return -1;
   }
   // write file header
   fprintf(fp,"P6\n %d %d %d\n",CAMERA_WIDTH , CAMERA_HEIGHT,255);
   int ind = 0;
   int row = 0;
   int col = 0;
   char red;
   char green;
   char blue;
   for ( row = 0 ; row < CAMERA_HEIGHT; row++){
     for ( col = 0 ; col < CAMERA_WIDTH; col++){
        red =  pixels_buf[ind];
		green =  pixels_buf[ind+1];
		blue =  pixels_buf[ind+2];
		fprintf(fp,"%c%c%c",red,green,blue);
		ind = ind + 3;
     }
   }
   fflush(fp);
   fclose(fp);
   return 0;
}

int main()
{
	// enter image file name
	/*char file_name[7];
	printf(" Enter input image file name(with extension:\n");
	scanf("%s",file_name);
	printf(" You enter:%s\n",file_name);
	// read image file
	if (ReadPPM(file_name) != 0){
		printf(" Can not open file\n");
		return -1;
	};*/
	
	if (ReadPPM(read_filename) != 0){
		printf(" Can not open file\n");
		return -1;
	}
	
	// do your processing here
	processImageChallenge();
	
	/*printf(" Enter output image file name(with extension:\n");
	scanf("%s",file_name);
	printf(" You enter:%s\n",file_name);
	if (SavePPM(file_name) != 0){
		printf(" Can not save file\n");
		return -1;
	};*/
	
	if (SavePPM(save_filename) != 0){
		printf(" Can not save file\n");
		return -1;
	};
	
    
	return 0;
}
 
