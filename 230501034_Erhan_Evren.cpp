#include <iostream>
#include<vector>
#include<string>
#include <cstdlib>
#include <time.h>
#include <algorithm>

#define Boyut 5
#define OS 2
#define RESET        "\033[0m"
#define ANSI_RED     "\033[31m"
#define ANSI_GREEN   "\033[32m"
#define ANSI_YELLOW  "\033[33m"
#define ANSI_BLUE    "\033[34m"
#define ANSI_WHİTE   "\037[34m"


using namespace std;
using std::vector;

int OyuncuSayisi = 0;
int TahtaBoyutu = 0;


class Oyuncu {
public:
    int id = 0;
    double oyuncuKaynak = 200;
    double AlanPayi = 0;

};
class Savasci {
public:
    int oyuncuId;
    int savasciKaynak;
    double can;
    int konum_x;
    int konum_y;
    virtual double hasar(double CanGelen) { return CanGelen; }
    virtual void Saldir() = 0;
    virtual string KarakterTuru() const { return ""; };

    Savasci(int _oyuncuId = 0, int _savasciKaynak = 0, double _can = 0, int _konum_x = -1, int _konum_y = -1) : oyuncuId(_oyuncuId), savasciKaynak(_savasciKaynak), can(_can), konum_x(_konum_x), konum_y(_konum_y) { }
};
Savasci* Tahta[32][32];
vector <Savasci*>SavasciDizi;
bool SinifSirala(const Savasci* a, const Savasci* b) {
    return a->can > b->can;
}

bool kaynakSirala(const Savasci* a, const Savasci* b) {
    return a->savasciKaynak > b->savasciKaynak;
}
class Muhafiz :public Savasci {
public:
    string KarakterTuru()const override { return "M"; }

    virtual double hasar(double CanGelen) {
        CanGelen -= 40;
        return CanGelen;
    }
    void Saldir() {
        cout << "Muhafizin" << konum_x << "-" << konum_y << endl;

        for (int j = konum_x - 1; j < konum_x + 2; j++) {
            for (int i = konum_y - 1; i < konum_y + 2; i++) {
                if (i > -1 && j > -1 && i < TahtaBoyutu && j < TahtaBoyutu) {
                    if (Tahta[i][j]->oyuncuId != oyuncuId && Tahta[i][j]->oyuncuId != 0) {
                        Tahta[i][j]->can -= 20;
                        cout << j + 1 << "," << i + 1 << " saldirildi" << endl;
                        if (Tahta[i][j]->can <= 0) {
                            Muhafiz* key = new Muhafiz;
                            //delete Tahta[i][j];
                            Tahta[i][j] = key;
                            cout << i << "," << j << " Öldürüldü" << endl;
                        }
                    }
                }
            }
        }
    }
    Muhafiz(int _oyuncuId = 0, int _savasciKaynak = 10, double _can = 80) : Savasci(_oyuncuId, _savasciKaynak, _can) {}


};
class Okcu :public Savasci {
public:
    string KarakterTuru() const override { return "O"; }


    virtual double hasar(double CanGelen) {
        CanGelen *= 4 / 10;
        return CanGelen;
    }
    void Saldir() {
        vector<Savasci*>DusmanVec;
        cout << "Okcunun" << konum_x << "-" << konum_y << endl;

        for (int j = konum_x - 2; j < konum_x + 3; j++) {
            for (int i = konum_y - 2; i < konum_y + 3; i++) {
                if (i > -1 && j > -1 && i < TahtaBoyutu && j < TahtaBoyutu) {
                    if (Tahta[i][j]->oyuncuId != oyuncuId && Tahta[i][j]->oyuncuId != 0) {
                        DusmanVec.push_back(Tahta[i][j]);
                    }
                }
            }
        }
        sort(DusmanVec.begin(), DusmanVec.end(), SinifSirala);
        for (int i = 0; i < 3; i++) {
            if (DusmanVec.size() > i) {
                DusmanVec.at(i)->can *= 0.4;
                //cout << DusmanVec.at(i)->oyuncuId << ". oyuncunun okcusu: " << DusmanVec.at(i)->konum_x << "-" <<
                //    DusmanVec.at(i)->konum_y << "  " << DusmanVec.at(i)->can << endl;
            }
        }
        //Menzildeki en yÃ¼ksek canÄ± olan 3 dÃ¼ÅŸman
    }
    Okcu(int _oyuncuId = 0, int _savasciKaynak = 20, double _can = 30) : Savasci(_oyuncuId, _savasciKaynak, _can) {}
};
class Topcu :public Savasci {
public:
    string KarakterTuru() const override { return "T"; }


