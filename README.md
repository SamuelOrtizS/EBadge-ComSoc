# E-Badge ComSoc — Escarapela Electrónica Inteligente y Sostenible

> Escarapela electrónica interactiva basada en **e-Paper de 2.9"**, **ESP32-C3** y el protocolo de comunicación de baja latencia **ESP-NOW**. Diseñada para la gestión de tiempos y señalización de estados en vivo durante eventos, ponencias y conferencias de ComSoc.

---

## 🌟 Características Principales

* **Pantalla e-Paper 2.9" (296 x 128 px):** Excelente visibilidad bajo luz de escenario sin deslumbramientos y consumo estático de **0 Watts**.
* **Comunicación ESP-NOW:** Sincronización inalámbrica de baja latencia sin necesidad de redes Wi-Fi ni routers intermediarios.
* **Indicadores LED Neopixel (WS2812B):** Señalización lumínica en vivo para gestión del tiempo de los oradores (*Palabra Asignada*, *Alerta de Tiempo*, *Tiempo Agotado*).
* **Alta Eficiencia Energética:** Integración de modo *Deep Sleep* en el ESP32-C3 con batería LiPo de 500 mAh para más de 12 horas de autonomía continua.
* **Diseño Hardware Extraplano:** PCB personalizada de **100 mm x 70 mm** con ruteado de 2 capas y componentes montados exclusivamente en la cara frontal (*Top Layer*).

---

## 📐 Diseño de Hardware (100 mm x 70 mm)

### Vista Frontal (*Top Layer*)

```text
+-------------------------------------------------------------------+  <-- 100 mm
|                                                                   |
|   +-----------------------------------------------------------+   |
|   |                                                           |   |
|   |                PANTALLA e-PAPER 2.9" SPI                  |   |  ~38 mm
|   |                    (296 x 128 px)                         |   |
|   |                                                           |   |
|   +-----------------------------------------------------------+   |
|                                                                   |
|   [LED 1]                   [LED 2]                   [LED 3]     |  ~8 mm
|  (Neopixel)                (Neopixel)                (Neopixel)   |
|                                                                   |
|   +------------+         +-------------+        +-------------+   |
|   | ESP32-C3   |         | SWITCH OFF  |        | TP4056      |   |  ~24 mm
|   | SuperMini  |         | [ON] [OFF]  |        | USB-C       |===| <-- Puerto USB
|   +------------+         +-------------+        +-------------+   |
+-------------------------------------------------------------------+  <-- 70 mm
```

### Vista Frontal Escarapela Presentador

```text
+-------------------------------------------------------------------+  <-- 100 mm
|                                                                   |
|   +-----------------------------------------------------------+   |
|   |                                                           |   |
|   |                PANTALLA e-PAPER 2.9" SPI                  |   |  ~38 mm
|   |                    (296 x 128 px)                         |   |
|   |                                                           |   |
|   +-----------------------------------------------------------+   |
|                                                                   |
|   [LED 1]                   [LED 2]                   [LED 3]     |  ~8 mm
|  (Neopixel)                (Neopixel)                (Neopixel)   |
|                                                                   |
|  +------------+   [BTN 1]   [BTN 2]   [BTN 3]   +-----+  +-----+  |
|  | ESP32-C3   |   ( ID )   (STATE)   ( SEND )   |SW-ON|  |TP4056|  |  ~24 mm
|  | SuperMini  |   [ O ]     [ O ]     [ O ]     |[OFF]|  |USB-C|==| <-- Puerto USB
|  +------------+                                 +-----+  +-----+  |
+-------------------------------------------------------------------+  <-- 70 mm
```

### Vista Posterior (Bottom Layer)

```text
+-------------------------------------------------------------------+
|                                                                   |
|              PLANO CONTINUO DE MASA EN COBRE (GND)                |
|                                                                   |
|              +------------------------------------+               |
|              |        BATERÍA LiPo 3.7V           |               |
|              |            500 mAh                 |               |
|              |     (35 mm x 30 mm x 5 mm)         |               |
|              +------------------------------------+               |
|                                                                   |
+-------------------------------------------------------------------+
```

## 🛠️ Lista de Materiales (BOM)

|Componente|Especificación|Cantidad|
|----------|--------------|--------|
|Microcontrolador|ESP32-C3 SuperMini|1|
|Pantalla|e-Paper 2.9" SPI (296x128 px)|1|
|LEDs RGB|Neopixel WS2812B (SMD 5050 o módulo)|3|
|Batería|LiPo 3.7V 500 mAh (35x30x5 mm)|1|
|Cargador|Módulo TP4056 USB-C con protección|1|
|Alimentación|Interruptor Deslizante SPDT + Conector JST-PH 2.0|1|
|Soporte|PCB Personalizada (100 mm x 70 mm) + Carcasa 3D (PLA)|1|
|Pulsadores Táctiles (Opcional)|Push-button 6x6 mm SMD|3|

