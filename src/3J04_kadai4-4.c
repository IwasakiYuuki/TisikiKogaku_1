#include <stdio.h>
#include <stdlib.h>

#define DIM    16    /* o·éÁ¥ÊÌ³ */


int main( )
{
    FILE    *fp_ftr, *fp_ave ;
    char    infn[0x80], ave[256]="Imgfiles/ave.dic";
    int     f[DIM][4]={0},sum[DIM][4]={0};
    int     i, j ,z,charcount;

	fp_ave = fopen( ave, "w" );

	if(fp_ave==NULL){
		printf("[ERROR] : can not open file [%s].\n",ave);
		exit(1);
	}

    for( i = 0 ; i < 46 ; i++ ) {

		sprintf( infn, "Imgfiles/%02d.ftr", i+1 ) ;
        fp_ftr = fopen( infn, "r" ) ;

		if(fp_ftr==NULL){
			printf("[ERROR] : can not open files [%02d.ftr].\n",i+1);
			exit(1);
		}

		charcount=0;

		while( 1 ) {
            
			for(j=0;j<DIM;j++){
				for(z=0;z<4;z++){
					if(fscanf(fp_ftr,"%d",&f[j][z])==EOF)goto out;
				}
			}

			for(j = 0 ; j < DIM ; j++ ){
				for(z = 0 ; z < 4 ; z++){
					sum[j][z]+=f[j][z];
				}
			}
			
			charcount++;
        }

		out:

		for(j=0;j<DIM;j++){
			for(z=0;z<4;z++){
				sum[j][z]=sum[j][z]/charcount;
				fprintf(fp_ave,"%d\n",sum[j][z]);
				sum[j][z]=0;
			}
		}

        fclose( fp_ftr );

    }
	fclose( fp_ave );

	return 0;
}



