# SEKURO 18 TUGAS 2 DIVISI PROGRAMMING

## INTRO
Halo, Kak!!! Perkenalkan, berikut ini identitas saya (selaku *engineer* yang disewa oleh Vanir untuk membantu party Kazuma menyelesaikan proyek titah Raja ~~wkakwaowk~~):
- Nama: Abraham Bagunna Adiwijaya;
- NIM: 13325090;
- Program Studi: Teknik Fisika '25;
- Departemen: Programming 
- Environment: Dual Boot Ubuntu

Terima kasih sudah meluangkan waktu untuk meninjau tugas saya ^_^. Semoga hasilnya tidak mengecewakan hehe...

## DESKRIPSI PROGRAM
Setelah insiden ledakan di kediaman bangsawan Alderp, saya menerima permintaan khusus dari Vanir untuk merancang sistem kendali dasar bagi *Mobile Fortress Destroyer Mark II*. Sebagai langkah awal, saya telah mengembangkan arsitektur program menggunakan ROS 2 yang mengedepankan efisiensi dan keamanan operasional agar kejadian "robot mengamuk" tidak terulang kembali.

### LOGIKA KENDALI
Inti dari program ini adalah implementasi *Inverse Kinematics* sederhana yang menerjemahkan instruksi kecepatan menjadi orientasi gerak fisik robot. Agar insiden "salah target" di masa lalu tidak terulang, sistem ini dirancang dengan dua mode operasi utama yang bekerja secara harmonis:

- Mode Patroli (Autonomous): Node auto_mode_destroyer secara otomatis mengirimkan instruksi gerak acak dengan frekuensi 2Hz. Mode ini digunakan untuk menjaga area perbatasan secara mandiri tanpa membebani operator.

- Mode Intervensi (Driver): Melalui node drive_mode_destroyer, operator (pahlawan) dapat mengambil alih kendali secara manual menggunakan keyboard untuk situasi darurat yang membutuhkan reaksi cepat.

### ARSITEKTUR
Untuk menjamin efisiensi, program ini memiliki node check_move_destroyer sebagai "pengambil keputusan" (saya tidak tahu title yang bagus untuk mendeskripsikan node ini ;p). Tiap kali ada input manual dari operator, sistem akan secara otomatis memprioritaskan instruksi tersebut dan mengesampingkan mode auto. Seluruh data keputusan ini kemudian diteruskan ke topic /power_destroyer dan /move_type untuk dieksekusi oleh unit penggerak kaki pada node move_destroyer. Dengan begitu, rancangan Destroyer II akan lebih *human-centered design* daripada yang sebelumnya. Seluruh proses pengembangan robot ini juga memperhatikan coding standard terbaik, termasuk penghindaran global namespace demi integritas sistem jangka panjang.

### ALUR KOMUNIKASI
1. Publishers:
   - auto_mode_destroyer : Mengirim data pergerakan auto melalui topic /autonomous_destroyer.
   - drive_mode_destroyer : Mengirimkan instruksi kendali manual dari operator melalui topic /drive_destroyer.

2. Arbiter:
   - check_move_destroyer: Bertindak sebagai pengambil keputusan. Berlangganan (subscribe) ke kedua topic input dan memutuskan data apa yang harus diteruskan berdasarkan sistem prioritas (driver > autonomous). Output-nya kemudian dikirim ke topic /power_destroyer (untuk data fisik gerak) dan /move_type (untuk status mode aktif).

3. Subscriber:
   - move_destroyer : Menerima instruksi final untuk mengalkulasi *Inverse Kinematics* dan menampilkan log gerak robot pada sistem.
  
## DEPENDENCIES
Sebelum memulai instalasi, pastikan environment sudah dikonfigurasi dengan spesifikasi:
- Sistem Operasi: Linux Ubuntu versi 22.04.
- Framework: ROS 2 Humble.
- Middleware (RMW) Eclipse CycloneDDS (untuk *discovery node* di environment *multi-user*).
- Bahasa Pemrograman: C++17 standar OOP.
- Build System: colcon dan CMake.
- Library: rclcpp, geometry_msgs (untuk tipe data *Twist*), dan std_msgs (untuk tipe data *String*).

## HOW TO RUN
### BAGIAN II (Single-User)
Sebelum menjalankan sistem, kita harus memastikan terminal berada di root direktori *workspace* (tugas-d2-sekuro18-13325090) untuk melakukan kompilasi.

```bash
# Masuk ke direktori utama (dalam kasus saya, folder tugas-d2-sekuro18-13325090 berda di dalam Documents)
cd ~/Documents/tugas-d2-sekuro18-13325090

# Kemudian, build package
colcon build --packages-select destroyer

# Terakhir, sourcing agar environment ROS 2 mengenali package
source install/setup.bash
```

