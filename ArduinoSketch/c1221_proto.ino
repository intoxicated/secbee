
// C12.22 protocol minimal prototype
// Author: Wooyoung
// 2/10/14
// minimal c12.22 implemenation 

const int led = 13;
int readyToRespond = 0;
byte incomingData[1024];
int counter = 0;

//send response to node
void sendResponse(int * buffer, int bufferSize)
{
  for(int i = 0; i < bufferSize; ++i)
    Serial.print(buffer[i]);
}

//request handler
void handleRequest(int request)
{
  switch (request)
    //case 0x20: //id request
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
    
    readyToRespond = true;
}


void setup(){
  Serial.begin(9600);  
  pinMode(led, OUTPUT);
}

void loop()
{
  if(readyToRespond == 1)
  {
    //handleRequest will fill buffer to respond 

    //Serial.write(re, sizeof(re));
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
  C1222_ACSE acse;
  acse.parse((void*)incomingData); //parse usrinfo part
  
  C1222_EPSEM epsem; 
  epsem.parse((void *)acse.get_usrinfo()); // pass usrinfo from
  
  //handleRequest(
}
