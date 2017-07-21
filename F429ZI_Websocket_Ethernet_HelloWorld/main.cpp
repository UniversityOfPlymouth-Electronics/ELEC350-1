#if !FEATURE_LWIP
    #error [NOT_SUPPORTED] LWIP not supported for this target
#endif
 
#include "mbed.h"
#include "EthernetInterface.h"
#include "TCPServer.h"
#include "TCPSocket.h"
#include "BMP180_Sensor.h"
 
#define HTTP_STATUS_LINE "HTTP/1.0 200 OK"
#define HTTP_HEADER_FIELDS "Content-Type: text/html; charset=utf-8"
#define HTTP_MESSAGE_BODY ""                                     \
"<html>" "\r\n"                                                  \
"  <body style=\"display:flex;text-align:center\">" "\r\n"       \
"    <div style=\"margin:auto\">" "\r\n"                         \
"      <h1>Hello World</h1>" "\r\n"                              \
"      <p>It works !</p>" "\r\n"                                 \
"    </div>" "\r\n"                                              \
"  </body>" "\r\n"                                               \
"</html>" "\r\n"
    
#define HTTP_RESPONSE HTTP_STATUS_LINE "\r\n"   \
                      HTTP_HEADER_FIELDS "\r\n" \
                      "\r\n"                    \
                      HTTP_MESSAGE_BODY "\r\n"
#define HTTP_TITLE     "<head><title> Plymouth Uni Weather Page </title></head>" "\r\n"
#define HTTP_FORMAT_1 "<body style=\"display:flex;text-align:center\">" "\r\n" \
                      "<div style=\"margin:auto\">" "\r\n"
#define HTTP_BOTTOM "</html>" "\r\n"

#define IP        "192.168.1.2"
#define NETMASK   "255.255.255.0"
#define GATEWAY   "192.168.1.1"



void ReadFlag_ISR(void);
bool ReadFlag=true;
char WeatherData[3000]={0};
		
int main()
{
    printf("Basic HTTP server example\r\n");
    //generic error code handle
    int err=0;
  
    //BMP180 Stuff here
    int T_Temp,T_Press, msTempNumber, msTempDecimal, msPressNumber, msPressDecimal;

    BMP180_Initialise();
    
    Ticker ticktock;
    ticktock.attach(ReadFlag_ISR,1);
    
    EthernetInterface eth;
    eth.set_network(IP, NETMASK, GATEWAY);
		
    eth.connect();
    printf("The target IP address is '%s'\r\n", eth.get_ip_address());
    
    TCPServer srv;
    TCPSocket clt_sock;
    SocketAddress clt_addr;

    
    /* Open the server on ethernet stack */
    srv.open(&eth);
    
    /* Bind the HTTP port (TCP 80) to the server */
    srv.bind(eth.get_ip_address(), 80);
    
    /* Can handle 5 simultaneous connections */
    srv.listen(5);

    while (true) {

        srv.accept(&clt_sock, &clt_addr);
        printf("accept %s:%d\r\n", clt_addr.get_ip_address(), clt_addr.get_port());

      //clt_sock.send(HTTP_RESPONSE, strlen(HTTP_RESPONSE));
        
        if (ReadFlag==true){
            T_Temp=Temperature();T_Press=Pressure();// Get the temperature and pressure from BMP180_Sensor.c
            msTempNumber=T_Temp/10;                    //convert into two parts by divide and mod X
            msTempDecimal=T_Temp%10;                    //For the purpose of making a char string constructor sprintf see below
            msPressNumber=T_Press/100;                 //convert into two parts by divide and mod X
            msPressDecimal=T_Press%100;                 //For the purpose of making a char string constructor sprintf see below
            sprintf(WeatherData,"\
  \r\n%s\r\n%s\r\n\r\n<html>\n\r%s%s\r\n\
  <h1>Temp:%d.%d&#8451</h1>\r\n<h1>Pressure:%d.%dmBar</h1>\
  \r\n</div>\r\n</body>\r\n</html>\r\n\r\n"\
  ,HTTP_STATUS_LINE, HTTP_HEADER_FIELDS, HTTP_TITLE, HTTP_FORMAT_1\
  , msTempNumber , msTempDecimal, msPressNumber, msPressDecimal);
					
					
						sptintf(WeatherData, "<!doctype html><html><head><title>Bootstrap Example</title><meta charset="utf-8"><meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css"><script src="https://ajax.googleapis.com/ajax/libs/jquery/3.2.0/jquery.min.js"></script>
  <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>
<style>.vcenter {display: flex; align-items: center; margin-top: 40px; } </style><style> .blocky { background-color: #80c0ff; border-radius: 10px;	} </style></head><body>
<div class="container-fluid"><div class="row vcenter"><div class="col-sm-3"></div><div class="col-sm-6"><p class="text-center">(c) 2017 University of Plymouth</p></div><div class="col-sm-3"></div></div>
<div class="row vcenter"><div class="col-sm-4"></div><div class="col-sm-4 blocky"><h1 class="text-center">ELEC350/1</h1></div><div class="col-sm-4"></div></div><div class="row vcenter">
<div class="col-sm-3"></div><div class="col-sm-3"><img class="img-responsive center-block" src="https://liveplymouthac-my.sharepoint.com/personal/nicholas_outram_plymouth_ac_uk/_layouts/15/guestaccess.aspx?docid=08547bac782314371bc5c4916b0eaea1d&authkey=AQrcBUKegX2XPX8u8Dd_okY" alt="thermometer" width="48" height="48">
    	</div>
    	<div class="col-sm-3">
    		<h2 class="text-center">25.5 C</h2>
    	</div>
    	<div class="col-sm-3"></div>
	</div>

	<div class="row vcenter">
    	<div class="col-sm-3"></div>
    	<div class="col-sm-3">
    		<img class="img-responsive center-block" src="https://liveplymouthac-my.sharepoint.com/personal/nicholas_outram_plymouth_ac_uk/_layouts/15/guestaccess.aspx?docid=02b3cd38de5504d71b10dc301ab0b8281&authkey=AcXMegXcpHQZENZeBKxSAV4" alt="barometer_2" width="96" height="96">
    	</div>
    	<div class="col-sm-3">
    		<h2 class="text-center">1020 kPA</h2>
    	</div>
    	<div class="col-sm-3"></div>
	</div>

	<div class="row vcenter">
    	<div class="col-sm-12">
    		<img class="img-responsive center-block" src="https://liveplymouthac-my.sharepoint.com/personal/nicholas_outram_plymouth_ac_uk/_layouts/15/guestaccess.aspx?docid=0301b44bbf8dd4d6d97fe810f88dcdead&authkey=AQdrJ94ySuVT2kYXA1xgnB0" alt="refresh" width="48" height="48">
    	</div>
	</div>


</div>
<div class="alert alert-success alert-dismissable">
  <a href="#" class="close" data-dismiss="alert" aria-label="close">&times;</a>
  <strong>Note!</strong> Refresh the page to update the data.
</div>



</body>
</html>
");
					
            ReadFlag=false;
            }
        printf("String Length/Error Number:%d\r\nWeather Data String is:%s\r\n",err,WeatherData);
        err=clt_sock.send(WeatherData, strlen(WeatherData));
        //printf("String Length/Error Number:%d\r\nWeather Data String is:%s\r\n",err,WeatherData);
         }
}
void ReadFlag_ISR(void){
    ReadFlag=true;
    }
