#ifndef ARBOLES_H
#define ARBOLES_H

#define TRAZA

#include <iostream>
#include <queue>
#include <utility>

#include "nodos.hpp"

// Clase genérica abstracta AB
template <class Key>
class AB {
 public:
  // Constructor:
  AB() : raiz_(NULL) {}
  virtual ~AB() { Podar_(raiz_); }
  // Métodos virtuales
  virtual bool Insertar(const Key& k) = 0;
  virtual bool Buscar(const Key& k) const = 0;

  // Getter
  NodoB<Key>* GetRaiz() { return raiz_; }
  NodoB<Key>* &GetRaiz2() { return raiz_; } 

  // Método y sobrecarga de operador << para mostrar por pantalla el árbol
  void Inorden(NodoB<Key>* nodo) const;

  template <class key>
  friend std::ostream& operator<<(std::ostream& os, AB<key>& arbol);

 protected:
  NodoB<Key>* raiz_;

 private:
  void Podar_(NodoB<Key>* nodo);
};

// Mostrar árbol mediante inorden
template <class Key>
void AB<Key>::Inorden(NodoB<Key>* nodo) const {
  if (nodo == NULL)
    return;
  else {
    Inorden(nodo->GetIzquierdo());
    std::cout << nodo->GetDato() << " - ";
    Inorden(nodo->GetDerecho());
  }
}

// Mostrar árbol mediante recorrido por niveles
template <class Key>
std::ostream& operator<<(std::ostream& os, AB<Key>& arbol) {
  std::queue<std::pair<NodoB<Key>*, int>> cola;
  NodoB<Key>* nodo;
  int nivel, nivel_actual{0};
  // Q.insertar(Raiz, 0);
  os << "\nNivel " << nivel_actual << ": ";
  cola.push(std::make_pair(arbol.GetRaiz(), 0));
  while (!cola.empty()) {
    std::pair<NodoB<Key>*, int> par;
    // Q.extraer(nodo, nivel);
    par = cola.front();
    cola.pop();
    nodo = par.first;
    nivel = par.second;
    if (nivel > nivel_actual) {
      nivel_actual = nivel;  // Incremento de nivel
      os << "\nNivel " << nivel_actual << ": ";
    }
    if (nodo != NULL) {
      // Procesar(nodo);
      os << "[" << nodo->GetDato() << "]";
      // Q.insertar(nodo->izdo, nivel+1);
      cola.push(std::make_pair(nodo->GetIzquierdo(), nivel + 1));
      // Q.insertar(nodo->dcho, nivel+1);
      cola.push(std::make_pair(nodo->GetDerecho(), nivel + 1));
    } else {
      std::cout << "[.] ";
    }
  }
  return os;
}

template <class Key>
void AB<Key>::Podar_(NodoB<Key>* nodo) {
  if (nodo == NULL) return;
  Podar_(nodo->GetIzquierdo());  // Podar subarbol izquierdo
  Podar_(nodo->GetDerecho());    // Podar subarbol derecho
  delete nodo;                   // Eliminar nodo
  nodo = NULL;
}

// ========================================================

// Clase ABB
template <class Key>
class ABB : public AB<Key> {
 public:
  // Constructor
  ABB() : AB<Key>() {}

  // Métodos heredados de AB
  bool Insertar(const Key& k);
  virtual bool Buscar(const Key& k) const;

 private:
  // NodoB<Key>* BuscarRama_(NodoB<Key>* nodo, const Key& clave_dada);
  void InsertarRama_(NodoB<Key>*& nodo, const Key& clave_dada);
};

template <class Key>
NodoB<Key>* BuscarRama_(NodoB<Key>* nodo, const Key& clave_dada) {
  if (nodo == NULL) return NULL;  // No se ha encontrado el nodo
  if (clave_dada == nodo->GetDato()) return nodo;  // Se ha encontrado el nodo
  if (clave_dada < nodo->GetDato())
    return BuscarRama_(nodo->GetIzquierdo(), clave_dada);
  return BuscarRama_(nodo->GetDerecho(), clave_dada);
}

template <class Key>
void ABB<Key>::InsertarRama_(NodoB<Key>*& nodo, const Key& clave_dada) {
  if (nodo == NULL) {
    nodo = new NodoB<Key>(clave_dada);
  } else if (clave_dada < nodo->GetDato()) {
    InsertarRama_(nodo->GetIzquierdo(), clave_dada);
  } else {
    InsertarRama_(nodo->GetDerecho(), clave_dada);
  }
}

template <class Key>
bool ABB<Key>::Insertar(const Key& k) {
  if (Buscar(k)) return false;
  InsertarRama_(this->raiz_, k);
  return true;
}

