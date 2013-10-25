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

#ifndef CYME_SIMD_WRAPPER_SSE_HPP
#define CYME_SIMD_WRAPPER_SSE_HPP

namespace numeric{

    template<>
    inline  simd_trait<double,memory::sse2>::register_type _mm_load1<double,memory::sse2>( simd_trait<double,memory::sse2>::register_type xmm0, const  simd_trait<double,memory::sse2>::value_type a){
        return (xmm0 =_mm_load1_pd(&a)); 
    }
   
    template<>
    inline  simd_trait<double,memory::sse2>::register_type _mm_load<double,memory::sse2>( simd_trait<double,memory::sse2>::register_type xmm0,  simd_trait<double,memory::sse2>::const_pointer a){
        return (xmm0 =_mm_load_pd(a)); 
    }

    template<>
    void _mm_store<double,memory::sse2>( simd_trait<double,memory::sse2>::register_type xmm0,  simd_trait<double,memory::sse2>::pointer a){
        _mm_store_pd(a,xmm0); 
    }
   
    template<>
    inline  simd_trait<double,memory::sse2>::register_type _mm_mul<double,memory::sse2>( simd_trait<double,memory::sse2>::register_type xmm0,  simd_trait<double,memory::sse2>::register_type xmm1){
        return (xmm0 = _mm_mul_pd(xmm0, xmm1));
    };
   
    template<>
    inline  simd_trait<double,memory::sse2>::register_type _mm_div<double,memory::sse2>( simd_trait<double,memory::sse2>::register_type xmm0,  simd_trait<double,memory::sse2>::register_type xmm1){
        return (xmm0 = _mm_div_pd(xmm0, xmm1));
    };
   
    template<>
    inline  simd_trait<double,memory::sse2>::register_type _mm_add<double,memory::sse2>( simd_trait<double,memory::sse2>::register_type xmm0,  simd_trait<double,memory::sse2>::register_type xmm1){
        return (xmm0 = _mm_add_pd(xmm0, xmm1));
    };

    template<>
    inline  simd_trait<double,memory::sse2>::register_type _mm_sub<double,memory::sse2>( simd_trait<double,memory::sse2>::register_type xmm0,  simd_trait<double,memory::sse2>::register_type xmm1){
        return (xmm0 = _mm_sub_pd(xmm0, xmm1));
    };

    template<>
    inline  simd_trait<double,memory::sse2>::register_type _mm_rec<double,memory::sse2>(simd_trait<double,memory::sse2>::register_type xmm0){
        xmm0 = _mm_cvtpd_ps(xmm0);
        xmm0 = _mm_rcp_ps(xmm0);
        return (xmm0 = _mm_cvtps_pd(xmm0));
    };

#ifdef __INTEL_COMPILER  
    template<>
    inline  simd_trait<double,memory::sse2>::register_type _mm_exp<double,memory::sse2>( simd_trait<double,memory::sse2>::register_type xmm0){
        return (xmm0 = _mm_exp_pd(xmm0));
    };
#endif

#ifdef __FMA__
    template<>
    inline  simd_trait<double,memory::sse2>::register_type _mm_fma<double,memory::sse2>(simd_trait<double,memory::sse2>::register_type xmm0,
                                                                                        simd_trait<double,memory::sse2>::register_type xmm1,
                                                                                        simd_trait<double,memory::sse2>::register_type xmm2){
        return  (xmm0 = _mm_fmadd_pd(xmm0, xmm1, xmm2));
    };

    template<>
    inline  simd_trait<double,memory::sse2>::register_type _mm_fms<double,memory::sse2>(simd_trait<double,memory::sse2>::register_type xmm0,
                                                                                        simd_trait<double,memory::sse2>::register_type xmm1,
                                                                                        simd_trait<double,memory::sse2>::register_type xmm2){
        return  (xmm0 = _mm_fmsub_pd(xmm0, xmm1, xmm2));
    };

    template<>
    inline  simd_trait<double,memory::sse2>::register_type _mm_nfma<double,memory::sse2>(simd_trait<double,memory::sse2>::register_type xmm0,
                                                                                         simd_trait<double,memory::sse2>::register_type xmm1,
                                                                                         simd_trait<double,memory::sse2>::register_type xmm2){
        return  (xmm0 = _mm_fnmadd_pd(xmm0, xmm1, xmm2));
    };
#endif //end FMA
   
