# 🚀 Multithreaded Performance Lab (C++)

Bu depo, C++ dilinde paralel programlama ve multithreading mimarileri üzerine geliştirdiğim projeleri içermektedir. Projeler, metin tabanlı veri analizinden gerçek zamanlı görüntü işlemeye kadar geniş bir yelpazeyi kapsar.

---

## 📸 1. Real-Time Face Blurring (SSD + Multithreading)

Bu proje, **OpenCV DNN** modülünü ve **Producer-Consumer** (Üretici-Tüketici) tasarım desenini kullanarak videolardaki yüzleri tespit eder ve gerçek zamanlı olarak bulanıklaştırır.

### 🛠 Teknik Özellikler
- **Model:** Caffe tabanlı ResNet-10 SSD (Single Shot MultiBox Detector).
- **Mimari:** Görüntü yakalama (Main Thread) ve işleme (Worker Threads) süreçleri `std::condition_variable` ve `std::mutex` ile senkronize edilmiştir.
- **Optimizasyon:** MacBook M-serisi (8-thread) üzerinde sıfır gecikme (**Zero Latency**) sağlamak için "Latest Frame Only" kuyruk yönetimi uygulanmıştır.

### 🚀 Nasıl Çalıştırılır?
```bash
g++ -std=c++17 Face-Blurring/video_processor.cpp -o blur_exe `pkg-config --cflags --libs opencv4`
./blur_exe


Gemini şunu dedi:
Görünüşe göre GitHub, yapıştırdığın metni "Markdown" formatı yerine düz yazı olarak algılamış ve her şeyi birbirine katmış. Markdown dosyasında başlıklar (#), listeler (-) ve kod blokları arasında mutlaka boş satır olması gerekir. Aksi takdirde böyle sıkışık görünür.

Hadi şu README.md dosyasını terminalden veya GitHub arayüzünden şu boşluklara dikkat ederek tekrar güncelleyelim.

Doğru Markdown Formatı (Kopyala ve Yapıştır):
Markdown
# 🚀 Multithreaded Performance Lab (C++)

Bu depo, C++ dilinde paralel programlama ve multithreading mimarileri üzerine geliştirdiğim projeleri içermektedir. Projeler, metin tabanlı veri analizinden gerçek zamanlı görüntü işlemeye kadar geniş bir yelpazeyi kapsar.

---

## 📸 1. Real-Time Face Blurring (SSD + Multithreading)

Bu proje, **OpenCV DNN** modülünü ve **Producer-Consumer** (Üretici-Tüketici) tasarım desenini kullanarak videolardaki yüzleri tespit eder ve gerçek zamanlı olarak bulanıklaştırır.

### 🛠 Teknik Özellikler
- **Model:** Caffe tabanlı ResNet-10 SSD (Single Shot MultiBox Detector).
- **Mimari:** Görüntü yakalama (Main Thread) ve işleme (Worker Threads) süreçleri `std::condition_variable` ve `std::mutex` ile senkronize edilmiştir.
- **Optimizasyon:** MacBook M-serisi (8-thread) üzerinde sıfır gecikme (**Zero Latency**) sağlamak için "Latest Frame Only" kuyruk yönetimi uygulanmıştır.

### 🚀 Nasıl Çalıştırılır?
```bash
g++ -std=c++17 Face-Blurring/video_processor.cpp -o blur_exe `pkg-config --cflags --libs opencv4`
./blur_exe

📊 2. Multi-Threaded Log Analyzer
Büyük ölçekli sistem log dosyalarını (Hadoop_2k.log gibi) analiz ederek performans farklarını karşılaştırır.

🛠 Teknik Özellikler
Kütüphaneler: <thread>, <mutex>, <chrono>.

Güvenlik: std::lock_guard kullanılarak thread-safe sayaç yönetimi yapılmıştır.
Yöntem,Analiz Süresi
Single-Thread,~0.002s
Multi-Thread,~0.001s

Gemini şunu dedi:
Görünüşe göre GitHub, yapıştırdığın metni "Markdown" formatı yerine düz yazı olarak algılamış ve her şeyi birbirine katmış. Markdown dosyasında başlıklar (#), listeler (-) ve kod blokları arasında mutlaka boş satır olması gerekir. Aksi takdirde böyle sıkışık görünür.

Hadi şu README.md dosyasını terminalden veya GitHub arayüzünden şu boşluklara dikkat ederek tekrar güncelleyelim.

Doğru Markdown Formatı (Kopyala ve Yapıştır):
Markdown
# 🚀 Multithreaded Performance Lab (C++)

Bu depo, C++ dilinde paralel programlama ve multithreading mimarileri üzerine geliştirdiğim projeleri içermektedir. Projeler, metin tabanlı veri analizinden gerçek zamanlı görüntü işlemeye kadar geniş bir yelpazeyi kapsar.

---

## 📸 1. Real-Time Face Blurring (SSD + Multithreading)

Bu proje, **OpenCV DNN** modülünü ve **Producer-Consumer** (Üretici-Tüketici) tasarım desenini kullanarak videolardaki yüzleri tespit eder ve gerçek zamanlı olarak bulanıklaştırır.

### 🛠 Teknik Özellikler
- **Model:** Caffe tabanlı ResNet-10 SSD (Single Shot MultiBox Detector).
- **Mimari:** Görüntü yakalama (Main Thread) ve işleme (Worker Threads) süreçleri `std::condition_variable` ve `std::mutex` ile senkronize edilmiştir.
- **Optimizasyon:** MacBook M-serisi (8-thread) üzerinde sıfır gecikme (**Zero Latency**) sağlamak için "Latest Frame Only" kuyruk yönetimi uygulanmıştır.

### 🚀 Nasıl Çalıştırılır?
```bash
g++ -std=c++17 Face-Blurring/video_processor.cpp -o blur_exe `pkg-config --cflags --libs opencv4`
./blur_exe
📊 2. Multi-Threaded Log Analyzer
Büyük ölçekli sistem log dosyalarını (Hadoop_2k.log gibi) analiz ederek performans farklarını karşılaştırır.

🛠 Teknik Özellikler
Kütüphaneler: <thread>, <mutex>, <chrono>.

Güvenlik: std::lock_guard kullanılarak thread-safe sayaç yönetimi yapılmıştır.

📉 Performans Verileri
Yöntem	Analiz Süresi
Single-Thread	~0.002s
Multi-Thread	~0.001s

🎓 Akademik Bağlam
Bu projeler, Konya Teknik Üniversitesi bünyesinde yürüttüğüm Veri Bilimi ve Yapay Zeka yüksek lisans çalışmalarımdaki "Paralel Programlama" ve "Görüntü İşleme" konularına pratik bir yaklaşım sunmaktadır.

