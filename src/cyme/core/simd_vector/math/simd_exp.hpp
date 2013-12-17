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

#ifndef CYME_SIMD_EXP_HPP
#define CYME_SIMD_EXP_HPP

#include "core/simd_vector/math/detail/remez.hpp"

namespace numeric{
    template<class T, memory::simd O, std::size_t n>
    struct helper_remez_exp{
        static inline vec_simd<T,O> exp(vec_simd<T,O> const& a){
#ifdef __FMA__
            return muladd(helper_remez_exp<T,O,n-1>::exp(a),a,vec_simd<T,O>(coeff_remez<T,coeff_remez_number::value-n>::coeff()));
#else
            return vec_simd<T,O>(coeff_remez<T,coeff_remez_number::value-n>::coeff()) + helper_remez_exp<T,O,n-1>::exp(a)*a;
#endif
        }
    };

    template<class T, memory::simd O>
    struct helper_remez_exp<T,O,0>{
        static inline vec_simd<T,O> exp(vec_simd<T,O> const& a){
            return vec_simd<T,O>(coeff_remez<T,coeff_remez_number::value>::coeff());
        }
    };

    template<class T, memory::simd O,std::size_t n>
    struct Remez_exp{
        /** fn template<std::size_t T, std::size_t n> Pade_exp  
            \brief clean wrapper of the Remez approximat method
        */
        static inline vec_simd<T,O> exp(vec_simd<T,O> const& a){
            return helper_remez_exp<T,O,n>::exp(a);
        }
    };
    
    template<class T, memory::simd O,std::size_t n>
    struct Pad_exp{
        /** fn template<std::size_t T, std::size_t n> Pade_exp  
            \brief clean wrapper of the Pade approximat method 
        */
        static inline vec_simd<T,O> exp(vec_simd<T,O> const& a){
            std::cout << " Pade, not implemented TO DO " << std::endl;
            vec_simd<T,O> tmp(0.0);
            return tmp;
        }
    };
    

    /** \class template<std::size_t T, std::size_t n, class Solver> exp  
        \brief final wrapper for the exp, pade approximant with n = 14 (maximum value before pb), remez calculate with n=20
    */
    template<class T, memory::simd O, std::size_t n = coeff_remez_number::value, class Solver = Remez_exp<T,O,n> > // Remez, series ...
    struct my_exp{
        static inline vec_simd<T,O> exp(vec_simd<T,O> x){
            /* Load calculate if into correct range */
/* optionnal presently I do not take care of the limite i.e. x < -700 and x > 700
            vec_simd<T,O> minlog(-7.08396418532264106224e2);
            vec_simd<T,O> maxlog(7.09782712893383996843e2);

            x = min(x,maxlog);
            x = max(x,minlog);
*/
            /* calculate k,  k = (int)floor(a); p = (float)k; */
            vec_simd<T,O>   log2e(1.4426950408889634073599);
            vec_simd<T,O>   y(x*log2e);
            vec_simd<int,O> k = floor(y); // k int
            vec_simd<T,O>   p(cast<T,O>(k)); // k float

            /* x -= p * log2; */
            vec_simd<T,O> c1(6.93145751953125E-1);
            vec_simd<T,O> c2(1.42860682030941723212E-6);

#ifdef __FMA__
            x = mulsub(p,c1,x);
            x = mulsub(p,c2,x);
#else
            x -= p*c1;
            x -= p*c2; // this corection I do not know ><
#endif
            /* Compute e^x using a polynomial approximation, remez order 9 by default */
            x = Solver::exp(x);
            /* p = 2^k; */
            p = twok<T,O>(k);
            /* e^x = 2^k * e^y */
            x *= p;
            return x;
        }
    };
    
    template<class T, memory::simd O, std::size_t n>
    struct Vendor_exp{
        static inline vec_simd<T,O> exp(vec_simd<T,O> const& a){
            return exp_v(a); /* call vendor wrapper */
        }
    };
    
    template<class T, memory::simd O, std::size_t n = coeff_remez_number::value, class Solver = my_exp<T,O,n> > // my_exp ou vendor
    struct Selector_exp{
         static inline vec_simd<T,O> exp(vec_simd<T,O> x){
               x = Solver::exp(x);
               return x;
         }
    };
    
    /**  
        \brief final wrapper for the exp, pade approximant with n = 14 (maximum value before pb), remez calculate with n=20
    */
    template<class T,memory::simd O>
    inline vec_simd<T,O> exp(const vec_simd<T,O>& rhs){
        return Selector_exp<T,O>::exp(rhs);
    }
}
#endif

