# Tetris: Bài tập lớn môn Lập trình Nâng cao
# Tác giả

- Họ và tên: Lê Nguyễn Việt Anh

- Lớp: K69I - CS1

- Mã sinh viên: 24021365

# Giới thiệu game

- ***Tetris*** là một trò chơi xếp hình kinh điển, nơi người chơi xoay và sắp xếp các khối hình học rơi xuống để tạo thành hàng ngang hoàn chỉnh và ghi điểm. Trò chơi được phát triển bởi kỹ sư máy tính người Nga Alexey Pajitnov vào năm 1984.

- Phiên bản clone này được lập trình bằng ***C++*** sử dụng thư viện ***SDL2***.

- Ngưỡng điểm mong muốn: 9.

# 🎮 Hướng dẫn chơi

| Phím         | Tác dụng                     |
|--------------|------------------------------|
| ⬅️ / ➡️     | Di chuyển sang trái/phải    |
| ⬆️           | Xoay                     |
| ⬇️           | Soft Drop              |
| Space        | Hard Drop        |
| C            | Hold |
|Z             | Bật/tắt nhạc nền  |
|X             |Bật/tắt hiệu ứng âm thanh (SFX)|

# Các điểm nổi bật

- Game state manager để chuyển trạng thái game dựa vào input của người chơi.

- Hệ thống UI đơn giản:

![image](https://github.com/user-attachments/assets/139bc437-bbdd-45d1-9278-1f87ad40fe53)

![image](https://github.com/user-attachments/assets/b7beadec-fb13-4743-a6ad-a3e9ccc73d2a)

![image](https://github.com/user-attachments/assets/346684cd-e63e-4524-9659-9f52decc6c8f)

- Các tính năng ***Hold block***, ***Next block*** và ***Ghost block*** giúp hỗ trợ người chơi.

![image](https://github.com/user-attachments/assets/d6283a1f-9769-4233-9d4e-95094c8ef0b1)

- Thuật toán ***7-bag randomizer*** giúp giảm tính "may rủi" của game so với việc dùng hàm `random()` để sinh các khối ngẫu nhiên.

- Độ khó tăng dần khi người chơi đạt được các ngưỡng điểm cố định.

- Hệ thống âm thanh:

  + SFX khi di chuyển các khối

  + SFX khi xoay các khối
  
  + SFX khi Hard Drop
  
  + SFX khi hoàn thành các hàng
  
  + SFX khi người chơi thua cuộc


# Tài liệu tham khảo

- Tham khảo Code mẫu trên **[Lazy Foo' Productions](https://lazyfoo.net/tutorials/SDL/index.php)**.

- Tham khảo phần Logic chính của game **[Tutorial](https://www.youtube.com/watch?v=zH_omFPqMO4&t=94s)**.

- Thuật toán **[7-bag randomizer](https://viblo.asia/p/cung-thu-viet-mot-game-xep-hinh-tetris-hoan-chinh-tu-con-so-0-phan-4-random-7-bag-oOVlYbvB58W)**.

- Tham khảo **[Timestepping](https://www.youtube.com/watch?v=RaB60Ujle7o&list=PL2RPjWnJduNmXHRYwdtublIPdlqocBoLS&index=10)**.

- Mức độ sử dụng AI: Sử dụng cho mục đích tra cứu và giải thích những phần code chưa hiểu khi tìm hiểu trên các nguồn tham khảo.

- Ngoài ra, không sử dụng Code từ những nguồn không được liệt kê ở trên.