template <class Key>
bool ABB<Key>::Buscar(const Key& k) const {
  if (BuscarRama_(this->raiz_, k) == NULL) return false;
  return true;
}

// ========================================================

// Clase AVL
template <class Key>
class AVL : public ABB<Key> {
 public:
  // Constructor
  AVL() : ABB<Key>() {}
  bool Insertar(const Key& k);

 private:
  void InsertaBal(NodoAVL<Key>*& nodo, NodoAVL<Key>* nuevo, bool& crece);
  void InsertReBalanceaIzda(NodoAVL<Key>*& nodo, bool& crece);
  void InsertReBalanceaDcha(NodoAVL<Key>*& nodo, bool& crece);
  void RotacionII(NodoAVL<Key>*& nodo);
  void RotacionDD(NodoAVL<Key>*& nodo);
  void RotacionID(NodoAVL<Key>*& nodo);
  void RotacionDI(NodoAVL<Key>*& nodo);
};

template <class Key>
void AVL<Key>::RotacionII(NodoAVL<Key>*& nodo) {
  // nodoAVL nodo1 = nodo->izdo;
  NodoAVL<Key>* nodo1 = reinterpret_cast<NodoAVL<Key>*>(nodo->GetIzquierdo());
  // nodo->izdo = nodo1->dcho;
  nodo->GetIzquierdo() = nodo1->GetDerecho();
  // nodo1->dcho = nodo;
  nodo1->GetDerecho() = nodo;
  if (nodo1->GetDato() == 1) {
    // nodo->bal = 0;
    nodo->SetBal(0);
    // nodo1->bal = 0;
    nodo1->SetBal(0);
  } else {  // nodo1-> bal == 0
    // nodo->bal = 1;
    nodo->SetBal(1);
    // nodo1->bal = -1;
    nodo->SetBal(-1);
  }
  nodo = nodo1;
}

template <class Key>
void AVL<Key>::RotacionDD(NodoAVL<Key>*& nodo) {
  // nodoAVL nodo1 = nodo->dcho;
  NodoAVL<Key>* nodo1 = reinterpret_cast<NodoAVL<Key>*>(nodo->GetDerecho());
  // nodo->dcho = nodo1->izdo;
  nodo->GetDerecho() = nodo1->GetIzquierdo();
  // nodo1->izdo = nodo;
  nodo1->GetIzquierdo() = nodo;
  if (nodo->GetBal() == -1) {
    // nodo->bal = 0;
    nodo->SetBal(0);
    // nodo1->bal = 0;
    nodo1->SetBal(0);
  } else {  // nodo1->bal == 0
    // nodo->bal = -1;
    nodo->SetBal(-1);
    // nodo1->bal = 1;
    nodo->SetBal(1);
  }
  nodo = nodo1;
}

template <class Key>
void AVL<Key>::RotacionID(NodoAVL<Key>*& nodo) {
  // nodoAVL* nodo1 = nodo->izdo;
  NodoAVL<Key>* nodo1 = reinterpret_cast<NodoAVL<Key>*>(nodo->GetIzquierdo());
  // nodoAVL* nodo2 = nodo1->dcho;
  NodoAVL<Key>* nodo2 = reinterpret_cast<NodoAVL<Key>*>(nodo1->GetDerecho());
  // nodo->izdo = nodo2->dcho;
  nodo->GetIzquierdo() = reinterpret_cast<NodoAVL<Key>*>(nodo2->GetDerecho());
  // nodo2->dcho = nodo;
  nodo2->GetDerecho() = nodo;
  // nodo1->dcho = nodo2->izdo;
  nodo1->GetDerecho() = reinterpret_cast<NodoAVL<Key>*>(nodo2->GetIzquierdo());
  // nodo2->izdo = nodo1;
  nodo2->GetIzquierdo() = nodo1;
  if (nodo2->GetBal() == -1) {
    // nodo1->bal = 1;
    nodo1->SetBal(1);
  } else {
    // nodo1->bal = 0;
    nodo1->SetBal(0);
  }
  if (nodo2->GetBal() == 1) {
    // nodo->bal = -1
    nodo->SetBal(-1);
  } else {
    // nodo->bal = 0;
    nodo->SetBal(0);
  }
  // nodo2->bal = 0;
  nodo2->SetBal(0);
  nodo = nodo2;
}

