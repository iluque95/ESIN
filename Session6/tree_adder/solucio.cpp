#include <iostream>
#include <cstddef>
using namespace std;

template <typename T>
class Arbre
{
private:
  Arbre() : _arrel(NULL){};
  struct node
  {
    T info;
    node *primf;
    node *seggerm;
  };
  node *_arrel;
  static node *copia_arbre(node *p);
  static void destrueix_arbre(node *p) throw();

  // My private methods.
  //------------------------------------------------------------------------------------------
  static void add_childs(node *a1, node *a2);
  static void add_brothers(node *a1, node *a2);
  //------------------------------------------------------------------------------------------

public:
  // Construeix un Arbre format per un únic node que conté a x.
  Arbre(const T &x);

  // Tres grans.
  Arbre(const Arbre<T> &a);
  Arbre &operator=(const Arbre<T> &a);
  ~Arbre() throw();

  // Col·loca l'Arbre donat com a primer fill de l'arrel de l'arbre sobre el que s'aplica el mètode i l'arbre a queda invalidat; després de fer b.afegir_fill(a), a no és un arbre vàlid.
  void afegir_fill(Arbre<T> &a);

  // My public methods.
  //------------------------------------------------------------------------------------------
  Arbre operator+(const Arbre<T> &a);
  // Retorna un arbre que és la suma del p.i. i l’arbre a que crida al mètode privat recursiu
  static node *suma_arbres(node *a1, node *a2);
  // Retorna el punter a l’arrel de la suma dels arbres apuntats per a1 i a2.
  void preordre() const;
  // Escriu una línia amb el recorregut en preordre de l’arbre general
  // Cada element ha de sortir precedit d’un espai que crida al mètode privat recursiu
  static void preordre(node *n);
  // Escriu una línia amb el recorregut en preordre de l’arbre general
  // que apunta n. Cada element ha de sortir precedit d’un espai.
  //------------------------------------------------------------------------------------------

  // Iterador sobre arbre general.
  friend class iterador;
  class iterador
  {
  public:
    friend class Arbre;

    // Construeix un iterador no vàlid.
    iterador() throw();

    // Retorna el subarbre al que apunta l'iterador; llança un error si l'iterador no és vàlid.
    Arbre<T> arbre() const;

    // Retorna l'element del node al que apunta l'iterador o llança un error si l'iterador no és vàlid.
    T operator*() const;

    // Retorna un iterador al primogenit del node al que apunta; llança un error si l'iterador no és vàlid.
    iterador primogenit() const;

    // Retorna un iterador al següent germà del node al que apunta; llança un error si l'iterador no és vàlid.
    iterador seg_germa() const;

    // Operadors de comparació.
    bool operator==(const iterador &it) const
    {
      return _p == it._p;
    };
    bool operator!=(const iterador &it) const
    {
      return _p != it._p;
    };
    static const int IteradorInvalid = 410;

  private:
    Arbre<T>::node *_p;
  };

  // Retorna un iterador al node arrel de l'Arbre (un iterador no vàlid si l'arbre no és vàlid).
  iterador arrel() const throw();

  // Retorna un iterador no vàlid.
  iterador final() const throw();

  static const int ArbreInvalid = 400;
};

// La còpia es fa seguint un recorregut en preordre.
template <typename T>
typename Arbre<T>::node *Arbre<T>::copia_arbre(node *p)
{
  node *aux = NULL;
  if (p != NULL)
  {
    aux = new node;
    try
    {
      aux->info = p->info;
      aux->primf = aux->seggerm = NULL;
      aux->primf = copia_arbre(p->primf);
      aux->seggerm = copia_arbre(p->seggerm);
    }
    catch (...)
    {
      destrueix_arbre(aux);
    }
  }
  return aux;
}

// La destrucció es fa seguint un recorregut en postordre.
template <typename T>
void Arbre<T>::destrueix_arbre(node *p) throw()
{
  if (p != NULL)
  {
    destrueix_arbre(p->primf);
    destrueix_arbre(p->seggerm);
    delete p;
  }
}

// Construcció d'un arbre que conté un sol element x a l'arrel.
template <typename T>
Arbre<T>::Arbre(const T &x)
{
  _arrel = new node;
  try
  {
    _arrel->info = x;
    _arrel->seggerm = NULL;
    _arrel->primf = NULL;
  }
  catch (...)
  {
    delete _arrel;
    throw;
  }
}

template <typename T>
Arbre<T>::Arbre(const Arbre<T> &a)
{
  _arrel = copia_arbre(a._arrel);
}

template <typename T>
Arbre<T> &Arbre<T>::operator=(const Arbre<T> &a)
{
  Arbre<T> tmp(a);
  node *aux = _arrel;
  _arrel = tmp._arrel;
  tmp._arrel = aux;
  return *this;
}

template <typename T>
Arbre<T>::~Arbre() throw()
{
  destrueix_arbre(_arrel);
}

template <typename T>
void Arbre<T>::afegir_fill(Arbre<T> &a)
{
  if (_arrel == NULL or a._arrel == NULL or
      a._arrel->seggerm != NULL)
  {
    throw ArbreInvalid;
  }
  a._arrel->seggerm = _arrel->primf;
  _arrel->primf = a._arrel;
  a._arrel = NULL;
}

