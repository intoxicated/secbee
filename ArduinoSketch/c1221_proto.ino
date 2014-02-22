#include <C1222.h>
#include <C1222_ACSE.h>
#include <C1222_EPSEM.h>
#include <C1222_Response.h>
#include <C1222_Request.h>

// C12.22 protocol minimal prototype
// Author: Wooyoung
// 2/10/14
// minimal c12.22 implemenation 

const int led = 13;
int readyToRespond = 0;
byte incomingData[2048];
int counter = 0;

C1222_ACSE acse;
C1222_EPSEM epsem;
C1222_Response res;

//request handler
void handleRequest(byte request, byte * data)
{
  C1222_Response res;
  switch (request){
    case 0x20: //id request
      res.identify(RES_OK, 12, 22, 1);
      break;
    //case 0x21 -- terminate 
    //case 0x22 -- disconnect
    
    //case 0x24 title -- dereg serv
    //case 0x25 title -- resolve serv
    //case 0x26 title* -- trace serv
    //case 0x27 node connect device title serial addrlen naaddr period pttrn
                //-- reg serv
            
    //case 0x30 to 0x39, 0x3E, 0x3F -- read
    //case 0x40 to 0x49, 0x4F -- write
    //case 0x50 id user timeout -- logon
    //case 0x51 pw [id] -- security 
    //case 0x52 logoff
    
    //case 0x70 time -- wait serv
    
    //form response into data
    //pass it to epsem to create epsem envolope
    //pass next to acse to create outer packet
  }
 
   readyToRespond = true;
}


void setup(){
  Serial.begin(9600);  
  pinMode(led, OUTPUT);
}

void loop()
{
  void * d;
  if(readyToRespond == true)
  {
    //build response 
    d = res.build();
    //build EPSEM
    epsem.set_data(d, res.get_data_len());
    d = epsem.build();
    //build ACSE
    acse.set_epsem(d,epsem.get_data_len());
    d = acse.build();
    //send back 
    
    int byteSent = Serial.write((uint8_t *)d, acse.get_data_len());
    if(byteSent != acse.get_data_len())
      Serial.println("byte was sent but not matched with size");
      
    //clear all memory allocation
    acse.clear();
  }
}

//function to handle when data is received 
void serialEvent() 
{
  while(Serial.available() > 0) //until null byte is received
  {
    //buffer in
    incomingData[counter++] = Serial.read();
  }
  int offset = 0;
  
  //parse c1222 packet
  
  acse.parse((void*)incomingData); //parse asce
 
  epsem.parse((void*)acse.get_epsem()); //parse epsem 
  
  handleRequest(*(epsem.get_data()), epsem.get_data());
  
}
