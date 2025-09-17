#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <ostream>

// PC1: deben hacer:
//      2 problemas de nivel 2
//      3 problemas de nivel 1
// Cada solucion enviarla como un Pull request

// TODO (Nivel 2): Agregar Traits

// TODO (Nivel 2): Agregar Iterators (forward, backward)

// TODO (Nivel 1): Agregar Documentacion para generar con doxygen

// TODO  (Nivel 2): Agregar control de concurrencia en todo el vector
template <typename T>
class CVector{
   
    T      *m_pVect = nullptr;
    size_t  m_count = 0; // How many elements we have now?
    size_t  m_max   = 0; // Max capacity

    double m_growthFactor = 2.0; // For growth delta - TODO (Nivel 1)

public:
    // TODO  (Nivel 1) Agregar un constructor por copia
    CVector(const CVector &v);

    CVector(size_t n);
    // TODO  (Nivel 2): Agregar un move constructor
    CVector(const CVector &&v);

    // TODO: (Nivel 1) implementar el destructor de forma segura
    virtual ~CVector();
    void insert(T &elem);
    void resize();

    T& operator[](size_t index);

    
};

template <typename T>
CVector<T>::CVector(size_t n){

}

template <typename T>
CVector<T>::CVector(const CVector<T>& v)
    : m_count(v.m_count), 
    m_max(v.m_max), 
    m_growthFactor(v.m_growthFactor)
{
    m_pVect = new T[m_max];

    for (size_t i = 0; i < m_count; ++i) {
        m_pVect[i] = v.m_pVect[i];
    }
}

template <typename T>
CVector<T>::CVector(const CVector<T>&& v)
    : m_pVect(v.m_pVect),
      m_count(v.m_count),
      m_max(v.m_max),
      m_growthFactor(v.m_growthFactor)
{
    v.m_count = 0;
    v.m_max = 0;
    v.m_pVect = nullptr;
}

template <typename T>
CVector<T>::~CVector()
{
    if (m_pVect != nullptr) {
        delete[] m_pVect;
        m_pVect = nullptr;
    }

    m_count = 0;
    m_max = 0;

}

// TODO (Nivel 1): hacer dinamico el delta de crecimiento
template <typename T>
void CVector<T>::resize(){

    size_t new_max = (n_max == 0) ? 10 : static_cast<size_t>(n_max * m_growthFactor);
    T *pTmp = new T[new_max];
    for(auto i=0; i < m_max ; ++i)
        pTmp[i] = std::move(m_pVect[i]); // Refactor using std::move
    delete [] m_pVect;
    m_max = new_max
    m_pVect = pTmp;
}

// TODO (ya está hecha): la funcion insert debe permitir que el vector crezca si ha desbordado
template <typename T>
void CVector<T>::insert(T &elem){
    if(m_count == m_max)
        resize();
    m_pVect[m_count++] = elem;
}

// TODO  (Nivel 1) habilitar el uso de []
template <typename T>
T& CVector<T>::operator[](size_t index)
{
    return m_pVect[index];
}


// TODO  (Nivel 2) habilitar que el vector pueda ser escrito con cout <<
template <typename T>
std::ostream& operator<<(std::ostream& os, const CVector<T>& v)
{
    for (size_t i = 0; i < v.m_count; ++i)
    {
        os << v.m_pVect[i] << " ";
    }

    return os;
}

#endif // __VECTOR_H__