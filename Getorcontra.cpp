#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include <algorithm>
using namespace std;

// Funciones de cifrado y descifrado
static bool Cipher(string input, string oldAlphabet, string newAlphabet, string &output) {
    output = "";
    int inputLen = input.size();

    if (oldAlphabet.size() != newAlphabet.size())
        return false;

    for (int i = 0; i < inputLen; ++i) {
        int oldCharIndex = oldAlphabet.find(tolower(input[i]));

        if (oldCharIndex >= 0)
            output += isupper(input[i]) ? toupper(newAlphabet[oldCharIndex]) : newAlphabet[oldCharIndex];
        else
            output += input[i];
    }

    return true;
}

static bool Encipher(string input, string cipherAlphabet, string &output) {
    string plainAlphabet = "abcdefghijklmnopqrstuvwxyz";
    return Cipher(input, plainAlphabet, cipherAlphabet, output);
}

static bool Decipher(string input, string cipherAlphabet, string &output) {
    string plainAlphabet = "abcdefghijklmnopqrstuvwxyz";
    return Cipher(input, cipherAlphabet, plainAlphabet, output);
}

// Clase para las entradas de contraseñas
class EntradaPassword {
private:
    string url;
    string usuario;
    string contra;
    string notas;

public:
    EntradaPassword() = default;

    EntradaPassword(const string& _url, const string& _usuario, const string& _contra, const string& _notas)
        : url(_url), usuario(_usuario), contra(_contra), notas(_notas) {}

    string aCadena() const {
        return url + "," + usuario + "," + contra + "," + notas;
    }

    void desdeCadena(const string& datos) {
        size_t pos1 = datos.find(",");
        size_t pos2 = datos.find(",", pos1 + 1);
        size_t pos3 = datos.find(",", pos2 + 1);

        url = datos.substr(0, pos1);
        usuario = datos.substr(pos1 + 1, pos2 - pos1 - 1);
        contra = datos.substr(pos2 + 1, pos3 - pos2 - 1);
        notas = datos.substr(pos3 + 1);
    }

    string getUrl() const { return url; }
    string getUsuario() const { return usuario; }
    string getContra() const { return contra; }
    string getNotas() const { return notas; }
};

// Clase para gestionar las contraseñas
class GestorPasswords {
private:
    vector<EntradaPassword> entradas;
    const string rutaArchivo = "InfoEncriptxd.txt";
    const string alfabetoCifrado = "zyxwvutsrqponmlkjihgfedcba";

    string cifrarTexto(const string& texto) const {
        string textoCifrado;
        if (Encipher(texto, alfabetoCifrado, textoCifrado)) {
            return textoCifrado;
        } else {
            cerr << "Error de cifrado: " << texto << endl;
            return "";
        }
    }

    string descifrarTexto(const string& textoCifrado) const {
        string textoDescifrado;
        if (Decipher(textoCifrado, alfabetoCifrado, textoDescifrado)) {
            return textoDescifrado;
        } else {
            cerr << "Error de cifrado: " << textoCifrado << endl;
            return "";
        }
    }

public:
    void agregarPassword(const EntradaPassword& entrada) {
        entradas.push_back(entrada);
        guardarPasswords(); // Guardar automáticamente.
    }

    void mostrartodoPasswords() const {
        if (entradas.empty()) {
            cout << "Vacio D:" << endl;
            return;
        }

        for (const auto& entrada : entradas) {
            cout << "URL: " << entrada.getUrl() << endl;
            cout << "Usuario: " << entrada.getUsuario() << endl;
            cout << "Contraseña: " << entrada.getContra() << endl;
            cout << "Notas: " << entrada.getNotas() << endl;
            cout <<endl<< " --- --- --- --- --- --- --- --- --- --- --- --- --- --- " << endl;
        }
    }

    void mostrarUrls() const {
        if (entradas.empty()) {
            cout << "No hay URL's guardadas." << endl;
            return;
        }
        for (const auto& entrada : entradas) {
            cout << "URL: " << entrada.getUrl() << endl;
        }
    }

    void buscarContraDeUrl(const string& url) const {
        for (const auto& entrada : entradas) {
            if (entrada.getUrl() == url) {
                cout << "URL: " << url << endl <<"Contraseña: "<< entrada.getContra() << endl;
                return;
            }
        }
        cout << "No se encontró ninguna contraseña de la URL" << endl;
    }

    void mostrarNotasDeUrl(const string& url) const {
        for (const auto& entrada : entradas) {
            if (entrada.getUrl() == url) {
                cout << "URL: " << url << endl <<"Notas: "<< entrada.getNotas() << endl;
                return;
            }
        }
        cout << "No se encontraron notas de la URL" << endl;
    }

