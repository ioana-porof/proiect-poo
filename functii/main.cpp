#include <iostream>
#include <cstring>
#include <fstream>
#include <ctime>
using namespace std;
//time_t t = time(nullptr);
//tm* now = localtime(&t);
fstream file1("nume.txt", std::ios::in | std::ios::out | std::ios::app);
fstream file2("camere.txt", std::ios::in | std::ios::out | std::ios::app);
fstream file3("date.txt", std::ios::in | std::ios::out | std::ios::app);
fstream file4("money.txt", std::ios::in | std::ios::out | std::ios::app);

bool hotel_gol( string name)//functia verifica daca exista ceva scris in fisierul cu nume, ca sa stiu daca trebuie sa salvez niste rezervari facute anterior
{
    ifstream  file(name);
    return file.peek() == std::ifstream::traits_type::eof();
}
int days_in_month(int mth)//functie pentru zilele dintr-o luna
{
    if(mth==1||mth==3||mth==5||mth==7||mth==8||mth==10||mth==12)
        return 31;
    else if(mth==2)
        return 28;
    else
        return 30;
}
void calculator2(int dd, int mth, int per)//ce face calculator, dar difera cout-ul, functia asta e pt fisier
{
    int zile=days_in_month(mth);
    if(dd+per>zile)
    {
        if(mth==12)
        {
            //cout<<dd+per-zile<<"/"<<"01";
            file3<<dd+per-zile<<" / "<<1<<'\n';
        }
        else
        {
            //cout<<dd+per-zile<<"/"<<mth+1;
            file3<<dd+per-zile<<" / "<<mth+1<<'\n';
        }
    }
    else
    {
        file3<<dd+per<<" / "<<mth<<'\n';
        //cout<<dd+per<<"/"<<mth;
    }

}
void calculator(int dd, int mth, int per)//calculeaza data plecarii unui client folosindu-se de data check-in-ului dd/mth si perioada in care vrea sa stea la hotel
{
    int zile=days_in_month(mth);
    if(dd+per>zile)//in caz ca se schimba luna
    {
        if(mth==12)//pentru decembrie, ca sa schimb in ianuarie
        {
            cout<<dd+per-zile<<"/"<<1;

        }
        else
        {
            cout<<dd+per-zile<<"/"<<mth+1;

        }
    }
    else//daca nu se schimba luna
    {

        cout<<dd+per<<"/"<<mth;
    }

}
int calculator_invers(int dd1, int mth1, int dd2, int mth2)//pentru clientii pe care vreau sa ii retin dintr-o rulare anterioara a programului. In fisierul date.txt am doar intervalul de sedere, asa ca aici calculez cate zile sta de fapt
{
    int per, zile=days_in_month(mth1);
    if(mth1==mth2)
    {
      per=dd2-dd1;
    }
    else
    {
        per=dd2+zile-dd1;
    }
    return per;
}
int e_liber(int **calendar, int dd, int mth, int stay)//verific daca o anumita camera e libera in perioada pe care o cer clientul
{
    int zile=days_in_month(mth);
    if(dd+stay>zile)
    {
        for(int i=dd-2; i<=zile; ++i)//nu se poate rezerva o camera cu 2 zile dupa ce a fost eliberata
        {
            if(calendar[mth][i]==1)
                return 0;
        }
        for(int i=1; i<=dd+stay-zile+2; ++i)//acelasi lucru cu 2 zile, dar pentru rezervarea de dupa, care nu ar putea exista daca diferenta ar fi mai mica de 2 zile
        {
            if(mth==12)
            {
                if(calendar[1][i]==1)
                    return 0;
            }
            else if(calendar[mth+1][i]==1)
                    return 0;
        }
        return 1;
    }
    else
    {
        if(dd-2<1)//aici verific daca scaderea celor 2 zile ma duce in alta luna
        {
            if(mth==1)//daca e ianuarie, sa stiu sa ma duc in decembrie
            {
                for(int i=zile-dd-1; i<=zile; ++i)
                    if(calendar[12][i]==1)
                        return 0;
                for(int i=dd; i<=dd+stay+2; ++i)
                    if(calendar[1][i]==1)
                        return 0;
            }
            else
            {
                for(int i=zile-dd-1; i<=zile; ++i)
                    if(calendar[mth-1][i]==1)
                        return 0;
                for(int i=dd; i<=dd+stay+2; ++i)
                    if(calendar[mth][i]==1)
                        return 0;
            }
        }
        else
            for(int i=dd-2; i<=dd+stay+2; ++i)
            if(calendar[mth][i]==1)
                return 0;
        return 1;
    }
}
class Camera
{
    int numar, etaj, pret, availability, **calendar;//calendar imi spune in ce zi e rezervata
    string tip;// single, double, apartament
public:
    Camera()
    {
        availability=0;
        numar = 0;
        etaj = 0;
        pret=0;
        tip="??";
        calendar = new int*[13];
        for (int i = 1; i < 13; ++i)
        {
            calendar[i] = new int[32];
            for (int j = 1; j < 32; ++j)
                calendar[i][j] = 0;
        }
    }
    Camera(int nr, int et, int pr, string tipul)
    {
        numar = nr;
        etaj = et;
        pret = pr;
        tip = tipul;
        availability=1;
        calendar = new int*[13];
        for (int i = 1; i < 13; ++i)
        {
            calendar[i] = new int[32];
            for (int j = 1; j < 32; ++j)
                calendar[i][j] = 0;
        }
    }
    Camera(Camera const& ob)
    {
        numar = ob.numar;
        availability=ob.availability;
        etaj = ob.etaj;
        pret = ob.pret;
        tip = ob.tip;
        calendar = new int*[13];
        for (int i = 0; i < 13; ++i)
        {
            calendar[i] = new int[32];
            for (int j = 0; j < 32; ++j)
                calendar[i][j] = ob.calendar[i][j];
        }
    }
    ~Camera()
    {
        for (int i = 0; i < 13; ++i)
            delete[] calendar[i];
        delete[] calendar;
    }
    //SETTERI
    void set_availability(int x)
    {
        availability=x;
    }
    void unset_calendar(int luna, int zi)//pentru check-out
    {
        calendar[luna][zi] = 0;
    }
    void set_calendar(int luna, int zi)//pentru check-in
    {
        calendar[luna][zi] = 1;
    }
    void set_pret(int pret_nou)
    {
        pret = pret_nou;
    }
    void set_tip(string tip_nou)
    {
        tip = tip_nou;
    }
    void set_numar(int x)//numarul camerei
    {
        numar = x;
    }
    void set_etaj(int et)
    {
        etaj = et;
    }
    //END SETTERI