Setelah tahap *build* selesai, jalankan node destroyer move_destroyer.
1. Terminal 1:
```bash
ros2 run destroyer move_destroyer
```

Kemudian, buat tiga terminal baru (jangan tutup terminal sebelumnya). Tiap terminal baru wajib dilakukan cd dan source agar tidak terjadi error "Package not found". Setelah itu, jalankan node check_move_destroyer di terminal 2, auto_mode_destroyer di terminal 3, dan drive_mode_destroyer di terminal 4.

2. Terminal 2:
```bash
cd ~/Documents/tugas-d2-sekuro18-13325090 && source install/setup.bash && ros2 run destroyer check_move_destroyer
```

3. Terminal 3:
```bash
cd ~/Documents/tugas-d2-sekuro18-13325090 && source install/setup.bash && ros2 run destroyer auto_mode_destroyer
```

4. Terminal 4:
```bash
cd ~/Documents/tugas-d2-sekuro18-13325090 && source install/setup.bash && ros2 run destroyer drive_mode_destroyer
```

Untuk memastikan program berjalan dengan benar, perhatikan: 

- Mode Autonomous: Saat hanya Terminal 1, 2, dan 3 yang berjalan, log pada move_destroyer harus menampilkan status Tipe Kendali Aktif: Autonomous. 

- Sistem Prioritas (Driver Interruption): Begitu menekan tombol kendali pada Terminal 4, status kendali harus segera berubah menjadi Driver. Sistem akan memprioritaskan input manual dan mengabaikan instruksi acak dari mode autonomous.

### BAGIAN III (Multi-User)
Sesuai standar kerajaan untuk memastikan sistem Destroyer II tidak bisa diretas dengan mudah, maka dilakukan simulasi multi-user. Pengujian ini membuktikan bahwa node eksekutor di cakru18 (user simulasi) dapat menerima data yang presisi dari pusat komando (user utama dalam kasus saya: venti) di dalam jaringan yang sama.

Buka terminal baru dan masuk ke user cakru18 menggunakan SSH:
```bash
ssh cakru18@localhost
```

Salin seluruh direktori proyek ke dalam environment cakru18:
```bash
cp -r /home/venti/Document/tugas-d2-sekuro18-13325090 /home/cakru18/
```

Lakukan build dan source ulang di dalam user cakru18 untuk memastikan path environment sudah sesuai dengan user tersebut. Kemudian, aktifkan middleware CycloneDDS (untuk sourcing dan pengaktifan middleware CycloneDDS, berlaku dan diterapkan di semua terminal).
```bash
# Sebagai cakru18, masuk ke direktori tugas-d2-sekuro18-13325090
cd ~/tugas-d2-sekuro18-13325090

# Build package di cakru18
colcon build --packages-select destroyer

# Sourcing
source install/setup.bash

# Set middleware ke CycloneDDS
export RMW_IMPLEMENTATION=rmw_cyclonedds_cpp
```

Jalankan node move_destroyer di terminal ini (dengan user cakru18):
1. Terminal user cakru18:
```bash
ros2 run destroyer move_destroyer
```

Setelah itu, siapkan node-node publishernya. Buka tiga terminal baru di luar SSH (jangan tutup terminal cakru18 sebelumnya) dan masuk ke user utama:
```bash
# Masuk ke user utama (lakukan di ketiga terminal baru)
su - venti
```

Kemudian, jalankan node-node publisher di user utama melalui ketiga terminal baru ini:

2. Terminal 1 (user utama):
```bash
cd ~/Documents/tugas-d2-sekuro18-13325090
export RMW_IMPLEMENTATION=rmw_cyclonedds_cpp
source install/setup.bash

ros2 run destroyer check_move_destroyer
```

3. Terminal 2 (user utama):
```bash
cd ~/Documents/tugas-d2-sekuro18-13325090
export RMW_IMPLEMENTATION=rmw_cyclonedds_cpp
source install/setup.bash

ros2 run destroyer auto_mode_destroyer
```
4. Terminal 3 (user utama):
```bash
cd ~/Documents/tugas-d2-sekuro18-13325090
export RMW_IMPLEMENTATION=rmw_cyclonedds_cpp
source install/setup.bash

ros2 run destroyer drive_mode_destroyer
```

## OUTRO
Sebagai penutup, saya menyerahkan base program Destroyer II ini ke hadapan Kerajaan dan *party* Kazuma. Saya hanya bisa berdoa semoga dengan berjalannya sistem auto dan kendali manual ini, Destroyer II tidak tiba-tiba mengamuk dan justru menambah daftar utang Kazuma ke bangsawan Alderp ~~LMAO~~.

(Catatan ekstra: Maaf, Kak... saya tidak sanggup mengerjakan spek bonus di tengah huru-hara minggu UTS ini :")
