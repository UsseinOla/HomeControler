
#include <ESP8266Client.h>

// Including the ESP8266 WiFi library
#include <ESP8266WiFi.h>

// Replace with your network details
//hotspot =iPhone password=12345678
//TP-LINK_E34E43 //password =71675772
const char* ssid = "iPhone";
const char* password = "12345678";
 String x;
 String flag;
 String serialRead[10];
 int countLine;

// Pass our oneWire reference to Dallas Temperature. 
//DallasTemperature DS18B20(&oneWire);
char temperatureCString[6];
char temperatureFString[6];

// Web Server on port 80
WiFiServer server(80);

// only runs once on boot
void setup()
{
  pinMode(4, OUTPUT);//pin2
  digitalWrite(4, LOW);
  pinMode(0, OUTPUT);//pin3
  digitalWrite(0, LOW);
 
  for(int i=0; i<10;i++)
  {
    serialRead[i]=String(i);
   }
 
  // Initializing serial port for debugging purposes
  Serial.begin(115200);
  delay(10);

  // Connecting to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Starting the web server
  server.begin();
  Serial.println("Web server running. Waiting for the ESP IP...");
  delay(10000);
  
  // Printing the ESP IP address
  Serial.println(WiFi.localIP());
}

 void getData()
 {
  while(Serial.available())
  {
     delay(50);
     x= Serial.readString();
      countLine=(getStringPartByNr(x,')',0)).toInt();
      if(countLine==0)
      serialRead[0]=getStringPartByNr(x,')',1);
      else serialRead[countLine]=getStringPartByNr(x,')',1);
      if(countLine!=10)
      getData();
      break;  
  }
 }

// runs over and over again
void loop() 
{
  // Listenning for new clients
  WiFiClient client = server.available();
  getData();
 
  if (client )
  {
    //  Serial.println("New client");
    // bolean to locate when the http request ends
    boolean blank_line = true;
   
    while (client.connected()) 
    {
      if (client.available()) 
      {
        String request = client.readStringUntil('\r');
        char c = client.read();
       
        if (request.indexOf("/PIN1=ON")>-1)  
        {
    digitalWrite(4, HIGH);
        }
  if (request.indexOf("/PIN1=OFF") != -1) 
  {
    digitalWrite(4, LOW);
  }
     if (request.indexOf("/PIN2=ON")>-1)
     {
    digitalWrite(0, HIGH);
     }
  if (request.indexOf("/PIN2=OFF") != -1)
  {
    digitalWrite(0, LOW);
  }
        if (c == '\n' && blank_line)
        {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
         
            // your actual web page that displays temperature
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<head><style>p{color:greenyellow;width:800px; }");
            client.println("#right{top:140px;left:550px;height:150px;width:350px;}");
            client.println("#top{top:140px;left:20px;height:150px;}");
            client.println(".body{top:0;left:0;height:700px;width:1350px;background:white;position:absolute;font-family:verdana;}");
            client.println("#basic{left:0;top:0;height:80px;width:1350px;;background:whitesmoke;}");  
            client.println(" #move2{left:350px;height:20px;width:400px;top:5px;background:whitesmoke;}");
            client.println(" #size{font-size:60px;font-style:italic;}");
            client.println(" #size1{font-size:40px;font-family:initial;color:#33C4FF;}");
            client.println("#FF{color:#33C4FF;font-size:24px}");
            client.println(" #sizetop{font-size:33px;font-style:italic;}");
            client.println("#sd{color:black;}");
            client.println("</style></head>");
            client.println("<body><div class=body><div id=basic class=body> <div id=move2 class=body>");
            client.println(" <table><tr><td id=size >Smart<td id=size1 >Weather</td></tr></table></div></div>");
            
            client.println(" <div class=body id=top>");
           
            client.println("<table><tr><td id=size >Current<td id=size1 >Data</td></tr></table><table><tr><td id=sizetop >Temperature(C):<td id=FF>");
            client.println(getStringPartByNr(serialRead[0],',',4));              
            client.println("*C</td></tr></table><table><tr><td id=sizetop >Humidity:<td id=FF>");
            client.println(getStringPartByNr(serialRead[0],',',5));
            client.println("</td></tr></table><table><tr><td id=sizetop >Weather:<td id=FF> ");
            client.println(getStringPartByNr(serialRead[0],',',3));             
            client.println("</td></tr></table><table><tr><td id=sizetop >Date:<td id=FF> ");
            client.println(getStringPartByNr(serialRead[0],',',2));
            client.println("</td></tr></table><table><tr><td id=sizetop >To Day is: <td id=FF> ");
            client.println(getStringPartByNr(serialRead[0],',',1));
            client.println("</td></tr></table><table><tr><td id=sizetop >Time:<td id=FF>  ");
            client.println(getStringPartByNr(serialRead[0],',',0));
            client.println("</td></tr></table></div><div class=body id=right > <table><tr><td id=size >Sd<td id=size1 >Card</td></tr></table><p>");
      
         for(int i=1;i<=10;i++)
         {
             client.println("<br>");
             client.println(serialRead[i]);
             client.println("<br>");
               }
         
         client.println("</p></div></div></body></html>");  
             break;
        }
        if (c == '\n')
        {
          // when starts reading a new line
          blank_line = true;
        }
        else if (c != '\r')
        {
          // when finds a character on the current line
          blank_line = false;
        }
      }
    }  
   
  }
}   

String getStringPartByNr(String data, char separator, int index)
{
    // spliting a string and return the part nr index
    // split by separator
    
    int stringData = 0;        //variable to count data part nr 
    String dataPart = "";      //variable to hole the return text
    
    for(int i = 0; i<data.length()-1; i++)
    {    
     //Walk through the text one letter at a time
      if(data[i]==separator)
      {
        //Count the number of times separator character appears in the text
        stringData++;
      }
     else if(stringData==index)
     {
        //get the text when separator is the rignt one
        dataPart.concat(data[i]);  
      }
     else if(stringData>index)
     {
        //return text and stop if the next separator appears - to save CPU-time
        return dataPart;
        break; 
      }
    }
    //return text if this is the last part
    return dataPart;
}
