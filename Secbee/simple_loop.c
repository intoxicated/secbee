#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <xbee.h>

//define common constants
//some of them are can be variable from user inputs
#define XBEE_TYPE "xbee1"
#define XBEE_BRATE 9600
#define XBEE_DEVICE "/dev/cu.usbserial-AD025EOA"

#define XBEER(x) xbee_errorToStr(x)

static int count = 0;
int process = 1;

//struct xbee *xbee;
//struct xbee_con *con;
//simple testing to communicate with standalone xbee
//

void signal_handler(int signum)
{
  switch(signum)
  {
    case SIGTSTP:
      printf("ctrl-z was pressed to stop..");
    default:
      break;
  } 
}
/* callback */
void myCB(struct xbee *xbee, struct xbee_con *con,
    struct xbee_pkt **pkt, void **data){
  printf("[*] Starting callback routine .. \n");
  //check data length 
  if((*pkt)->dataLen == 0)
  {
    printf("[!] data length is too short. \n");
    return;
  }

  printf("rx: [%s]\n", (*pkt)->data);
}

void banner(void){
  puts("****************************************************\n\
**************** Simple loop xbee ******************\n\
***************** Created by Woo *******************\n\
****************************************************");
}

int main(int argc, char ** argv)
{
  struct xbee *xbee;
  struct xbee_con *con;
  unsigned char txRet;
  xbee_err ret;
  FILE * log;
  struct xbee_conInfo info;
  
  banner();

  //increase limit of opening files to make libxbee run
  system("ulimit -n 1024");
  //signal handler
  struct sigaction sa;
  sa.sa_handler = signal_handler;
  sigaction(SIGTSTP, &sa, NULL);

  //set up xbee instance
  if((ret = xbee_setup(&xbee, XBEE_TYPE, XBEE_DEVICE, XBEE_BRATE)) != XBEE_ENONE)
  {
    printf("xbee_setup error(%d): %s\n", ret, XBEER(ret));
    return ret;
  }

  printf("[*] Successfully set up Xbee instance\n");

  if ((log = fopen("libxbee.log", "w+")) == NULL)
    return -1;
  if (xbee_logTargetSet(xbee, log) != XBEE_ENONE) 
    return -1;
  if (xbee_logLevelSet(xbee,100) != XBEE_ENONE)
    return -1;

  //just double check mode 
  char *mode;
  xbee_modeGet(xbee, &mode);
  printf("[*] xbee is running in %s mode\n", mode);

  //create connection
  struct xbee_conAddress address;
  //16 bit address hense DL == MY
  //this is not working somehow..
  //and send to the destination that is already defined by coolterm
  memset(&address, 0, sizeof(address));
  /*address.addr64_enabled = 1;
  address.addr64[0] = 0x00;
  address.addr64[1] = 0x13;
  address.addr64[2] = 0xA2;
  address.addr64[3] = 0x00;
  address.addr64[4] = 0x40;
  address.addr64[5] = 0x86;
  address.addr64[6] = 0x36;
  address.addr64[7] = 0xAB;
  */
  address.addr16_enabled = 1;
  address.addr16[0] = 0x56;
  address.addr16[1] = 0x78;

  //printf("[*] Dest addr: 0x%02x%02x\n", address.addr16[0], address.addr16[1]);
  if((ret = xbee_conNew(xbee, &con, "16-bit Data", &address)) != XBEE_ENONE) {
    xbee_log(xbee, -1, "xbee_newCon() returned: %d (%s)\n", ret, XBEER(ret));
    return ret;
  }

  //not sure what this is about
  if((ret = xbee_conDataSet(con, xbee, NULL)) != XBEE_ENONE){
    xbee_log(xbee, -1, "xbee_conDataSet returned: %d (%s)\n", ret, XBEER(ret));
    return ret;
  }

  //callback is not getting called, no receive data???
  if((ret = xbee_conCallbackSet(con, myCB, NULL)) != XBEE_ENONE){
    xbee_log(xbee, -1, "xbee_conCallbackSet returned: %d (%s)\n", ret, XBEER(ret));
    return ret;
  }

  //this is actually sent out to end point 
  ret = xbee_conTx(con, NULL, "L\r\n");
  printf("Ret: %d\n", ret);
  usleep(3000000);

  ret = xbee_conTx(con, NULL, "H\r\n");
  printf("Ret: %d\n", ret);
  
  usleep(3000000);
  
  //while(process)
  //{
    //receive user input and send it to endpoint 
    //after signal handler was called, out of loop
  //}


  if((ret = xbee_conEnd(con)) != XBEE_ENONE){
    xbee_log(xbee, -1, "xbee_conEnd returned: %d\n", ret);
    return ret;
  }

  //uncomment if needs debug
  //system("rm -f libxbee.log");

  xbee_shutdown(xbee);
  return 0;





}