    //GETTERI
    int get_availability()
    {
        return availability;
    }
    int get_calendar(int mth, int dd)//imi spune daca acea camera e ocupata in data de dd/mth
    {
        return calendar[mth][dd];
    }
    int getEtaj()
    {
        return etaj;
    }
    int getNumar()
    {
        return numar;
    }
    int getPret()
    {
        return pret;
    }
    string getTip()
    {
        return tip;
    }
    void rezervari()
    {
        cout<<"Camera cu numarul "<<numar<<" este rezervata in urmatoarele zile: ";
        for(int i=1; i<=12; i++)
        {
            for(int j=1; j<=31; j++)
            {
                if(calendar[i][j]==1)
                {
                    cout<<j<<"/"<<i<<" ,";
                }
            }
        }
    }
    bool operator==(Camera c)//pentru a verifica daca doua camere sunt la fel
    {

        if(numar!=c.numar||etaj!=c.etaj||pret!=c.pret||tip!=c.tip)
            return 0;
        for(int i=1; i<=12; ++i)
            for(int j=1; j<=31; ++j)
                if(calendar[i][j]!=c.calendar[i][j])
                    return 0;
        return 1;
    }
    friend std::ostream& operator<<(std::ostream& stream, const Camera& c);
    friend class Receptie;
    friend void ocupa(Camera& cam, int dd, int mth, int stay);
};
std::ostream& operator<<(std::ostream& stream, const Camera& c)
{
    stream << "Camera cu numarul " << c.numar << " este de tip " << c.tip << " si costa " << c.pret << " de euro pe noapte.";
    stream << " Aceasta camera se afla la etajul " << c.etaj << " si este momentan ";
    if (c.availability == 0)
    {
        stream << "disponibila.";
    }
    else if (c.availability == 1)
    {
        stream << "ocupata.";
    }
    stream << '\n';
    return stream;
}
void ocupa(Camera* cam, int dd, int mth, int stay)//pentru a rzerva camera, imi pune calendarul pe 1 in zilele rezervarii
{
    int zile=days_in_month(mth);
    if(dd+stay>zile)
    {
        for(int i=dd; i<=zile; ++i)
        {
            cam->set_calendar(mth, i);
        }
        for(int i=1; i<=dd+stay-zile; ++i)
        {
            if(mth==12)
                cam->set_calendar(1, i);
            else
                cam->set_calendar(mth+1, i);
        }
    }
    else
    {
        for(int i=dd; i<=dd+stay; ++i)
            cam->set_calendar(mth, i);
    }
}
void dezocupa(Camera* cam, int dd, int mth, int stay)//pt check-out, face calendarul 0
{
    int zile=days_in_month(mth);
    if(dd+stay>zile)
    {
        for(int i=dd; i<=zile; ++i)
        {
            cam->unset_calendar(mth, i);
        }
        for(int i=1; i<=dd+stay-zile; ++i)
        {
            if(mth==12)
                cam->unset_calendar(1, i);
            else
                cam->unset_calendar(mth+1, i);
        }
    }
    else
    {
        for(int i=dd; i<=dd+stay; ++i)
            cam->unset_calendar(mth, i);
    }
}
class Client
{
    string nume, prenume, tip_camera;
    int bani_cont, camera, nr_zile, zi, luna;//camera--> adica nr camerei
public:
    Client()
    {
        tip_camera="??";
        zi=0;
        luna=0;
        bani_cont = 0;
        camera = 0;
        nr_zile = 0;
        nume = "??";
        prenume="??";
    }
    Client(string name, int mth, int dd, int bani, int cat_timp, string roomie, string pren)
    {
        nume = name;
        bani_cont = bani;
        nr_zile = cat_timp;
        zi=dd;
        luna=mth;
        tip_camera=roomie;
        camera=0;
        prenume=pren;
    }
    Client(string name, int mth, int dd, int bani, int cat_timp, string roomie, string pren, int nr_cam)
    {
        nume = name;
        bani_cont = bani;
        nr_zile = cat_timp;
        zi=dd;
        luna=mth;
        tip_camera=roomie;
        camera=nr_cam;
        prenume=pren;
    }
    Client(Client const& ob)
    {
        nume = ob.nume;
        camera = ob.camera;
        bani_cont = ob.bani_cont;
        nr_zile = ob.nr_zile;
        zi=ob.zi;
        luna=ob.luna;
        tip_camera=ob.tip_camera;
        prenume=ob.prenume;
    }

