#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define RADS 6.283185
#define PI 3.1415926
#define DEG_TO_RADS  .017453
typedef  char byte;

extern struct NGC_info {
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

double julian_day(short,byte,byte,byte,byte,byte);
void read_NGC();
int find_ngc(int);
double az,alt;

void alt_az( double,double, double);

int main(){
    int x1,y1,ut_hour;
    short x;
    int year,id;
    int month,date,hour,minute,zone;
    double  latitude,longitude,lst,gmt,final_jd,magnitude,dec;
    FILE *fp,*fin,*fout;
    double jd,ra,ha,min_alt;
   char input_file[256],output_filename[256],object_name[32],*type,*constellation,temp[16],display_name[32];;
   byte special = 0;
   
    printf("\n\n");
    printf("**********************************************\n"); 
    printf("*                 Observation              *\n");
    printf("*                 by G. McLean               *\n");
    printf("* (c) 2022 G McLean Creative Commons 4.0     *\n");
    printf("* No commercial use | attribution required   *\n");
    printf ("*********************************************\n\n\n\n ");
   
   printf("Makes a list of available M/NGC/IC objects given time, location, and desired minimum altitude.  You will need a file that the program reads with data in it as follow ie:\n\nyear 2022\nmonth 6\ndate 1\nzone -5\nhour 21\nminute 30\nlatitude 41.55\nlongitude -72.1\nmin_alt 35\n\nOutputs an html file for printing or display\n\n");
   
    printf("Input file name:"); scanf("%s",input_file);
    fp = fopen(input_file,"r");
    for (x=0;x<9;x++){
        fscanf(fp,"%s",temp);

        if (!(strcmp(temp,"year"))) fscanf(fp,"%d",&year);
        else if (!(strcmp(temp,"month"))) fscanf(fp,"%d",&month);
        else if (!(strcmp(temp,"date"))) fscanf(fp,"%d",&date);
       else if (!(strcmp(temp,"zone"))) fscanf(fp,"%d",&zone);
       else if (!(strcmp(temp,"hour"))) fscanf(fp,"%d",&hour);
       else if (!(strcmp(temp,"minute"))) fscanf(fp,"%d",&minute);
       else if (!(strcmp(temp,"latitude"))) fscanf(fp,"%lf",&latitude);
       else if (!(strcmp(temp,"longitude"))) fscanf(fp,"%lf",&longitude);
       else if (!(strcmp(temp,"min_alt"))) fscanf(fp,"%lf",&min_alt);
    }

    
    jd = julian_day(year,month,date, hour, minute,zone);
      ut_hour = hour - zone;
    lst= (100.46 + 0.985647 * jd + longitude + 15 * (ut_hour + minute/60.) +360)-(((int)((100.46 + 0.985647 * jd + longitude + 15 * (hour + minute/60.) +360)/360))*360);
        lst /=15;
        printf("1. LST:%lf\n",lst);
        latitude *= DEG_TO_RADS;

    read_NGC(); 
    sprintf(output_filename,"%s_obs.html",input_file);
     fout = fopen(output_filename,"w");
    fprintf (fout,"<html><body><br />%d-%02d-%02d %02d:%02d %fN %fW<br />\n",year,month,date,hour,minute,latitude,longitude);
    fprintf(fout,"<table border='1'><tr><th>Name</th><th>AZ</th><th>ALT</th><th>Mag</th><th>Type</th><th>Const</th></tr>");
    x1=1;
    while (x1 != EOF){
        x1 = fscanf(fp,"%s",object_name);
        strcpy(display_name,object_name);
        if (object_name[0] == 'I' )   id = atoi(&object_name[1]);
        else if (object_name[0] == 'M')  {
            id = atoi(&object_name[1]);
            printf ("looking for M%d\n",id);
            id = find_ngc(id);
            printf("found:%d\n",id);
            if (id > 10000){
                id -= 10000;
                object_name[0] = 'I';
            } else object_name[0]='N';
            special= 0;
            switch(id){
                case 40:ra=12.366667;dec=58.083333333;magnitude=9.6;type="STR";special=1;constellation="UMA";break;
                case 45:ra=3.7833333;dec=24.166666667;magnitude=1.2;type="OC";special=1;constellation = "TAU"; break;
            }
            printf("got %d special:%d\n",id,special);
            if (special == 1){
                alt_az (dec,latitude,ha);
                if ( alt > min_alt) fprintf(fout,"<tr><td>%s</td><td>%f</td><td>%f</td><td>%4.1f</td><td>%s</td><td>%s</td></tr>\n",display_name,az,alt,magnitude,type,constellation);
                continue;
            }
        } else if (object_name[0] == 'N')   id = atoi(&object_name[3]);
        else {
            printf("bad entry in %s:  %s\n",input_file,object_name);
            fclose (fp);
            fclose(fout);
            exit(999);
        }
        printf("got %s\n",object_name);
        for (y1=0;y1<13226;y1++){
                if (NGC[y1].catalog == object_name[0]){
                    if (id == NGC[y1].id){
                        printf("working: %s \n",object_name);
                        ra = (NGC[y1].RA_hours + NGC[y1].RA_minutes/60.);
                        ha = (( lst - ra) *15) *DEG_TO_RADS ;
                        dec = (NGC[y1].DEC_degrees + NGC[y1].DEC_minutes/60.0)*DEG_TO_RADS ;    
                        alt_az (dec,latitude,ha);
                        if ( alt > min_alt)  fprintf(fout,"<tr><td>%s</td><td>%f</td><td>%f</td><td>%4.1f</td><td>%s</td><td>%s</td></tr>\n",display_name,az,alt,NGC[y1].magnitude,NGC[y1].type,NGC[y1].constellation);
                        break;
                    }
                }
        }
    }
     fclose(fp);
     fprintf(fout,"</table></body></html>\n");
     fclose(fout);
     return(0);
}

void alt_az( double dec,double latitude, double ha){
    double sin_alt,sinaz,cosaz;
    
                        sin_alt =  sin(dec)*sin(latitude)+cos(dec)*cos(ha)*cos(latitude);
                        alt = asin(sin_alt);
                        sinaz=-cos(dec)*sin(ha)/cos(alt);
                        cosaz=sin(dec)*cos(latitude)-cos(dec)*cos(ha)*sin(latitude);
                        if(cosaz <= 0.0) az= PI - asin(sinaz);
                        else  if(sinaz <= 0.0)   az=2*PI+asin(sinaz);
                        else  az=asin(sinaz);
                        az /= DEG_TO_RADS;
                        alt /= DEG_TO_RADS;
}

int find_ngc(int id){
    switch (id){
        case 1:  return 1952;
        case 2:  return  7089;
        case  3:   return                              5272;
        case  4    : return                            6121;
        case  5    : return                            5904;
        case  6   : return                             6405;
        case  7    : return                            6475;
        case  8     : return                           6523;
        case  9      : return                          6333;
        case 10     : return                           6254;
        case    11    : return                            6705;
        case    12   : return                             6218;
        case    13    : return                            6205;
        case    14    : return                            6402;
        case    15    : return                            7078;
        case    16     : return                           6611;
        case    17    : return                            6618;
        case    18    : return                            6613;
        case    19      : return                          6273;
        case    20      : return                          6514;
        case    21      : return                          6531;
        case    22     : return                           6656;
        case    23     : return                           6494;
        case    24: return 14715;
        case    25   : return                            14725;
        case    26    : return                            6694;
        case    27    : return                            6853;
        case    28    : return                            6626;
        case    29    : return                            6913;
        case    30    : return                            7099;
        case    31   : return                              224;
        case    32    : return                             221;
        case    33    : return                             598;
        case    34    : return                            1039;
        case    35    : return                            2168;
        case    36   : return                             1960;
        case    37   : return                             2099;
        case    38   : return                             1912;
        case    39   : return                             7092;
        case    40: return 0;
        case    41      : return                          2287;
        case    42        : return                        1976;
        case    43      : return                          1982;
        case    44     : return                           2632;
        case    45: return 0;
        case    46        : return                        2437;
        case    47     : return                           2422;
        case    48       : return                         2548;
        case    49      : return                          4472;
        case    50       : return                         2323;
        case    51      : return                          5194;
        case    52        : return                        7654;
        case    53        : return                        5024;
        case    54       : return                         6715;
        case    55      : return                          6809;
        case    56     : return                           6779;
        case    57      : return                          6720;
        case    58      : return                          4579;
        case    59      : return                          4621;
        case    60       : return                         4649;
        case    61       : return                         4303;
        case    62        : return                        6266;
        case    63       : return                         5055;
        case    64      : return                          4826;
        case    65        : return                        3623;
        case    66       : return                         3627;
        case    67       : return                        2682;
        case    68       : return                        4590;
        case    69     : return                           6637;
        case    70    : return                            6681;
        case    71    : return                            6838;
        case    72   : return                             6981;
        case    73    : return                            6994;
        case    74     : return                            628;
        case    75     : return                           6864;
        case   76     : return                            650;
        case    77     : return                           1068;
        case    78    : return                            2068;
        case   79     : return                           1904;
        case    80     : return                           6093;
        case    81     : return                           3031;
        case   82      : return                          3034;
        case    83      : return                          5236;
        case    84      : return                          4374;
        case    85    : return                            4382;
        case    86     : return                           4406;
        case    87      : return                          4486;
        case    88     : return                           4501;
        case    89     : return                           4552;
        case    90   : return                             4569;
        case    91 : return                               4548;
        case    92      : return                          6341;
        case    93      : return                          2447;
        case    94     : return                           4736;
        case    95     : return                           3351;
        case    96      : return                          3368;
        case    97       : return                         3587;
        case    98       : return                         4192;
        case    99     : return                           4254;
        case   100   : return                             4321;
        case   101   : return                             5457;
        case   102:  return     5866;
        case   103     : return                            581;
        case   104    : return                            4594;
        case   105     : return                           3379;
        case   106   : return                             4258;
        case   107    : return                            6171;
        case   108    : return                            3556;
        case   109    : return                            3992;
        case  110 : return                                205;
    }
}
