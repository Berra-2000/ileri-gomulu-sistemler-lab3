# İleri Gömülü Sistemler – Lab 3

## ESP32 ile UART, SPI ve I²C Haberleşme Protokollerinin İncelenmesi

Bu GitHub deposu, İleri Gömülü Sistemler dersi kapsamında ESP32 geliştirme kartı kullanılarak gerçekleştirilen UART, SPI ve I²C haberleşme deneylerine ait kaynak kodları, yapılandırma dosyaları ve deney kayıtlarını içermektedir.

Çalışmanın amacı, gömülü sistemlerde yaygın kullanılan haberleşme protokollerinin çalışma prensiplerini uygulamalı olarak öğrenmek, veri iletimini test etmek ve donanım-yazılım etkileşimini incelemektir.

Deneylerde ESP-IDF geliştirme ortamı kullanılmıştır. Çalışma sürecinde OpenAI tarafından geliştirilen ChatGPT (GPT-6) aracından teknik açıklama, hata ayıklama ve öğrenme desteği alınmıştır.

## 1. UART Haberleşme Deneyi

UART (Universal Asynchronous Receiver/Transmitter), cihazlar arasında asenkron seri veri iletişimi sağlayan bir haberleşme yöntemidir.

Bu deneyde ESP32 üzerinde UART loopback bağlantısı kullanılarak gönderilen verilerin geri alınması test edilmiştir.

**Gerçekleştirilen işlemler:**

- GPIO17 ve GPIO18 pinleri üzerinden UART loopback bağlantısının denenmesi.
- Seri veri gönderme ve alma işlemlerinin uygulanması.
- Gönderilen ve alınan verilerin karşılaştırılması.
- Farklı baud rate ayarlarının haberleşme üzerindeki etkisinin incelenmesi.

Deneyin kaynak kodları ve ilgili dosyaları `lab3_uart/` klasöründe bulunmaktadır.

## 2. SPI Haberleşme Deneyi

SPI (Serial Peripheral Interface), senkron seri haberleşme sağlayan ve veri aktarımında saat sinyalinden yararlanan bir protokoldür.

Bu deneyde ESP32 üzerinde MOSI ve MISO hatları arasında loopback bağlantısı kullanılarak veri iletimi test edilmiştir.

**Gerçekleştirilen işlemler:**

- MOSI ve MISO hatlarının loopback bağlantısının kurulması.
- Dört baytlık veri gönderme ve alma işlemlerinin uygulanması.
- Gönderilen ve alınan verilerin karşılaştırılması.
- SPI Mode 0 ve Mode 3 yapılandırmalarının denenmesi.
- Deney sonuçlarının kayıt altına alınması.

Deneyin kaynak kodları `lab3_spi/` klasöründe, ölçüm notları ise `spi_olcum_sonuclari.txt` dosyasında bulunmaktadır.

## 3. I²C Haberleşme Deneyi – MPU6050

I²C (Inter-Integrated Circuit), SDA veri hattı ve SCL saat hattı üzerinden haberleşme sağlayan senkron bir iletişim protokolüdür.

Bu deneyde ESP32 geliştirme kartı ile GY-521 MPU6050 sensör modülü arasında I²C haberleşmesi gerçekleştirilmiştir.

### Donanım Bağlantıları

| MPU6050 | ESP32 |
|---|---|
| VCC | 3V3 |
| GND | GND |
| SDA | GPIO21 |
| SCL | GPIO22 |

I²C haberleşme frekansı 400 kHz olarak kullanılmıştır.

### Gerçekleştirilen İşlemler

- ESP32 ile MPU6050 arasında I²C bağlantısının kurulması.
- Sensörün `WHO_AM_I` yazmacının (`0x75`) okunması.
- Deney sırasında `0x72` değerinin gözlemlenmesi.
- `PWR_MGMT_1` yazmacına (`0x6B`) `0x00` değeri yazılarak sensörün uyandırılması.
- X, Y ve Z eksenlerine ait ivme verilerinin okunması.
- Sensör hareket ettirildiğinde ivme verilerindeki değişimlerin gözlemlenmesi.
- Yaklaşık 100 milisaniye aralıklarla sensör verilerinin alınması.

**Deney gözlemi:** Sensörün fiziksel konumu değiştirildiğinde X, Y ve Z eksenlerinden okunan değerlerin de değiştiği görülmüştür.

