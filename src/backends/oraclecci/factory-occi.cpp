//
// Copyright (C) 2004-2006 Maciej Sobczak, Stephen Hutton
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#define SOCI_ORACLECCI_SOURCE
#include "soci/oraclecci/soci-oraclecci.h"
#include "soci/backend-loader.h"

#ifdef _MSC_VER
#pragma warning(disable:4355)
#endif

using namespace soci;
using namespace soci::details;

// concrete factory for ORACLECCI concrete strategies
oraclecci_session_backend* oraclecci_backend_factory::make_session(
     connection_parameters const& parameters) const
{
     return new oraclecci_session_backend(parameters);
}

oraclecci_backend_factory const soci::oraclecci;

extern "C"
{

// for dynamic backend loading
SOCI_ORACLECCI_DECL backend_factory const* factory_oraclecci()
{
    return &soci::oraclecci;
}

SOCI_ORACLECCI_DECL void register_factory_oraclecci()
{
    soci::dynamic_backends::register_backend("oraclecci", soci::oraclecci);
}

} // extern "C"
