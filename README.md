# 🎯 BÁO CÁO ĐỒ ÁN: TỐI ƯU HÓA THUẬT TOÁN SẮP XẾP (BENCHMARK SORT)

> **Mục tiêu cốt lõi:** Không chỉ dừng ở tính đúng đắn của logic, đồ án này là một nghiên cứu thực chiến về tối ưu hóa phần cứng (Hardware-aware), ép xung hiệu năng CPU (Cache Locality, Zero Copy, Pointer Array) để bứt phá giới hạn `O(N log N)` trước các luồng dữ liệu cực đoan.

**THÔNG TIN**
* **Môn học:** Cấu trúc dữ liệu và Giải thuật 
* **Thời gian:** Học kì 2 - Năm học 2025-2026
* **Sinh viên đại diện:** Lê Quang Quốc Tuấn
* **Mã số sinh viên:** 25120461
* **Lớp:** 25CTT6
  
**Thành viên nhóm:**
1. **Lê Quang Quốc Tuấn** - MSSV: 25120461
2. **Nguyễn Đình Nhân** - MSSV: 25120402
3. **Nguyễn Quang Trường** - MSSV: 25120460
---

## 🎯 PHẦN 1: CHIẾN LƯỢC NỀN TẢNG (BENCHMARK 1)

Ở lần chạy đầu tiên, chiến lược cốt lõi là từ bỏ hoàn toàn tư duy "sắp xếp dựa trên so sánh" thông thường đối với chuỗi, thay vào đó là phân phối chuyên biệt cho từng kiểu dữ liệu.

### Bài A (Integer Sort) — In-place QuickSort
* **Cơ chế:** Thuật toán QuickSort cổ điển thao tác trực tiếp trên mảng gốc.
* **Bản chất tối ưu:** Điểm chốt (Pivot) được khởi tạo tại vị trí giữa mảng `(l + r) / 2`. Việc này giúp thuật toán miễn nhiễm với các test case mảng đã sắp xếp sẵn (tăng dần/giảm dần), tận dụng tối đa tốc độ trên dữ liệu phân bố đều mà không tốn thêm bộ nhớ phụ.

### Bài B (Lexicographical String Sort) — LSD Radix Sort
* **Cơ chế:** Duyệt từ ký tự cuối của chuỗi dài nhất lùi ngược về đầu, dùng mảng đếm tần suất.
* **Bản chất tối ưu:** Sử dụng kỹ thuật *Zero-Copy* thông qua lệnh `move(a[i])`. Thay vì sao chép toàn bộ chuỗi $O(L)$, hệ thống chỉ chuyển giao quyền sở hữu vùng nhớ. Mảng đếm tĩnh `cnt[27]` tận dụng bộ nhớ Cache L1 cực nhanh của CPU.

### Bài C (Length-Lexicographical Sort) — Bucket Sort + LSD Radix Sort
* **Cơ chế:** Dùng mảng `vector<vector<string>> bucket(101)` để nhóm chuỗi theo độ dài.
* **Bản chất tối ưu:** Giải quyết quy luật "ưu tiên độ dài" chỉ trong một vòng quét tuyến tính. Mọi chuỗi rơi vào cùng một bucket chắc chắn có chung chiều dài, triệt tiêu hoàn toàn các vòng lặp đệm ký tự ảo của hàm Radix Sort ở bước sau.

---

## 🧪 PHẦN 2: THIẾT KẾ TEST CASE VÀ MỤC TIÊU ĐỐI KHÁNG

Bộ sinh test case không kiểm tra tính đúng đắn, mà đóng vai trò như các cuộc tấn công nhằm kích hoạt lỗi tràn RAM, TLE hoặc phá vỡ cấu trúc Pipeline của phần cứng.

### 1. Bài A (Integer Sort)
* **Bẫy Overflow:** Mảng chứa `INT_MIN` xen kẽ `INT_MAX`. Test này đánh trực diện vào dòng `(l + r) / 2` của QuickSort Lần 1, gây tràn số nguyên, tạo ra index âm và dẫn đến Runtime Error.
* **Test mảng ngược / Răng cưa:** Gây nhiễu vị trí Pivot cố định, làm tăng số lần đệ quy, ép QuickSort thoái hóa về O(N^2) và dính Time Limit Exceeded (TLE).

### 2. Bài B (Lexicographical String Sort)
* **Khóa chết vòng lặp (TLE):** 1 chuỗi dài 900.000 ký tự và 99.999 chuỗi dài 1 ký tự. Đánh sập LSD Radix Sort vì thuật toán bị ép chạy 900.000 vòng lặp vô nghĩa cho toàn bộ phần tử, tạo ra hàng tỷ phép tính thừa.
* **Mảng tiền tố chung (Long Prefix):** Mảng chuỗi giống hệt nhau ở 99 ký tự đầu. Kéo sập tốc độ của các thuật toán đọ chuỗi thông thường.

### 3. Bài C (Length-Lexicographical Sort)
* **Tràn bộ nhớ (Segmentation Fault):** Chuỗi dài ngẫu nhiên vượt mức 100 ký tự. Đánh trực diện vào khai báo tĩnh `bucket(101)` ở Lần 1, gây lỗi truy cập vượt quá giới hạn mảng (Out of Bounds).

---

## 🛡️ PHẦN 3: TIẾN HÓA THUẬT TOÁN CỰC HẠN (BENCHMARK 2)

Từ sự suy biến bộc lộ qua bộ test sát thủ, cấu trúc Lần 1 được dỡ bỏ và tái thiết kế hoàn toàn để chạm đến giới hạn vật lý của kiến trúc Máy tính (Computer Architecture).

