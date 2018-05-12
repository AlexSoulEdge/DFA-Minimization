#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

ifstream fin("dfa.txt");

struct tranzitie
{
    int stare1,stare2;
    char alf;
};

//Se citesc n si n stari
void citire_stari(vector <int> &stari_initiale, int &n)
{
    int x;
    fin >> n;
    for(int i = 0; i < n; i ++)
    {
        fin >> x;
        stari_initiale.push_back(x);
    }
}

//se citesc n si n carectere (Ex: 4\n abcd)
void citire_alfabet(vector <char> &alfabet, int &n)
{
    char x;
    fin >> n;
    for(int i = 0; i < n;i ++)
    {
        fin >> x;
        alfabet.push_back(x);
    }
}

//Se cinteste starea initiala
void citire_stare_initiala(int &n)
{
    fin >> n;
}

//Se citeste n si n functii (Ex:1\n 1b2)
void citire_functii(vector <tranzitie> &functii, int &n)
{
    tranzitie x;
    fin >> n;
    for(int i = 0; i < n;i ++)
    {
        fin >> x.stare1 >> x.alf >> x.stare2;
        functii.push_back(x);
    }
}

//Se citesc n si n stari finale
void citire_stari_finale(vector <int> &v, int &n)
{
    int x;
    fin >> n;
    for(int i = 0; i < n; i ++)
    {
        fin >> x;
        v.push_back(x);
    }
}

//Se verifica daca datele sunt valide
bool verificari_stari(vector <int> v1,int n1, vector <int> v2,int n2, vector <tranzitie> v3,int n3,vector <char> v4, int n4)
{
    bool e = 0;
    for(int i = 0; i < n2; i ++)
    {
        e = 0;
        for(int j = 0; j < n1;j ++)
            if(v1[j] == v2[i])
            {
                e = 1;
                break;
            }
        if(e == 0)
        {
            cout << "Una din starile finale este invalida!";
            return false;
        }
    }

    for(int i = 0; i < n3; i ++)
    {
        e = 0;
        for(int j = 0;j < n1;j ++)
            if(v1[j] == v3[i].stare1)
            {
                e = 1;
                break;
            }
        if(e == 0)
        {
            cout << "Una din starile initiale ale functiei nu este valabila!";
            return false;
        }
    }
    for(int i = 0;i < n3;i ++)
    {
        e = 0;
        for(int j = 0; j < n1; j++)
            if(v1[j] == v3[i].stare2)
            {
                e = 1;
                break;
            }
        if(e == 0)
        {
            cout << "Una din starile finale ale functiei nu este valabila!";
            return false;
        }
    }
    for(int i = 0; i < n3;i ++)
    {
        e = 0;
        for(int j = 0; j < n4; j ++)
            if(v4[j] == v3[i].alf)
            {
                e = 1;
                break;
            }
        if(e == 0)
        {
            cout << "Una din alfabetul functiei nu este valabila!";
            return false;
        }
    }
    return true;
}

bool verifica_complet(vector <tranzitie> v, int n, int cate, int totale, vector<int>v2)
{
	char f[100] = {0};
	for(int i = 0; i<n; i++)
        f[v[i].stare1]++;

    for(int i=0;i<totale;i++)if(f[i]!=cate)
        return false;

    return true;
}

