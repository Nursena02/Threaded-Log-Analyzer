#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp> // DNN modülünü ekledik!

// ============================================================
// KUYRUK YAPısı
// Ana thread kameradan kare alır ve buraya ekler.
// Worker thread'ler buradan kare çeker ve yüzleri blurlar.
// ============================================================
struct KareKuyrugu {
    std::queue<cv::Mat> kuyruk; //kare kuyruğu
    std::mutex mtx; // kuyruğu korur
    std::condition_variable cv; // thread'leri uyutur/uyandırır
    bool bitti = false; // "artık kare gelmeyecek" sinyali
};

//// ============================================================
// SONUÇ KUYRUĞU
// Worker thread'ler işlenmiş kareleri buraya koyar.
// Ana thread buradan alıp ekranda gösterir.
// ============================================================
struct SonucKuyrugu {
    std::queue<cv::Mat> kuyruk;
    std::mutex mtx;
    std::condition_variable cv;
};
void workerThread(KareKuyrugu& isKuyrugu, SonucKuyrugu& sonucKuyrugu, 
                  const std::string& protoPath, const std::string& modelPath) {
    
    // 1. Modeli Belleğe Yükle (SSD Caffe Model)
    cv::dnn::Net net = cv::dnn::readNetFromCaffe(protoPath, modelPath);
    
    // Mac M1/M2/M3 için CPU/OpenCV optimizasyonunu seçiyoruz
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

    while (true) {
        cv::Mat frame;
        {
            // İş kuyruğundan kare gelmesini bekle
            std::unique_lock<std::mutex> lock(isKuyrugu.mtx);
            isKuyrugu.cv.wait(lock, [&] { 
                return !isKuyrugu.kuyruk.empty() || isKuyrugu.bitti; 
            });

            // Eğer kuyruk boşsa ve bitti sinyali geldiyse çık
            if (isKuyrugu.kuyruk.empty() && isKuyrugu.bitti) break;

            // Kareyi al ve kuyruktan çıkar
            frame = isKuyrugu.kuyruk.front();
            isKuyrugu.kuyruk.pop();
        } // kilit burada otomatik açılır
        // 1. Görüntüyü "Blob" formatına çevir (Modelin anlayacağı 300x300 boyutuna getirir)
        cv::Mat blob = cv::dnn::blobFromImage(frame, 1.0, cv::Size(300, 300), 
                                              cv::Scalar(104.0, 177.0, 123.0));
        
        net.setInput(blob); // Veriyi ağın girişine takıyoruz
        cv::Mat tespitler = net.forward(); // Sinir ağı çalışıyor ve tahminleri üretiyor

        // 2. Tespit sonuçlarını bir matrise döküyoruz
        cv::Mat tespitMat(tespitler.size[2], tespitler.size[3], CV_32F, tespitler.ptr<float>());

        for (int i = 0; i < tespitMat.rows; i++) {
            float guvenSkoru = tespitMat.at<float>(i, 2); // Modelin tahmin güveni

            // %50'den fazla eminse işlem yapıyoruz
            if (guvenSkoru > 0.5) {
                // Koordinatları orijinal resim boyutuna geri ölçekliyoruz
                int x1 = static_cast<int>(tespitMat.at<float>(i, 3) * frame.cols);
                int y1 = static_cast<int>(tespitMat.at<float>(i, 4) * frame.rows);
                int x2 = static_cast<int>(tespitMat.at<float>(i, 5) * frame.cols);
                int y2 = static_cast<int>(tespitMat.at<float>(i, 6) * frame.rows);

                // Görüntü sınırları dışına taşmamak için kesişim (intersection) alıyoruz
                cv::Rect yuzBolgesi(cv::Point(x1, y1), cv::Point(x2, y2));
                yuzBolgesi &= cv::Rect(0, 0, frame.cols, frame.rows);

                // Geçerli bir bölgeyse blurluyoruz
                if (yuzBolgesi.width > 0 && yuzBolgesi.height > 0) {
                    cv::GaussianBlur(frame(yuzBolgesi), frame(yuzBolgesi), cv::Size(99, 99), 30);
                }
            }
        }

        // 3. İşlenmiş kareyi sonuç kuyruğuna ekle
        {
            std::lock_guard<std::mutex> lock(sonucKuyrugu.mtx);
            sonucKuyrugu.kuyruk.push(frame);
        }
        sonucKuyrugu.cv.notify_one(); // "Görüntü hazır" mesajı gönder
    } // While döngüsü sonu
} // workerThread fonksiyonu sonu
int main() {
    // 1. Model Dosyalarının Yolları
    const std::string protoPath = "deploy.prototxt";
    const std::string modelPath = "res10_300x300_ssd_iter_140000.caffemodel";

    // 2. Kamerayı Başlat (0 = Varsayılan MacBook Kamerası)
// Bir internet videosu veya IP kamera adresi
const std::string videoUrl = "https://raw.githubusercontent.com/intel-iot-devkit/sample-videos/master/face-demographics-walking-and-pause.mp4"; 

cv::VideoCapture kamera(videoUrl);

if (!kamera.isOpened()) {
    std::cerr << "Hata: Video akisina (URL) ulasilamadi!" << std::endl;
    return -1;
}
    // 3. Thread Sayısını Belirle (İşlemci çekirdek sayısına göre)
    unsigned int threadSayisi = std::thread::hardware_concurrency();
    if (threadSayisi == 0) threadSayisi = 2; // Güvenli bir alt sınır
    std::cout << "Calisan Thread Sayisi: " << threadSayisi << std::endl;

    // Kuyrukları oluştur
    KareKuyrugu isKuyrugu;
    SonucKuyrugu sonucKuyrugu;

    // 4. Worker Thread'leri Başlat (Havuz oluşturma)
    std::vector<std::thread> threadHavuzu;
    for (unsigned int i = 0; i < threadSayisi; i++) {
        threadHavuzu.push_back(std::thread(workerThread, 
                                           std::ref(isKuyrugu), 
                                           std::ref(sonucKuyrugu), 
                                           protoPath, 
                                           modelPath));
    }
    std::cout << "Cikis icin 'q' tusuna basin." << std::endl;

while (true) {
        cv::Mat hamKare;
        kamera >> hamKare; 
        if (hamKare.empty()) break;

        // 1. Kareyi iş kuyruğuna gönder (Kuyruğu temizleyerek)
        {
            std::lock_guard<std::mutex> lock(isKuyrugu.mtx);
            while(!isKuyrugu.kuyruk.empty()) isKuyrugu.kuyruk.pop();
            isKuyrugu.kuyruk.push(hamKare.clone());
        }
        isKuyrugu.cv.notify_one();

        // 2. KRİTİK NOKTA: İşlenmiş kare gelene kadar bu thread'i kısa bir süre beklet
        // Bu sayede ekrana "ham" kare sızmasını engelliyoruz.
        cv::Mat islenmişSonuc;
        bool yeniKareVar = false;

        {
            std::unique_lock<std::mutex> lock(sonucKuyrugu.mtx);
            // Eğer sonuç kuyruğu boşsa, işçinin bitirmesi için çok kısa bekle (max 30ms)
            if (sonucKuyrugu.cv.wait_for(lock, std::chrono::milliseconds(30), 
                [&] { return !sonucKuyrugu.kuyruk.empty(); })) {
                
                islenmişSonuc = sonucKuyrugu.kuyruk.front();
                sonucKuyrugu.kuyruk.pop();
                yeniKareVar = true;
            }
        }

        // 3. Sadece işlenmiş ve yüzü bulunmuş kareyi göster
        if (yeniKareVar && !islenmişSonuc.empty()) {
            cv::imshow("Canli Blurlanmis Video", islenmişSonuc);
        }

        if (cv::waitKey(1) == 'q') break;
    }
    // Bitti sinyali gönder
    {
        std::lock_guard<std::mutex> lock(isKuyrugu.mtx);
        isKuyrugu.bitti = true;
    }
    isKuyrugu.cv.notify_all();

    // Tüm thread'lerin güvenle kapanmasını bekle (Join)
    for (auto& t : threadHavuzu) {
        if (t.joinable()) t.join();
    }

    kamera.release();
    cv::destroyAllWindows();
    return 0;
}