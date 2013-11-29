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
#ifndef CYME_SERIAL_HPP
#define CYME_SERIAL_HPP

namespace cyme{

    template<class T, memory::order O>
    class serial{
    };


    template<class T>
    struct serial<T,memory::AoS>{
        typedef T value_type;

        serial(T m=T()):a(m){}

        inline serial& operator =(value_type b){
            a = b;
            return *this; 
        }

        inline operator T (){ //implicit conversion operator
            return a;
        }

        inline const value_type& operator ()() const{
            return a;
        }
    
        T a;
    };


    template<class T>
    struct serial<T,memory::AoSoA>{
        typedef T value_type;
        typedef numeric::vec<value_type,memory::__GETSIMD__()> base_type;        

        serial(base_type m = base_type()):a(m){}

        template<class T2, memory::simd O, class Rep>
        serial(numeric::vec<T2,O,Rep > const& rhs){
            a.rep()() = rhs.rep()(); // not a = rhs.rep()() else I call store -> a crash (pointer not initialized)
        }

        inline serial& operator=(base_type b){
            a.rep()() = b.rep()(); 
            return *this; 
        }

        inline operator base_type (){ //implicit conversion operator
            return a;
        }

        inline const base_type& operator ()() const{
            return a;
        }

        base_type a;
    };

}

#endif
