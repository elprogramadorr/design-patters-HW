#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

class Adoptable {
public:
    virtual string getNombre()=0;
    virtual int getEdad()=0;
    virtual void mostrarInformacionGeneral()=0;
    virtual bool adoptar()=0;
};

class Perro : public Adoptable{
private:
    string nombre;
    int edad;
    bool adoptado;
public:
    Perro(string nombre, int edad) : nombre(nombre), edad(edad), adoptado(false) {}

    string getNombre(){
        return nombre;
    }
    int getEdad(){
        return edad;
    }
    void mostrarInformacionGeneral(){
        cout<<"Nombre: "<<nombre<<endl;
        cout<<"Edad: "<<edad<<endl;
    }
    bool adoptar(){
        adoptado = true;
        return adoptado;
    }
};

class Adoptador {
public:
    virtual string getNombre()=0;
};

class Persona : public Adoptador{
public:
    string nombre;
    int edad;
    int numeroCelular;
    string correo;
    string direccion;
    string getNombre(){
        return nombre;
    }
};

// Patron strategy

class Ordenamiento{
public:
    virtual bool cmp(Adoptable *a, Adoptable* b) = 0;
};

class OrdenarEdad : public Ordenamiento {
public:
    bool cmp(Adoptable *a, Adoptable* b){
        return a->getEdad() < b->getEdad();
    } 
};

class OrdenarNombre : public Ordenamiento {
    bool cmp(Adoptable *a, Adoptable* b){
        return a->getNombre() < b->getNombre();
    } 
};

class Albergue{
private:
    vector<Adoptable*> disponible;
    map<Adoptable*, Adoptador*> adoptados;
    Ordenamiento* estrategia;

public:
    Albergue(Ordenamiento* estrategia): estrategia(estrategia) {}

    void listarDisponibles(){
        sort(disponible.begin(), disponible.end(), [this](Adoptable* a, Adoptable* b){
            return estrategia->cmp(a, b);
        });
        int ind = 1;
        cout<<"disponibles"<<endl;
        for(auto it : disponible){
            cout<<ind++<<endl;
            it->mostrarInformacionGeneral();
        }
    }

    void adoptar(Adoptable* mascota, Adoptador* persona){
        for(int i=0;i<disponible.size();i++){
            if(disponible[i]==mascota){
                if(mascota->adoptar()){
                    cout<<persona->getNombre()<<"  adopto a  "<<mascota->getNombre()<<endl;
                    adoptados[mascota] = persona;
                    disponible.erase(disponible.begin() + i);
                }
            }
        }
    }

    void agregar(Adoptable* mascota){
        disponible.push_back(mascota);
    }
};

int main(){
    Perro* toby = new Perro("Toby", 13);
    Perro* max = new Perro("Max", 5);
    Perro* rex = new Perro("Rex", 8);

    // Strategy pattern
    Albergue refugio(new OrdenarEdad());

    refugio.agregar(toby);
    refugio.agregar(max);
    refugio.agregar(rex);
    refugio.listarDisponibles();

    Persona* juan = new Persona();
    juan->nombre = "Juan";

    refugio.adoptar(max, juan);

    // cout<<"------"<<endl;

    refugio.listarDisponibles();
    
}