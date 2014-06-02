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
#ifndef CYME_VECTOR_HPP
#define CYME_VECTOR_HPP

#include <vector>
#include "memory/detail/simd.h"
#include "memory/allocator.hpp"
#include "memory/detail/storage.hpp"

namespace cyme{

    template<class T, memory::order O>
    class vector{};

    template<class T>
    class vector<T,memory::AoS>{
    public:
        const static memory::order order_value = memory::AoSoA;
        typedef std::size_t size_type;
        typedef typename    T::value_type value_type;
        typedef value_type&                                               reference;
        typedef const value_type&                                         const_reference;
        typedef memory::storage<value_type,T::value_size,memory::AoS>              storage_type;
        typedef std::vector<storage_type, memory::Allocator<storage_type> >  base_type; //default template seems impossible on partial specialization
        typedef typename base_type::iterator iterator;
        typedef typename base_type::const_iterator const_iterator;

        vector(const size_t size=1, value_type value=value_type())
        :data(size,value){
        }

        vector(vector& v):data(v.size()){
            std::copy(v.begin(),v.end(),this->begin());
        }

        void resize(size_type size){
            this->data.resize(size);
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
            BOOST_ASSERT_MSG( i < data.size(), "out of range: block_v AoS i" );
            BOOST_ASSERT_MSG( j < T::value_size, "out of range: block_v AoS j" );
            return this->data[i](j);
        }

        inline const_reference operator()(size_type i, size_type j) const{
            BOOST_ASSERT_MSG( i < data.size(), "out of range: block_v AoS i" );
            BOOST_ASSERT_MSG( j < T::value_size, "out of range: block_v AoS j" );
            return this->data[i](j);
        }

    private:
        base_type data;
    };


    template<class T>
    class vector<T,memory::AoSoA>{
    public:
        const static memory::order order_value = memory::AoSoA;
        typedef std::size_t size_type;
        typedef typename    T::value_type value_type;
        typedef value_type&                                               reference;
        typedef const value_type&                                         const_reference;
        static const size_type  storage_width = memory::unroll_factor::N*memory::__GETSIMD__()/sizeof(value_type)*T::value_size;
        typedef memory::storage<value_type,storage_width,memory::AoSoA>  storage_type;
        typedef std::vector<
        storage_type,
        memory::Allocator<storage_type>
        > base_type;
        typedef typename base_type::iterator iterator;
        typedef typename base_type::const_iterator const_iterator;

        vector(const size_t size=1  , value_type value=value_type())
        :data(size/(memory::unroll_factor::N*memory::__GETSIMD__()/sizeof(value_type))+1,value),size_cyme(size){
        }
        
        vector(vector& v):data(v.size()),size_cyme(v.cyme_size()){
            std::copy(v.begin(),v.end(),this->begin());
        }
        
        void resize(size_type size){
            this->data.resize(size/(memory::unroll_factor::N*memory::__GETSIMD__()/sizeof(value_type))+1);
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
        
        inline size_type cyme_size() {
            return this->size_cyme;
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
