#ifndef MY_VECTOR_H
#define MY_VECTOR_H

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <ostream>

namespace my_std {

template < class T >
class Iterator {
   public:
    using iterator_category = std::random_access_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = T;
    using pointer           = T *;
    using reference         = T &;
    // 构造函数
    Iterator( T *ptr ) : ptr_( ptr ) {}

    // 解引用操作符
    reference operator*() const { return *ptr_; }
    pointer operator->() const { return ptr_; }
    // 前缀递增
    Iterator &operator++() {
        ++ptr_;
        return *this;
    }
    // 后缀递增
    Iterator operator++( int ) {
        Iterator temp = *this;
        ++ptr_;
        return temp;
    }
    // 前缀递减
    Iterator &operator--() {
        --ptr_;
        return *this;
    }

    // 后缀递减
    Iterator operator--( int ) {
        Iterator temp = *this;
        --ptr_;
        return temp;
    }

    // 随机访问
    Iterator operator+=( difference_type n ) {
        ptr_ += n;
        return *this;
    }

    Iterator operator-=( difference_type n ) {
        ptr_ -= n;
        return *this;
    }

    Iterator operator+( difference_type n ) const {
        return Iterator( ptr_ + n );
    }
    Iterator operator-( difference_type n ) const {
        return Iterator( ptr_ - n );
    }
    difference_type operator-( const Iterator &other ) const {
        return ptr_ - other.ptr_;
    }

    // 比较操作符
    bool operator==( const Iterator &other ) const {
        return ptr_ == other.ptr_;
    }
    bool operator!=( const Iterator &other ) const {
        return ptr_ != other.ptr_;
    }
    bool operator<( const Iterator &other ) const { return ptr_ < other.ptr_; }
    bool operator>( const Iterator &other ) const { return ptr_ > other.ptr_; }
    bool operator<=( const Iterator &other ) const {
        return ptr_ <= other.ptr_;
    }
    bool operator>=( const Iterator &other ) const {
        return ptr_ >= other.ptr_;
    }

   private:
    T *ptr_;
};

inline int countDigits( int n ) {
    if ( n == 0 ) return 1;
    return static_cast< int >( std::log10( n ) ) + 1;
}

template < class T >
class vector {
   public:
    /**
     * @brief 格式化输出 vector 元素
     */
    void inspect() {
        size_t max = 0;
        for ( size_t i = 0; i < currentSize; i++ ) {
            if ( max < elements[i] ) {
                max = elements[i];
            }
        }

        int maxDigits = countDigits( max );

        for ( size_t i = 0; i < currentSize; i++ ) {
            size_t currentDigits = countDigits( elements[i] );
            std::string placeholder( maxDigits - currentDigits, ' ' );
            std::cout << "index: "
                      << "\033[1;33m" << i << "\033[0m"
                      << " | " << placeholder << elements[i] << " | "
                      << "\033[32m" << &elements[i] << "\033[0m" << std::endl;
        }
    }

    // --------------------------------------------------------------- //
    // 构造与析构函数                                                    //
    // --------------------------------------------------------------- //

    /**
     * @brief 默认构造函数， 创建一个空白的 vector
     */
    vector() : elements( nullptr ), currentSize( 0 ), allocatedCapacity( 0 ){};

    /**
     * @brief 创建一个包含 count 个默认元素的 vector
     */
    vector( T count ) {
        allocatedCapacity = static_cast< size_t >( count * sizeof( T ) );
        currentSize       = static_cast< size_t >( count );
        elements          = new T[allocatedCapacity];
        for ( size_t i = 0; i < count; i++ ) {
            elements[i] = 0;
        }
    }

    /**
     * @brief 用指定个数 count 个值 value 重新填充 vector。
     */
    vector( T count, const T &value ) {
        allocatedCapacity = static_cast< size_t >( count * sizeof( T ) );
        currentSize       = static_cast< size_t >( count );
        elements          = new T[allocatedCapacity];
        for ( size_t i = 0; i < count; i++ ) {
            elements[i] = value;
        }
    }