    void guardarPasswords() const {
        ofstream archivo(rutaArchivo, ios::out);
        if (!archivo.is_open()) {
            cerr << "Error al abrir el archivo para guardar D:" << endl;
            return;
        }

        for (const auto& entrada : entradas) {
            string datosCifrados = cifrarTexto(entrada.aCadena());
            archivo << datosCifrados << endl;
        }

        archivo.close();
    }

    void borrarUrl(const string& url) {
        auto it = remove_if(entradas.begin(), entradas.end(),
                            [&](const EntradaPassword& entrada) { return entrada.getUrl() == url; });

        if (it != entradas.end()) {
            entradas.erase(it, entradas.end());
            guardarPasswords(); // Guardar cambios en el archivo.
            cout << "Se elimino la URL: " << url << endl;
        } else {
            cout << "No se encontró la URL" << endl;
        }
    }

    void editarUrl(const string& url) {
        for (auto& entrada : entradas) {
            if (entrada.getUrl() == url) {
                string nuevaContra, nuevasNotas, nuevoUsuario;
                cout << "Ingrese el nuevo usuario para la URL '" << url << "': ";
                getline(cin, nuevoUsuario);
                cout << "Ingrese la nueva contraseña para la URL '" << url << "': ";
                getline(cin, nuevaContra);
                cout << "Ingrese las nuevas notas para la URL '" << url << "': ";
                getline(cin, nuevasNotas);

                entrada = EntradaPassword(url, nuevoUsuario, nuevaContra, nuevasNotas);
                guardarPasswords();
                cout << "¡Actualización exitosa!" << endl;
                return;
            }
        }
        cout << "No se encontró la URL" << endl;
    }


    void cargarPasswords() {
        ifstream archivo(rutaArchivo, ios::in);
        if (!archivo.is_open()) {
            cerr << "No se encontró el archivo, creando... " << endl;
            return;
        }

        string linea;
        while (getline(archivo, linea)) {
            string datosDescifrados = descifrarTexto(linea);
            if (!datosDescifrados.empty()) {
                EntradaPassword entrada;
                entrada.desdeCadena(datosDescifrados);
                entradas.push_back(entrada);
            }
        }

        archivo.close();
        cout << "Archivo cargado con exito" << endl;
    }
};

int main() {
    GestorPasswords gestor;
    gestor.cargarPasswords();

    int opcion;

    while(1){ //menu
        cout <<endl << "Gestor de Contraseñas" << endl;
        cout << "1. Agregar nueva contraseña" << endl;
        cout << "2. Mostrar todo" << endl;
        cout << "3. Mostrar URLs" << endl;
        cout << "4. Buscar contraseña por URL" << endl;
        cout << "5. Mostrar notas por URL" << endl;
        cout << "6. Borrar URL" << endl;
        cout << "7. Editar información de URL" << endl;
        cout << "8. Salir" << endl;
        cout << "Seleccione una opción: ";
        cin >> opcion;
        cout<<endl;
        cin.ignore();

        if (opcion == 1) { //ingreso de datos
            string url, usuario, contraseña, notas;
            cout << "Ingrese la URL: ";
            getline(cin, url);
            cout << "Ingrese el usuario: ";
            getline(cin, usuario);
            cout << "Ingrese la contraseña: ";
            getline(cin, contraseña);
            cout << "Ingrese notas: ";
            getline(cin, notas);
            gestor.agregarPassword(EntradaPassword(url, usuario, contraseña, notas));
        } else if (opcion == 2) {
            gestor.mostrartodoPasswords(); 
        }else if (opcion ==3){
            gestor.mostrarUrls();
        }else if (opcion == 4){
            string url;
            cout << "Ingrese la URL para buscar su contraseña: ";
            getline(cin, url);
            gestor.buscarContraDeUrl(url);
        }else if (opcion == 5){
            string url;
            cout << "Ingrese la URL para buscar sus notas: ";
            getline(cin, url);
            gestor.mostrarNotasDeUrl(url);
        }else if (opcion == 6) {
            string url;
            cout << "Ingrese la URL para borrar todos los datos relacionados: ";
            getline(cin, url);
            gestor.borrarUrl(url);
        } else if (opcion == 7) {
            string url;
            cout << "Ingrese la URL para editar su contraseña y notas: ";
            getline(cin, url);
            gestor.editarUrl(url);
        }else if (opcion == 8){
            cout<<endl<<"Saliendo..."<<endl;
            return 0;
        }else if (opcion<=0 || opcion>8){
            cout<<"Ingrese una opcion valida >:v"<<endl;
        }
    }
    return 0;
}