    virtual double hasar(double CanGelen) {
        CanGelen = 0;
        return CanGelen;
    }
    void Saldir() {
        vector <Savasci*>key;
        Muhafiz* MuhafizKey = new Muhafiz;
        MuhafizKey->can = 0;
        key.push_back(MuhafizKey);
        cout << "Topcununclmkd" << konum_x << "-" << konum_y << endl;

        int i = konum_x - 2;
        for (; i < konum_x + 3; i++) {
            cout << "1*" << endl;

            if (i > -1 && i < TahtaBoyutu) {
                if (Tahta[i][konum_y]->oyuncuId != oyuncuId && Tahta[i][konum_y]->oyuncuId != 0) {
                    if (key.at(0)->can < Tahta[i][konum_y]->can)
                        key.at(0) = Tahta[i][konum_y];
                }
            }
        }
        for (int i = konum_y - 2; i < konum_y + 3; i++) {
            if (i > -1 && i < TahtaBoyutu) {
                if (Tahta[konum_x][i]->oyuncuId != oyuncuId && Tahta[konum_x][i]->oyuncuId != 0) {
                    if (key.at(0)->can < Tahta[konum_x][i]->can)
                        key.at(0) = Tahta[konum_x][i];
                }
            }
            cout << "2*" << endl;

        }
        //cout << key.->oyuncuId << ". oyuncunun yopcusu: " << key->konum_x << "-" <<
        //             key->konum_y << "  " <<key->can << endl;
        key.at(0)->oyuncuId = 0;

    }


    Topcu(int _oyuncuId = 0, int _savasciKaynak = 50, double _can = 30) : Savasci(_oyuncuId, _savasciKaynak, _can) {}
};
class Atli :public Savasci {
public:
    string KarakterTuru() const override { return "A"; }

    virtual double hasar(double CanGelen) {
        CanGelen *= 7 / 10;
        return CanGelen;
    }
    void Saldir() {
       /* cout << "Atlinin" << konum_x << "-" << konum_y << endl;
        vector<Savasci*>DusmanVec;

        for (int j = konum_x - 3; j < konum_x + 4; j++) {
            for (int i = konum_y - 3; i < konum_y + 4; i++) {
                if (i > -1 && j > -1 && i < TahtaBoyutu && j < TahtaBoyutu && i != konum_y && j != konum_x) {
                    if (Tahta[i][j]->oyuncuId != oyuncuId && Tahta[i][j]->oyuncuId != 0) {
                        DusmanVec.push_back(Tahta[i][j]);
                        cout << DusmanVec.at(i)->oyuncuId << ". oyuncunun atlisi: " << DusmanVec.at(i)->konum_x << "-" <<
                            DusmanVec.at(i)->konum_y << "  " << DusmanVec.at(i)->can << endl;
                    }
                }
            }

        }*/

        //sort(DusmanVec.begin(), DusmanVec.end(), kaynakSirala);
        //for (int i = 0; i < 2; i++) {
        //    if (DusmanVec.size() > i) {
        //       // DusmanVec.at(i)->can -= 30;
        //        /*cout << DusmanVec.at(i)->oyuncuId << ". oyuncunun atlisi: " << DusmanVec.at(i)->konum_x << "-" <<
        //            DusmanVec.at(i)->konum_y << "  " << DusmanVec.at(i)->can << endl;*/
        //    }
        //}
       //Menzildeki en pahalÄ± 2 dÃ¼ÅŸman
    }
    Atli(int _oyuncuId = 0, int _savasciKaynak = 30, double _can = 40) : Savasci(_oyuncuId, _savasciKaynak, _can) {}
};
class Saglikci :public Savasci {
public:
    string KarakterTuru() const override { return "S"; }

    virtual double hasar(double CanGelen) {
        CanGelen *= 15 / 10;
        return CanGelen;
    }
    void Saldir() {
        vector<Savasci*>DostVec;
        cout << "Saglikcinin" << konum_x << "-" << konum_y << endl;

        for (int j = konum_x - 2; j < konum_x + 3; j++) {
            for (int i = konum_y - 2; i < konum_y + 3; i++) {
                if (i > -1 && j > -1 && i < TahtaBoyutu && j < TahtaBoyutu) {
                    if (Tahta[i][j]->oyuncuId == oyuncuId) {
                        if (Tahta[i][j]->konum_x != konum_x && Tahta[i][j]->konum_y != konum_y)
                        {
                            DostVec.push_back(Tahta[i][j]);
                        }
                    }
                }
            }
        }
        sort(DostVec.begin(), DostVec.end(), SinifSirala);
        int size = DostVec.size();
        cout << size << endl;
        for (int i = 0; i < 3; i++) {
            if (DostVec.size() > i) {
                DostVec.at(size-i)->can *= 1.5;
                //cout << DusmanVec.at(i)->oyuncuId << ". oyuncunun okcusu: " << DusmanVec.at(i)->konum_x << "-" <<
                //    DusmanVec.at(i)->konum_y << "  " << DusmanVec.at(i)->can << endl;
            }
        }
        //Menzildeki en yÃ¼ksek canÄ± olan 3 dÃ¼ÅŸman
    }
    
