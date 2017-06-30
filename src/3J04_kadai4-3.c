/*
 *    課題4-3　参考プログラム
 */
#include <stdio.h>
#include <stdlib.h>

#define DIM    16    /* 抽出する特徴量の次元 */
#define P_DATA_SIZE 64
#define DATA_SIZE 512

unsigned char mask=0b00000000;

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
void extract(unsigned char p[][P_DATA_SIZE], int f[DIM][4] );
void thinning(unsigned char p[][P_DATA_SIZE]);
int checkContinuity(unsigned char p[][P_DATA_SIZE],int i,int j);
int getOneNum(unsigned char p[][P_DATA_SIZE],int i,int j);

/*
 *    この位置に，前処理で使う関数定義をコピーしておく．
 *    expand(),noise(),smooth(),normalize(),outline()など．
 */


/*
 *    特徴量抽出のための関数extract()を定義する．
 *    pで与えられた画像から，DIM個の数値を特徴として
 *    抽出し，fに書き込む関数．
 */


int main( )
{
    FILE    *fp1, *fp2 ;
    char    infn[0x80], outfn[0x80] ;
    unsigned char    img[512] ;
    unsigned char    p[64][64] ;
    int     f[DIM][4]={0} ;
    int     num, i, j ,z;

    /*
     *    以下，データファイルごとに特徴抽出を行う
     */
    for( i = 0 ; i < 46 ; i++ ) {
        /*
         *    画像ファイル・特徴ファイルの名前を設定する
         */
        sprintf( infn, "%02d.img", i+1 ) ;
        sprintf( outfn, "%02d.ftr", i+1 ) ;

        /*
         *    文字画像ファイルおよび特徴ファイルを開く
         */
        fp1 = fopen( infn, "rb" ) ;
        fp2 = fopen( outfn, "w" ) ;

        /*
         *    ファイル内に文字画像データがある限り
         *    下記の処理を繰り返す
         */
        while( fread( img, 512, 1,fp1 ) == 1 ) {
            /*
             *    前処理
             */
			expand( img, p ) ;
            noise( p , 4) ;
            smooth( p ) ;
            thinning(p);
			normalize( p ) ;
//            outline( p ) ;
            /*
             *    特徴抽出．ここではDIM個の数値を抽出する．
             */
            extract( p, f ) ;
            /*
             *    特徴出力
             */
            for( j = 0 ; j < DIM ; j++ ) {
            	for(z=0;z<4;z++){
					fprintf( fp2, "%d\n", f[j][z] ) ;
					f[j][z]=0;
            	}
			}
        }

        /*
         *    ファイルのクローズ
         */
        fclose( fp1 ) ;
        fclose( fp2 ) ;
    }
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
	
//	printf("%f %f\n",height/64.0,width/64.0);

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

//ｿｿｿｿｿｿｿｿｿｿｿｿ

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

//ｿｿｿｿｿｿｿｿｿｿｿ

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

void extract(unsigned char p[][P_DATA_SIZE], int f[DIM][4] ){
	
	int i,j,z,z2;

	for(i=0;i<P_DATA_SIZE;i+=16){
		for(j=0;j<P_DATA_SIZE;j+=16){
			for(z=0;z<16;z++){
				for(z2=0;z2<16;z2++){
					if(p[i+z][j+z2]==1){
						if(p[i+z][j+z2-1]==1&&p[i+z][j+z2+1]==1){
							f[i/4+j/16][0]++;
							p[i+z][j+z2-1]=0;
							p[i+z][j+z2+1]=0;
						}
						if(p[i+z-1][j+z2]==1&&p[i+z+1][j+z2]==1){
							f[i/4+j/16][1]++;
							p[i+z-1][j+z2]=0;
							p[i+z+1][j+z2]=0;
						}
						if(p[i+z-1][j+z2+1]==1&&p[i+z+1][j+z2-1]==1){
							f[i/4+j/16][2]++;
							p[i+z-1][j+z2+1]=0;
							p[i+z+1][j+z2-1]=0;
						}
						if(p[i+z-1][j+z2+1]==1&&p[i+z][j+z2-1]==1){
							f[i/4+j/16][2]++;
							p[i+z-1][j+z2+1]=0;
							p[i+z][j+z2-1]=0;
						}
						if(p[i+z][j+z2+1]==1&&p[i+z+1][j+z2-1]==1){
							f[i/4+j/16][2]++;
							p[i+z][j+z2+1]=0;
							p[i+z+1][j+z2-1]=0;
						}
						if(p[i+z-1][j+z2-1]==1&&p[i+z+1][j+z2+1]==1){
							f[i/4+j/16][3]++;
							p[i+z-1][j+z2-1]=0;
							p[i+z+1][j+z2+1]=0;
						}
						if(p[i+z][j+z2-1]==1&&p[i+z+1][j+z2+1]==1){
							f[i/4+j/16][3]++;
							p[i+z][j+z2-1]=0;
							p[i+z+1][j+z2+1]=0;
						}
						if(p[i+z-1][j+z2-1]==1&&p[i+z][j+z2+1]==1){
							f[i/4+j/16][3]++;
							p[i+z-1][j+z2-1]=0;
							p[i+z-1][j+z2+1]=0;
						}
					}
				}
			}
		}
	}
	
	return;
}

void thinning(unsigned char p[][P_DATA_SIZE]){

	int i,j,count=0;
	

	do{
		count=0;
		for(i=1;i<P_DATA_SIZE-1;i++){
			for(j=1;j<P_DATA_SIZE-1;j++){
				if((p[i][j]==1)&&(checkContinuity(p,i,j)==2)){
					if(getOneNum(p,i,j)>=4&&getOneNum(p,i,j)<=6){
						p[i][j]=0;
						count++;
					}
				}
			}
		}
/*		for(i=P_DATA_SIZE-2;i>0;i--){
			for(j=P_DATA_SIZE-2;j>0;j--){
				if((p[i][j]==1)&&(checkContinuity(p,i,j)==2)){
					if(getOneNum(p,i,j)>=3&&getOneNum(p,i,j)<=6){
						p[i][j]=0;
						count++;
					}
				}
			}
		}*/
	}while(count!=0);

	return;
}

int checkContinuity(unsigned char p[][P_DATA_SIZE],int i,int j){
	
	int count=0;

	if(p[i+1][j]!=p[i][j-1])count++;
	if(p[i][j-1]!=p[i-1][j-1])count++;
	if(p[i-1][j-1]!=p[i-1][j])count++;
	if(p[i-1][j]!=p[i-1][j+1])count++;
	if(p[i-1][j+1]!=p[i][j+1])count++;
	if(p[i][j+1]!=p[i+1][j+1])count++;
	if(p[i+1][j+1]!=p[i+1][j])count++;

	return count;
}

int getOneNum(unsigned char p[][P_DATA_SIZE],int i,int j){
	
	int count=0;
	int x,y;

	for(int x=-1;x<2;x++){
		for(int y=-1;y<2;y++){
			if(p[i+x][j+y]==1)count++;
		}
	}

	return count;
}
