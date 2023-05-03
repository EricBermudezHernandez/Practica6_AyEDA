#include <iostream>

#include "arboles.hpp"
#include "nodos.hpp"

int main() {
  #ifdef TRAZA
    std::cout << "MODO TRAZA" << std::endl;
  #endif
  int opcion;
  long clave;
  std::cout << "¿Qué tipo de árbol binario quiere crear? \n 1) ABB \n 2) AVL\n";
  std::cin >> opcion;
  AB<long>* arbol;
  if (opcion == 1) {
    arbol = new ABB<long>{};
  } else if (opcion == 2) {
    arbol = new AVL<long>{};
  } else {
    std::cout << "La opción " << opcion << " es incorrecta" << std::endl;
    return 1;
  }

  do {
    std::cout
        << "Elija una opción:\n[0] Salir\n[1] Insertar clave\n[2] Buscar "
           "clave\n[3] Mostrar árbol inorden\n";
    std::cin >> opcion;
    switch (opcion) {
      case 0:
        break;

      case 1:  // Inserción
        std::cout << "Introduzca el valor de la clave: ";
        std::cin >> clave;
        arbol->Insertar(clave);
        std::cout << "Árbol vacío\nNivel 0: [.]\nInsertar: " << clave << std::endl;
        std::cout << *arbol << std::endl;
        break;

      case 2:  // Búsqueda
        std::cout << "Introduzca el valor de la clave que quiera buscar: ";
        std::cin >> clave;
        if (arbol->Buscar(clave)) {
          std::cout << "Clave encontrada" << std::endl;
        } else {
          std::cout << "Clave no encontrada" << std::endl;
        }
        break;

      case 3:  // Mostrar árbol inorden
        std::cout << "Recorrido inorden del árbol:" << std::endl;
        arbol->Inorden(arbol->GetRaiz());
        std::cout << std::endl;
        break;

      default:
        std::cout << "La opción introducida " << opcion << " es incorrecta"
                  << std::endl;
        break;
    }
  } while (opcion != 0);
  delete arbol; // Liberamos la memoria del árbol una vez acabe el programa
  return 0;
}
