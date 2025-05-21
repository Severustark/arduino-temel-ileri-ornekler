// WEB sunucusuna client olarak bağlanıp veri çeken program
// Çekilen veri satır satır okunup seri porttan gönderiliyor
#include <ESP8266WiFi.h>

const char *ssid = "xxxxxxxxxxx";                 // bağlanılacak olan kablosuz ağın SSID si
const char *password = "xxxxxxxxxxxxx";           // bağlanılacak olan kablosuz ağın şifresi

const char *sunucu = "xxx.xxx.xxx.xxx";           // bağlanılacak olan web sayfasının adresi veya IP adresi
String uzanti="/";                                // bağlanılacak olan web sayfasının uzantısı

String okunan_veri="";
//------------------------------------------------------------------------------------
void setup() 
{
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
}
//------------------------------------------------------------------------------------
void loop() 
{
  WiFiClient istemci;                           // istemci nesnesi oluşturuldu
  
  if (!istemci.connect(sunucu, 80))             // web bağlantısı yap, bağlanamadı mı?
  {
    Serial.println("Baglanti kurulamadi!!!");   // bağlanamadı, hata ver
    return;
  }
  
  Serial.println("Web Sunucusuna baglandi");

  // sunucudan "***.***.***.***/" adresindeki siteyi iste, aşağıdaki kod HTTP protokolü ile sayfa isteği
  istemci.print(String("GET ") + uzanti + " HTTP/1.1\r\n" + "Host: " + sunucu + "\r\n" + "Connection: close\r\n\r\n");


  //-------- 5 sn lik zaman döngüsü kur ve istek geldi mi diye kontrol et ------
  // çalışma zamanı milisaniye cinsinden tutan millis() fonksiyonundan süre al, başlangıc zamanı
  unsigned long zaman_asimi = millis();
  while (istemci.available() == 0)            // veri gelmedi mi?
  {                                           // evet gelmedi
    if (millis() - zaman_asimi > 5000)        // 5 sn doldu mu?
    {                                         // evvet süre doldu
      Serial.println(">>> Zaman asimi !");  
      istemci.stop();
      return;
    }
  }
  //**************** veri gelmis okuma kısmı **********************
  while (istemci.available())                 // sunucudan gelen bilgi var mı?
  {
    okunan_veri = istemci.readStringUntil('\r');  // sunucu tarafından gönderilen tüm bilgiyi satır sonuna kadar oku
    Serial.print(okunan_veri);                    // seri porttan gönder
  }
  Serial.println("");
  Serial.println("....................");
}
