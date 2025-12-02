# Struktur-Data-FP
Particle Collision Simulation

Simulasi sederhana partikel 2D menggunakan C++ dan SFML.
Partikel bergerak bebas, memiliki ukuran dan warna acak, saling bertabrakan, dan memantul dari dinding.

Fitur

- Partikel dengan ukuran random
- Warna partikel beragam (random RGB)
- Gerakan dan kecepatan random
- Tabrakan elastis antar partikel
- Memantul dari batas window
- Deteksi tabrakan menggunakan brute force

Cara Compile
- g++ main.cpp -o sim -lsfml-graphics -lsfml-window -lsfml-system
- ./sim

Deskripsi Singkat

Program membuat beberapa partikel warna-warni dengan radius acak.
Partikel bergerak dan ketika bertabrakan, posisinya diperbaiki agar tidak tembus dan kecepatannya berubah seperti tabrakan fisika elastis.

Algoritma
- Brute Force untuk pengecekan tabrakan (cek semua pasangan partikel).
- Respon tabrakan menggunakan perhitungan normal dan impulse sederhana.


//  BRUTE FORCE ALGORITHM
void bruteForceCollision(std::vector<Partikel>& list) {
    for (int i = 0; i < list.size(); i++) {
        for (int j = i + 1; j < list.size(); j++) {
            if (cekTabrakan(list[i], list[j])) {
                tanganiTabrakan(list[i], list[j]);
            }
        }
    }
}
