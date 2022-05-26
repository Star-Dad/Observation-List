#include <math.h>
#include <stdio.h>
#include <strings.h>

typedef char byte;

double julian_day (short year,byte month,byte date,byte hour, byte minute,byte zone){
    short days;
    int current_day,julian_day;
        double julian,delta;
    
 /*
  *   year -= 100;
    switch (month){
        case 1:
            break;
        case 2:
            days=31;
            break;
        case 3:
            days=59;
            break;
        case 4:
            days=90;
            break;
        case 5:
            days=120;
            break;
        case 6:
            days=151;
            break;
        case 7:
            days=181;
            break;
        case 8:
            days=212;
            break;
        case 9:
            days=243;
            break;
        case 10:
            days=273;
        case 11:
            days=304;
            break;
        case 12:
            days=334;
            break;
    };
    current_day = date+days;
    julian_day = 365*year+(int)(year/4) - 2;
    julian_day +=( 15020 + current_day);

    if( ((int)(year/4) == year/4.0) && month < 3) julian_day--;
    julian_day += 2400000;
    julian_day -= (int)(year-1900/100);
    julian_day +=3;
    julian_day -= 2451545; /*J2000 */	


 	julian = 367*year - (int) (7*(year  +(int)( (month+9)/12))/4) +(int)(275*month/9) + date -730531.5 ;
/*        printf ("367:%d 7:%d year:%d month:%d\n",367*year,(int) (7*(year + (int)((month+9)/12))/4),(int)(3 * (int)((year+(month-9)/7/100) + 1)/4), (int)(275*month/9));
	printf ("jd:%d year%d month:%d date:%d\n",julian,year,month,date);
	julian -= 2451545;
*/
/*	julian= (1461. * (year + 4800. + (month - 14.)/12.))/4. +(367. * (month - 2. - 12. * ((month - 14.)/12.)))/12. - (3. * ((year + 4900. + (month - 14.)/12.)/100.))/4. + date - 32075.;
  */
    julian +=2451545;
printf("first julian:%f\n",julian);
/*	julian -= 2451547.78472; /* J2000 */


	/*
	 printf("%d %d %d\n",(1461 * (year + 4800 + (month - 14)/12))/4,(367 * (month - 2 - 12 * ((month - 14)/12)))/12,(3 * ((year + 4900 + (month - 14)/12)/100))/4 + date - 32073);
*/
        
    hour = hour + zone;
    if (hour > 24) {
            julian++;
            hour -=24;
    }
  delta = (hour + minute/60.)/24.;
  julian += delta;
/*    julian += 2451545; */
        printf("hour:%d minute:%d delta:%f actual JD: %lf\n",hour,minute,delta,julian);
        julian -=2451545;
    	printf ("final julian:%lf\n",julian);
return(julian);
}

