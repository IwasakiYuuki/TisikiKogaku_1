#include <stdio.h>

#define DATA_SIZE 512

unsigned char data[DATA_SIZE];
unsigned char mask;

char loadImg(char *img,char *mapdata);
char printimg(char *mapdata);
void enterPrintImg(char *filename,char *datamap);

int main(){
	char filename[100];
	
	printf("Enter img filename...>>");
	gets(filename);
	printf("load FILE : %s...",filename);
	
	enterPrintImg(filename,data);
	
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

//return 0 : success
//char *mapdata : data of img
char printimg(char mapdata[512]){
	
	int i,j;
	
	for(j=0;j<DATA_SIZE;j++){
		mask=0b10000000;
		
		for(i=0;i<8;i++){
			if((mapdata[j]&mask)==0b00000000){
				putc('.'
				,stdout);
			}else{
				putc('*',stdout);
			}
			mask=mask>>1;
		}
		
		if((j+1)%8==0){
			printf("\n");
		}
	}	
	return 0;
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

