# OracleCCI Backend Reference

SOCI backend for accessing Oracle database using [Oracle C++ Call Interface](https://docs.oracle.com/en/database/oracle/oracle-database/21/lncpp/introduction-to-occi.html).

It is not a backend fully based on OCCI. It is made to start using SOCI in a large system (>1M LOC) with tight dependencies on OCCI classes, so it only uses the minimum from OCCI to start replacing its connections. Hopefully, at the end of the project, there would be no need to use OracleCCI backend and it will be replaced by Oracle backend.

## Prerequisites

OracleCCI Backend it is build on top of [Oracle Backend](oracle.md)

### Supported Versions

See [Oracle Backend](oracle.md).

### Tested Platforms

See [Oracle Backend](oracle.md).

### Required Client Libraries

The SOCI OracleCCI backend requires SOCI Oracle backend and Oracle's `libclntsh` client library. Depending on the particular system, the `libnnz10` library might be needed as well.

Note that the SOCI library itself depends also on `libdl`, so the minimum set of libraries needed to compile a basic client program is:

```console
-lsoci_core -lsoci_oracle -lsoci_oraclecci -ldl -lclntsh -lnnz10
```

### Connecting to the Database

To establish a connection to an Oracle database, create a `session` object using the oraclecci backend factory together with a connection string:

```cpp
session sql(oraclecci, "service=orcl user=scott password=tiger");

// or:
session sql("oraclecci", "service=orcl user=scott password=tiger");

// or:
session sql("oraclecci://service=orcl user=scott password=tiger");

// or:
session sql(oraclecci, "service=//your_host:1521/your_sid user=scott password=tiger");
```

The set of parameters used in the connection string for Oracle is:

* `service`
* `user`
* `password`
* `mode` (optional; valid values are `object`, `no_usercallbacks`, `threaded_mutexed`, `threaded_unmutexed`, `events`, `use_ldap` and `default`)
* `charset` and `ncharset` (optional; valid values can be found in [Oracle Database Reference](https://docs.oracle.com/en/database/oracle/oracle-database/21/nlspg/appendix-A-locale-data.html#GUID-344E34CA-DD16-4979-8CF1-6D0A7AEBAA4A))

If both `user` and `password` are provided, the session will authenticate using the database credentials, whereas if none of them is set, then external Oracle credentials will be used - this allows integration with so called Oracle wallet authentication.

Once you have created a `session` object as shown above, you can use it to access the database, for example:

```cpp
int count;
sql << "select count(*) from user_tables", into(count);
```

(See the [connection](../connections.md) and [data binding](../binding.md) documentation for general information on using the `session` class.)

## SOCI Feature Support

See [Oracle Backend](oracle.md).

## Native API Access

SOCI provides access to underlying datbabase APIs via several `get_backend()` functions, as described in the [Beyond SOCI](../beyond.md) documentation.

The OracleCCI backend provides the following concrete classes for navite API access:

|Accessor Function|Concrete Class|
|--- |--- |
|session_backend * session::get_backend()|oraclecci_session_backend|
|statement_backend * statement::get_backend()|oracle_statement_backend|
|blob_backend * blob::get_backend()|oracle_blob_backend|
|rowid_backend * rowid::get_backend()|oracle_rowid_backend|

Since it is not a backend fully based on OCCI, only session-level classes provides native access to OCCI's `Environment` and `Session` classes. `statement_backend` is not based on OCCI, so there is no access to OCCI's `Statement` class.

## Backend-specific extensions

### oraclecci_soci_error

The OracleCCI backend can throw instances of classes `oraclecci_soci_error` or `oracle_soci_error` which are publicly derived from `soci_error` and have an additional public `err_num_` member containing the Oracle error code:

```cpp
int main()
{
    try
    {
        // regular code
    }
    catch (oracle_soci_error const & e)
    {
        cerr << "Oracle error: " << e.err_num_
            << " " << e.what() << endl;
    }
    catch (exception const & e)
    {
        cerr << "Some other error: "<< e.what() << endl;
    }
}
```
