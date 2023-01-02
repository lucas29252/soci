//
// Copyright (C) 2004-2006 Maciej Sobczak, Stephen Hutton
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#define SOCI_ORACLECCI_SOURCE
#include "soci/oraclecci/soci-oraclecci.h"
#include "soci/connection-parameters.h"
#include "soci/session.h"

#ifdef _MSC_VER
#pragma warning(disable:4355)
#endif

using namespace soci;
using namespace soci::details;


oraclecci_session_backend::oraclecci_session_backend(
    connection_parameters const & parameters)
    : oracle_session_backend("", "", "", OCI_AUTH_RESERVED_3),
      env_(NULL), conn_(NULL)
{
    std::string serviceName;
    std::string userName;
    std::string password;
    std::string charset;
    std::string ncharset;
    oracle::occi::Environment::Mode mode = oracle::occi::Environment::DEFAULT;
    std::string const & connectString = parameters.get_connect_string();
    std::stringstream ssconn(connectString);
    while (!ssconn.eof() && ssconn.str().find('=') != std::string::npos)
    {
        std::string key, val;
        std::getline(ssconn, key, '=');
        std::getline(ssconn, val, ' ');

        if (val.size()>0 && val[0]=='\"')
        {
            std::string quotedVal = val.erase(0, 1);

            if (quotedVal[quotedVal.size()-1] ==  '\"')
            {
                quotedVal.erase(val.size()-1);
            }
            else // space inside value string
            {
                std::getline(ssconn, val, '\"');
                quotedVal = quotedVal + " " + val;
                std::string keepspace;
                std::getline(ssconn, keepspace, ' ');
            }

            val = quotedVal;
        }

        if ("service" == key)
        {
            serviceName = val;
        }
        else if ("user" == key)
        {
            userName = val;
        }
        else if ("password" == key)
        {
            password = val;
        }
        else if (key == "mode")
        {
            if ("default" == val)
            {
                mode = oracle::occi::Environment::DEFAULT;
            }
            else if ("object" == val)
            {
                mode = oracle::occi::Environment::OBJECT;
            }
            else if ("no_usercallbacks" == val)
            {
                mode = oracle::occi::Environment::NO_USERCALLBACKS;
            }
            else if ("threaded_mutexed" == val)
            {
                mode = oracle::occi::Environment::THREADED_MUTEXED;
            }
            else if ("threaded_unmutexed" == val)
            {
                mode = oracle::occi::Environment::THREADED_UNMUTEXED;
            }
            else if ("events" == val)
            {
                mode = oracle::occi::Environment::EVENTS;
            }
            else if ("use_ldap" == val)
            {
                mode = oracle::occi::Environment::USE_LDAP;
            }
            else
            {
                throw soci_error("Invalid connection mode.");
            }
        }
        else if (key == "decimals_as_strings")
        {
            decimals_as_strings_ = val == "1" || val == "Y" || val == "y";
        }
        else if (key == "charset")
        {
            charset = val;
        }
        else if (key == "ncharset")
        {
            ncharset = val;
        }
    }

    try
    {
        if (!charset.empty() && !ncharset.empty())
        {
            env_ = oracle::occi::Environment::createEnvironment(charset, ncharset, mode);
        }
        else
        {
            env_ = oracle::occi::Environment::createEnvironment(mode);
        }

        envhp_ = env_->getOCIEnvironment();

        // create the error handle
        sword res = OCIHandleAlloc(envhp_, reinterpret_cast<dvoid**>(&errhp_),
            OCI_HTYPE_ERROR, 0, 0);
        if (res != OCI_SUCCESS)
        {
            clean_up();
            throw soci_error("Cannot create error handle");
        }

        conn_ = env_->createConnection(userName, password, serviceName);

        srvhp_ = conn_->getOCIServer();
        svchp_ = conn_->getOCIServiceContext();
        usrhp_ = conn_->getOCISession();
    }
    catch(const oracle::occi::SQLException& se)
    {
        clean_up();
        throw oraclecci_soci_error(se);
    }
    catch(const std::exception& e)
    {
        clean_up();
        throw soci_error(e.what());
    }
}

oraclecci_session_backend::~oraclecci_session_backend()
{
    clean_up();
}

void oraclecci_session_backend::begin()
{
    // ...
}

void oraclecci_session_backend::commit()
{
    try
    {
        conn_->commit();
    }
    catch(const oracle::occi::SQLException& se)
    {
        throw oraclecci_soci_error(se);
    }
    catch(const std::exception& e)
    {
        throw soci_error(e.what());
    }
}

void oraclecci_session_backend::rollback()
{
    try
    {
        conn_->rollback();
    }
    catch(const oracle::occi::SQLException& se)
    {
        throw oraclecci_soci_error(se);
    }
    catch(const std::exception& e)
    {
        throw soci_error(e.what());
    }
}

void oraclecci_session_backend::clean_up()
{
    if (env_ && conn_) { env_->terminateConnection(conn_);   }
    if (errhp_) { OCIHandleFree(errhp_, OCI_HTYPE_ERROR); }
    if (env_) { oracle::occi::Environment::terminateEnvironment(env_);   }

    usrhp_ = NULL;
    svchp_ = NULL;
    srvhp_ = NULL;
    envhp_ = NULL;
    errhp_ = NULL;
    conn_ = NULL;
    env_ = NULL;
}

oraclecci_statement_backend * oraclecci_session_backend::make_statement_backend()
{
    return new oraclecci_statement_backend(*this);
}

oraclecci_rowid_backend * oraclecci_session_backend::make_rowid_backend()
{
    return new oraclecci_rowid_backend(*this);
}

oraclecci_blob_backend * oraclecci_session_backend::make_blob_backend()
{
    return new oraclecci_blob_backend(*this);
}
