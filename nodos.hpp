#ifndef NODOS_H
#define NODOS_H

#include <iostream>

// Clase Genérica Nodo Binario
template <class Key>
class NodoB {
 public:
  // Constructor
  NodoB(const Key& k, NodoB<Key>* izquierdo = NULL, NodoB<Key>* derecho = NULL)
      : dato_(k), izquierdo_(izquierdo), derecho_(derecho) {}
  // Getters
  Key GetDato() const { return dato_; }
  NodoB<Key>* GetIzquierdo() const { return izquierdo_; }
  NodoB<Key>* &GetIzquierdo() { return izquierdo_; }

  NodoB<Key>* GetDerecho() const { return derecho_; }
  NodoB<Key>* &GetDerecho() { return derecho_; }

 protected:
  Key dato_;
  NodoB<Key>* izquierdo_;
  NodoB<Key>* derecho_;
};

template<class Key>
std::ostream& operator<<(std::ostream& os, const NodoB<Key>& nodo) {
  return os << nodo.GetDato();
}

// Clase Nodo AVL
template <class Key>
class NodoAVL : public NodoB<Key> {
 public:
  // Constructor:
  NodoAVL(const Key& k, NodoB<Key>* izquierdo = NULL,
          NodoB<Key>* derecho = NULL, int factor_balanceo = 0)
      : NodoB<Key>(k, izquierdo, derecho), factor_balanceo_(factor_balanceo) {}

  int GetBal() const { return factor_balanceo_; }
  void SetBal(const int factor_balanceo) { factor_balanceo_ = factor_balanceo; }

 private:
  int factor_balanceo_;  // Factor de balanceo del nodo
};

#endif
