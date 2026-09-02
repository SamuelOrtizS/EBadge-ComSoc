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

## 📌 Asignación de Pines (Pinout Sugerido)

|Periférico|Pin e-Paper / Neopixel|Pin ESP32-C3|
|----------|----------------------|------------|
|e-Paper 2.9"|SPI BUSY|GPIO 3|
|-|SPI RST|GPIO 4|
|-|SPI DC|GPIO 5|
|-|SPI CS|GPIO 6|
|-|SPI CLK (SCK)|GPIO 8|
|-|SPI DIN (MOSI)|GPIO 10|
|Neopixels (x3)|DATA IN|GPIO 2|

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
| **ESP32-C3 SuperMini** | RISC-V 32-bit, WiFi + BLE5, USB-C | 5 | $20,000 | $100,000 | $25.00 |
| **Pantalla e-Paper 2.9"** | Resolución 296x128 px, interfaz SPI | 5 | $48,000 | $240,000 | $60.00 |
| **LEDs RGB Neopixel** | Tira / Módulo WS2812B (3 LEDs por placa) | 5 | $5,000 | $25,000 | $6.25 |
| **Batería LiPo 3.7V** | Capacidad 500 mAh (35x30x5 mm) | 5 | $16,000 | $80,000 | $20.00 |
| **Cargador TP4056** | Módulo de carga con protección USB-C | 5 | $4,000 | $20,000 | $5.00 |
| **Control & Conexión** | Interruptor SPDT + Conector JST + Cables | 5 | $4,000 | $20,000 | $5.00 |
| **Fabricación PCB** | Lote mínimo de 5 PCBs (100x70 mm, 2 capas) | 1 | $20,000 | $20,000 | $5.00 |
| **Envío PCB Express** | Envío internacional (DHL / FedEx / Express) | 1 | $80,000 | $80,000 | $20.00 |
| **Carcasa 3D & Agarre** | Impresión 3D en PLA + Clip de fijación / Imán | 5 | $10,000 | $50,000 | $12.50 |
| **TOTAL PROYECTO** | **Lote completo de 5 escarapelas terminadas** | **—** | **—** | **$635,000** | **$158.75** |

> **Nota de Patrocinio Académico:** En caso de aprobarse la esponsorización de las PCBs (JLCPCB / PCBWay / etc.), el costo total se reduce a **~$535,000 COP (~$133.75 USD)**, dejando cada unidad en **~$107,000 COP (~$26.75 USD)**.
