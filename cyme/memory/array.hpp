/*
 * CYME, License
 * 
 * Timothee Ewart - Swiss Federal Institute of technology in Lausanne 
 * 
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 * 
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#ifndef CYME_ARRAY_HPP
#define CYME_ARRAY_HPP

#include "memory/detail/simd.h"
#include "memory/detail/array.hpp" // copy of boost::array + align
#include "memory/detail/storage.hpp"

namespace memory{

    /** \cond I do not need this part in the doc*/
    // declaration for the partial specialization onlye
    // M = #element into the mechanism, N = #number of synapse, Order SoA or SoAoS
    // or M size of block_a, N number of block_a
    template<class T, std::size_t M, std::size_t N, memory::order O>
    class block_a{
    };
    /* \endcond */

    /**
        \brief Block of the memory (partial specialization) is instantiated following  AoS layout
        T is the type, M the size of the subblock_a and N the total number of subblock_a. For AoS  specialization
        datas are contiguous block_a after block_a. I just instantiate a boost::array of block_a
    */
    template<class T, std::size_t M, std::size_t N>
    class block_a<T,M,N,AoS> : public boost::array<storage<T,M,AoS>,N> {
    public:
        typedef std::size_t                    size_type;
        typedef T                              value_type; 
        typedef value_type&                    reference;
        typedef const value_type&              const_reference;
        typedef storage<T,M,AoS>               storage_type;
        typedef boost::array<storage_type,N>   base_type; //default template seems impossible on partial specialization
        typedef typename  base_type::iterator  iterator;

        /**
        \brief Default constructor, the block_a is set up to 0
        */
        explicit block_a(){
            std::fill(base_type::begin(),base_type::end(),storage_type());
        }

        /**
        \brief set up the block_a to the desired value
        */
        block_a(value_type value){
            std::fill(base_type::begin(),base_type::end(),storage_type(value));
        }

        /**
        \brief return the value of the block_a i, element j, write only
        */
        inline reference operator()(size_type i, size_type j){
            BOOST_ASSERT_MSG( i < N, "out of range: block_a AoS i" );
            BOOST_ASSERT_MSG( j < M, "out of range: block_a AoS j" );
            return base_type::operator[](i)(j);
        }

        /**
        \brief return the value of the subblock_a i, element j, read only
        */
        inline const_reference operator()(size_type i, size_type j) const{
            BOOST_ASSERT_MSG( i < N, "out of range: block_a AoS i" );
            BOOST_ASSERT_MSG( j < M, "out of range: block_a AoS j" );
            return base_type::operator[](i)(j);
        }

        /**
        \brief return the size of basic subblock_a 
        */
        static inline size_type size_block() {
            return M;
        }

    };

    /**
        \brief Block of the memory (partial specialization) is instantiated following  AoSoS layout
        T is the type, M the size of the subblock_a and N the total number of subblock_a. For AoS  specialization
        datas are contiguous and interleaved. The boost::array is instantiated but the size depends of the SIMD registers.
        It is calculated during compile time.
    */
    template<class T, std::size_t M, std::size_t N>
    class block_a<T,M,N,AoSoA> : public boost::array<storage<T,unroll_factor::N*__GETSIMD__()/sizeof(T)*M,AoSoA>, N/(unroll_factor::N*__GETSIMD__()/sizeof(T))+1>{
    public:
        typedef std::size_t                                               size_type;
        typedef T                                                         value_type;
        typedef value_type&                                               reference;
        typedef const value_type&                                         const_reference;
        typedef storage<T,unroll_factor::N*__GETSIMD__()/sizeof(T)*M,AoSoA>                storage_type;
        typedef boost::array<storage_type,N/(unroll_factor::N*__GETSIMD__()/sizeof(T))+1>  base_type; //default template seems impossible on partial specialization
        typedef typename  base_type::iterator                             iterator;

        /**
        \brief Default constructor, the block_a is set up to 0
        */
        explicit block_a(){
            for(size_type i(0); i<N/(unroll_factor::N*__GETSIMD__()/sizeof(T))+1; ++i)
                base_type::operator[](i) = storage_type(); // fill up to 0
        }

        /**
        \brief set up the block_a to the desired value
        */
        block_a(value_type value){
            for(size_type i(0); i<N/(unroll_factor::N*__GETSIMD__()/sizeof(T))+1; ++i)
                base_type::operator[](i) = storage_type(value); // fill up to value
        }

        /**
        \brief return the value of the block_a i, element j, write only
        */
        inline reference operator()(size_type i, size_type j){
            BOOST_ASSERT_MSG( i < N, "out of range: block_a AoSoA i" );
            BOOST_ASSERT_MSG( j < M, "out of range: block_a AoSoA j" );
            // Please tune me ! (does it exist an alternative to this ? ^_^
            return base_type::operator[]((i*M+j)/(M*unroll_factor::N*__GETSIMD__()/sizeof(T)))                         //(i)
                                        (j*(unroll_factor::N*__GETSIMD__()/sizeof(T)) + i%(unroll_factor::N*__GETSIMD__()/sizeof(T)));  //(j)
        }

        /**
        \brief return the value of the block_a i, element j, read only
        */
        inline const_reference operator()(size_type i, size_type j) const{
            BOOST_ASSERT_MSG( i < N, "out of range: block_a AoSoA i" );
            BOOST_ASSERT_MSG( j < M, "out of range: block_a AoSoA j" );
            // Please tune me ! (does it exist an alternative to this ? ^_^
            return base_type::operator[]((i*M+j)/(M*unroll_factor::N*__GETSIMD__()/sizeof(T))) //(i)
                                        (j*(unroll_factor::N*__GETSIMD__()/sizeof(T)) + i%(unroll_factor::N*__GETSIMD__()/sizeof(T)));  //(j)
        }

        /**
        \brief return the size of basic subblock_a 
        */
        static inline size_type size_block() {
            return M;
        }

        static inline size_type size(){
            return N;
        }
    };
}

