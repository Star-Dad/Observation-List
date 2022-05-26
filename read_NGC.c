#include <stdio.h>
#include <strings.h>
#include <stdlib.h>

struct NGC_info {
	char catalog;
	short id;
	char type[4];
	short RA_hours;
	float RA_minutes;
	short DEC_degrees;
	short DEC_minutes;
	char source;
	char constellation[4];
	char size_limit;
	float size;
	float magnitude;
	char description[60];
}NGC[13226];


void read_NGC(){
	FILE *fp;
	short x;
	char temp[70],c;
	
	fp = fopen("NGC/ngc2000.dat","r");
	if (fp) puts("Opened file\n");
	for (x=0;x<13226;x++){
                c=fgetc(fp);
                if (c== EOF)break;
                if (c== 'I')NGC[x].catalog= c;
                else NGC[x].catalog='N';
		fgets(temp,5,fp);
		temp[5] = 0;
		NGC[x].id = atoi(temp);
		fgets(NGC[x].type,5,fp);
		NGC[x].type[5]=0;
		fgets(temp,4,fp);
		temp[3] = 0;
		NGC[x].RA_hours = atoi(temp);
		fgets(temp,6,fp);
		temp[6] = 0;
		NGC[x].RA_minutes = atof(temp);
		fgets(temp,6,fp);
		temp[6] = 0;
		NGC[x].DEC_degrees = atoi(temp);
		fgets(temp,5,fp);
		temp[5] = 0;
		NGC[x].DEC_minutes = atoi(temp);
		fgets(temp,4,fp);
		NGC[x].source = temp[3];
		fgets(NGC[x].constellation,4,fp);
		NGC[x].constellation[4] = 0;
		fgets(temp,1,fp);
		NGC[x].size_limit = atoi(temp);
		fgets(temp,7,fp);
		temp[7] = 0;
		NGC[x].size = atof(temp);
		fgets(temp,5,fp);
		temp[5] = 0;
		NGC[x].magnitude = atof(temp);
		fgets(NGC[x].description,60,fp);
/*		NGC[x].description[60]=0;*/
				
/*		if (NGC[x].catalog == 'I') printf("catalog: IC\n");
		else printf("catalog: NGC\n");
		printf("ID:     %d\n",NGC[x].id);
		printf("Type:   %s\n",NGC[x].type);
		printf("RA:     %d %f\n",NGC[x].RA_hours,NGC[x].RA_minutes);
		printf("DEC:    %d %d\n",NGC[x].DEC_degrees,NGC[x].DEC_minutes);
		printf("Source: %c\n",NGC[x].source);
		printf("Const:  %s\n",NGC[x].constellation);
		printf("Limit:  %d\n",NGC[x].size_limit);
		printf("Size:   %f\n",NGC[x].size);
		printf("Mag:    %f\n",NGC[x].magnitude);
		printf("descrip:%s\n",NGC[x].description);
*/
            
        }
	fclose(fp);
        printf("Done read NGC\n");
}