`WHO_AM_I` yazmacından okunan `0x72` değeri deneyde elde edilen gözlemdir; sensör kimliğinin kesin doğrulaması olarak değerlendirilmemiştir.

Deneyin kaynak kodları ve yapılandırma dosyaları `lab3_i2c/` klasöründe bulunmaktadır.

## 4. Kullanılan Donanım ve Yazılımlar

**Donanım:**

- ESP32 DEVKIT V1 geliştirme kartı
- GY-521 MPU6050 sensör modülü
- USB bağlantı kablosu
- Breadboard ve jumper kablolar

**Yazılım ve geliştirme ortamı:**

- macOS
- ESP-IDF 6.1
- C programlama dili
- GitHub
- ChatGPT (GPT-6) – eğitim ve teknik destek amacıyla

## 5. GitHub Proje Yapısı

```text
ileri-gomulu-sistemler-lab3/
│
├── lab3_uart/
│   └── UART deney dosyaları
│
├── lab3_spi/
│   ├── main/
│   ├── CMakeLists.txt
│   ├── README.md
│   ├── sdkconfig
│   └── spi_olcum_sonuclari.txt
│
├── lab3_i2c/
│   ├── main/
│   │   ├── i2c_basic_example_main.c
│   │   ├── CMakeLists.txt
│   │   └── Kconfig.projbuild
│   ├── CMakeLists.txt
│   ├── README.md
│   └── sdkconfig
│
└── README.md
```

## 6. Sonuç ve Değerlendirme

Bu laboratuvar çalışmasında ESP32 üzerinde UART, SPI ve I²C haberleşme protokolleri uygulamalı olarak incelenmiştir.

UART ve SPI deneylerinde loopback yöntemiyle veri gönderme ve alma işlemleri denenmiştir. I²C deneyinde ise MPU6050 sensöründen ivme verileri okunmuş ve sensör hareket ettirildiğinde değerlerin değiştiği gözlemlenmiştir.

Bu çalışmalar sayesinde seri haberleşme protokolleri, ESP-IDF ile gömülü yazılım geliştirme, GPIO yapılandırmaları, sensör haberleşmesi ve hata ayıklama konularında uygulamalı deneyim kazanılmıştır.

I²C deneyinde oluşturulan ESP32–MPU6050 haberleşme altyapısı, ilerleyen aşamada geliştirilecek eğim algılama ve alarm sistemi için de temel oluşturmaktadır.

Bu depoda mantık analizörüyle elde edilmiş ölçüm kayıtları sunulmamaktadır.

## 7. Yapay Zekâ Kullanım Beyanı

Bu laboratuvar çalışmasının hazırlanması sırasında OpenAI tarafından geliştirilen **ChatGPT (GPT-6)** yapay zekâ aracından eğitim, teknik danışmanlık ve dokümantasyon desteği amacıyla yararlanılmıştır.

Yapay zekâ desteği aşağıdaki alanlarda kullanılmıştır:

- UART, SPI ve I²C haberleşme protokollerinin teorik olarak açıklanması.
- ESP32 ve ESP-IDF geliştirme ortamının anlaşılması.
- C programlama kodlarının açıklanması ve hata ayıklama sürecinde öneriler sunulması.
- MPU6050 sensörüyle haberleşme sürecindeki teknik sorunların değerlendirilmesi.
- Deneylerin aşamalara ayrılması ve uygulama sürecinin planlanması.
- GitHub dosya ve klasör yapısının düzenlenmesi.
- README dokümantasyonunun hazırlanması ve akademik anlatımın iyileştirilmesi.

**Deneylerin fiziksel uygulanması, donanım bağlantılarının gerçekleştirilmesi, kodların çalıştırılması, çıktılarının gözlemlenmesi ve sonuçlar değerlendirilmiştir.**

ChatGPT, çalışmanın bağımsız yazarı veya deneyleri gerçekleştiren tarafı değildir. Yapay zekâ tarafından sağlanan teknik öneriler, uygulama sürecinde test edilerek değerlendirilmiştir.

## 8. Genel Not

Bu depo, İleri Gömülü Sistemler dersi kapsamında eğitim ve akademik çalışma amacıyla hazırlanmıştır. Deney sonuçları kullanılan donanım, yazılım sürümü ve yapılandırmaya bağlı olarak değişiklik gösterebilir.
