#include <WiFi.h>
#include <esp_now.h>
#include <Adafruit_GFX.h>
#include <GxEPD2_BW.h>
#include <Adafruit_NeoPixel.h>

// ============================================================================
// CONFIGURACIÓN DE IDENTIFICACIÓN
// ============================================================================
#define MY_BADGE_ID 1 // Cambiar a 1, 2, 3 o 4 para cada escarapela de orador

// ============================================================================
// ASIGNACIÓN DE PINES (ESP32-C3)
// ============================================================================
#define EPD_BUSY 3
#define EPD_RST  4
#define EPD_DC   5
#define EPD_CS   6
#define EPD_SCK  8
#define EPD_MOSI 10

#define PIN_NEOPIXEL 2
#define NUM_LEDS     3

// ============================================================================
// ESTRUCTURA DE DATOS ESP-NOW
// ============================================================================
typedef struct struct_mensaje {
  uint8_t id_orador;      // 1-4 para específicas, 255 para Broadcast (Todas)
  char nombre[32];        // Nombre completo del orador
  char rol[32];           // Cargo, Institución o Rol
  uint8_t estado_led;     // 0: Standby, 1: Hablando (Verde), 2: Alerta (Amarillo), 3: Fin (Rojo)
} struct_mensaje;

struct_mensaje datosRecibidos;

// ============================================================================
// INICIALIZACIÓN DE PERIFÉRICOS
// ============================================================================
// Ajustar GxEPD2_290_T94 según el modelo exacto del controlador del e-Paper de 2.9"
GxEPD2_BW<GxEPD2_290_T94, GxEPD2_290_T94::HEIGHT> display(
    GxEPD2_290_T94(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY)
);

Adafruit_NeoPixel strip(NUM_LEDS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

// Variables de estado local
char nombreActual[32] = "Orador Invitar";
char rolActual[32]    = "ComSoc Student Chapter";
uint8_t estadoLedActual = 0;

// ============================================================================
// FUNCIONES DE CONTROL DE LEDS Y PANTALLA
// ============================================================================

void aplicarEstadoLED(uint8_t estado) {
  estadoLedActual = estado;
  switch (estado) {
    case 1: // Hablando - Verde constante
      for (int i = 0; i < NUM_LEDS; i++) strip.setPixelColor(i, strip.Color(0, 255, 0));
      break;
    case 2: // Alerta - Amarillo / Naranja
      for (int i = 0; i < NUM_LEDS; i++) strip.setPixelColor(i, strip.Color(255, 180, 0));
      break;
    case 3: // Tiempo Agotado - Rojo constante
      for (int i = 0; i < NUM_LEDS; i++) strip.setPixelColor(i, strip.Color(255, 0, 0));
      break;
    case 0: // Standby / Apagado
    default:
      strip.clear();
      break;
  }
  strip.show();
}

void renderizarPantalla(const char* nombre, const char* rol) {
  display.setRotation(1); // Orientación Horizontal (Landscape)
  display.setFont();      // Fuente por defecto de Adafruit GFX (escalable)
  display.setFullWindow();

  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    
    // Encabezado institucional
    display.fillRect(0, 0, 296, 20, GxEPD_BLACK);
    display.setTextColor(GxEPD_WHITE);
    display.setTextSize(1);
    display.setCursor(10, 6);
    display.print("IEEE ComSoc -- EVENTO PRINCIPAL");

    // Nombre del Orador (Texto Principal)
    display.setTextColor(GxEPD_BLACK);
    display.setTextSize(2);
    display.setCursor(10, 45);
    display.print(nombre);

    // Línea divisora
    display.drawFastHLine(10, 75, 276, GxEPD_BLACK);

    // Rol / Afiliación
    display.setTextSize(1);
    display.setCursor(10, 88);
    display.print(rol);

    // Pie de página
    display.setCursor(10, 110);
    display.print("ID Badge: ");
    display.print(MY_BADGE_ID);

  } while (display.nextPage());
}

// ============================================================================
// CALLBACK RECEPCIÓN ESP-NOW
// ============================================================================
#if defined(ESP_IDF_VERSION_MAJOR) && ESP_IDF_VERSION_MAJOR >= 5
void OnDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len) {
#else
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
#endif
  memcpy(&datosRecibidos, incomingData, sizeof(datosRecibidos));

  // Verificar si el mensaje es para esta escarapela o es un Broadcast
  if (datosRecibidos.id_orador == MY_BADGE_ID || datosRecibidos.id_orador == 255) {
    
    // 1. Actualizar estado de LEDs
    aplicarEstadoLED(datosRecibidos.estado_led);

    // 2. Si el nombre o rol cambiaron, actualizar la pantalla e-Paper
    if (strcmp(nombreActual, datosRecibidos.nombre) != 0 || strcmp(rolActual, datosRecibidos.rol) != 0) {
      strncpy(nombreActual, datosRecibidos.nombre, sizeof(nombreActual));
      strncpy(rolActual, datosRecibidos.rol, sizeof(rolActual));
      renderizarPantalla(nombreActual, rolActual);
    }
  }
}

// ============================================================================
// SETUP & LOOP
// ============================================================================
void setup() {
  Serial.begin(115200);

  // Inicializar LEDs Neopixel
  strip.begin();
  strip.setBrightness(80); // Ajuste de brillo (0 - 255)
  aplicarEstadoLED(0);

  // Inicializar Pantalla e-Paper (Bus SPI)
  SPI.begin(EPD_SCK, -1, EPD_MOSI, EPD_CS);
  display.init(115200, true, 2, false);

  // Renderizado Inicial
  renderizarPantalla(nombreActual, rolActual);

  // Inicializar Wi-Fi en modo Station para ESP-NOW
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Inicializar ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error al inicializar ESP-NOW");
    return;
  }

  // Registrar función Callback de recepción
  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("Escarapela Lista. Esperando comandos ESP-NOW...");
}

void loop() {
  // El manejo de comunicación es por interrupciones/callbacks.
  // Mantenemos el loop liviano para máximo ahorro de batería.
  delay(100);
}