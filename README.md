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

### 1. Bài A (Integer Sort) — Quy mô: $N = 100,000$ phần tử

* **`test001.in` (Mảng đã sắp xếp tăng dần):** Sinh dãy số liên tục từ $0$ đến $99,999$. Kịch bản này nhắm thẳng vào các thuật toán QuickSort ngây thơ chọn phần tử đầu hoặc cuối làm chốt (Pivot), ép độ phức tạp từ $O(N \log N)$ chạm đáy suy biến $O(N^2)$ gây TLE.
* **`test002.in` (Mảng đã sắp xếp giảm dần):** Sinh ngược dãy số từ $100,000$ lùi về $1$. Mục tiêu tương tự test 1, thử thách khả năng phân hoạch đối xứng của các bộ lọc dữ liệu cơ sở.
* **`test003.in` (Giá trị trùng lặp hoàn toàn):** Toàn bộ $100,000$ phần tử đều mang giá trị cố định `1005`. Nếu hàm phân hoạch QuickSort không xử lý chặt chẽ điều kiện biên dính dấu bằng (`<=` hoặc `>=`), con trỏ sẽ nhảy vô hạn hoặc phân rã mảng lệch hoàn toàn, gây tràn tầng đệ quy (Stack Overflow).
* **`test004.in` (Cực đại và Cực tiểu đan xen):** Lặp liên tục chuỗi xen kẽ giữa số nguyên lớn nhất `2147483647` ($INT\_MAX$) và nhỏ nhất `-2147483648` ($INT\_MIN$). Test case này trực tiếp đánh sập thuật toán Counting Sort thông thường (gây tràn RAM do mảng đếm quá lớn), đồng thời kiểm tra tính đúng đắn khi xử lý bit dấu của Radix Sort.
* **`test005.in` (Ngẫu nhiên diện rộng):** Sử dụng bộ sinh số ngẫu nhiên `mt19937` (Seed: `2111`) rải đều trên toàn bộ dải giá trị từ `-2147483648LL` đến `2147483647LL` để đánh giá hiệu năng tổng thể.

---

### 2. Bài B (Lexicographical String Sort) — Quy mô: $N = 100,000$ chuỗi

* **`test001.in` (Chuỗi giống hệt nhau tuyệt đối):** Sinh ra $100,000$ chuỗi toàn ký tự `'a'` với độ dài kịch trần là 100 ký tự. Kịch bản này ép mọi hàm so sánh chuỗi nền tảng bắt buộc phải quét sâu từ ký tự thứ 1 đến ký tự thứ 100 mới đưa ra được kết luận, đẩy chi phí đọ chuỗi lên mức tối đa.
* **`test002.in` (Trùng lặp tiền tố sâu - Long Prefix):** Tất cả chuỗi đều dùng chung 99 ký tự đầu là `'a'`, ký tự thứ 100 cuối cùng mới xoay vòng từ `'a'` đến `'z'`. Test case này đánh lừa các cơ chế ngắt nhánh sớm, ép CPU phải tốn chi phí duyệt sâu đến tận byte cuối cùng.
* **`test003.in` (Nghịch đảo từ điển kịch trần):** Sinh các chuỗi dài 100 ký tự có thành phần giảm dần, sau đó tiến hành đảo ngược toàn mảng (`sort(v.rbegin(), v.rend())`). Đây là cấu trúc tồi tệ nhất phá vỡ các luồng chia tách dữ liệu của QuickSort gốc chuỗi.
* **`test004.in` (Trùng tiền tố, lệch độ dài):** Sinh các chuỗi toàn ký tự `'a'` nhưng tăng dần độ dài từ 10 đến 100 ký tự, sau đó đảo ngược trật tự mảng. Test này bẫy các hàm đo chiều dài và kiểm tra tính bền vững của các thuật toán Radix dựa trên độ dài chuỗi biến thiên.
* **`test005.in` (Ngẫu nhiên hỗn hợp):** Dùng `mt19937` (Seed: `2111`) sinh ngẫu nhiên cả độ dài chuỗi (từ 10 đến 100) lẫn phân bổ ký tự ngẫu nhiên trong dải từ `'a'` đến `'z'`.

---

### 3. Bài C (Length-Lexicographical Sort) — Quy mô: $N = 10,000$ chuỗi

* **`test001.in` (Đồng nhất độ dài, ngược từ điển):** Khởi tạo $10,000$ chuỗi có cùng độ dài 100 ký tự (99 ký tự đầu là `'a'`, ký tự cuối giảm dần từ `'z'` về `'a'`) rồi xếp ngược. Test này triệt tiêu hoàn toàn lợi thế của bộ lọc độ dài, ép hệ thống phải quay về bài toán so sánh ký tự thô đắt đỏ.
* **`test002.in` (Xen kẽ cực ngắn và cực dài):** Đan xen liên tục: một chuỗi dài 10 ký tự `'a'` rồi đến một chuỗi dài 100 ký tự `'z'`. Test case này ép các thuật toán hoán đổi vùng nhớ thô phải liên tục dịch chuyển các khối dữ liệu có kích thước chênh lệch lớn, làm lộ rõ chi phí ẩn của việc cấp phát RAM liên tục.
* **`test003.in` (Độ dài suy biến giảm dần):** Sinh chuỗi có độ dài tụt dốc từ 100 xuống 10 ký tự (ngược hoàn toàn với yêu cầu đề bài là xếp tăng dần độ dài). Đây là kịch bản phá hoại cấu trúc phân hoạch, bẫy các thuật toán gom nhóm không tối ưu.
* **`test004.in` (Trùng lặp chuỗi dài tối đa):** Sinh $10,000$ chuỗi giống hệt nhau, đều dài đúng 100 ký tự toàn chữ `'z'`. Mục tiêu là thử thách điều kiện dừng của hàm phân hoạch nội bộ trong các Bucket khi gặp dữ liệu bằng nhau tuyệt đối.
* **`test005.in` (Ngẫu nhiên toàn vẹn):** Dùng `mt19937` (Seed: `2111`) sinh ngẫu nhiên chiều dài từ 10 đến 100 ký tự và phân bổ chữ cái ngẫu nhiên từ `'a'` đến `'z'` để đo thông số hiệu năng thực tế.

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
