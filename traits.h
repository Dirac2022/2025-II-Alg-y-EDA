#ifndef __TRAITS_H__
#define __TRAITS_H__

#include <functional>

template <typename T, typename _Func>
struct ListTrait{
    using value_type = T;
    using Func       = _Func;
};

template <typename T>
struct AscendingTrait : 
    public ListTrait<T, std::less<T> >{
};

template <typename T>
struct DescendingTrait : 
    public ListTrait<T, std::greater<T> >{
};

template <typename T>
struct BinaryTreeAscTraits {
    using value_type = T;
    using CompareFn = std::less<T>;
};

template <typename T>
struct BinaryTreeDescTraits {
    using value_type = T;
    using CompareFn = std::greater<T>;
};

// --- NUEVO: Traits para Árbol AVL ---
// Necesitamos una declaración anticipada de CAVLNode para poder usarlo en los Traits.
template <typename T> class CAVLNode;

// // Traits para un AVL ordenado ascendentemente
// template <typename T>
// struct AVLAscTraits {
//     using value_type = T;
//     using CompareFn  = std::less<T>;
//     using Node       = CAVLNode<AVLAscTraits<T>>; // El nodo usa el Trait completo
// };

// // Traits para un AVL ordenado descendentemente
// template <typename T>
// struct AVLDescTraits {
//     using value_type = T;
//     using CompareFn  = std::greater<T>;
//     using Node       = CAVLNode<AVLDescTraits<T>>; // El nodo usa el Trait completo
// };



template <typename T>
struct AVLAscTraits : public BinaryTreeAscTraits<T> {};

template <typename T>
struct AVLDescTraits : public BinaryTreeDescTraits<T> {};

#endif // __TRAITS_H__