    template<>
     simd_trait<float,memory::sse2>::register_type _mm_load1<float,memory::sse2>(simd_trait<float,memory::sse2>::register_type xmm0, const  simd_trait<float,memory::sse2>::value_type a){
        return (xmm0 =_mm_load1_ps(&a)); 
    }
   
    template<>
     simd_trait<float,memory::sse2>::register_type _mm_load<float,memory::sse2>( simd_trait<float,memory::sse2>::register_type xmm0,  simd_trait<float,memory::sse2>::const_pointer a){
        return (xmm0 =_mm_load_ps(a)); 
    }

    template<>
    void _mm_store<float,memory::sse2>( simd_trait<float,memory::sse2>::register_type xmm0,  simd_trait<float,memory::sse2>::pointer a){
        _mm_store_ps(a,xmm0);
    }
   
    template<>
    inline  simd_trait<float,memory::sse2>::register_type _mm_mul<float,memory::sse2>( simd_trait<float,memory::sse2>::register_type xmm0,  simd_trait<float,memory::sse2>::register_type xmm1){
        return (xmm0 = _mm_mul_ps(xmm0, xmm1));
    };
   
    template<>
    inline  simd_trait<float,memory::sse2>::register_type _mm_div<float,memory::sse2>( simd_trait<float,memory::sse2>::register_type xmm0,  simd_trait<float,memory::sse2>::register_type xmm1){
        return (xmm0 = _mm_div_ps(xmm0, xmm1));
    };
   
    template<>
    inline  simd_trait<float,memory::sse2>::register_type _mm_add<float,memory::sse2>( simd_trait<float,memory::sse2>::register_type xmm0,  simd_trait<float,memory::sse2>::register_type xmm1){
        return (xmm0 = _mm_add_ps(xmm0, xmm1));
    };

    template<>
    inline  simd_trait<float,memory::sse2>::register_type _mm_sub<float,memory::sse2>( simd_trait<float,memory::sse2>::register_type xmm0,  simd_trait<float,memory::sse2>::register_type xmm1){
        return (xmm0 = _mm_sub_ps(xmm0, xmm1));
    };

    template<>
    inline simd_trait<float,memory::sse2>::register_type _mm_rec<float,memory::sse2>(simd_trait<float,memory::sse2>::register_type xmm0){
        return (xmm0 = _mm_rcp_ps(xmm0));
    };

#ifdef __INTEL_COMPILER 
    template<>
    inline  simd_trait<float,memory::sse2>::register_type _mm_exp<float,memory::sse2>( simd_trait<float,memory::sse2>::register_type xmm0){
        return (xmm0 = _mm_exp_ps(xmm0));
    };
#endif

#ifdef __FMA__
    template<>
    inline  simd_trait<float,memory::sse2>::register_type _mm_fma<float,memory::sse2>(simd_trait<float,memory::sse2>::register_type xmm0,
                                                                                      simd_trait<float,memory::sse2>::register_type xmm1,
                                                                                      simd_trait<float,memory::sse2>::register_type xmm2){
        return  (xmm0 = _mm_fmadd_ps(xmm0, xmm1, xmm2));
    };

    template<>
    inline  simd_trait<float,memory::sse2>::register_type _mm_fms<float,memory::sse2>(simd_trait<float,memory::sse2>::register_type xmm0,
                                                                                      simd_trait<float,memory::sse2>::register_type xmm1,
                                                                                      simd_trait<float,memory::sse2>::register_type xmm2){
        return  (xmm0 = _mm256_fmsub_ps(xmm0, xmm1, xmm2));
    };

    template<>
    inline  simd_trait<float,memory::sse2>::register_type _mm_nfma<float,memory::sse2>(simd_trait<float,memory::sse2>::register_type xmm0,
                                                                                       simd_trait<float,memory::sse2>::register_type xmm1,
                                                                                       simd_trait<float,memory::sse2>::register_type xmm2){
        return  (xmm0 = _mm256_fnmadd_ps(xmm0, xmm1, xmm2));
    };
#endif //end FMA


} //end namespace 

#endif 
