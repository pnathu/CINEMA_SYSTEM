#include "../include/Promotion.h"
#include "../include/Database.h"
#include "../include/DateFormat.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <sstream>

using namespace std;

Promotion::Promotion() {
    this->phanTramGiam = 0.0;
    this->mucGiamToiDa = 0.0;
}

Promotion::Promotion(string maKM, string tenKM, string code, double ptGiam, double maxGiam, string start, string end, string phamVi) {
    this->maKhuyenMai = maKM;
    this->tenKhuyenMai = tenKM;
    this->maCode = code;
    this->phanTramGiam = ptGiam;
    this->mucGiamToiDa = maxGiam;
    this->ngayBatDau = DateFormat::normalizeDate(start);
    this->ngayKetThuc = DateFormat::normalizeDate(end);
    this->phamViApDung = phamVi;
}

string Promotion::getMaKhuyenMai() const { return maKhuyenMai; }
string Promotion::getTenKhuyenMai() const { return tenKhuyenMai; }
string Promotion::getMaCode() const { return maCode; }
double Promotion::getPhanTramGiam() const { return phanTramGiam; }
double Promotion::getMucGiamToiDa() const { return mucGiamToiDa; }
string Promotion::getNgayBatDau() const { return ngayBatDau; }
string Promotion::getNgayKetThuc() const { return ngayKetThuc; }
string Promotion::getPhamViApDung() const { return phamViApDung; }

void Promotion::setTenKhuyenMai(const string& ten) { tenKhuyenMai = ten; }
void Promotion::setMaCode(const string& code) { maCode = code; }
void Promotion::setPhanTramGiam(double pt) { if (pt >= 0 && pt <= 100) phanTramGiam = pt; }
void Promotion::setMucGiamToiDa(double maxGia) { if (maxGia >= 0) mucGiamToiDa = maxGia; }
void Promotion::setNgayBatDau(const string& start) { ngayBatDau = DateFormat::normalizeDate(start); }
void Promotion::setNgayKetThuc(const string& end) { ngayKetThuc = DateFormat::normalizeDate(end); }
void Promotion::setPhamViApDung(const string& pv) { phamViApDung = pv; }
string Promotion::toCSV() const {
    stringstream ssPT, ssMax;
    ssPT << fixed << setprecision(1) << phanTramGiam;
    ssMax << fixed << setprecision(0) << mucGiamToiDa;
    return maKhuyenMai + "," + tenKhuyenMai + "," + maCode + "," + ssPT.str() + "," + ssMax.str() + "," + ngayBatDau + "," + ngayKetThuc + "," + phamViApDung;
}

Promotion Promotion::fromCSV(const string& line) {
    stringstream ss(line);
    string maKM, tenKM, code, strPT, strMax, start, end, phamVi;

    getline(ss, maKM, ',');
    getline(ss, tenKM, ',');
    getline(ss, code, ',');
    getline(ss, strPT, ',');
    getline(ss, strMax, ',');
    getline(ss, start, ',');
    getline(ss, end, ',');
    getline(ss, phamVi, ',');

    double pt = 0.0, maxG = 0.0;
    if (!strPT.empty()) { stringstream(strPT) >> pt; }
    if (!strMax.empty()) { stringstream(strMax) >> maxG; }

    return Promotion(maKM, tenKM, code, pt, maxG, start, end, phamVi);
}
void Promotion::hienThi() const {
    cout << left << setw(10) << maKhuyenMai
         << setw(20) << tenKhuyenMai
         << setw(12) << maCode
         << setw(10) << fixed << setprecision(1) << phanTramGiam << "%"
         << setw(15) << setprecision(0) << mucGiamToiDa
         << setw(12) << ngayBatDau
         << setw(12) << ngayKetThuc
         << setw(15) << phamViApDung << endl;
}

PromotionManager::PromotionManager() {}

void PromotionManager::taiDuLieu(Database& db) {
    db.docDanhSachKhuyenMai(danhSachKhuyenMai);
}