    //GETTERI
    string get_prenume()
    {
        return prenume;
    }
    string get_tipulcamerei()
    {
        return tip_camera;
    }
    string get_nume()
    {
        return nume;
    }
    int get_luna()
    {
        return luna;
    }
    int get_zi()
    {
        return zi;
    }
    int get_room()
    {
        return camera;
    }
    int get_money()
    {
        return bani_cont;
    }
    int get_stay()
    {

        return nr_zile;
    }
    //END GETTERI


    //SETTERI
    void set_prenume(string x)
    {
        prenume=x;
    }
    void set_tipulcamerei(string x)
    {
        tip_camera=x;
    }
    void set_name(string x)
    {
        nume=x;
    }
    void set_money(int x)
    {
        bani_cont = x;
    }
    void set_rezerv(int dd, int mm)//pentru a seta ziua si luna de cand sa inceapa rezervarea
    {
        zi = dd;
        luna = mm;
    }
    void set_ziua(int dd)//doar pt zi
    {
        zi = dd;
    }
    void set_luna(int mm)//doar pt luna
    {
        luna = mm;
    }
    void set_roomie(int new_room)//seteaza nr camerei oferite la check-in
    {
        camera = new_room;
    }
    void set_stay(int x)//seteaza nr de zile din sejur
    {
        nr_zile = x;
    }
    void display_info()
    {
        cout << "Clientul se numeste " << nume << " "<< prenume<< " si camera pe numele lui are numarul " << camera << ". Are rezervare pe data de: " << zi<<"/"<<luna << " si are " << bani_cont << " de euro in cont." << '\n';
        cout << "Planuieste sa stea " << nr_zile << " zile." << '\n';
        cout << endl;
    }
    bool operator==(const Client& c)//verifica daca doi clienti sunt identici
    {

        if(bani_cont!=c.bani_cont||camera!=c.camera||c.nr_zile!=nr_zile||nume!=c.nume||zi!=c.zi||luna!=c.luna||tip_camera!=c.tip_camera||prenume!=c.prenume)
            return false;
        return true;
    }
    friend std::istream& operator>>(std::istream& is, Client& c);
    friend std::ostream& operator<<(std::ostream& stream, const Client& c);
};
std::ostream& operator<<(std::ostream& stream, const Client& c)
{
    stream << "Numele clientului este " << c.nume <<" "<<c.prenume<< '\n' << "Acesta a rezervat camera cu numarul " << c.camera << " pentru data de: " << c.zi<<"/"<<c.luna << '\n' << "Durata sejurului este de " << c.nr_zile << " zile" << '\n'<< "Clientul are " << c.bani_cont << " euro in cont." << '\n'<<'\n';
    return stream;
}
std::istream& operator>>(std::istream& is, Client& c)
{
    std::cout << "Cum va numiti? Nume si prenume"<<'\n';
    is >> c.nume>>c.prenume;
    std::cout << "Ce fel de camera v-ati dori sa rezervati? (single, double sau apartament)"<<'\n';
    is>>c.tip_camera;
    std::cout << "Pentru ce data ati vrea sa faceti rezervarea si cat timp ati dori sa stati?" << '\n';
    is >> c.zi >> c.luna >> c.nr_zile;
    std::cout << "Soldul actual in euro: ";
    is >> c.bani_cont;
    return is;
}
class Receptie
{
    Client **clients;//vector de clienti
    Camera ***rooms;//matrice de camere
    int capacitate, nr_etaje, nr_camere;//capacitate--> nr maxim de clienti pe care ii poate avea hotelul simultan
    static int nrclienti;//nr total de clienti din hotel
public:
    Receptie(int cap, int eta, int roomie)
    {
        //cout<<"bip";
        capacitate = cap;
        nr_etaje = eta;
        nr_camere = roomie;
        clients = new Client*[capacitate];
        for(int i=0; i<capacitate; ++i)
            clients[i]=new Client;
        rooms = new Camera**[nr_etaje];
        for (int i = 0; i < nr_etaje; ++i)
        {
            rooms[i] = new Camera*[nr_camere];
            for (int j = 0; j < nr_camere; ++j)
            {
                rooms[i][j] = new Camera();
            }
        }
    }
    Receptie(Receptie const& ob)
    {
        capacitate = ob.capacitate;
        nr_etaje = ob.nr_etaje;
        nr_camere = ob.nr_camere;
        clients=ob.clients;
        rooms=ob.rooms;
    }
    ~Receptie()
    {
        for (int i = 0; i < capacitate; ++i)
        {
            delete clients[i];
        }
        delete[] clients;

        for (int i = 0; i < nr_etaje; ++i)
        {
            for (int j = 0; j < nr_camere; ++j)
            {
                delete rooms[i][j];
            }
            delete[] rooms[i];
        }
        delete[] rooms;
    }

