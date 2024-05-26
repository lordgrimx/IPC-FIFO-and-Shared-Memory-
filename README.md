
## README

## Language
- [Türkçe](#türkçe)
- [English](#english)

## Contents
- [Overview](#overview)
- [Files](#files)
- [Compilation](#compilation)
- [Execution](#execution)
- [Detailed Explanation](#detailed-explanation)
  - [Client (Client.cxx)](#client-clientcxx)
  - [Server (Server.cxx)](#server-servercxx)
- [Important Notes](#important-notes)
- [Example Output](#example-output)
  - [Server](#server-1)
  - [Client](#client)
- [Language](#language)

### Overview
This project demonstrates inter-process communication between a client and server using shared memory and named pipes (FIFO). The client sends a list of programs it has to the server through a named pipe, and the server checks for updates and communicates back to the client through shared memory.

### Files
- `Client.cxx`
- `Server.cxx`

### Compilation
To compile the client and server programs, use the following commands:
```bash
g++ -o client Client.cxx
g++ -o server Server.cxx
```

### Execution
1. Start the server:
    ```bash
    ./server
    ```
2. In another terminal, start the client:
    ```bash
    ./client
    ```

### Detailed Explanation

#### Client (Client.cxx)

1. **Includes and Defines**
    - Includes necessary headers for standard I/O, system calls, shared memory, and FIFO.
    - Defines constants for shared memory key and size, and the FIFO name.

2. **Structure Definition**
    - `program`: Represents a program with an ID, version, and name.

3. **Main Function Workflow**
    - Creates or gets the shared memory segment.
    - Attaches to the shared memory segment.
    - Creates a named pipe (FIFO) for communication.
    - Defines a list of programs.
    - Writes the program list to the named pipe.
    - Continuously reads update messages from shared memory and prints them until a counter reaches a set limit.
    - Detaches from the shared memory.

#### Server (Server.cxx)

1. **Includes and Defines**
    - Includes necessary headers for standard I/O, system calls, shared memory, and FIFO.
    - Defines constants for shared memory key and size, and the FIFO name.

2. **Structure Definition**
    - `program`: Represents a program with an ID, version, and name.

3. **Helper Functions**
    - `sendUpdate`: Sends update messages and download links to the shared memory.
    - `isSharedMemoryEmpty`: Checks if the shared memory is empty.
    - `cleanupSharedMemory`: Cleans up any existing shared memory segments.

4. **Server Function Workflow**
    - Cleans up any existing shared memory segments.
    - Creates a new shared memory segment.
    - Defines a list of programs.
    - Creates and opens a named pipe for reading.
    - Continuously reads the client's program list from the named pipe.
    - Checks for updates and sends update messages to the shared memory.
    - Closes and unlinks the named pipe.
    - Removes the shared memory segment.
    - Recursively restarts the server function.

5. **Main Function**
    - Prints a starting message and calls the `serverFunction`.

### Important Notes
- Ensure the server is started before the client.
- The server continuously restarts to handle new clients.
- Shared memory cleanup is handled by the server.
- The named pipe must be unique and accessible by both client and server.

### Example Output
#### Server
```
Server started. Waiting for clients...
Previous shared memory segment removed successfully.
****************************************
Received 72 bytes from client
Sending update for Program Name: CinsCalculator, Version: 3
Shared memory is empty. Waiting for next update...
```

#### Client
```
Client started. FIFO created: /tmp/myfifo
*********************************************************
Update Message: Update available for CinsCalculator.
Download Link: http://example.com/download/CinsCalculator
```

This README provides a detailed guide to compile, execute, and understand the client-server interaction using shared memory and FIFO. Adjust the shared memory size and other parameters as per your requirements.

## Türkçe
### Genel Bakış
Bu proje, bir istemci ve sunucu arasındaki iletişimi ortak bellek ve adlandırılmış borular (FIFO) kullanarak gösterir. İstemci, sahip olduğu programların bir listesini adlandırılmış boru aracılığıyla sunucuya gönderir ve sunucu güncellemeleri kontrol eder ve ortak bellek aracılığıyla istemciye geri bildirir.

### Dosyalar
- `Client.cxx`
- `Server.cxx`

### Derleme
İstemci ve sunucu programlarını derlemek için aşağıdaki komutları kullanın:
```bash
g++ -o client Client.cxx
g++ -o server Server.cxx
```

### Çalıştırma
1. Sunucuyu başlatın:
    ```bash
    ./server
    ```
2. Başka bir terminalde istemciyi başlatın:
    ```bash
    ./client
    ```

### Detaylı Açıklama

#### İstemci (Client.cxx)

1. **Dahil Edilenler ve Tanımlamalar**
    - Standart G/Ç, sistem çağrıları, ortak bellek ve FIFO için gerekli başlık dosyalarını içerir.
    - Ortak bellek anahtarı ve boyutu ile FIFO adı için sabitleri tanımlar.

2. **Yapı Tanımı**
    - `program`: Bir kimlik, sürüm ve isim ile bir programı temsil eder.

3. **Ana Fonksiyon Akışı**
    - Ortak bellek segmentini oluşturur veya alır.
    - Ortak bellek segmentine bağlanır.
    - İletişim için bir adlandırılmış boru (FIFO) oluşturur.
    - Programların bir listesini tanımlar.
    - Program listesini adlandırılmış boruya yazar.
    - Bir sayaç belirli bir sınıra ulaşana kadar ortak bellekten güncelleme mesajlarını sürekli okur ve yazdırır.
    - Ortak bellekten ayrılır.

#### Sunucu (Server.cxx)

1. **Dahil Edilenler ve Tanımlamalar**
    - Standart G/Ç, sistem çağrıları, ortak bellek ve FIFO için gerekli başlık dosyalarını içerir.
    - Ortak bellek anahtarı ve boyutu ile FIFO adı için sabitleri tanımlar.

2. **Yapı Tanımı**
    - `program`: Bir kimlik, sürüm ve isim ile bir programı temsil eder.

3. **Yardımcı Fonksiyonlar**
    - `sendUpdate`: Güncelleme mesajlarını ve indirme bağlantılarını ortak belleğe gönderir.
    - `isSharedMemoryEmpty`: Ortak belleğin boş olup olmadığını kontrol eder.
    - `cleanupSharedMemory`: Mevcut ortak bellek segmentlerini temizler.

4. **Sunucu Fonksiyonu Akışı**
    - Mevcut ortak bellek segmentlerini temizler.
    - Yeni bir ortak bellek segmenti oluşturur.
    - Programların bir listesini tanımlar.
    - Okuma için bir adlandırılmış boru oluşturur ve açar.
    - İstemcinin program listesini adlandırılmış borudan sürekli olarak okur.
    - Güncellemeleri kontrol eder ve güncelleme mesajlarını ortak belleğe gönderir.
    - Adlandırılmış boruyu kapatır ve bağlantısını keser.
    - Ortak bellek segmentini kaldırır.
    - Sunucu fonksiyonunu tekrar başlatmak için özyinelemeli olarak çağırır.

5. **Ana Fonksiyon**
    - Başlangıç mesajı yazdırır ve `serverFunction` fonksiyonunu çağırır.

### Önemli Notlar
- İstemciden önce sunucunun başlatıldığından emin olun.
- Sunucu yeni istemcileri karşılamak için sürekli olarak yeniden başlatılır.
- Ortak bellek temizliği sunucu tarafından yapılır.
- Adlandırılmış boru hem istemci hem de sunucu tarafından erişilebilir ve benzersiz olmalıdır.

### Örnek Çıktı
#### Sunucu
```
Server started. Waiting for clients...
Previous shared memory segment removed successfully.
****************************************
Received 72 bytes from client
Sending update for Program Name: CinsCalculator, Version: 3
Shared memory is empty. Waiting for next update...
```

#### İstemci
```
Client started. FIFO created: /tmp/myfifo
*********************************************************
Update Message: Update available for CinsCalculator.
Download Link: http://example.com/download/CinsCalculator
```

Bu README, ortak bellek ve FIFO kullanarak istemci-sunucu etkileşimini derlemek, çalıştırmak ve anlamak için detaylı bir kılavuz sağlar. Ortak bellek boyutu ve diğer parametreleri ihtiyaçlarınıza göre ayarlayın.