    /**
     * @brief 复制[first, last) 范围内的元素到 vector 中
     */
    vector( Iterator< T > first, Iterator< T > last ) {
        std::ptrdiff_t newSize = std::distance( first, last );
        currentSize            = newSize;
        allocatedCapacity      = newSize * 2;
        elements               = new T[allocatedCapacity];

        std::copy( first, last, elements );
    }

    /**
     * @brief 通过初始化列表来初始化 vector。
     */
    vector( std::initializer_list< T > init ) {
        size_t newSize     = init.size();
        size_t newCapacity = init.size() * 2;
        T *newElements     = new T[newCapacity];

        std::copy( init.begin(), init.end(), newElements );

        elements          = newElements;
        currentSize       = newSize;
        allocatedCapacity = newCapacity;
    }

    /**
     * @brief 拷贝构造函数，用另一个 vector 初始化
     */
    vector( const vector< T > &other ) {
        currentSize       = other.currentSize;
        allocatedCapacity = other.allocatedCapacity;
        elements          = new T[allocatedCapacity];
        for ( size_t i = 0; i < currentSize; i++ ) {
            elements[i] = other.elements[i];
        }
    }

    /**
     * @brief 移动构造函数，用另一个 vector 移动初始化。
     */
    vector( vector &&other ) noexcept
        : currentSize( other.currentSize ),
          allocatedCapacity( other.allocatedCapacity ),
          elements( other.elements ) {
        other.elements          = nullptr;
        other.currentSize       = 0;
        other.allocatedCapacity = 0;
    }

    /**
     * @brief 析构函数，销毁 vector。
     */
    ~vector() { delete[] elements; }

    // --------------------------------------------------------------- //
    // 赋值操作                                                         //
    // --------------------------------------------------------------- //

    /**
     * @brief 拷贝赋值运算符。
     */
    vector &operator=( const vector &other ) {
        currentSize       = other.currentSize;
        allocatedCapacity = other.allocatedCapacity;
        elements          = new T[other.allocatedCapacity];
        for ( size_t i = 0; i < currentSize; i++ ) {
            elements[i] = other[i];
        }
        return *this;
    }

    /**
     * @brief 移动赋值运算符。
     */
    vector &operator=( vector< T > &&other ) {
        if ( this == &other ) return *this;
        delete[] elements;

        currentSize       = other.size;
        allocatedCapacity = other.capacity;
        elements          = new T[allocatedCapacity];
        for ( size_t i = 0; i < currentSize; i++ ) {
            elements[i] = other.data[i];
        }
        return *this;
    }

    /**
     * @brief 使用初始化列表进行赋值。
     */
    vector &operator=( std::initializer_list< T > ilist ) {
        clear();
        size_t newSizw = ilist.size();

        reserve( newSizw );

        std::copy( ilist.begin(), ilist.end(), elements );
        currentSize       = newSizw;
        allocatedCapacity = newSizw * 2;

        return *this;
    }

    /**
     * @brief 用指定个数 count 个值 value 重新填充 vector。
     */
    void assign( size_t count, const T &value ) {
        if ( count == 0 ) {
            clear();
            return;
        }

        size_t newCapacity = count * 2;
        T *newElements     = new T[allocatedCapacity];

        for ( int i = 0; i < count; i++ ) {
            newElements[i] = value;
        }

        clear();

        elements          = newElements;
        allocatedCapacity = newCapacity;
        currentSize       = count;
    }

    /**
     * @brief 用[first, last) 范围内的元素重新填充 vector。
     */
    void assign( Iterator< T > first, Iterator< T > last ) {
        clear();
        std::ptrdiff_t newSize = std::distance( first, last );
        allocatedCapacity      = newSize * 2;
        currentSize            = newSize;
        delete[] elements;
        elements = new T[allocatedCapacity];

        std::copy( first, last, elements );
    }

    /**
     * @brief 用初始化列表的元素重新填充 vector。
     */
    void assign( std::initializer_list< T > ilist ) {
        size_t newSize     = ilist.size();
        size_t newCapacity = ilist.size() * 2;
        T *newElements     = new T[newCapacity];

        std::copy( ilist.begin(), ilist.end(), newElements );

        clear();

        elements          = newElements;
        allocatedCapacity = newCapacity;
        currentSize       = newSize;
    }

