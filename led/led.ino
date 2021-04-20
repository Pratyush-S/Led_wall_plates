#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

const char* ssid = "appricot";
const char* password = "poolloop";

const char* PARAM_INPUT_1 = "universal_color";
const char* PARAM_INPUT_2 = "Brightness";
const char* PARAM_INPUT_3 = "profile";

#include <FastLED.h>

#define LED_PIN     5
#define NUM_LEDS    36
#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

void set_color(int led_id[3],int R,int G,int B);
void RedlolBlueTrail();
void radioactive(int time_1);
void pokeball(int time_1);
void reset_all();
void randow_updown();
void FillLEDsFromPaletteColors( uint8_t colorIndex);


String inputMessage;
String inputParam;
int inputparam_int;

int triangle_2[3]={1,2,3};
int triangle_1[3]={4,5,6};
int triangle_4[3]={10,11,12};
int triangle_3[3]={7,8,9};
int triangle_5[3]={13,14,15};
int triangle_6[3]={16,17,18};
int triangle_7[3]={19,20,21};
int triangle_8[3]={22,23,24};
int triangle_9[3]={25,26,27};
int triangle_10[3]={28,29,30};
int triangle_11[3]={31,32,33};
int triangle_12[3]={34,35,36};


int user_input=0;  
#define UPDATES_PER_SECOND 100


CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

//////////// PROFILES  //////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////


// HTML web page to handle 3 input fields (input1, input2, input3)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <form action="/get">  
  Color Picker:  
   <input type="color" name="universal_color">
 <input type="submit" value="Submit">
</form><br>
  <form action="/get">
    Brightness: <input type="range" name="Brightness" min="1" max="255" value="50" class="slider" id="myRange" value="50">
  <input type="submit" value="Submit">
  
  </form><br>
  <form action="/get">
<label for="profile">Choose Color profile:</label>

  <select name="profile" id="profile">
  <option value="radioactive">Radioactive</option>
  <option value="pokeball">Pokeball</option>
  <option value="seaborn">seaborn</option>
  <option value="solid">Solid</option>
  </select>
    <input type="submit" value="Submit">
  </form><br></body></html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}



void setup() {

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  pinMode(18,OUTPUT);
  
  pinMode(2,OUTPUT);
  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {

  
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      inputparam_int=1;
      
    }
    
    // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_2)->value();
      inputParam = PARAM_INPUT_2;
     inputparam_int=2;
    }

  // GET input3 value on <ESP_IP>/get?input2=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_3)) {
      inputMessage = request->getParam(PARAM_INPUT_3)->value();
      inputParam = PARAM_INPUT_3;
      inputparam_int=3;
    }

    
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
                                     + inputParam + ") with value: " + inputMessage +
                                     "<br><a href=\"/\">Return to Home Page</a>");
  });
  server.onNotFound(notFound);
  server.begin();
  
    Serial.begin(9600);
    delay( 3000 ); // power-up safety delay
    
    
    
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    
    
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
    
    
}


void loop()
{

Serial.println(inputMessage);


switch(inputparam_int) {

   case 2  :
        FastLED.setBrightness(  inputMessage.toInt() );
   break; 


    case 3  :

        if (inputMessage=="radioactive"){radioactive(400);}
        else if (inputMessage=="pokeball"){pokeball(400);}
        else if (inputMessage=="seaborn"){
          
              currentPalette = PartyColors_p;           
              currentBlending = LINEARBLEND;
              static uint8_t startIndex = 0;
              startIndex = startIndex + 1; /* motion speed */
              FillLEDsFromPaletteColors( startIndex);
              FastLED.show();
              FastLED.delay(1000 / UPDATES_PER_SECOND);
              
          }
        else if (inputParam=="solid"){
          
          }
        
        
   break; 

  
}    
  }   


///////////////////////////////////////////////////////////////////////////////////////////////////////
void set_color(int led_id[3],int R,int G,int B){
int i=0;
for(i=0;i<3;i++){
leds[led_id[i]-1] = CRGB (R,G,B);  
} 
}

//////////// PATTERNS  //////////////////////////////////////////////////////////////////////////////////






void reset_all(){ 
for (int i = 0; i <= NUM_LEDS; i++)
{
leds[i] = CRGB (0,0,0);
FastLED.show();
}
}

void radioactive(int time_1){

set_color(triangle_2,0,0,0);
set_color(triangle_4,0,0,0);
set_color(triangle_6,0,0,0);
set_color(triangle_8,0,0,0);
set_color(triangle_10,0,0,0);
set_color(triangle_12,0,0,0);



set_color(triangle_1,255,255,0);
set_color(triangle_3,0,0,0);
set_color(triangle_5,255,255,0);
set_color(triangle_7,0,0,0);
set_color(triangle_9,255,255,0);
set_color(triangle_11,0,0,0);
FastLED.show();
delay(time_1);
set_color(triangle_3,255,255,0);
set_color(triangle_1,0,0,0);
set_color(triangle_7,255,255,0);
set_color(triangle_5,0,0,0);
set_color(triangle_11,255,255,0);
set_color(triangle_9,0,0,0);
FastLED.show();
delay(time_1);
}


void pokeball(int time_1){
  
set_color(triangle_2,0,0,0);
set_color(triangle_4,0,0,0);
set_color(triangle_6,0,0,0);
set_color(triangle_8,0,0,0);
set_color(triangle_10,0,0,0);
set_color(triangle_12,0,0,0);

set_color(triangle_1,255,255,255);          //1
set_color(triangle_3,255,255,255);
set_color(triangle_5,255,0,0);
set_color(triangle_7,255,0,0);
set_color(triangle_9,255,0,0);
set_color(triangle_11,255,255,255);
FastLED.show();
delay(time_1);
set_color(triangle_3,255,255,255);        //2
set_color(triangle_5,255,255,255);
set_color(triangle_7,255,0,0);
set_color(triangle_9,255,0,0);
set_color(triangle_11,255,0,0);
set_color(triangle_1,255,255,255);
FastLED.show();
delay(time_1);
set_color(triangle_5,255,255,255);      //3
set_color(triangle_7,255,255,255);
set_color(triangle_9,255,0,0);
set_color(triangle_11,255,0,0);
set_color(triangle_1,255,0,0);
set_color(triangle_3,255,255,255);
FastLED.show();
delay(time_1);
set_color(triangle_7,255,255,255);        //4
set_color(triangle_9,255,255,255);  
set_color(triangle_11,255,0,0);
set_color(triangle_1,255,0,0);
set_color(triangle_3,255,0,0);
set_color(triangle_5,255,255,255);
FastLED.show();
delay(time_1);
set_color(triangle_9,255,255,255);          //5
set_color(triangle_11,255,255,255);
set_color(triangle_1,255,0,0);
set_color(triangle_3,255,0,0);
set_color(triangle_5,255,0,0);
set_color(triangle_7,255,255,255);
FastLED.show();
delay(time_1);
set_color(triangle_11,255,255,255);            //6
set_color(triangle_1,255,255,255);
set_color(triangle_3,255,0,0);
set_color(triangle_5,255,0,0);
set_color(triangle_7,255,0,0);
set_color(triangle_9,255,255,255);
FastLED.show();
delay(time_1);
}
 

    
void RedlolBlueTrail()//-------------------Blue Red Trail
{
for (int i = 0; i <= NUM_LEDS; i++)
{
leds[i] = CRGB (255,0,0);
FastLED.show();
delay(40);
}
for (int i = 0; i <= NUM_LEDS; i++)
{
leds[i] = CRGB (0,0,255);
FastLED.show();
delay(40);
}
}




void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}
