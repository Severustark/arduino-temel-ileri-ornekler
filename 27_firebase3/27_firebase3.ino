// firebase realtime database'e bağlanıp sicaklik degerini yazma ve 
// ayarlanan kombi açma değerini okuma
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>                        // firebase kütüphanesini ekle

const char *ssid = "xxxxxxxxxxxxx";                 // bağlanılacak olan kablosuz ağın SSID si
const char *password = "xxxxxxxx";                  // bağlanılacak olan kablosuz ağın şifresi

#define FIREBASE_HOST "********.firebaseio.com"     // veritabanının linki
#define FIREBASE_AUTH "**************"              // veritabanı secret key

int ADC_okunan=0;           // adc'den okunana ham değer
float sensor_gerilim=0.0;   // sensorden okunan degerin gerilim karsiligi mV cinsinden
float sicaklik=0.0;         // okunan sıcaklık değeri
//*******************************************************************
void setup() 
{
  pinMode(D6,OUTPUT);                             // röle portunu çıkış yap
  Serial.begin(9600);                             //Seri iletişim hızı 9600 bps olarak ayarlanarak başlatılıyor.
  delay(1000);
  Serial.println("Wifi agina baglaniliyor");

  WiFi.mode(WIFI_STA);                           //ESP8266 istasyon moduna alınıyor. 
  WiFi.begin(ssid, password);                    //SSID ve şifre girilierek wifi başlatılıyor.

  //------- Wifi ağına bağlanıncaya kadar beklenilen kısım ------------
  while (WiFi.status() != WL_CONNECTED) 
  { 
    delay(500);                                 // Bu arada her 500 ms de bir seri ekrana yan yana noktalar yazdırılarak
    Serial.print(".");                          //görsellik sağlanıyor.
  }
  //-------------------------------------------------------------------

  Serial.println("");                           //Bir alt satıra geçiliyor.
  Serial.println("Bağlantı sağlandı...");       //seri ekrana bağlantının kurulduğu bilgisi gönderiliyor.

  Serial.print("Alınan IP addresi: ");          // kablosuz ağdan alınan IP adresi
  Serial.println(WiFi.localIP());               // kablosuz ağdan alınan IP adresi

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); // firabase i başlat
}
//*******************************************************************
void loop() 
{

  sicaklik_olc();                       // sıcaklıgı ölç

  int ara_deger=int(sicaklik*100);      // virgülden sonra 2 hane bırakma işlemi
  sicaklik=ara_deger/100.0;             // virgülden sonra 2 hane bırakma işlemi
  

  Firebase.setFloat("sicaklik", sicaklik);  // sicaklik etiketine degeri yaz
  
  // handle error
  if (Firebase.failed()) {
      Serial.print("yazdirma hatasi:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
  //----------------------------------------------------------------
  int ayarlanan_deger=Firebase.getInt("ayarlanan");   // ayarlanan etiketini oku
 
  if(sicaklik<ayarlanan_deger)          // ölçülen sıcaklık ayarlanandan küçük mü?
  {
    digitalWrite(D6, HIGH);             // evet kombiyi aç
    Firebase.setString("durum", "KOMBI ON");    // veritabanında durumu güncelle
  }
  else
  {
    digitalWrite(D6, LOW);
    Firebase.setString("durum", "KOMBI OFF");
  }
}
//*******************************************************************
void sicaklik_olc()
{
  ADC_okunan=analogRead(A0);              // ADC yi oku, A0 girişine bağlı LM35
  sensor_gerilim=(ADC_okunan*1000.0)/1023.0; //bu kismin açıklaması en alttadır
  sicaklik=sensor_gerilim/10.0;
}