namespace cyme {
    /**
     \brief  This class facilitates the creation of an array of synapses (or whatever), the condition the class
     must encapsulate the basic type (value_type) and the size (value_size) of the basic object 
    */
    template<class T, std::size_t N, memory::order O>
    class array : public memory::block_a<typename T::value_type, T::value_size, N, O>{
    public:
        const static memory::order order_value = O;
        typedef typename T::value_type value_type;

        explicit array(const value_type value = value_type())
        :memory::block_a<value_type, T::value_size, N, O>(value){
        }
    };
}

namespace test{

template<class T, std::size_t N, memory::order O>
class array{};

template<class T, std::size_t N>
class array<T,N,memory::AoS>{
public:
    const static memory::order order_value = memory::AoSoA;
    typedef std::size_t size_type;
    typedef typename    T::value_type value_type;
    typedef value_type&                                               reference;
    typedef const value_type&                                         const_reference;
    typedef memory::storage<value_type,T::value_size,memory::AoS>              storage_type;
    typedef boost::array<storage_type,N>   base_type; //default template seems impossible on partial specialization

    typedef typename base_type::iterator iterator;

    array(value_type value=value_type()){
        std::fill(data.begin(),data.end(),storage_type(value));
    }

    iterator begin(){
        return this->data.begin();
    }

    iterator end(){
        return this->data.end();
    }

    inline storage_type& operator [](int i){
        return this->data[i];
    }

    const inline storage_type& operator [](int i) const{
        return this->data[i];
    }

    static inline size_type size_block() {
        return T::value_size;
    }

    inline size_type size() {
        return this->data.size();
    }

    inline reference operator()(size_type i, size_type j){
        BOOST_ASSERT_MSG( i < base_type::size(), "out of range: block_v AoS i" );
        BOOST_ASSERT_MSG( j < T::value_size, "out of range: block_v AoS j" );
        return this->data[i](j);
    }

    inline const_reference operator()(size_type i, size_type j) const{
        BOOST_ASSERT_MSG( i < base_type::size(), "out of range: block_v AoS i" );
        BOOST_ASSERT_MSG( j < T::value_size, "out of range: block_v AoS j" );
        return this->data[i](j);
    }

private:
    base_type data;
};


template<class T, std::size_t N>
class array<T,N,memory::AoSoA>{
public:
    const static memory::order order_value = memory::AoSoA;
    typedef std::size_t size_type;
    typedef typename    T::value_type value_type;
    typedef value_type&                                               reference;
    typedef const value_type&                                         const_reference;
    static const size_type  storage_width = N/(memory::unroll_factor::N*memory::__GETSIMD__()/sizeof(T))+1;
    typedef memory::storage<T,memory::unroll_factor::N*memory::__GETSIMD__()/sizeof(T)*T::value_size,memory::AoSoA>                storage_type;
    typedef boost::array<storage_type,storage_width>  base_type; //default template seems impossible on partial specialization

    typedef typename base_type::iterator iterator;

    array(value_type value=value_type()){
        for(size_type i(0); i<storage_width; ++i)
            data[i] = storage_type(); // fill up to 0
    }

    iterator begin(){
        return this->data.begin();
    }

    iterator end(){
        return this->data.end();
    }

    inline  storage_type& operator [](int i){
        return this->data[i];
    }

    const inline  storage_type& operator [](int i) const{
        return this->data[i];
    }

    static inline size_type size_block() {
        return T::value_size;
    }

    inline size_type size() {
        return this->data.size();
    }

    inline reference operator()(size_type i, size_type j){
        // nothing on i as the original size is destroyed in the constructor
        // BOOST_ASSERT_MSG( i < size(), "out of range: block_v AoS i" );
        BOOST_ASSERT_MSG(     j < T::value_size, "out of range: block_v AoSoA j" );
        // Please tune me ! (does it exist an alternative to this ? ^_^
        return this->data[(i*T::value_size+j)/storage_width] //(i)
        (j*(memory::unroll_factor::N*memory::__GETSIMD__()/sizeof(value_type)) + i%(memory::unroll_factor::N*memory::__GETSIMD__()/sizeof(value_type)));      //(j)
    }

    inline const_reference operator()(size_type i, size_type j) const{
        // nothing on i as the original size is destroyed in the constructor
        BOOST_ASSERT_MSG(     j < T::value_size, "out of range: block_v AoSoA j" );
        // Please tune me ! (does it exist an alternative to this ? ^_^
        return this->data[(i*T::value_size+j)/(T::value_size*memory::unroll_factor::N*memory::__GETSIMD__()/sizeof(value_type))] //(i)
        (j*(memory::unroll_factor::N*memory::__GETSIMD__()/sizeof(value_type)) + i%(memory::unroll_factor::N*memory::__GETSIMD__()/sizeof(value_type)));      //(j)
    }

private:
    base_type data;
    size_type size_cyme;
};

}

#endif
