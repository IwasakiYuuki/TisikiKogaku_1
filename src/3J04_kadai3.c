#include <stdio.h>
#include <stdlib.h>

#define P_DATA_SIZE 64
#define DATA_SIZE 512
#define NOISE_SIZE 4

unsigned char data[DATA_SIZE];
unsigned char pattern[P_DATA_SIZE][P_DATA_SIZE];
unsigned char mask;

void outline(unsigned char p[][P_DATA_SIZE]);
int check_smooth(unsigned char p[][P_DATA_SIZE],int row,int col);
void smooth(unsigned char p[][P_DATA_SIZE]);
char loadImg(char *img,unsigned char mapdata[DATA_SIZE]);
void printexpand(unsigned char pattern[][P_DATA_SIZE]);
void expand(unsigned char data[DATA_SIZE],unsigned char pattern[][P_DATA_SIZE]);
void normalize(unsigned char p[][P_DATA_SIZE]);
int label(unsigned char p[][P_DATA_SIZE]);
void noise(unsigned char p[][P_DATA_SIZE] , int size );
int recLabel(unsigned char p[][P_DATA_SIZE]);
void fill(unsigned char p[][P_DATA_SIZE],int i,int j,int label);


int main(){
	
	char filename[128];
	
	
	printf("Enter img filename...>>");
	gets((char *)filename);
	printf("load FILE : %s...\n",filename);
	loadImg((char *)filename,data);
	expand(data,pattern);
//	printexpand(pattern);
//	smooth(pattern);
//	normalize(pattern);
//	outline(pattern);
//	label(pattern);
	noise(pattern,NOISE_SIZE);
//	recLabel(pattern);
	printexpand(pattern);

	return 0;
}

char loadImg(char *img,unsigned char mapdata[DATA_SIZE]){
	
	FILE *fpr;
	
	fpr=fopen(img,"rb");

	if(fpr==NULL){
		printf("file not found...\n");
		exit(1);
	}

	if(fread(mapdata,sizeof(char),DATA_SIZE,fpr)<=0){
		fclose(fpr);
		return 1;
	}
	
	fclose(fpr);
	
	return 0;
}

void outline(unsigned char p[][P_DATA_SIZE]){
	
	int i,j;
	
	for(i=1;i<P_DATA_SIZE-1;i++){
		for(j=1;j<P_DATA_SIZE-1;j++){
			if(p[i][j]>=1){
				if(p[i][j+1]>=1&&p[i+1][j]>=1&&p[i][j-1]>=1&&p[i-1][j]>=1){
					p[i][j]=2;
				}
			}
		}
	}

	for(i=0;i<P_DATA_SIZE;i++){
		for(j=0;j<P_DATA_SIZE;j++){
			if(p[i][j]==2){
				p[i][j]=0;
			}
		}
	}
	
	return;
}

int check_smooth(unsigned char p[][P_DATA_SIZE],int row,int col){
	int i,j;
	char buf=0b00000000;
	char mask1_1=0b11111000,mask1_2=0b01101011,mask1_3=0b00011111,mask1_4=0b11010110;
	char mask2_1=0b11100000,mask2_2=0b00101001,mask2_3=0b00000111,mask2_4=0b10010100;

	for(i=-1;i<2;i++){
		for(j=-1;j<2;j++){
			if(i==0&&j==0)continue;
			buf=buf<<1;
			if(p[row+i][col+j]==0)buf|=0b00000000;
			if(p[row+i][col+j]==1)buf|=0b00000001;
		}
	}
	
	if(p[row][col]==0){
		if((buf&mask1_1)==mask1_1||(buf&mask1_2)==mask1_2||(buf&mask1_3)==mask1_3||(buf&mask1_4)==mask1_4)return 1;
	}else if(p[row][col]==1){
		if(mask2_1==buf||mask2_2==buf||mask2_3==buf||mask2_4==buf)return 1;
	}
	
	return 0;
	
}

void smooth(unsigned char p[][P_DATA_SIZE]){
	int i,j;
	
	for(i=1;i<P_DATA_SIZE-1;i++){
		for(j=1;j<P_DATA_SIZE-1;j++){
			if(p[i][j]==1){
				if(check_smooth(p,i,j)==1)p[i][j]=0;
			}else if(p[i][j]==0){
				if(check_smooth(p,i,j)==1)p[i][j]=1;
			}
		}
	}
	return;
	
}

void printexpand(unsigned char pattern[][P_DATA_SIZE]){
	
	int i,j;
	
	for(i=0;i<P_DATA_SIZE;i++){
		for(j=0;j<P_DATA_SIZE;j++){
//			if(pattern[i][j]==0){
//				putc('0',stdout);
//			}else if(pattern[i][j]){
//				putc('1',stdout);
//			}
			putc((pattern[i][j]+48),stdout);
		}
		putc('\n',stdout);
	}
	return;
}

