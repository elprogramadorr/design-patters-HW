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

// Clase base Decorator
class AdoptableDecorator : public Adoptable {
protected:
    Adoptable* adoptable;
public:
    AdoptableDecorator(Adoptable* a) : adoptable(a) {}
    string getNombre(){ return adoptable->getNombre(); }
    int getEdad(){ return adoptable->getEdad(); }
    void mostrarInformacionGeneral(){ adoptable->mostrarInformacionGeneral(); }
    bool adoptar(){ return adoptable->adoptar(); }
};

// Decorator: Mascota Entrenada
class EntrenadoDecorator : public AdoptableDecorator {
public:
    EntrenadoDecorator(Adoptable* a) : AdoptableDecorator(a) {}
    void mostrarInformacionGeneral() override {
        AdoptableDecorator::mostrarInformacionGeneral();
        cout<<"-Entrenado"<<endl;
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
    Adoptable* maxEntrenado = new EntrenadoDecorator(new Perro("Max", 5));

    Albergue refugio(new OrdenarEdad());

    refugio.agregar(new Perro("Toby", 13));
    refugio.agregar(maxEntrenado);
    refugio.agregar(new Perro("Rex", 8));
    refugio.listarDisponibles();

    Persona* juan = new Persona();
    juan->nombre = "Juan";

    refugio.adoptar(maxEntrenado, juan);

    cout << "------" << endl;
    refugio.listarDisponibles();
    
}