    Saglikci(int _oyuncuId = 0, int _savasciKaynak = 10, double _can = 100) : Savasci(_oyuncuId, _savasciKaynak, _can) {}
};

vector<Oyuncu>OyuncuDizi;
vector<string>SecilebilirAlanlar;

void TahtaDoldur(int tahtaBoyutu) {

    for (int i = 0; i < TahtaBoyutu; i++) {

        for (int j = 0; j < TahtaBoyutu; j++) {
            Muhafiz* key = new Muhafiz;
            key->konum_x = j;
            key->konum_y = i;

            Tahta[i][j] = key;
            //cout<<Tahta[i][j]->savasciKaynak<<endl;
        }
    }
    cout << "Tahta Olusturuldu\n";
}
void KarakterTablosu() {
    cout << "KARAKTER TURU--KAYNAK--CAN" << endl;
    cout << "Muhafiz          10     80" << endl;
    cout << "Okcu             20     30" << endl;
    cout << "Topcu            50     30" << endl;
    cout << "Atli             30     40" << endl;
    cout << "Saglikci         10     100" << endl;

    cout << "\nSecilen Karakter:Muhafiz[1]\nOkcu[2]\nTopcu[3]\nAtli[4]\nSaglikci[5]\n\n";

}
void SecilebilirAlanGoster(int id) {

    SecilebilirAlanlar.clear();

    for (int i = 0; i < TahtaBoyutu; i++) {

        for (int j = 0; j < TahtaBoyutu; j++) {
            if (Tahta[i][j]->oyuncuId == id) {
                for (int x = i - 1; x < i + 2; x++) {
                    for (int y = j - 1; y < j + 2; y++) {
                        if (x < TahtaBoyutu && x >= 0 && y < TahtaBoyutu && y >= 0) {
                            string key = "(" + to_string(y) + "," + to_string(x) + ")";
                            int lenVector = SecilebilirAlanlar.size();
                            int k = 0;
                            for (k = 0; k < lenVector; k++) {
                                if (SecilebilirAlanlar.at(k) == key) {
                                    break;
                                }
                            }
                            if (k == lenVector) {
                                if (Tahta[x][y]->oyuncuId == 0 || Tahta[x][y]->oyuncuId == id) {
                                    SecilebilirAlanlar.push_back(key);
                                }

                            }
                        }
                    }
                }
            }
        }
    }
}
void TahtayaEkle(int x, int y, int id, int karakterTercih) {

    if (Tahta[x][y]->oyuncuId == id) {
        OyuncuDizi.at(id - 1).oyuncuKaynak += Tahta[x][y]->savasciKaynak * 8 / 10;
    }

    if (karakterTercih == 1)   Tahta[x][y] = new Muhafiz;
    else if (karakterTercih == 2)   Tahta[x][y] = new Okcu;
    else if (karakterTercih == 3)   Tahta[x][y] = new Topcu;
    else if (karakterTercih == 4)   Tahta[x][y] = new Atli;
    else if (karakterTercih == 5)   Tahta[x][y] = new Saglikci;


    Tahta[x][y]->oyuncuId = id;
    Tahta[x][y]->konum_x = y;
    Tahta[x][y]->konum_y = x;
    OyuncuDizi.at(id - 1).oyuncuKaynak -= Tahta[x][y]->savasciKaynak;
    SavasciDizi.push_back(Tahta[x][y]);

}
void Goster() {

    for (int i = 0; i < TahtaBoyutu; i++) {


        for (int j = 0; j < TahtaBoyutu; j++) {
            if (Tahta[i][j]->oyuncuId == 1) { cout << ANSI_BLUE << Tahta[i][j]->KarakterTuru() << Tahta[i][j]->can << RESET; }
            else if (Tahta[i][j]->oyuncuId == 2) { cout << ANSI_RED << Tahta[i][j]->KarakterTuru() << Tahta[i][j]->can << RESET; }
            else if (Tahta[i][j]->oyuncuId == 3) { cout << ANSI_GREEN << Tahta[i][j]->KarakterTuru() << Tahta[i][j]->can << RESET; }
            else if (Tahta[i][j]->oyuncuId == 4) { cout << ANSI_YELLOW << Tahta[i][j]->KarakterTuru() << Tahta[i][j]->can << RESET; }
            else { cout << " . "; }
            cout << " | ";

        }
        cout << endl;
        for (int i = 0; i < TahtaBoyutu * 4 - 1; i++)cout << "--";
        cout << endl;
    }
}
void Basla() {

    for (int a = 0; a < 5; a++) {
        for (int i = 0; i < OyuncuSayisi; i++) {
            KarakterTablosu();
            int karakterTercih = -1;
            int kordinatTercih = -1;
            while (karakterTercih < 0 || karakterTercih>5) {
                cout << i + 1 << ". Oyuncu karakter secimi(Mevcut Kaynaginiz = " << OyuncuDizi[i].oyuncuKaynak << " )" << endl;
                cin >> karakterTercih;
            }
            SecilebilirAlanGoster(i + 1);
            for (int i = 0; i < SecilebilirAlanlar.size(); i++) {
                cout << SecilebilirAlanlar.at(i) << " kordinati icin " << i + 1 << endl;
            }
            while (kordinatTercih<0 || kordinatTercih>SecilebilirAlanlar.size() + 1)
            {
                cout << "Kordinat tercihiniz:"; cin >> kordinatTercih;
            }
            string key = SecilebilirAlanlar.at(kordinatTercih - 1);
            key = key.substr(1, key.length() - 2);
            cout << key << endl;
            int x = stoi(key.substr(0, key.find(',')));
            int y = stoi(key.substr(key.find(',') + 1, key.length() - 1));
            TahtayaEkle(y, x, i + 1, karakterTercih);
            Goster();

            //cout << Tahta[y - 1][x - 1]->KarakterTuru()<<" canı" << Tahta[y - 1][x - 1]->can << endl;
        }
        for (int i = 0; i < SavasciDizi.size(); i++) {
            SavasciDizi.at(i)->Saldir();
        }
        //Tahta[][]->Saldir();
        cout << "SALDIRI SONUCU\n";
        Goster();
    }

}
void MuhafizAta() {

    srand(time(NULL));
    int SayiDizi[4] = { 0,0,0,0 };
    int j = 0;

    for (int i = 0; i < 4; i++) {
        int key = rand() % 4 + 1;
        for (j = 0; j < 4; j++) {
            if (SayiDizi[j] == key) {
                i--;
                break;
            }
        }
        if (j == 4) {
            SayiDizi[i] = key;
        }
    }

    for (int i = 0; i < OyuncuSayisi; i++) {

        if (SayiDizi[i] == 4) {
            Tahta[0][0]->oyuncuId = i + 1;
            SavasciDizi.push_back(Tahta[0][0]);
        }
        if (SayiDizi[i] == 1) {
            Tahta[0][TahtaBoyutu - 1]->oyuncuId = i + 1;
            SavasciDizi.push_back(Tahta[0][TahtaBoyutu - 1]);
        }
        if (SayiDizi[i] == 2) {
            Tahta[TahtaBoyutu - 1][0]->oyuncuId = i + 1;
            SavasciDizi.push_back(Tahta[TahtaBoyutu - 1][0]);
        }
        if (SayiDizi[i] == 3) {
            Tahta[TahtaBoyutu - 1][TahtaBoyutu - 1]->oyuncuId = i + 1;
            SavasciDizi.push_back(Tahta[TahtaBoyutu - 1][TahtaBoyutu - 1]);
        }
        //  OyuncuDizi.at(i - 1).oyuncuKaynak -= Tahta[x][y]->savasciKaynak;
    }
    Goster();
}


void main() {

    /* while (OyuncuSayisi != 2 && OyuncuSayisi != 3 && OyuncuSayisi != 4) {
         cout << "Lord Of The Polywarphism\n";
         cout << "Oyuncu Sayisini Giriniz\n[2]-[3]-[4]\n";
         cin >> OyuncuSayisi;
     }*/
    OyuncuSayisi = OS;
    for (int i = 1; i <= OyuncuSayisi; i++) {

        Oyuncu Oyuncukey;
        Oyuncukey.id = i;
        OyuncuDizi.push_back(Oyuncukey);
    }
    /*while (TahtaBoyutu < 4 || TahtaBoyutu >32) {
        cout << "\nTahata Boyutunu Giribiz \n[8-32]\n";
        cin >> TahtaBoyutu;
    }*/
    TahtaBoyutu = Boyut;
    TahtaDoldur(TahtaBoyutu);
    MuhafizAta();
    SecilebilirAlanGoster(1);
    Basla();
}