void expand(unsigned char mapdata[DATA_SIZE],unsigned char pattern[][P_DATA_SIZE]){
	
	int i,j,z;
	
	for(i=0;i<P_DATA_SIZE;i++){
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

void normalize(unsigned char p[][P_DATA_SIZE]){
	int i,j,startheight=-1,endheight=0,startwidth=P_DATA_SIZE,endwidth=0;
	int height,width;
	unsigned char cache[P_DATA_SIZE][P_DATA_SIZE];

	for(i=0;i<P_DATA_SIZE;i++){
		for(j=0;j<P_DATA_SIZE;j++){
			if(p[i][j]==1){
				if(startheight==-1)startheight=i;
				if(startheight>i)startheight=i;
				if(endheight<i)endheight=i;
				if(startwidth>j)startwidth=j;
				if(endwidth<j)endwidth=j;
			}
		}
	}
	height=endheight-startheight+1;
	width=endwidth-startwidth+1;
	
	printf("%f %f\n",height/64.0,width/64.0);

	for(i=0;i<P_DATA_SIZE;i++){
		for(j=0;j<P_DATA_SIZE;j++){
			cache[i][j]=p[(int)(startheight+((double)i*((double)height/(double)P_DATA_SIZE))+0.5)][(int)(startwidth+((double)j*((double)width/(double)P_DATA_SIZE))+0.5)];
		}
	}
	for(i=0;i<P_DATA_SIZE;i++){
		for(j=0;j<P_DATA_SIZE;j++){
			p[i][j]=cache[i][j];
		}
	}
	return;
}

int label(unsigned char p[][P_DATA_SIZE]){

//再帰を使わないラベル処理

	int i,j,z,z2,buf;
	char count=2;
	int dic[512]={0};

	for(i=0;i<P_DATA_SIZE;i++){
		for(j=0;j<P_DATA_SIZE;j++){
			if(p[i][j]==1){
				for(z=-1;z<2;z++){
					for(z2=-1;z2<2;z2++){
						if(i+z<0||i+z>P_DATA_SIZE)continue;
						if(j+z2<0||j+z2>P_DATA_SIZE)continue;
						if(p[i+z][j+z2]>1){
							p[i][j]=p[i+z][j+z2];
						}
					}
				}
				if(p[i][j]==1){
					p[i][j]=count;
					count++;
				}
			}
		}
	}

	do{
		count=0;
		for(i=0;i<P_DATA_SIZE;i++){
			for(j=0;j<P_DATA_SIZE;j++){
				if(p[i][j]>1){
					for(z=-1;z<2;z++){
						for(z2=-1;z2<2;z2++){
							if(i+z<0||i+z>P_DATA_SIZE)continue;
							if(j+z2<0||j+z2>P_DATA_SIZE)continue;
							if(p[i+z][j+z2]>p[i][j]){
								dic[p[i+z][j+z2]]=p[i][j];
								count++;
							}
						}
					}
				}
			}
		}
		for(i=0;i<P_DATA_SIZE;i++){
			for(j=0;j<P_DATA_SIZE;j++){
				if(p[i][j]>1){
					if(dic[p[i][j]]!=0){
						p[i][j]=dic[p[i][j]];
					}
				}
			}
		}
	}while(count!=0);

	

	return 0;
}

void noise(unsigned  char p[][P_DATA_SIZE] , int size ){
	
	int i,j;
	int dic[512]={0};

//	label(p);
	recLabel(p);

	for(i=0;i<P_DATA_SIZE;i++){
		for(j=0;j<P_DATA_SIZE;j++){
			dic[p[i][j]]++;
		}
	}

	for(i=0;i<P_DATA_SIZE;i++){
		for(j=0;j<P_DATA_SIZE;j++){
			if(dic[p[i][j]]<size){
				p[i][j]=0;
			}
		}
	}
	

	for(i=0;i<P_DATA_SIZE;i++){
		for(j=0;j<P_DATA_SIZE;j++){
			if(p[i][j]>0){
				p[i][j]=1;
			}
		}
	}

	return;
}

int recLabel(unsigned char p[][P_DATA_SIZE]){

//再帰を使ったラベル処理

	int i,j,count=2;

	for(i=0;i<P_DATA_SIZE;i++){
		for(j=0;j<P_DATA_SIZE;j++){
			if(p[i][j]==1){
				fill(p,i,j,count);
				count++;
			}
		}
	}
	
	if(count>255)return 1;

	return 0;
}

void fill(unsigned char p[][P_DATA_SIZE],int i,int j,int label){
	
	p[i][j]=label;
	if(p[i-1][j]==1)fill(p,i-1,j,label);
	if(p[i+1][j]==1)fill(p,i+1,j,label);
	if(p[i][j-1]==1)fill(p,i,j-1,label);
	if(p[i][j+1]==1)fill(p,i,j+1,label);
	return;
}