    void citire_camere()//seteaza numarul si etajul camerelor
    {
       // cout<<"BIP";
        for (int i = 0; i < nr_etaje; i++)
        {
            for (int j = 0; j < nr_camere; j++)
            {
                rooms[i][j] = new Camera((i + 1) * 100 + j + 1, i + 1, 0, "??");
                rooms[i][j]->set_availability(1);
                rooms[i][j]->set_etaj(i + 1);
            }
        }
    }
    void afisare_camere()
    {
        for (int i = 0; i < nr_etaje; i++)
        {
            cout << "Etajul " << i + 1 << ": ";
            for (int j = 0; j < nr_camere; j++)
            {
                cout << rooms[i][j]->getNumar() << " ";
            }
            cout << '\n';
        }
    }
    //GETTERI
    int get_capacity()
    {
        return capacitate;
    }
    int get_nretaje()
    {
        return nr_etaje;
    }
    int get_nrcamere()
    {
        return nr_camere;
    }
    int get_clienti()
    {
        return nrclienti;
    }
    Client* find_client(string name, string pren)//gaseste un obiect de tip Client in functie de numele si prenumele oferite
    {
        for (int i = 1; i <= nrclienti; ++i)
        {
            if (clients[i]->get_nume() == name)
            {
                if(clients[i]->get_prenume()==pren)
                    return clients[i];
            }
        }
        return nullptr;
    }
    Client* find_client(int nrcam)//gaseste un obiect de tip Client in functie de numarul camerei
    {
        for(int i=1; i<=nrclienti; ++i)
        {
            if(clients[i]->get_room()==nrcam)
                return clients[i];
        }
        return nullptr;;
    }
    Camera* gasire_camera(string x, int zi, int luna, int per)//gaseste camera cautata in functie de rezervarea facuta si de tipul camerei
    {
        for(int i=0; i<nr_etaje; ++i)
        {
            for(int j=0; j<nr_camere; ++j)
            {
                if(rooms[i][j]->tip==x)
                {
                    if(e_liber(rooms[i][j]->calendar, zi, luna, per)==1)
                    {
                        return rooms[i][j];
                    }
                }
            }
        }
        return nullptr;
    }
    Camera* gasire_camera(int banutzi, int per)//gaseste o camera potrivita in functie de banii clientului si perioada in care vrea sa stea
    {
        for (int i = 0; i < nr_etaje; ++i)
        {
            for (int j = 0; j < nr_camere; ++j)
            {
                if (rooms[i][j]->pret <= banutzi*per)
                {
                    return rooms[i][j];
                }
            }
        }
        return nullptr;
    }
    Camera* gasire_camera(int nr)//gaseste o camera in functie de numarul acesteia
    {
        for (int i = 0; i < nr_etaje; ++i)
        {
            for (int j = 0; j < nr_camere; ++j)
            {
                if (rooms[i][j]->numar == nr)
                {
                    return rooms[i][j];
                }
            }
        }
        return nullptr;
    }

