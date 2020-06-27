

#ifndef JSON_LIKELY
#if defined(__GNUC__) || defined(__clang__)
#define JSON_LIKELY(x) __builtin_expect(!!(x), 1)
#else
#define JSON_LIKELY(x) (x)
#endif
#endif

#ifndef JSON_UNLIKELY
#if defined(__GNUC__) || defined(__clang__)
#define JSON_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define JSON_UNLIKELY(x) (x)
#endif
#endif



template <typename T>
void Swap(T& a, T& b){
    T tmp = a;
    a = b;
    b =tmp;
}