template <typename T>
Arbre<T> Arbre<T>::operator+(const Arbre<T> &a)
{
  Arbre<T> b(*this);
  b._arrel = suma_arbres(b._arrel, a._arrel);

  return b;
}

template <typename T>
void Arbre<T>::add_childs(node *a1, node *a2)
{
  if (a1->primf == NULL and a2->primf != NULL)
  {
    a1->primf = new node;
    a1->primf->primf = a1->primf->seggerm = NULL;
    a1->primf->info = a2->primf->info;
    add_childs(a1->primf, a2->primf);
    add_brothers(a1->primf, a2->primf);
  }
}

template <typename T>
void Arbre<T>::add_brothers(node *a1, node *a2)
{
  if (a1->seggerm == NULL and a2->seggerm != NULL)
  {
    a1->seggerm = new node;
    a1->seggerm->primf = a1->seggerm->seggerm = NULL;
    a1->seggerm->info = a2->seggerm->info;
    add_brothers(a1->seggerm, a2->seggerm);
    add_childs(a1->seggerm, a2->seggerm);
  }
}

template <typename T>
typename Arbre<T>::node *Arbre<T>::suma_arbres(node *a1, node *a2)
{
  if (a1 != NULL and a2 != NULL)
  {

    suma_arbres(a1->primf, a2->primf);
    suma_arbres(a1->seggerm, a2->seggerm);

    a1->info += a2->info;

    add_childs(a1, a2);
    add_brothers(a1, a2);
  }

  return a1;
}

template <typename T>
void Arbre<T>::preordre() const
{
  preordre(_arrel);
  cout << endl;
}

template <typename T>
void Arbre<T>::preordre(node *n)
{
  if (n != NULL)
  {
    cout << " " << n->info;
    preordre(n->primf);
    preordre(n->seggerm);
  }
}

template <typename T>
typename Arbre<T>::iterador Arbre<T>::arrel() const throw()
{
  iterador it;
  it._p = _arrel;
  return it;
}

template <typename T>
typename Arbre<T>::iterador Arbre<T>::final() const throw()
{
  return iterador();
}

template <typename T>
Arbre<T>::iterador::iterador() throw() : _p(NULL)
{
}

template <typename T>
T Arbre<T>::iterador::operator*() const
{
  if (_p == NULL)
  {
    throw IteradorInvalid;
  }
  return _p->info;
}

template <typename T>
typename Arbre<T>::iterador Arbre<T>::iterador::primogenit() const
{
  if (_p == NULL)
  {
    throw IteradorInvalid;
  }
  iterador it;
  it._p = _p->primf;
  return it;
}

template <typename T>
typename Arbre<T>::iterador Arbre<T>::iterador::seg_germa() const
{
  if (_p == NULL)
  {
    throw IteradorInvalid;
  }
  iterador it;
  it._p = _p->seggerm;
  return it;
}

template <typename T>
Arbre<T> Arbre<T>::iterador::arbre() const
{
  if (_p == NULL)
  {
    throw IteradorInvalid;
  }
  Arbre<T> a;
  a._arrel = _p;
  Arbre<T> aux(a);
  a._arrel = NULL;
  return aux;
}
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

typedef vector<int> VE;

struct Node
{
    int valor;
    VE fill;
};

// Llegeix un arbre i el desa a un tros del vector v començant a la posició j.
// Modifica la variable j perque apunti a la següent posició lliure de v.
// Retorna la posició dins de v de l'arrel del (sub)arbre llegit.

int arbre(int &j, vector<Node> &v)
{
    int a = j, f;

    ++j;

    cin >> v[a].valor >> f;
    v[a].fill = VE(f);

    for (int i = 0; i < f; ++i)
        v[a].fill[i] = arbre(j, v);

    return a;
}

Arbre<unsigned int> mount(const vector<Node> &v, unsigned int pos)
{
    // Case base
    if (v[pos].fill.size() == 0)
    {
        return Arbre<unsigned int>(v[pos].valor);
    }
    else
    {
        // Construct tree from the last element to first and construct from right to left.

        Arbre<unsigned int> a(v[pos].valor);

        for (int i = v[pos].fill.size() - 1; i >= 0; --i)
        {
            Arbre<unsigned int> b(mount(v, v[pos].fill[i]));
            a.afegir_fill(b);
        }

        return a;
    }
}

Arbre<unsigned int> mount(const vector<Node> &v)
{
    unsigned int pos = 0;

    return mount(v, pos);
}

int main()
{
    int n, j = 0;

    // General trees can not be empty, so to can declare them the solution is use pointers.
    Arbre<unsigned int> *a, *b;

    // Do twice
    for (int i = 0; i < 2; ++i)
    {
        // Get total number of nodes of the tree.
        cin >> n;

        vector<Node> v(n);

        // Store tree
        arbre(j, v);

        // Build trees
        if (i == 0)
            a = new Arbre<unsigned int>(mount(v));
        else
            b = new Arbre<unsigned int>(mount(v));

        j = 0;
    }

    *a = *a + *b;
    a->preordre();

    delete a;
    delete b;
}