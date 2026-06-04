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

Hệ thống sử dụng bộ sinh dữ liệu mã nguồn `test_gen.cpp` để tạo ra các kịch bản kiểm thử có tính đối kháng cao. Thay vì sinh dữ liệu ngẫu nhiên một cách mù quáng, mỗi test case (từ 1 đến 5) được thiết kế nhằm kích hoạt các trạng thái suy biến tệ nhất của các thuật toán: gây tràn bộ nhớ, lặp vô hạn (TLE), hoặc bẻ gãy luồng chia để trị.

---

### Cách thức sinh test case:

### A:

* **test001.in** (Đã sắp xếp tăng dần): Mảng đã được sắp xếp sẵn. Nếu ai dùng Quick Sort mà luôn chọn phần tử đầu tiên làm chốt (pivot), thuật toán sẽ bị biến thành O(N^2) và chạy quá thời gian (TLE).

* **test002.in** (Sắp xếp giảm dần): Tương tự Test 1, mảng ngược hoàn toàn sẽ làm khó các thuật toán thiếu tối ưu.

* **test003.in** (Toàn số giống nhau): 10^5 phần tử có giá trị y hệt nhau. Nếu code Quick Sort phân chia mảng không khéo (không xử lý tốt dấu bằng), chương trình sẽ bị lặp vô hạn hoặc tràn bộ nhớ .

* **test004.in** (Cực đại và Cực tiểu xen kẽ): Xen kẽ số lớn nhất (2147483647) và số nhỏ nhất (-2147483648) của kiểu int. Test này tránh những người dùng mảng đếm (Counting Sort) bị tràn RAM hoặc xử lý bit dấu bị sai.

* **test005.in** (Ngẫu nhiên): Kiểm tra tổng thể với thuật toán.

### B:

test001.in (100,000 chuỗi giống hệt nhau): Sinh ra 10^5 chuỗi toàn ký tự 'a' dài đúng 100 ký tự. Khi so sánh hai chuỗi giống nhau, máy tính bắt buộc phải chạy từ ký tự thứ 1 đến ký tự thứ 100 mới đưa ra kết luận. Thao tác so sánh chuỗi lúc này đạt độ phức tạp tối đa.

test002.in (Chỉ khác biệt ký tự cuối cùng): Tạo ra các chuỗi có 99 ký tự đầu hoàn toàn giống nhau, ký tự thứ 100 sẽ tăng dần theo vòng lặp từ a đến z. Thuật toán sắp xếp sẽ phải qua 99 ký tự đầu vô ích trước khi tìm thấy sự khác biệt ở ký tự cuối cùng.

test003.in (Chuỗi dài 100 ký tự, thứ tự giảm dần): Sinh ra các chuỗi dài tối đa 100 ký tự nhưng xếp ngược từ điển từ trước. Phá vỡ luồng phân chia của các thuật toán Quick Sort dùng chốt mặc định.

test004.in (Trùng tiền tố, độ dài tăng dần): Sinh các chuỗi dạng a, aa, aaa, aaaa... tăng dần độ dài lên tới 100 nhưng xếp lộn xộn hoặc giảm dần để test khả năng xử lý độ dài chuỗi của thuật toán.

test005.in (Ngẫu nhiên): Sinh ngẫu nhiên cả độ dài (10 - 100) và ký tự ngẫu nhiên để test tính ổn định.

### C:

test001.in (Cùng độ dài tối đa 100, xếp ngược từ điển): Sinh ra 10000 chuỗi đều có chiều dài tối đa là 100 ký tự, nhưng các ký tự được xếp giảm dần. Test này triệt tiêu hoàn toàn lợi thế so sánh độ dài, ép thuật toán phải quay về bài toán so sánh chuỗi của những code không tối ưu.

test002.in (Xen kẽ độ dài cực ngắn và cực dài): Sinh ra các chuỗi xen kẽ liên tục: một chuỗi dài 10 ký tự aaaa... rồi đến một chuỗi dài 100 ký tự zzzz.... Test này ép các thuật toán phải liên tục hoán đổi các vùng nhớ có kích thước chênh lệch lớn, dễ làm lộ lỗi tràn bộ nhớ hoặc lỗi index mảng đệm.

test003.in (Độ dài giảm ): Sinh ra các chuỗi có độ dài giảm dần từ 100 xuống 10 (ngược hoàn toàn với yêu cầu đề bài là xếp tăng dần độ dài). Đây là cấu trúc dữ liệu tồi tệ nhất đối với những người sử dụng các thuật toán sắp xếp dựa trên phân hoạch.

test004.in (Tất cả chuỗi giống hệt nhau và dài tối đa): Sinh ra 10000 chuỗi zzzz... dài đúng 100 ký tự. Test này bẫy các hàm Quick Sort tự viết không xử lý tốt trường hợp các phần tử bằng nhau.

test005.in (Ngẫu nhiên): Sinh ngẫu nhiên cả độ dài (10 - 100) và ký tự ngẫu nhiên để test tính ổn định.


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