    //END GETTERI


    //SETTERI
    void set_clienti(Client c)//adauga un nou client
    {
        clients[nrclienti]=new Client(c);
    }
    void set_clienti()//creste contorul clientilor
    {
        nrclienti++;
    }
    void set_capacity(int new_cap)
    {
        capacitate = new_cap;
    }
    void rezerva_camera(Client pers, Camera* cam)//face rezervarile
    {
        int pret_total = cam->pret * pers.get_stay();//calculeaza pretul total al sederii, pretul camerei inmultit cu zilele de sedere
        if (pret_total <= pers.get_money())//verifica daca persoana are destui bani pentru a plati
        {
            ocupa(cam, pers.get_zi(), pers.get_luna(), pers.get_stay());//seteaza calendarul camerei la 1 in perioada rezervata
            pers.set_money(pers.get_money() - pret_total);//ia banii din contul clientului
            nrclienti++;
            clients[nrclienti]=new Client(pers);//se adauga un nou client in hotel
            pers.set_roomie(cam->numar);//i se ofera clientului camera
            //cout<<pers.get_nume();
           // file1<<"cmooon";
            //file1<<pers.get_nume()<<" "<<pers.get_prenume()<<'\n';//se afiseaza numele, datele si numarul camerei in fisier pentru a putea retine clientii si intre rulari
            file3<<pers.get_zi()<<" / "<<pers.get_luna()<<" - ";
            calculator2(pers.get_zi(), pers.get_luna(), pers.get_stay());//calculeaza data check-out-ului
            file2<<cam->numar<<'\n';
            file4<<pers.get_money()<<'\n';
            cout << "Felicitari! Rezervarea a fost efectuata" << '\n';
        }
        else//daca persoana nu are destui bani
        {
            if (cam->tip == "single")//cea mai ieftina camera
            {
                cout << "Ne pare rau. Fonduri insuficiente. Va recomandam sa va micsorati perioada de sedere." << '\n';
            }
            else if (cam->tip == "double")//ii ofera drept posibilitate o camera mai ieftina
            {
                cout << "Ne pare rau. Fonduri insuficiente. Va recomandam sa va micsorati perioada de sedere sau sa alegeti o camera de tipul 'single'." << '\n';
            }
            else
            {
                cout << "Ne pare rau. Fonduri insuficiente. Va recomandam sa va micsorati perioada de sedere sau sa alegeti o camera de tipul 'single' sau 'double'." << '\n';
            }
        }
    }
    void rezerva_camera2(Client pers, Camera* cam)// ca prima functie, dar e pentru cand citesc din fisier, ca sa nu mai afiseze mesajul cu felicitari
    {
        ocupa(cam, pers.get_zi(), pers.get_luna(), pers.get_stay());
    }
    void set_tipsipretcamere()//seteaza tipul si pretul camerelor
    {
        //cout<<"BIPP ";
        for (int i = 0; i < nr_etaje / 2; i++)//jumatate din camere sunt single
        {
            for (int j = 0; j < nr_camere; j++)
            {
                rooms[i][j]->set_tip("single");
                rooms[i][j]->set_pret(30);
            }
        }
        for (int i = nr_etaje / 2; i < nr_etaje - 1; i++)//restul de camere sunt double, cu exceptia ultimului etaj care are doar apartamente
        {
            for (int j = 0; j < nr_camere; j++)
            {
                rooms[i][j]->set_tip("double");
                rooms[i][j]->set_pret(50);
            }
        }
        for (int i = 0; i < nr_camere; i++)
        {
            rooms[nr_etaje - 1][i]->set_tip("apartament");
            rooms[nr_etaje - 1][i]->set_pret(60);
        }
       /* for (int i = 0; i < nr_etaje; ++i)
        {
            for (int j = 0; j < nr_camere; ++j)
            {
                cout << *rooms[i][j];
            }
            //file1<<'\n';
        }
        //file1.flush();*/
    }
};
int Receptie::nrclienti=0;

