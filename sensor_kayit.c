#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      // open, O_CREAT, O_WRONLY vb.
#include <unistd.h>     // write, read, close
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

// Sensör verilerini indeks ile birlikte temsil eden yapı
struct SensorVerisi {
    int index;
    int sicaklik;
    int nem;
};

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Kullanım: %s <veri_adedi> <dosya_adi>\n", argv[0]);
        return 1;
    }

    int veriAdedi = atoi(argv[1]);
    const char *dosyaAdi = argv[2];

    srand(time(NULL));

    struct SensorVerisi *veriler = (struct SensorVerisi *)malloc(sizeof(struct SensorVerisi) * veriAdedi);
    if (veriler == NULL) {
        perror("Bellek ayrılırken hata oluştu");
        return 1;
    }

    // Rastgele sensör verileri üret (index dahil)
    for (int i = 0; i < veriAdedi; i++) {
        veriler[i].index = i;
        veriler[i].sicaklik = rand() % (50 - (-20) + 1) + (-20);
        veriler[i].nem = rand() % (100 - 10 + 1) + 10;
    }

    int dosya = open(dosyaAdi, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (dosya < 0) {
        perror("Dosya açılamadı");
        free(veriler);
        return 1;
    }

    ssize_t yazilanBayt = write(dosya, veriler, sizeof(struct SensorVerisi) * veriAdedi);
    if (yazilanBayt < 0) {
        perror("Dosyaya yazılamadı");
        close(dosya);
        free(veriler);
        return 1;
    }

    close(dosya);
    free(veriler);

    dosya = open(dosyaAdi, O_RDONLY);
    if (dosya < 0) {
        perror("Dosya okunamadı");
        return 1;
    }

    off_t boyut = lseek(dosya, 0, SEEK_END);
    lseek(dosya, 0, SEEK_SET);

    int okunanAdet = boyut / sizeof(struct SensorVerisi);
    struct SensorVerisi *okunanVeriler = (struct SensorVerisi *)malloc(boyut);
    if (okunanVeriler == NULL) {
        perror("Bellek ayrılırken hata");
        close(dosya);
        return 1;
    }

    ssize_t okunanBayt = read(dosya, okunanVeriler, boyut);
    if (okunanBayt < 0) {
        perror("Dosyadan okuma hatası");
        free(okunanVeriler);
        close(dosya);
        return 1;
    }

    close(dosya);

    printf("\n--- Okunan Sensör Verileri ---\n");
    for (int i = 0; i < okunanAdet; i++) {
        printf("Index %d -> Sıcaklık: %d°C, Nem: %d%%\n",
               okunanVeriler[i].index,
               okunanVeriler[i].sicaklik,
               okunanVeriler[i].nem);
    }

    free(okunanVeriler);
    return 0;
} bu olabilri mi