### Cải tiến Bài A: Radix Sort Base-256 tích hợp Bitwise XOR
* **Bản chất kỹ thuật:** Loại bỏ QuickSort để chấm dứt lỗi tràn số. Cắt số nguyên 32-bit thành 4 khối 8-bit và dùng phép dịch bit `>>` kết hợp `& 255`. Các toán tử này chạy trực tiếp ở bóng bán dẫn, tốn đúng **1 chu kỳ máy**. Đặc biệt, xử lý dải số âm bằng phép `byte ^= 128` (đảo bit dấu) giúp phân bổ chính xác số âm/dương mà không cần chia tách mảng.

```cpp
// Minh họa xử lý bit dấu an toàn bằng phép XOR
int byte = (x >> shift) & 255;
if (shift == 24) byte ^= 128; // Đảo bit dấu để ép số âm xuống dưới số dương
cnt[byte]++;
```

### Cải tiến Bài B: 3-Way Radix QuickSort (Multikey QuickSort)
* **Bản chất kỹ thuật:** Để khắc phục nhược điểm duyệt vô ích của LSD, thuật toán chuyển sang Multikey QuickSort. Nó phân tách mảng thành 3 nhóm (`< pivot`, `== pivot`, `> pivot`). Điểm thiên tài của thuật toán là khả năng **tự động cắt nhánh**: Nhóm `== pivot` sẽ được gọi đệ quy xét luôn ký tự tiếp theo (`depth + 1`), bỏ qua hoàn toàn việc so sánh lại các tiền tố đã biết. Miễn nhiễm tuyệt đối với Test chuỗi trùng lặp.

```cpp
// Minh họa Multikey Quicksort: Bỏ qua tiền tố đã đọ (depth + 1)
multikey_quicksort(a, lo, lt - 1, depth);
if (pivot != -1) multikey_quicksort(a, lt, gt, depth + 1); // Bỏ qua ký tự trùng
multikey_quicksort(a, gt + 1, hi, depth);
```

### Cải tiến Bài C: Flat 1D Pointer Placement + Multikey QuickSort
* **Bản chất kỹ thuật (Zero-Copy & Zero-Allocation):** Lệnh `push_back()` của mảng 2 chiều `vector<vector>` sinh ra chi phí ngầm cực lớn do HĐH phải liên tục cấp phát lại RAM (Reallocation). 
* Thuật toán Lần 2 dùng **Counting Sort** đếm tần suất chiều dài, dùng Prefix Sum tính toán vị trí, và gán thẳng **con trỏ chuỗi (`string*`)** vào một mảng tĩnh 1 chiều (`sorted_by_len`). Toàn bộ $10^5$ phần tử được phân loại theo độ dài mà không có bất kỳ thao tác copy chuỗi hay cấp phát động nào diễn ra. Sau đó, gọi Multikey Quicksort trên từng phân đoạn độ dài để xử lý phần từ điển.

```cpp
// Minh họa trải phẳng mảng 1 chiều bằng con trỏ (Tránh copy chuỗi)
vector<int> pos(max_len + 1, 0);
for (int i = 1; i <= max_len; ++i) pos[i] = pos[i - 1] + count[i - 1]; // Prefix sum

vector<string*> sorted_by_len(n);
for (int i = 0; i < n; ++i) {
    sorted_by_len[pos[s[i].length()]++] = &s[i]; // Đẩy con trỏ thay vì chuỗi thô
}
```

---

## 📊 PHẦN 4: ĐÁNH GIÁ VÀ TỔNG KẾT BÀI HỌC

### Bảng so sánh kiến trúc cấp hệ thống

| Tiêu chí | Benchmark 1 | Benchmark 2 (Tối ưu cực hạn) |
| :--- | :--- | :--- |
| **Thuật toán Số nguyên** | QuickSort tự cài (Tràn số Pivot, TLE mảng ngược) | Radix Base-256 (Bitwise siêu tốc, ổn định tuyệt đối) |
| **Thuật toán Chuỗi** | LSD Radix Sort (Bị TLE bởi chuỗi dài đột biến) | **Multikey QuickSort** (Chặn nhánh thông minh theo tiền tố) |
| **Quản lý bộ nhớ (Bài C)** | `vector<vector<string>>` cấp phát động liên tục | **Flat 1D Pointer Array** (Trải phẳng bằng con trỏ, Zero Copy) |
| **Sức chịu đựng** | Dễ sập trước dữ liệu có tính đối kháng cao | Hoạt động trơn tru bất chấp Test Case phá hoại |

### Bài học cốt lõi (Core Insights)

| # | Insight (Góc nhìn kỹ thuật chuyên sâu) |
| :---: | :--- |
| **1** | **Rủi ro tràn số (Integer Overflow):** Một thuật toán tưởng chừng hoàn hảo như QuickSort có thể sụp đổ chỉ vì dòng lệnh `(l + r) / 2`. Tư duy phòng thủ hệ thống là bắt buộc. |
| **2** | **Sự ưu việt của chia để trị trên chuỗi:** LSD bắt buộc duyệt hết chiều dài, trong khi **Multikey Quicksort** kết hợp tốc độ của Quicksort và khả năng phân rã tiền tố của Radix, tối ưu hóa bài toán so sánh chuỗi trùng lặp một cách triệt để. |
| **3** | **Chi phí ẩn từ thư viện chuẩn (STL):** Mảng hai chiều động là "sát thủ tàng hình" ở scale lớn do ngắt luồng Pipeline của CPU để cấp phát RAM. Kỹ thuật đếm phân phối (Counting Placement) kết hợp mảng con trỏ (`Pointer Array`) là chân lý để đạt tốc độ trần vật lý. |
