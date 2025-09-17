#ifndef __FOREACH_H__
#define __FOREACH_H__

template <typename Container, typename Func>
void foreach(Container &container, Func fn){
    auto iter = container.begin();
    for( ; iter != container.end() ; ++iter)
        fn( *iter );
}

#endif // __FOREACH_H__