template <class Key>
void AVL<Key>::RotacionDI(NodoAVL<Key>*& nodo) {
  // nodoAVL* nodo1 = nodo->dcho;
  NodoAVL<Key>* nodo1 = reinterpret_cast<NodoAVL<Key>*>(nodo->GetDerecho());
  // nodoAVL* nodo2 = nodo1->izdo;
  NodoAVL<Key>* nodo2 = reinterpret_cast<NodoAVL<Key>*>(nodo1->GetIzquierdo());
  // nodo->dcho = nodo2->izdo;
  nodo->GetDerecho() = reinterpret_cast<NodoAVL<Key>*>(nodo2->GetIzquierdo());
  // nodo2->izdo = nodo;
  nodo2->GetIzquierdo() = nodo;
  // nodo1->izdo = nodo2->dcho;
  nodo1->GetIzquierdo() = reinterpret_cast<NodoAVL<Key>*>(nodo2->GetDerecho());
  // nodo2->dcho = nodo1;
  nodo2->GetDerecho() = nodo1;
  if (nodo2->GetBal() == 1) {
    // nodo1->bal = -1;
    nodo1->SetBal(-1);
  } else {
    // nodo1->bal = 0;
    nodo1->SetBal(0);
  }
  if (nodo2->GetBal() == -1) {
    // nodo->bal = 1
    nodo->SetBal(1);
  } else {
    // nodo->bal = 0;
    nodo->SetBal(0);
  }
  // nodo2->bal = 0;
  nodo2->SetBal(0);
  nodo = nodo2;
}

template <class Key>
void AVL<Key>::InsertReBalanceaIzda(NodoAVL<Key>*& nodo, bool& crece) {
  switch (nodo->GetBal()) {
    case -1: {
      nodo->SetBal(0);
      crece = false;
      break;
    }
    case 0: {
      nodo->SetBal(1);
      break;
    }
    case 1: {
      NodoAVL<Key>* nodo1 =
          reinterpret_cast<NodoAVL<Key>*>(nodo->GetIzquierdo());
      if (nodo1->GetBal() == 1) {
#ifdef TRAZA
        std::cout << "Desbalanceo:";
        std::cout << *this << std::endl;
        std::cout<< "==========================";
        std::cout << "\nRotación II en [" << *nodo << "]:";
#endif
        RotacionII(nodo);
      } else {
#ifdef TRAZA
        std::cout << "Desbalanceo:";
        std::cout << *this << std::endl;
        std::cout<< "==========================";
        std::cout << "\nRotación ID en [" << *nodo << "]:";
#endif
        RotacionID(nodo);
      }
      crece = false;
    }
  }
}

template <class Key>
void AVL<Key>::InsertReBalanceaDcha(NodoAVL<Key>*& nodo, bool& crece) {
  switch (nodo->GetBal()) {
    case 1: {
      nodo->SetBal(0);
      crece = false;
      break;
    }
    case 0: {
      nodo->SetBal(-1);
      break;
    }
    case -1:
      NodoAVL<Key>* nodo1 = reinterpret_cast<NodoAVL<Key>*>(nodo->GetDerecho());
      if (nodo1->GetBal() == -1) {
#ifdef TRAZA
        std::cout << "Desbalanceo:";
        std::cout << *this << std::endl;
        std::cout<< "==========================";
        std::cout << "\nRotación DD en [" << *nodo << "]:";
#endif
        RotacionDD(nodo);
      } else {
#ifdef TRAZA
        std::cout << "Desbalanceo:";
        std::cout << *this << std::endl;
        std::cout<< "==========================";
        std::cout << "\nRotación DI en [" << *nodo << "]:";
#endif
        RotacionDI(nodo);
      }
      crece = false;
  }
}

template <class Key>
void AVL<Key>::InsertaBal(NodoAVL<Key>*& nodo, NodoAVL<Key>* nuevo,
                          bool& crece) {
  if (nodo == NULL) {
    nodo = nuevo;
    crece = true;
  } else if (nuevo->GetDato() < nodo->GetDato()) {
    InsertaBal(reinterpret_cast<NodoAVL<Key>*&>(nodo->GetIzquierdo()), nuevo,
               crece);
    if (crece) {
      InsertReBalanceaIzda(nodo, crece);
    }
  } else {
    InsertaBal(reinterpret_cast<NodoAVL<Key>*&>(nodo->GetDerecho()), nuevo,
               crece);
    if (crece) {
      InsertReBalanceaDcha(nodo, crece);
    }
  }
}

template <class Key>
bool AVL<Key>::Insertar(const Key& k) {
  NodoAVL<Key>* nuevo = new NodoAVL<Key>(k, 0);
  bool crece{false};
  std::cout << "Insertar: " << k << std::endl;
  InsertaBal(reinterpret_cast<NodoAVL<Key>*&>(this->GetRaiz2()), nuevo, crece);
  return true;
}

#endif