void PromotionManager::hienThiDanhSach() const {
    cout << "\n========================================= DANH SÁCH KHUY?N MÃI & VOUCHER =========================================\n";
    cout << left << setw(10) << "Mã KM"
         << setw(20) << "Tên Chuong Trình"
         << setw(12) << "Mã Code"
         << setw(10) << "Gi?m (%)"
         << setw(15) << "Gi?m T?i Ða"
         << setw(12) << "B?t Ð?u"
         << setw(12) << "K?t Thúc"
         << setw(15) << "Ph?m Vi" << endl;
    cout << "-----------------------------------------------------------------------------------------------------------------\n";
    if (danhSachKhuyenMai.empty()) {
        cout << "Chua có chuong trình khuy?n mãi nào du?c c?u hình!\n";
    } else {
        for (size_t i = 0; i < danhSachKhuyenMai.size(); ++i) {
        danhSachKhuyenMai[i].hienThi();
    }
    }
    cout << "=================================================================================================================\n";
}

void PromotionManager::themKhuyenMai(Database& db) {
    string maKM, tenKM, code, start, end, phamVi;
    double ptGiam, maxGiam;

    cout << "\n--- THÊM CHUONG TRÌNH KHUY?N MÃI / VOUCHER M?I ---\n";
    cout << "Nh?p mã khuy?n mãi (VD: KM01): ";
    cin >> maKM;

    for (size_t i = 0; i < danhSachKhuyenMai.size(); ++i) {
        if (danhSachKhuyenMai[i].getMaKhuyenMai() == maKM) {
            cout << "Loi: Ma khuyen mai da ton tai!\n";
            return;
        }
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Nh?p tên chuong trình: ";
    getline(cin, tenKM);

    cout << "Nh?p mã code dùng khi thanh toán (VD: TET2026): ";
    cin >> code;

    for (size_t i = 0; i < danhSachKhuyenMai.size(); ++i) {
        if (danhSachKhuyenMai[i].getMaCode() == code) {
            cout << "Loi: Ma code nay da duoc su dung cho chuong trinh khac!\n";
            return;
        }
    }

    cout << "Nh?p ph?n tram gi?m giá (% t? 0 d?n 100): ";
    cin >> ptGiam;

    cout << "Nh?p m?c gi?m ti?n t?i da (VNÐ): ";
    cin >> maxGiam;

    cout << "Nhap ngay bat dau (DD/MM/YYYY): ";
    cin >> start;
    cout << "Nhap ngay ket thuc (DD/MM/YYYY): ";
    cin >> end;

    if (!DateFormat::isValidDate(start) || !DateFormat::isValidDate(end) ||
        DateFormat::dateKey(start) > DateFormat::dateKey(end)) {
        cout << "L?i: Ngày b?t d?u không th? sau ngày k?t thúc! H?y thao tác thêm.\n";
        return;
    }

    cout << "Nh?p ph?m vi áp d?ng ('All' ho?c mã phim c? th? nhu 'P01'): ";
    cin >> phamVi;

    Promotion newPromo(maKM, tenKM, code, ptGiam, maxGiam, start, end, phamVi);
    danhSachKhuyenMai.push_back(newPromo);

    db.ghiDanhSachKhuyenMai(danhSachKhuyenMai);
    cout << "Thêm chuong trình khuy?n mãi thành công!\n";
}

void PromotionManager::suaKhuyenMai(Database& db) {
    string maKM;
    cout << "\n--- S?A THÔNG TIN KHUY?N MÃI ---\n";
    cout << "Nh?p mã khuy?n mãi c?n s?a: ";
    cin >> maKM;

    bool found = false;
    for (size_t i = 0; i < danhSachKhuyenMai.size(); ++i) {
        if (danhSachKhuyenMai[i].getMaKhuyenMai() == maKM) {
            found = true;
            string tenKM, start, end, phamVi;
            double ptGiam, maxGiam;

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Nhap ten moi (" << danhSachKhuyenMai[i].getTenKhuyenMai() << "): ";
            getline(cin, tenKM);
            if (!tenKM.empty()) danhSachKhuyenMai[i].setTenKhuyenMai(tenKM);

            cout << "Nhap phan tram giam moi (%): ";
            cin >> ptGiam;
            danhSachKhuyenMai[i].setPhanTramGiam(ptGiam);

            cout << "Nhap muc giam toi da moi: ";
            cin >> maxGiam;
            danhSachKhuyenMai[i].setMucGiamToiDa(maxGiam);

            cout << "Nhap ngay bat dau moi (DD/MM/YYYY): ";
            cin >> start;
            cout << "Nhap ngay ket thuc moi (DD/MM/YYYY): ";
            cin >> end;

            if (!DateFormat::isValidDate(start) || !DateFormat::isValidDate(end) ||
                DateFormat::dateKey(start) > DateFormat::dateKey(end)) {
                cout << "Loi: Khoang ngay khong hop le! Giu nguyen muc thoi gian cu.\n";
            } else {
                danhSachKhuyenMai[i].setNgayBatDau(start);
                danhSachKhuyenMai[i].setNgayKetThuc(end);
            }

            cout << "Nhap pham vi ap dung moi ('All' hoac ma phim): ";
            cin >> phamVi;
            danhSachKhuyenMai[i].setPhamViApDung(phamVi);

            db.ghiDanhSachKhuyenMai(danhSachKhuyenMai);
            cout << "Cap nhat thong tin khuyen mai thanh cong!\n";
            break;
        }
    }
    if (!found) {
        cout << "Không tìm th?y mã khuy?n mãi c?n s?a!\n";
    }
}

void PromotionManager::xoaKhuyenMai(Database& db) {
    string maKM;
    cout << "\n--- XÓA CHUONG TRÌNH KHUY?N MÃI ---\n";
    cout << "Nh?p mã khuy?n mãi c?n xóa: ";
    cin >> maKM;

    bool deleted = false;
    for (size_t i = 0; i < danhSachKhuyenMai.size(); ++i) {
        if (danhSachKhuyenMai[i].getMaKhuyenMai() == maKM) {
            danhSachKhuyenMai.erase(danhSachKhuyenMai.begin() + i);
            deleted = true;
            break;
        }
    }

    if (deleted) {
        db.ghiDanhSachKhuyenMai(danhSachKhuyenMai);
        cout << "Ðã xóa chuong trình khuy?n mãi thành công!\n";
    } else {
        cout << "Không tìm th?y mã khuy?n mãi tuong ?ng!\n";
    }
}

bool PromotionManager::apDungVoucher(const string& maCodeNhap, const string& maPhimChieu, double tongTienDonHang, double& soTienDuocGiam) const {
    const Promotion* matchedPromo = NULL;

    for (size_t i = 0; i < danhSachKhuyenMai.size(); ++i) {
        if (danhSachKhuyenMai[i].getMaCode() == maCodeNhap) {
            matchedPromo = &danhSachKhuyenMai[i];
            break;
        }
    }

    if (!matchedPromo) {
        cout << "C?nh báo: Mã code voucher không t?n t?i trong h? th?ng!\n";
        soTienDuocGiam = 0.0;
        return false;
    }

    int todayKey;
    int startKey = DateFormat::dateKey(matchedPromo->getNgayBatDau());
    int endKey = DateFormat::dateKey(matchedPromo->getNgayKetThuc());
    if (!DateFormat::todayDateKey(todayKey)) {
        cout << "Loi: Khong the xac dinh ngay hien tai de kiem tra voucher!\n";
        soTienDuocGiam = 0.0;
        return false;
    }
    if (startKey == 0 || endKey == 0 || todayKey < startKey || todayKey > endKey) {
        cout << "Voucher khong nam trong thoi gian ap dung!\n";
        soTienDuocGiam = 0.0;
        return false;
    }

    string phamVi = matchedPromo->getPhamViApDung();
    if (phamVi != "All" && phamVi != maPhimChieu) {
        cout << "Thanh toán th?t b?i: Mã d?c quy?n [" << maCodeNhap << "] ch? áp d?ng cho phim [" << phamVi
             << "], không kh?p v?i phim khách ch?n (" << maPhimChieu << ")!\n";
        soTienDuocGiam = 0.0;
        return false;
    }

    double giamTheoPhanTram = tongTienDonHang * (matchedPromo->getPhanTramGiam() / 100.0);
    if (matchedPromo->getMucGiamToiDa() > 0 && giamTheoPhanTram > matchedPromo->getMucGiamToiDa()) {
        soTienDuocGiam = matchedPromo->getMucGiamToiDa();
    } else {
        soTienDuocGiam = giamTheoPhanTram;
    }

    cout << "Áp d?ng voucher thành công! Tên uu dãi: " << matchedPromo->getTenKhuyenMai()
         << " | Ðu?c gi?m: " << fixed << setprecision(0) << soTienDuocGiam << " VNÐ\n";
    return true;
}
