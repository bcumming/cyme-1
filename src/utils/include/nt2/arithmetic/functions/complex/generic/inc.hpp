//==============================================================================
//         Copyright 2003 - 2011 LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2011 LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================
#ifndef NT2_ARITHMETIC_FUNCTIONS_COMPLEX_GENERIC_INC_HPP_INCLUDED
#define NT2_ARITHMETIC_FUNCTIONS_COMPLEX_GENERIC_INC_HPP_INCLUDED
#include <nt2/arithmetic/functions/inc.hpp>
#include <nt2/include/functions/real.hpp>
#include <nt2/include/functions/imag.hpp>
#include <nt2/include/constants/one.hpp>
#include <nt2/sdk/complex/meta/as_complex.hpp>
#include <nt2/sdk/complex/meta/as_real.hpp>

namespace nt2 { namespace ext
{
  NT2_FUNCTOR_IMPLEMENTATION( nt2::tag::inc_, tag::cpu_, (A0)
                            , (generic_< complex_< arithmetic_<A0> > >)
                            )
  {
    typedef A0 result_type;
    NT2_FUNCTOR_CALL(1)
    {
      typedef typename nt2::meta::as_real<A0>::type rA0;
      return result_type(nt2::real(a0)+One<rA0>(),nt2::imag(a0));
    }
  };

  NT2_FUNCTOR_IMPLEMENTATION( nt2::tag::inc_, tag::cpu_, (A0)
                            , (generic_< dry_< arithmetic_<A0> > >)
                            )
  {
    typedef A0 result_type;
    NT2_FUNCTOR_CALL(1)
    {
      return bitwise_cast<result_type>(inc(nt2::real(a0)));
    }
  };

} }

#endif
