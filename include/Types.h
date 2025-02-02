/*
 * Types.h
 *
 *  Created on: Jul 24, 2012
 *      Author: dzhou
 */


#ifndef TYPES_H_
#define TYPES_H_

#include <cfloat>
#include <limits.h>
#include <string>

namespace dolphindb {

#define CONSTANT_VOID 0
#define CONSTANT_BOOL 1
#define CONSTANT_CHAR 2
#define CONSTANT_SHORT 3
#define CONSTANT_INT 4
#define CONSTANT_LONG 5

#define CONSTANT_DATE 6
#define CONSTANT_MONTH 7
#define CONSTANT_TIME 8
#define CONSTANT_MINUTE 9
#define CONSTANT_SECOND 10
#define CONSTANT_DATETIME 11
#define CONSTANT_TIMESTAMP 12
#define CONSTANT_NANOTIME 13
#define CONSTANT_NANOTIMESTAMP 14

#define CONSTANT_FLOAT 15
#define CONSTANT_DOUBLE 16

#define CONSTANT_SYMBOL 17
#define CONSTANT_STRING 18
#define CONSTANT_UUID 19
#define CONSTANT_DOUBLE_ENUM 20

#define CONSTANT_DATEHOUR 28
#define CONSTANT_DATEMINUTE 29
#define CONSTANT_IP 30
#define CONSTANT_INT128 31

    const double MC_PI = 3.1415926535897932384626433832795028841971693994L;
    const double MC_E = 2.7182818284590452353602874713526624977572470937L;
    const float FLT_ONE_LIMIT = 0.9999999;
    const double DBL_ONE_LIMIT = 0.9999999999999;
    const float FLT_NMIN = -FLT_MAX;
    const double DBL_NMIN = -DBL_MAX;
    const int MAX_SHARED_OBJ_INDEX = 65536;
    const int MAX_FREEABLE_OBJ_COUNT = 4096;
    const size_t MAX_ARRAY_BUFFER = 1677721600;
    const int MAX_PEICE_FOR_PEICEWISE = 16;
    const int MAX_POLYNOMIAL_ORDER = 128;
    const int MAX_ROWS_FOR_MATRIX_COMPUTING = 8192;
    const std::string functionKeyword = "def";
    const std::string aggregationKeyword = "defg";
    const std::string mapreduceKeyword = "mapr";

    const int ARRAY_TYPE_BASE = 64;

    enum DATA_TYPE {
        DT_VOID,
        DT_BOOL,
        DT_CHAR,
        DT_SHORT,
        DT_INT,
        DT_LONG,
        DT_DATE,
        DT_MONTH,
        DT_TIME,
        DT_MINUTE,
        DT_SECOND,
        DT_DATETIME,
        DT_TIMESTAMP,
        DT_NANOTIME,
        DT_NANOTIMESTAMP,
        DT_FLOAT,
        DT_DOUBLE,
        DT_SYMBOL,
        DT_STRING,
        DT_UUID,
        DT_FUNCTIONDEF,
        DT_HANDLE,
        DT_CODE,
        DT_DATASOURCE,
        DT_RESOURCE,
        DT_ANY,
        DT_COMPRESS,
        DT_DICTIONARY,
        DT_DATEHOUR,
        DT_DATEMINUTE,
        DT_IP,
        DT_INT128,
        DT_BLOB,
        DT_DECIMAL,
        DT_COMPLEX,
        DT_POINT,
        DT_DURATION,
        DT_DECIMAL32,
        DT_DECIMAL64,
        DT_DECIMAL128,
        DT_OBJECT,
        DT_PYNONE,
        DT_COUNT,
        DT_VOID_ARRAY = ARRAY_TYPE_BASE,
        DT_BOOL_ARRAY,
        DT_CHAR_ARRAY,
        DT_SHORT_ARRAY,
        DT_INT_ARRAY,
        DT_LONG_ARRAY,
        DT_DATE_ARRAY,
        DT_MONTH_ARRAY,
        DT_TIME_ARRAY,
        DT_MINUTE_ARRAY,
        DT_SECOND_ARRAY,
        DT_DATETIME_ARRAY,
        DT_TIMESTAMP_ARRAY,
        DT_NANOTIME_ARRAY,
        DT_NANOTIMESTAMP_ARRAY,
        DT_FLOAT_ARRAY,
        DT_DOUBLE_ARRAY,
        DT_SYMBOL_ARRAY,
        DT_STRING_ARRAY,
        DT_UUID_ARRAY,
        DT_FUNCTIONDEF_ARRAY,
        DT_HANDLE_ARRAY,
        DT_CODE_ARRAY,
        DT_DATASOURCE_ARRAY,
        DT_RESOURCE_ARRAY,
        DT_ANY_ARRAY,
        DT_COMPRESS_ARRAY,
        DT_DICTIONARY_ARRAY,
        DT_DATEHOUR_ARRAY,
        DT_DATEMINUTE_ARRAY,
        DT_IP_ARRAY,
        DT_INT128_ARRAY,
        DT_BLOB_ARRAY,
        DT_DECIMAL32_ARRAY = 101,
        DT_DECIMAL64_ARRAY,
        DT_OBJECT_ARRAY
    };