    // --------------------------------------------------------------- //
    // 元素访问                                                         //
    // --------------------------------------------------------------- //

    /**
     * @brief 返回指定位置 pos 处元素的引用，带边界检查
     */

    T &at( size_t pos ) const {
        if ( pos >= currentSize ) {
            throw std::out_of_range( "索引超出范围" );
        }
        return elements[pos];
    }

    /**
     * @brief 返回指定位置 pos 处元素的引用，不带边界检查
     */
    T &operator[]( size_t pos ) const { return elements[pos]; }

    /**
     * 返回第一个元素的引用。
     */
    inline T &front() const { return elements[0]; }

    /**
     * 返回最后一个元素的引用。
     */
    inline T &back() const { return elements[currentSize]; }

    /**
     * 返回指向底层数组的指针
     */
    inline T *data() const { return &elements[0]; }

    // --------------------------------------------------------------- //
    // 迭代器类                                                         //
    // --------------------------------------------------------------- //

    Iterator< T > begin() { return Iterator< T >( elements ); }
    Iterator< T > end() { return Iterator< T >( elements + currentSize ); }

    // --------------------------------------------------------------- //
    // 容量                                                             //
    // --------------------------------------------------------------- //

    /**
     * @brief 判断 vector 是否为空
     */
    inline bool empty() const { return currentSize == 0; }

    /**
     * @brief 获取 vector 大小
     */
    size_t size() const { return currentSize; }

    /**
     * @brief 返回容器能够容纳的最大元素数量
     */
    inline size_t max_size() const {
        return std::numeric_limits< size_t >::max() / sizeof( T );
    }

    /*
     * @brief 获取 vector 容量 字节为单位
     */
    size_t capacity() const { return allocatedCapacity; }

    /**
     * @brief 重新调整 vector 容量
     */
    void reserve( size_t newCapacity ) {
        if ( newCapacity <= allocatedCapacity ) {
            return;
        }

        T *newData = new T[newCapacity];

        for ( size_t i = 0; i < currentSize; ++i ) {
            newData[i] = std::move( elements[i] );
        }

        delete[] elements;
        elements          = newData;
        allocatedCapacity = newCapacity;
    }

    /**
     * @brief 减少内存使用，将 capacity 调整为等于 size。
     */
    inline void shrink_to_fit() {
        if ( currentSize < allocatedCapacity ) {
            T *newData = new T[currentSize];

            for ( int i = 0; i < currentSize; i++ ) {
                newData[i] = std::move( elements[i] );
            }

            delete[] elements;
            elements          = newData;
            allocatedCapacity = currentSize;
        }
    }

    // --------------------------------------------------------------- //
    // 修改操作                                                         //
    // --------------------------------------------------------------- //

    /**
     * @brief 清除 vector 所有元素
     */
    inline void clear() {
        currentSize       = 0;
        allocatedCapacity = 0;
        if ( elements ) {
            delete[] elements;
        }
        elements = new T[allocatedCapacity];
    }

    /**
     * @brief 在 pos 位置前插入 value。
     */
    Iterator< T > insert( Iterator< T > pos, const T &value ) {
        size_t index = pos - elements;

        if ( currentSize == allocatedCapacity ) {
            reserve( allocatedCapacity * 2 );
        }

        for ( size_t i = currentSize; i > index; i-- ) {
            elements[i] = elements[i - 1];
        }

        elements[index] = value;
        ++currentSize;
        return elements + index;
    }

    /**
     * @brief 向 vector 末尾添加元素
     */
    void push_back( const T &value ) {
        if ( currentSize >= allocatedCapacity ) {
            reserve( allocatedCapacity == 0 ? 1 : allocatedCapacity * 2 );
        }
        elements[currentSize++] = value;
    }

    /**
     * @brief 从 vector 末尾移除元素
     */
    void pop_back() {
        if ( currentSize > 0 ) {
            --currentSize;
            elements[currentSize].~T();
        }
    }

   private:
    T *elements;
    size_t currentSize;
    size_t allocatedCapacity;
};

}  // namespace my_std

#endif