## 📌 Asignación Completa de Pines (Pinout)

| Periférico | Función / Pin | Pin ESP32-C3 |
| :--- | :--- | :--- |
| **e-Paper 2.9"** | SPI BUSY | GPIO 3 |
| | SPI RST | GPIO 4 |
| | SPI DC | GPIO 5 |
| | SPI CS | GPIO 6 |
| | SPI CLK (SCK) | GPIO 8 |
| | SPI DIN (MOSI) | GPIO 10 |
| **Neopixels (x3)** | DATA IN | GPIO 2 |
| **Botón 1 (Maestro)** | BTN_ID (Seleccionar Orador 1-4 / Broadcast) | GPIO 0 |
| **Botón 2 (Maestro)** | BTN_STATE (Seleccionar Estado / Color) | GPIO 1 |
| **Botón 3 (Maestro)** | BTN_SEND (Confirmar y Transmitir ESP-NOW) | GPIO 7 |

## 💻 Arquitectura de Software e Instalación

Requisitos

1. Arduino IDE 2.x o PlatformIO en VS Code.
2. Soporte para placas ESP32 instalado en el entorno (esp32 de Espressif).
3. Librerías requeridas:
    * GxEPD2 (Control de pantalla e-Paper).
    * Adafruit_GFX (Gráficos y fuentes).
    * Adafruit_NeoPixel (Efectos de luces RGB).
    * esp_now.h y WiFi.h (Nativas de ESP32).

## Estructura del Repositorio

```text
EBadge-ComSoc/
├── hardware/
│   ├── gerber/               # Archivos Gerber para fabricación de PCB
│   └── schematics/           # Esquemático en KiCad / EasyEDA
├── firmware/
│   ├── speaker_badge/        # Código para las escarapelas de oradores (Slaves)
│   └── master_controller/    # Código para la escarapela del presentador (Master)
├── 3d_models/                # Archivos STL/STEP para impresión 3D de carcasas
└── README.md
```

## 📡 Estructura de Datos ESP-NOW

```cpp
typedef struct struct_mensaje {
  uint8_t id_orador;      // ID de la escarapela (1-4, 255 = Broadcast)
  char nombre[32];        // Nombre completo a renderizar
  char rol[32];           // Keynote Speaker / Moderador / etc.
  uint8_t estado_led;     // 0: Standby, 1: Hablando (Verde), 2: Alerta (Amarillo), 3: Fin (Rojo)
} struct_mensaje;
```

### 📊 Presupuesto Estimado — E-Badge ComSoc (5 Unidades)

| Componente / Concepto | Especificación | Cant. | Costo Unit. (COP) | Costo Total (COP) | Costo Total (USD) |
| :--- | :--- | :---: | :---: | :---: | :---: |
| **ESP32-C3 SuperMini** | RISC-V 32-bit, WiFi + BLE5, USB-C | 5 | $20,000 | $100,000 | $31.70 |
| **Pantalla e-Paper 2.9"** | Resolución 296x128 px, interfaz SPI | 5 | $48,000 | $240,000 | $76.09 |
| **LEDs RGB Neopixel** | Tira / Módulo WS2812B (3 LEDs por placa) | 5 | $5,000 | $25,000 | $7.93 |
| **Batería LiPo 3.7V** | Capacidad 500 mAh (35x30x5 mm) | 5 | $16,000 | $80,000 | $25.36 |
| **Cargador TP4056** | Módulo de carga con protección USB-C | 5 | $4,000 | $20,000 | $6.34 |
| **Pulsadores Táctiles** | Push-button 6x6 mm SMD | 3 | $1,500 | $7,500 | $2.38 |
| **Control & Conexión** | Interruptor SPDT + Conector JST + Cables | 5 | $4,000 | $20,000 | $6.34 |
| **Fabricación PCB** | Lote mínimo de 5 PCBs (100x70 mm, 2 capas) | 1 | $20,000 | $20,000 | $6.34 |
| **Envío PCB Express** | Envío internacional (DHL / FedEx / Express) | 1 | $80,000 | $80,000 | $25.36 |
| **Carcasa 3D & Agarre** | Impresión 3D en PLA + Clip de fijación / Imán | 5 | $10,000 | $50,000 | $15.85 |
| **TOTAL PROYECTO** | **Lote completo de 5 escarapelas terminadas** | **—** | **—** | **$642,500** | **$203.67** |