int main()
{

    int x=-1, zi1, mth1, zi2, mth2, cam, ban;//sunt pentru citire din fisier, nu sunt  variabile neaparat relevante
    string n, p, h, nume, prenume, sh1, sh2, lin ;
    Receptie R(10, 3, 5);//initializare receptie
    R.citire_camere();//numarul si etajul camerelor
    R.set_tipsipretcamere();//tipul si pretul lor
    if(!hotel_gol("nume.txt"))//verifica daca aveam rezervari anterioare
    {
        while(file1>>nume>>prenume)//citeste din fisier numele si prenumele clientilor pe rand
        {//aici practic citesc datele din fisier si rezerv camera pentru persoanele aflate in fisier
            R.set_clienti();
            file2>>cam;
            Camera *room;
            room=R.gasire_camera(cam);//gaseste un obiect de tip Camera dupa numarul camerei
            file3>>zi1>>sh1>>mth1>>lin>>zi2>>sh2>>mth2;
            int per=calculator_invers(zi1, mth1, zi2, mth2);//afla perioada de sedere
            file4>>ban;
            Client C(nume, mth1, zi1, ban, per, room->getTip(), prenume, room->getNumar());//constructor Client
           // cout<<C;
            //clients[nrclienti]=C;
            R.set_clienti(C);
            R.rezerva_camera2(C, room);
        }
    }
    cout<<"Buna ziua! Bine ati venit la hotelul 'Nostru'. Cu ce va putem ajuta?"<<'\n';
    cout<<"Pentru a efectua o rezervare, apasati tasta 1."<<'\n'<<"Pentru a face check-in-ul, apasati tasta 2."<<'\n'<<"Pentru check-out, va rugam apasati tasta 3."<<'\n'<<"Pentru iesirea din meniu, apasati tasta 0."<<'\n';
    while(x!=0)//meniu
    {
        cin>>x;//citire optiuni
        if(x==1)
        {
            cout<<"REZERVARI"<<'\n';
            Client A;
            Camera *B;
            cin>>A;//citesc detaliile clientului
            B = R.gasire_camera(A.get_tipulcamerei(), A.get_zi(), A.get_luna(), A.get_stay());//gasesc o camera care sa satisfaca cerintele clientului
            if(B==nullptr)//daca u exista o camera potrivita
            {
                cout<<"Ne cerem scuze, se pare ca nu mai avem acest tip de camere disponibil in perioada selectata. Va rugam sa schimbati perioada sau tipul camerei.";
            }
            cout<<"Doriti sa rezervati aceasta camera?"<<'\n';//in caz ca se razgandeste
            cin>>h;
            if(h=="da")
            {
                R.rezerva_camera(A, B);
                //file1<<"bip";//rezervarea camerei
                cout<<endl;

               // cout<<"Vreti sa continuati in a rezerva si alte camere? Daca da, apasati tasta 1, altfel, apasati tasta 4"<<'\n';

            }
            else
            {
                cout<<"Ne pare rau sa auzim asta. Este altceva ce ati dori sa faceti? Apasati 1 daca doriti sa continuati sa vedeti detaliile camerelor sau 0 daca vreti sa va intoarceti la meniul principal"<<'\n';
            }
            cout<<endl;
         cout<<"Pentru a efectua o rezervare, apasati tasta 1."<<'\n'<<"Pentru a face check-in-ul, apasati tasta 2."<<'\n'<<"Pentru check-out, va rugam apasati tasta 3."<<'\n'<<"Pentru iesirea din meniu, apasati tasta 0."<<'\n';
        }
        else if(x==2)
        {
            cout<<"CHECK-IN"<<'\n';
            cout<<"Va rugam introduceti-va numele pentru verificare si pentru a primi cheia camerei!"<<'\n';
            cin>>n>>p;
            Client *C;
            C=R.find_client(n, p);//gaseste clientul dupa nume si prenume
           // if(now->tm_mday==C->get_zi()||now->tm_mon+1==C->get_luna())
           // {
            if(C!=nullptr)
            {

            Camera *room;
            room=R.gasire_camera(C->get_tipulcamerei(), C->get_zi(), C->get_luna(), C->get_stay());//gaseste camera alocata acelui client
            if(room!=nullptr&&room->getNumar()!=C->get_room())
            {
                cout<<"Check-in reusit! Acestea sunt detaliile camerei dumneavoastra: "<<'\n';
                cout<<"Etaj: "<<room->getEtaj()<<'\n';
                cout<<"Numar: "<<room->getNumar()<<'\n';
                cout<<"Check-out-ul se va face in data de ";
                calculator(C->get_zi(), C->get_luna(), C->get_stay());//calculeaza ziua check-out-ului
                cout<<'\n';
                C->set_roomie(room->getNumar());
                cout<<"Va dorim o sedere cat mai placuta si va mai asteptam la noi!"<<'\n';
            }
            else if(room->getNumar()==C->get_room())//daca a fost facut check-in-ul deja
            {
                cout<<"Check-in deja facut! Camera dumneavoastra are numarul "<<room->getNumar()<<" si se afla la etajul "<<room->getEtaj()<<'\n';
            }
            }
            else//daca nu am gasit clientul si camera
            {
                cout<<"Numele dumneavoastra nu a fost gasit in baza de date a hotelului. Verificati daca ati introdus numele corect sau daca aveti rezervarea facuta pentru aceasta perioada."<<'\n'<<"Va multumim!"<<'\n';
            }
            cout<<endl;
             cout<<"Pentru a efectua o rezervare, apasati tasta 1."<<'\n'<<"Pentru a face check-in-ul, apasati tasta 2."<<'\n'<<"Pentru check-out, va rugam apasati tasta 3."<<'\n'<<"Pentru iesirea din meniu, apasati tasta 0."<<'\n';
        }
            //else
                //cout<<"Ne scuzati, dar inca nu a sosit momentul pentru check-in. Va rugam sa reveniti pe data rezervarii."<<'\n';
       // }
       else if(x==3)
        {
            cout<<"CHECK_OUT"<<'\n';
            cout<<"Va rugam introduceti numarul camerei pentru check-out:"<<'\n';
            int n;
            cin>>n;
            Camera *room;
            Client *pa;
            pa=R.find_client(n);//gaseste clientul dupa numarul camerei
            room=R.gasire_camera(n);//gaseste camera dupa numarul camerei
            if(room!=nullptr)
            {dezocupa(room, pa->get_zi(), pa->get_luna(), pa->get_stay());//seteaza calendarul camerei la 0
            cout<<"Check-out realizat cu succes! Speram ca v-ati simtit bine si va mai asteptam!"<<'\n';
            }
            else
            {
                cout<<"Ne pare rau, nu am gasit aceasta camera. Incercati sa reintroduceti numarul"<<'\n';
            }
            cout<<endl;
             cout<<"Pentru a efectua o rezervare, apasati tasta 1."<<'\n'<<"Pentru a face check-in-ul, apasati tasta 2."<<'\n'<<"Pentru check-out, va rugam apasati tasta 3."<<'\n'<<"Pentru iesirea din meniu, apasati tasta 0."<<'\n';
        }

    }

    file1.close();
    file2.close();
    file3.close();
    file4.close();
    return 0;
}
