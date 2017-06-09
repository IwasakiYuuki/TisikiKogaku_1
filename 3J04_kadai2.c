#include <stdio.h>

#define DATA_SIZE 512
#define P_DATA_SIZE 64

unsigned char data[DATA_SIZE];
unsigned char pattern[P_DATA_SIZE][P_DATA_SIZE];
unsigned char mask;

char loadImg(char *img,char *mapdata);
void printimg(char mapdata[DATA_SIZE]);
void enterPrintImg(char *filename,char *datamap);
void expand(char data[DATA_SIZE],char pattern[][P_DATA_SIZE]);
void printexpand(char pattern[][P_DATA_SIZE]);
void compress(char data[DATA_SIZE],char pattern[][P_DATA_SIZE]);

int main(){
	char filename[128];
	int x,y;
	
	printf("Enter img filename...>>");
	gets(filename);
	printf("load FILE : %s...\n",filename);
	
//	enterPrintImg(filename,data);
	loadImg(filename,data);
/*	expand(data,pattern);
	printexpand(pattern);
	compress(data,pattern);
	printimg(data);*/
	
	printimg( data ) ;
	expand( data, pattern ) ;
		for( y = 0 ; y < 64 ; y++ ) {
			for( x = 0 ; x < 64 ; x++ ) {
				putchar( pattern[y][x] == 1 ? '@' : 'o' ) ;
			}fi
			putchar('\n') ;
		}
	/* data[]‚ðƒNƒŠƒA‚·‚é*/
	compress( data, pattern ) ;
	printimg( data ) ;
	
}

//return 0 : success  return 1:faulse
//char *img : filename of img
char loadImg(char *img,char *mapdata){
	
	FILE *fpr;
	
	fpr=fopen(img,"rb");

	if(fpr==NULL){
		printf("file not found...\n");
		return 1;
	}

	if(fread(mapdata,sizeof(char),DATA_SIZE,fpr)<=0){
		fclose(fpr);
		return 1;
	}
	
	fclose(fpr);
	
	return 0;
}

//char *mapdata : data of img
void printimg(char mapdata[DATA_SIZE]){
	
	int i,j;
	
	for(j=0;j<DATA_SIZE;j++){
		mask=0b10000000;
		
		for(i=0;i<8;i++){
			if((mapdata[j]&mask)==0b00000000){
				putc('.',stdout);
			}else{
				putc('*',stdout);
			}
			mask=mask>>1;
		}
		
		if((j+1)%8==0){
			printf("\n");
		}
	}	
	return;
}

void enterPrintImg(char *filename,char *mapdata){

	FILE *fpr;
	int n=0;
	
	fpr=fopen(filename,"rb");

	if(fpr==NULL){
		printf("file not found...\n");
		return;
	}
	
	while(getchar()){
		if(fread(mapdata,sizeof(char),DATA_SIZE,fpr)<=0){
			fclose(fpr);
			return;
		}
		printimg(mapdata);
		puts(" ");
	}
	
	fclose(fpr);
	
	return;
}

void expand(char mapdata[DATA_SIZE],char pattern[][P_DATA_SIZE]){
	
	int i,j,z;
	
	for(i=0;i<P_DATA_SIZE;ir++){
		for(j=0;j<P_DATA_SIZE/8;j++){
			mask=0b10000000;
			for(z=0;z<8;z++){
				if((mapdata[i*8+j]&mask)==0b00000000){
					pattern[i][j*8+z]=0;
				}else{
					pattern[i][j*8+z]=1;
				}
				mask=mask>>1;
			}
		}
	}
	return;
	
}

void printexpand(char pattern[][P_DATA_SIZE]){
	
	int i,j;
	
	for(i=0;i<P_DATA_SIZE;i++){
		for(j=0;j<P_DATA_SIZE;j++){
			if(pattern[i][j]==0){
				putc('0',stdout);
			}else if(pattern[i][j]){
				putc('1',stdout);
			}
		}
		putc('\n',stdout);
	}
	return;
}

void compress(char data[DATA_SIZE],char pattern[][P_DATA_SIZE]){
	
	char bitdata=0b00000000;
	int i,j,z;
	
	for(i=0;i<P_DATA_SIZE;i++){
		for(j=0;j<P_DATA_SIZE/8;j++){
			for(z=0;z<7;z++){
				bitdata=bitdata<<1;
				if(pattern[i][j*8+z]==0){
					bitdata|=0b00000000;
				}else if(pattern[i][j*8+z]==1){
					bitdata|=0b00000001;
				}
			}
			data[i*8+j]=bitdata;
		}
	}
	
	return;
	
}
