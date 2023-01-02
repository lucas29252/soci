//
// Copyright (C) 2004-2006 Maciej Sobczak, Stephen Hutton
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef SOCI_ORACLECCI_H_INCLUDED
#define SOCI_ORACLECCI_H_INCLUDED

#include <soci/soci-platform.h>

#ifdef SOCI_ORACLECCI_SOURCE
# define SOCI_ORACLECCI_DECL SOCI_DECL_EXPORT
#else
# define SOCI_ORACLECCI_DECL SOCI_DECL_IMPORT
#endif

#include <soci/oracle/soci-oracle.h>
#include <occi.h> // OCCI


namespace soci
{

class SOCI_ORACLECCI_DECL oraclecci_soci_error : public oracle_soci_error
{
public:
    oraclecci_soci_error(std::string const & msg, int errNum = 0);
    oraclecci_soci_error(oracle::occi::SQLException const & e);
};


struct oraclecci_statement_backend;
struct oraclecci_standard_into_type_backend : oracle_standard_into_type_backend
{
    oraclecci_standard_into_type_backend(oracle_statement_backend &st)
        : oracle_standard_into_type_backend(st) {}
};

struct oraclecci_vector_into_type_backend : oracle_vector_into_type_backend
{
    oraclecci_vector_into_type_backend(oracle_statement_backend &st)
        : oracle_vector_into_type_backend(st) {}
};

struct oraclecci_standard_use_type_backend : oracle_standard_use_type_backend
{
    oraclecci_standard_use_type_backend(oracle_statement_backend &st)
        : oracle_standard_use_type_backend(st) {}
};

struct oraclecci_vector_use_type_backend : oracle_vector_use_type_backend
{
    oraclecci_vector_use_type_backend(oracle_statement_backend &st)
        : oracle_vector_use_type_backend(st) {}
};

struct oraclecci_session_backend;
struct oraclecci_statement_backend : oracle_statement_backend
{
    oraclecci_statement_backend(oracle_session_backend &session)
        : oracle_statement_backend(session) {}
};

struct oraclecci_rowid_backend : oracle_rowid_backend
{
    oraclecci_rowid_backend(oracle_session_backend &session)
        : oracle_rowid_backend(session) {}
};

struct oraclecci_blob_backend : oracle_blob_backend
{
    oraclecci_blob_backend(oracle_session_backend& session)
        : oracle_blob_backend(session) {}
};

struct oraclecci_session_backend : oracle_session_backend
{
    oraclecci_session_backend(connection_parameters const & parameters);

    ~oraclecci_session_backend() SOCI_OVERRIDE;

    void begin() SOCI_OVERRIDE;
    void commit() SOCI_OVERRIDE;
    void rollback() SOCI_OVERRIDE;

    std::string get_backend_name() const SOCI_OVERRIDE { return "oraclecci"; }

    void clean_up();

    oraclecci_statement_backend* make_statement_backend() SOCI_OVERRIDE;
    oraclecci_rowid_backend* make_rowid_backend() SOCI_OVERRIDE;
    oraclecci_blob_backend* make_blob_backend() SOCI_OVERRIDE;

    oracle::occi::Environment *env_;
    oracle::occi::Connection *conn_;
};

struct SOCI_ORACLECCI_DECL oraclecci_backend_factory : backend_factory
{
    oraclecci_backend_factory() {}
    oraclecci_session_backend* make_session(
        connection_parameters const& parameters) const SOCI_OVERRIDE;
};

extern SOCI_ORACLECCI_DECL oraclecci_backend_factory const oraclecci;

extern "C"
{

// for dynamic backend loading
SOCI_ORACLECCI_DECL backend_factory const* factory_oraclecci();
SOCI_ORACLECCI_DECL void register_factory_oraclecci();

} // extern "C"

} // namespace soci

#endif // SOCI_ORACLECCI_H_INCLUDED
