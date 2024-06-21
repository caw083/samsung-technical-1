#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

#define DHTPIN 4         // Pin dimana sensor DHT terhubung
#define DHTTYPE DHT22    // Tipe sensor DHT

const char* ssid = "Your_SSID";
const char* password = "Your_Password";
const char* serverName = "http://192.168.1.100:5000/data"; // Ganti dengan alamat API Flask Anda

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(115200);

    // Menghubungkan ke Wi-Fi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("Connected to WiFi");

    dht.begin();
}

void loop() {
    // Membaca data dari sensor DHT22
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // Memeriksa apakah pembacaan berhasil
    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    // Menambahkan timestamp
    String timestamp = String(millis());

    // Membuat payload JSON
    String jsonPayload = "{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity) + ",\"timestamp\":\"" + timestamp + "\"}";
    Serial.println("Payload: " + jsonPayload);

    // Mengirim data ke server Flask
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(serverName);
        http.addHeader("Content-Type", "application/json");
        int httpResponseCode = http.POST(jsonPayload);

        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.println(httpResponseCode);
            Serial.println(response);
        } else {
            Serial.print("Error on sending POST: ");
            Serial.println(httpResponseCode);
        }

        http.end();
    } else {
        Serial.println("WiFi Disconnected");
    }

    delay(60000); // Mengirim data setiap 60 detik
}