void algoritm(vector <int> stari, int nr_stari, vector<int> stari_finale, int nr_stari_finale, vector<tranzitie> functie, int nr_functii, vector <char> alfabet, int nr_alfabet)
{
    int matrice[nr_stari][nr_stari];    // Tabelul pe care vom aplica teorema Myhill-blahblah;

        //initializare matrice in forma piramidala
            for(int i = 0; i < nr_stari;i ++)
                for(int j = 0; j < nr_stari;j ++)
                {
                    if(j >= i)
                        matrice[i][j] = -1;
                    else
                        matrice[i][j] = 0;
                }

    //Marcam toate perechile (stare1,stare2) cu prop ca cele 2 stari nu sunt ambele finale sau nefinale

    char e; // Numara cate din starile din perechea curenta sunt finale. Daca doar 1 e atunci trebuie marcata in tabel;
    for(int i = 0; i < nr_stari; i ++)
        for(int j = 0; j < i; j ++)
        {
            e = 0;
            for(int k = 0; k < nr_stari_finale; k ++)
                if(stari_finale[k] == i || stari_finale[k] == j)
                    e ++;
            if(e == 1)
                matrice[i][j] = 1;
        }

    // Parcurgem toate campurile marcate cu 0, iar pt fiecare litera din alfabet
    // si stari x si y, verificam perechea (f(x,i),f(y,i)). Daca aceasta pereche
    // este marcata in tabel, marcam si perechea initiala (x,y). Algoritmul se
    // opreste cand dupa o parcurgere intreaga de tabel, nu mai marcam nimic

    bool e_marcat;
    do
    {
        e_marcat = false;
        for(int i = 0;i < nr_stari;i ++)
        for(int j = 0;j < i;j ++)
            {
                //verificam daca perechea (i,j) este marcata in tabel,
                if(matrice[i][j] == 0)
                {
                    //generam perechile (f(i,x),f(j,x)), cu x fiecare litera din alfabet, Daca nu a fost marcata pana acum - verificam daca dupa tranzitia cu o litera ajungem intr-o alta stare deja marcata
                    for(int k = 0;k < nr_alfabet;k ++) // cautam pt litera curenta din alfabet, functia asociata lui i si j cu litera x
                    {
                        //perechile rezultate
                        int p1, p2;
                        char litera = alfabet[k];
                        //cautam pt litera curenta din alfabet, functia asociata lui i si j cu litera x
                        for(int x = 0;x < nr_functii; x ++)
                        {
                            if(functie[x].alf == litera && functie[x].stare1 == i)
                                p1 = functie[x].stare2;
                            if(functie[x].alf == litera && functie[x].stare1 == j)
                                p2 = functie[x].stare2;
                        }
                        //vedem daca perechea (f(i,x),f(j,x))este marcata. In caz afirmativ, marcam perechea
                        //(i,j) si facem variabila booleana true, pentru a repeta alg dupa parcurgerea matricei
                        if(matrice[p1][p2] == 1 || matrice[p2][p1] == 1)
                        {
                            matrice[i][j] = 1;
                            e_marcat = true;
                            break;
                        }

                    }
                }
            }

    }while(e_marcat == true);

    //Perechile din tabel care nu sunt marcate, reprezinta starile ce pot fi unite
    // Creem perechile de stari, si le reunim pt a forma o stare mai mare

    //O sa folosim o matrice ptconcatenarea starilor, astfel incat fiecare linie
    //reprezinta o stare, iar fiecare coloana reprezinta o stare din starea initiala

    int matrice_stari[nr_stari][nr_stari], ki, kj, p1, p2, f[100] = {0};

    for(int i = 0; i < nr_stari;i ++)
    for(int j = 0;j < nr_stari;j ++)
        matrice_stari[i][j] = -1;

    for(int i = 0; i < nr_stari;i ++)
    for(int j = 0; j < i;j ++)
        {
            e = false;
            if(matrice[i][j] == 0)
            {
                ki = 0;
                kj = 0;
                p1 = -1;
                p2 = -1;
                for(ki = 0; ki < nr_stari && matrice_stari[ki][0] != -1; ki ++)
                {
                    for(kj = 0; kj < nr_stari && matrice_stari[ki][kj] != -1 ;kj ++)
                    {
                        if(matrice_stari[ki][kj] == i)
                        {
                            p1 = j;
                            e = true;
                        }
                        if(matrice_stari[ki][kj] == j)
                        {
                            p2 = i;
                            e = true;
                        }

                    }
                    if(p1 != -1 && p2 != -1)
                    {
                        continue;
                    }
                    else if(p1 != -1)
                        {
                            matrice_stari[ki][kj] = p1;
                            f[p1] ++;
                        }
                    else
                        {
                            matrice_stari[ki][kj] = p2;
                            f[p2] ++;
                        }
                }
                if(e == false)
                {
                    matrice_stari[ki][0] = i;
                    matrice_stari[ki][1] = j;
                    f[i] ++;
                    f[j] ++;
                }
            }
        }

        for(ki = 0; matrice_stari[ki][0]!= -1; ki++);

        for(int i = 0; i < nr_stari; i++)
            if(f[i] == 0)
            {
                matrice_stari[ki][0] = i;
                ki ++;
            }

        int finale[100] = {0}, i1;
        for(int i = 0; i < nr_stari && matrice_stari[i][0] != -1; i ++)
        for(int j = 0; j < nr_stari && matrice_stari[i][j] != -1; j ++)
        for(int k = 0; k < nr_stari_finale; k ++)
            if(matrice_stari[i][j] == stari_finale[k])
                finale[i] = 1;

        cout << "Starile rezultate sunt: ";
        for(int i = 0; i < nr_stari && matrice_stari[i][0] != -1;i ++)
        {
            for(int j = 0; j < nr_stari && matrice_stari[i][j] != -1; j ++)
                cout << matrice_stari[i][j];
            cout << " ";
        }

        /*cout << endl << endl;
        cout << "Starile initiale sunt: ";
        for(int i = 0; i < nr_stari && matrice_stari[i][0] != -1; i ++)
        {
            for(int j = 0; j < nr_stari && matrice_stari[i][j] != -1; j ++)
                if(finale[i] == 0)
                    cout << matrice_stari[i][j];
            cout << " ";
        }*/

        cout << endl << endl;
        cout << "Starile finale sunt: ";
        bool printed;
        for(int i = 0; i < nr_stari && matrice_stari[i][0] != -1; i ++)
        {
            printed = false;
            for(int j = 0; j < nr_stari && matrice_stari[i][j] != -1; j ++)
                if(finale[i] == 1)
                {
                    cout << matrice_stari[i][j];
                    printed = true;
                }

            if(printed)
                cout << " ";
        }

        cout << endl << endl;
        cout << "Vectorul de tranzitie este: " << endl;
        for(int i = 0; i < nr_stari; i ++)
        for(int k = 0; k < nr_alfabet; k ++)
        for(int j = 0; j < nr_functii; j ++)
            if(matrice_stari[i][0] == functie[j].stare1 && alfabet[k] == functie[j].alf)
            {
                for(int x = 0; matrice_stari[i][x] != -1; x ++)
                    cout << matrice_stari[i][x];
                cout << " " << alfabet[k] << " ";
                e = 0;
                for(i1 = 0; i1 < nr_stari && matrice_stari[i1][0] != -1 && e == 0; i1 ++)
                for(int j1 = 0; j1 < nr_stari && matrice_stari[i1][j1] != -1; j1 ++)
                    if(matrice_stari[i1][j1] == functie[j].stare2)
                    {
                        e = 1;
                        break;
                    }

                for(int j1 = 0; matrice_stari[i1-1][j1] != -1; j1 ++)
                    cout << matrice_stari[i1-1][j1];
                cout << endl;
                break;
            }
}
int main()
{

    int nr_stari_initiale, nr_alfabet, stare_initiala, nr_functii, nr_stari_finale, nr_litere, stare_cuvant;
    vector <int> stari,stari_finale;
    vector <char> alfabet;
    vector <tranzitie> functii;

    citire_stari(stari, nr_stari_initiale);
    citire_alfabet(alfabet, nr_alfabet);
    citire_stare_initiala(stare_initiala);
    citire_functii(functii, nr_functii);
    citire_stari_finale(stari_finale, nr_stari_finale);

    fin.close();


    if(verificari_stari(stari, nr_stari_initiale, stari_finale, nr_stari_finale, functii, nr_functii, alfabet, nr_alfabet) == true)
    {
    	if(verifica_complet(functii, nr_functii, nr_alfabet, nr_stari_initiale, stari) == true)
    	{
            int matrice[nr_stari_initiale][nr_stari_initiale];

            //initializare matrice in forma piramidala
            for(int i = 0; i < nr_stari_initiale; i ++)
            for(int j = 0;j < nr_stari_initiale;j ++)
            {
                if(j >= i)
                    matrice[i][j] = -1;
                else
                    matrice[i][j] = 0;
            }
            algoritm(stari, nr_stari_initiale, stari_finale, nr_stari_finale, functii, nr_functii, alfabet, nr_alfabet);
    	}
        else
            cout << "Incomplet";
    }
    return 0;
}
