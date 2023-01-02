//
// Copyright (C) 2004-2007 Maciej Sobczak, Stephen Hutton
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#define SOCI_ORACLECCI_SOURCE
#include "soci/oraclecci/soci-oraclecci.h"

#ifdef _MSC_VER
#pragma warning(disable:4355)
#endif

using namespace soci;
using namespace soci::details;

oraclecci_soci_error::oraclecci_soci_error(std::string const & msg, int errNum)
    : oracle_soci_error(msg, errNum)
{
}

oraclecci_soci_error::oraclecci_soci_error(oracle::occi::SQLException const & e)
    : oracle_soci_error(e.getMessage(), e.getErrorCode())
{
}