    const int TYPE_COUNT = DT_COUNT;

    enum DATA_CATEGORY {
        NOTHING,
        LOGICAL,
        INTEGRAL,
        FLOATING,
        TEMPORAL,
        LITERAL,
        SYSTEM,
        MIXED,
        BINARY,
        COMPLEX,
        ARRAY,
        DENARY
    };

    enum class VECTOR_TYPE {
        ARRAY,
        BIGARRAY,
        SUBVECTOR,
        INDEXVECTOR,
        REPVECTOR,
        IOTVECTOR,
        ARRAYVECTOR,
        OTHER
    };


    /**
 * The order of the enumerated DATA_FORM items matters. In Constant class, we use 4 bits rather than a enum variable to
 * indicate the data form of the constant object. In other words, the value of each DATA_FORM instance is hard coded, i.e.
 * DF_SCALAR = 0
 * DF_VECTOR = 1
 * DF_PAIR = 2
 * DF_MATRIX = 3
 * DF_SET = 4
 * DF_DICTIONARY = 5
 * DF_TABLE = 6
 * DF_CHART = 7
 * DF_CHUNK = 8
 *
 * In case we have to change the value, please review the constructor of Constant, Vector, Set, Dictionary, and Table.
 */
    enum DATA_FORM {
        DF_SCALAR,
        DF_VECTOR,
        DF_PAIR,
        DF_MATRIX,
        DF_SET,
        DF_DICTIONARY,
        DF_TABLE,
        DF_CHART,
        DF_CHUNK
    };

    enum WORD_TYPE {
        KEYWORD,
        ENUM,
        CONSTANT,
        VARIABLE,
        FUNCNAME,
        GLOBALVARIABLE,
        OPERATOR,
        FUNCPATTERN,
        ASSIGNMENT,
        REFERENCE,
        BRACKET,
        DELIMITOR,
        COMMA,
        NAMESPACE,
        UNKNOWN
    };

    enum STATEMENT_TYPE {
        IF,
        DO,
        FOR,
        ASSIGN,
        ATTRASSIGN,
        MULTIASSIGN,
        GLOBALASSIGN,
        PROCEDURE,
        RETURN,
        CONTINUE,
        BREAK,
        EMPTY,
        ADHOC,
        SHOWTIME,
        INSERT,
        UPDATE,
        DEL,
        ASSERT,
        ANNOTATE,
        TRYCATCH,
        THROW,
        MODULE,
        USE,
        SHARE,
        INCLUDE,
        JITHEADER,
        MAX_STMT_TYPES
    };

    enum OBJECT_TYPE {
        CONSTOBJ,
        VAR,
        GLOBAL,
        ATTR,
        DIM,
        TUPLE,
        FUNCTION,
        EXPRESSION,
        COLUMN,
        COLUMNDEF,
        SQLQUERY,
        TABLEJOINER,
        VIRTUALCONST,
        MAPPEDCOL,
        GLOBALTABLE,
        GROUPTASK,
        DIMTABLE,
        MAX_OBJECT_TYPES
    };

    enum TABLE_TYPE {
        BASICTBL,
        REALTIMETBL,
        SNAPTBL,
        FILETBL,
        CHUNKTBL,
        JOINTBL,
        SEGTBL,
        ALIASTBL,
        COMPRESSTBL,
        LOGROWTBL,
        MVCCTBL,
        WIDETBL,
        DIMTBL,
        SNAPDIMTBL,
        CUSTOMIZEDTBL,
        IPCTBL,
        MAX_TABLE_TYPES
    };

