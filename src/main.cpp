#include "mbed.h"
#include "FastAnalogIn.h"
/*
 *	NR DAQ Code for Circuit Testing for LPC 1768
 *	===============================
 * 	Circuit Diagram:
 *	(For detailed circuit, refer the pin diagram in the root dir)
 *				_________
 				|		|
 				|		|
 				|		|
 				|		|
 				|		|
 				|		|
 				|		|
 				|		|
 				|		|
 				|		|
 				|		|
 				|		|
 				|		|
 				|		|=>p27:dss
 	   TPS:p15<=|		|=>p26:uss
	   LAM:p16<=|		|=>p25:lcs
 	   LON:p17<=|		|=>p24:rws
 	   LAT:p18<=|		|=>p23:lws
 	   BAT:p19<=|		|=>p22:uws
 	   ECT:p20<=|_______|=>p21:RPM
 */

/*
 * Array to denote values of the interrupts we are using
 *	0 : RPM
 *  1 : UWS
 *  2 : LWS
 *  3 : RWS
 *  4 : LCS
 *  5 : USS
 *  6 : DSS
 */
int inter[] = {0,0,0,0,0,0,0};

Serial pc(p13,p14);
        
void rpm_set(){
  inter[0] = 1;
}
void uws_set(){
  inter[1] = 1;
}
void lws_set(){
  inter[2] = 1;
}
void rws_set(){
  inter[3] = 1;
}
void lcs_set(){
  inter[4] = 1;
}
void uss_set(){
  inter[5] = 1;
}
void dss_set(){
  inter[6] = 1;
}

DigitalOut myled(LED1);

DigitalOut myled2(LED2);
  
int main(){
    pc.baud(38400);

    FastAnalogIn tps(p15);
    FastAnalogIn lam(p16);
    FastAnalogIn lon(p17);
    FastAnalogIn lat(p18);
    FastAnalogIn bat(p19);
    FastAnalogIn ect(p20);
    
    
    InterruptIn rpm(p21);
    InterruptIn uws(p22);
    InterruptIn lws(p23);
    InterruptIn rws(p24);
    InterruptIn lcs(p25);
    InterruptIn uss(p26);
    InterruptIn dss(p27);

    rpm.rise(&rpm_set);
    uws.rise(&uws_set);
    lws.rise(&lws_set);
    rws.rise(&rws_set);
    lcs.rise(&lcs_set);
    uss.rise(&uss_set);
    dss.rise(&dss_set);

	setbuf(pc, NULL);
    // No interrupts in the beginning, so everything set to 0
    
    int j = 0;
    float buff[6];
    
    Timer t;
    t.start();
    
    pc.printf("\n\n\nSTARTING...\n");
    
    while(j < 5000){
            
            buff[0] = tps.read();
            buff[1] = lam.read();
            buff[2] = lon.read();
            buff[3] = lat.read();
            buff[4] = bat.read();
            buff[5] = ect.read();    
        
            pc.printf("%d => %f,%f,%f,%f,%f,%f,%i,%i,%i,%i,%i,%i,%i\n",j,buff[0],buff[1],buff[2],buff[3],buff[4],buff[5],inter[0],inter[1],inter[2],inter[3],inter[4],inter[5],inter[6]);
            
            inter[0] = inter[1] = inter[2] = inter[3] = inter[4] = inter[5] = inter[6] = 0;
            
            j++;
            
        }
    t.stop();
    pc.printf("samples per second:%f", (5000*13)/t.read());
}