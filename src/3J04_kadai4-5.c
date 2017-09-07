#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define AVEFILE	"Imgfiles/ave.dic"
#define DIM   	16    /* o·éÁ¥ÊÌ³ */
#define DNUM	20	//一つのファイルのデータ数
#define CNUM	46	//文字数


int min_num(double *array,int num);

int main(){
	double dic[CNUM]={0},sum=0,f_ftr[DIM][4]={0},f_ave[DIM][4]={0};
	int i,i2,i3,j,z,num=0,count=0;
	FILE *fp_ave,*fp_ftr;
	int filenumber=0;
	char filename[256];
	int sub=0;
	printf("pls input filenumber ->");
	scanf("%d",&filenumber);
	sprintf(filename,"Imgfiles/%02d.ftr",filenumber);
	fp_ftr=fopen(filename,"r");
	fp_ave=fopen(AVEFILE,"r");
	for(i=0;i<20;i++){
		for(j=0;j<DIM;j++){
			for(z=0;z<4;z++){
				fscanf(fp_ftr,"%lf",&f_ftr[j][z]);
			}
		}
		fseek(fp_ave,0,SEEK_SET);
		for(i2=0;i2<CNUM;i2++){
			sum=0;
			for(j=0;j<DIM;j++){
				for(z=0;z<4;z++){
					fscanf(fp_ave,"%lf",&f_ave[j][z]);
					if((sub=(f_ftr[j][z]-f_ave[j][z]))<0.0)sub*=-1.0;
					sum+=sub*sub;//差の２乗の和（三平方をしたい）
				}
			}
			dic[i2]=sqrt(sum);
		}
		num=min_num(dic,CNUM)+1;
		if(num==filenumber)count++;
	}
	printf("正確に認識できたのは%d/%d個です。\n",count,DNUM);
	fclose(fp_ave);
	fclose(fp_ftr);
}

int min_num(double *array,int num){
	int n,ans=0;
	double buf=0;
	int count=0;
	buf=array[0];
	for(n=0;n<num;n++){
		if(buf>array[n]){
			buf=array[n];
			ans=n;
			count++;
		}
	}
	return ans;
}