    enum IO_ERR {
        OK,
        DISCONNECTED,
        NODATA,
        NOSPACE,
        TOO_LARGE_DATA,
        INPROGRESS,
        INVALIDDATA,
        END_OF_STREAM,
        READONLY,
        WRITEONLY,
        NOTEXIST,
        CORRUPT,
        NOT_LEADER,
        OTHERERR
    };

    enum CONSOLE_TYPE {
        STD,
        HTTP,
        TERMINAL,
        API,
        API2,
        JSON,
        BATCHJOB,
        UNSUPPORTED
    };

    enum HANDLE_TYPE {
        FILE_HANDLE,
        SOCKET_HANDLE,
        REMOTE_HANDLE,
        DB_HANDLE
    };

    enum FUNCTIONDEF_TYPE {
        SYSFUNC,
        SYSPROC,
        OPTRFUNC,
        USERDEFFUNC,
        PARTIALFUNC,
        DYNAMICFUNC,
        PIECEWISEFUNC
    };

    enum FUNCTIONCALL_TYPE {
        THREADCALL,
        REGULARCALL,
        TEMPLATECALL
    };

    enum CHART_TYPE {
        CT_AREA,
        CT_BAR,
        CT_COLUMN,
        CT_HISTOGRAM,
        CT_LINE,
        CT_PIE,
        CT_SCATTER
    };

    enum OUTPUT_TYPE {
        STDOUT,
        HTTPOUT,
        APIOUT,
        LOGOUT,
        JOBOUT
    };

    enum PARTITION_TYPE {
        SEQ,
        VALUE,
        RANGE,
        LIST,
        HIER,
        HASH
    };

    enum SERVER_TYPE {
        DATANODE,
        AGENT,
        CONTROLLER,
        SINGLE
    };

    enum TranCmdType {
        NEW_FILE,
        REPLACE_FILE,
        UPDATE_HEADER_APPEND,
        APPEND_FILE,
        UPDATE_FILE_HEADER,
        RENAME_FILEDIR,
        INMEMORY_CHANGE
    };

    enum CHUNK_TYPE {
        FILE_CHUNK,
        TABLET_CHUNK,
        SPLIT_TABLET_CHUNK,
        SMALLFILE_CHUNK,
        ADDITIONAL_CHUNK
    };

    enum DURATION_UNIT {
        DU_NANOSECOND,
        DU_MICROSECOND,
        DU_MILLISECOND,
        DU_SECOND,
        DU_MINUTE,
        DU_HOUR,
        DU_DAY,
        DU_WEEK,
        DU_MOUNTH,
        DU_YEAR
    };

    enum STREAM_TYPE {
        ARRAY_STREAM,
        SOCKET_STREAM,
        QUEUE_STREAM,
        FILE_STREAM,
        BIGARRAY_STREAM,
        FILEBLOCK_STREAM,
        OBJECT_STREAM
    };

    enum ACL_ACCESS_TYPE : short {
        TABLE_READ,
        TABLE_WRITE,
        DBOBJ_CREATE,
        DBOBJ_DELETE,
        DB_MANAGE,
        VIEW_EXEC,
        SCRIPT_EXEC,
        TEST_EXEC,
        MAX_PRIORITY_ACCESS,
        MAX_PARALLELISM_ACCESS
    };

    enum COMPRESS_METHOD {
        COMPRESS_NONE = 0,
        COMPRESS_LZ4 = 1,
        COMPRESS_DELTA = 2
    };

#ifdef INDEX64
    typedef long long INDEX;
    typedef unsigned long long UINDEX;
#define DT_INDEX DT_LONG
    const long long INDEX_MIN = LLONG_MIN;
    const long long INDEX_MAX = LLONG_MAX;
#else
    typedef int INDEX;
    typedef unsigned int UINDEX;
#define DT_INDEX DT_INT
    const int INDEX_MIN = INT_MIN;
    const int INDEX_MAX = INT_MAX;
#endif

    typedef union {
        long long longVal;
        int intVal;
        short shortVal;
        char charVal;
        double doubleVal;
        float floatVal;
        char *pointer;
        int intArray[2];
        float floatArray[2];
        short shortArray[4];
        char charArray[8];
    } U8;

    typedef union {
        float floatVal;
        int intVal;
#ifdef BIT32
        char *pointer;
#endif
        short shortVal;
        char charVal;
        short shortArray[2];
        char charArray[4];
    } U4;

};     // namespace dolphindb
#endif /* TYPES_H_ */
