# Collision Simulation dengan SFML (Brute Force vs Quadtree)

Program ini adalah simulasi tabrakan ratusan hingga ribuan partikel menggunakan **SFML**.
Simulasi ini menampilkan dua mode pendeteksian tabrakan:

1. **Brute Force Collision** – membandingkan setiap partikel dengan semua partikel lainnya (kompleksitas O(n²)).
2. **Quadtree Collision** – menggunakan struktur data Quadtree untuk mempercepat pendeteksian tabrakan (mendekati O(n log n)).

Pengguna dapat mengganti mode **secara real-time** dengan menekan tombol **Q**.

---

## 🎮 Fitur Utama

* Simulasi pergerakan partikel dengan kecepatan acak.
* Tabrakan elastis sederhana antar partikel.
* Pantulan dari dinding jendela simulasi.
* Perbandingan real-time antara metode **Brute Force** dan **Quadtree**.
* Mampu menangani hingga ribuan partikel (contoh: 4000).

---

## 🔧 Teknologi yang Digunakan

* **C++**
* **SFML (Simple and Fast Multimedia Library)**
* Struktur data **Quadtree**

---

## 📂 Struktur Kode Utama

* `Partikel`
  Struktur untuk menyimpan posisi, kecepatan, radius, dan bentuk SFML.

* `cekTabrakan()`
  Mengecek jarak antar partikel untuk menentukan tabrakan.

* `tanganiTabrakan()`
  Menangani tabrakan elastis dengan perhitungan momentum sederhana.

* `bruteForceCollision()`
  Melakukan pengecekan tabrakan dengan cara membandingkan semua pasangan.

* `Quadtree`
  Struktur data hierarkis untuk membagi ruang menjadi empat bagian agar pencarian objek terdekat lebih efisien.

* `quadtreeCollision()`
  Mengambil kandidat tabrakan dari Quadtree sebelum memeriksa tabrakan.

* Input Keyboard

  * **Q** = toggle mode BruteForce / Quadtree

---

## ▶️ Cara Menjalankan

1. Pastikan SFML sudah terinstall.
   Install di Linux (contoh Ubuntu):

   ```
   sudo apt-get install libsfml-dev
   ```

   Atau gunakan SFML dari [https://www.sfml-dev.org/](https://www.sfml-dev.org/)

2. Kompilasi dengan command:

   ```
   g++ main.cpp -o collision -lsfml-graphics -lsfml-window -lsfml-system
   ```

3. Jalankan:

   ```
   ./collision
   ```

4. Tekan **Q** untuk mengubah mode deteksi tabrakan.

---

## 📊 Perbandingan Mode

| Mode        | Kompleksitas | Kelebihan                 | Kekurangan                   |
| ----------- | ------------ | ------------------------- | ---------------------------- |
| Brute Force | O(n²)        | Sederhana, mudah dipahami | Sangat berat di jumlah besar |
| Quadtree    | O(n log n)   | Lebih cepat & scalable    | Implementasi lebih kompleks  |

---

## 📝 Catatan

* Font yang digunakan: **arial.ttf**
  Pastikan file tersebut berada dalam direktori yang sama dengan executable.
* Jumlah partikel dapat diubah pada bagian:

  ```cpp
  for (int i = 0; i < 4000; i++)
  ```

---

## 📸 Preview (Konsep)

Simulasi akan menampilkan titik-titik berwarna yang bergerak dan saling bertabrakan.
Status mode ditampilkan di kiri atas jendela.

---

## 📃 Lisensi

Silakan gunakan, modifikasi, dan kembangkan kode ini bebas sesuai kebutuhan.
