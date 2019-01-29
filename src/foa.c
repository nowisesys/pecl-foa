/* PECL FOA - PHP extension implementing the FOA encoding/decoding method.
 * Copyright (C) 2009-2019 Anders Lövgren
 * 
 * THIS SOFTWARE IS PROVIDED BY THE PHP DEVELOPMENT TEAM ``AS IS'' AND 
 * ANY EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
 * PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE PHP
 * DEVELOPMENT TEAM OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>
#include <php_ini.h>
#include <ext/standard/info.h>
#ifdef HAVE_FOALIB
#include <libfoa.h>
#endif
#include "php_foa.h"

/* 
 * If you declare any globals in php_foa.h uncomment this:
 */
ZEND_DECLARE_MODULE_GLOBALS(foa)

/*
 * True global resources - no need for thread safety here.
 */
static int le_foa;

/* {{{ foa_functions[]
 *
 * Every user visible function must have an entry in foa_functions[].
 */
zend_function_entry foa_functions[] = {
	PHP_FE(foa_set_stream, NULL)
	PHP_FE(foa_set_buffer, NULL)
	PHP_FE(foa_set_mode, NULL)
	PHP_FE(foa_get_mode, NULL)
	PHP_FE(foa_encode, NULL)
	PHP_FE(foa_decode, NULL)
	PHP_FE(foa_has_error, NULL)
	PHP_FE(foa_last_error, NULL)
	PHP_FE(foa_reset_error, NULL)
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ foa_module_entry
 */
zend_module_entry foa_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"foa",
	foa_functions,
	PHP_MINIT(foa),
	PHP_MSHUTDOWN(foa),
	PHP_RINIT(foa),
	PHP_RSHUTDOWN(foa),
	PHP_MINFO(foa),
#if ZEND_MODULE_API_NO >= 20010901
	"1.0.1",
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_FOA

ZEND_GET_MODULE(foa)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("foa.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_foa_globals, foa_globals)
    STD_PHP_INI_ENTRY("foa.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_foa_globals, foa_globals)
PHP_INI_END()
 */
/* }}} */

/* {{{ php_foa_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_foa_init_globals(zend_foa_globals *foa_globals)
{
	foa_globals->global_value = 0;
	foa_globals->global_string = NULL;
}
 */
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(foa)
{
	/* 
	 * If you have INI entries, uncomment this line:
	 * REGISTER_INI_ENTRIES();
	 */

	REGISTER_LONG_CONSTANT("FOA_TYPE_DATA_ENTITY", FOA_TYPE_DATA_ENTITY, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("FOA_TYPE_START_OBJECT", FOA_TYPE_START_OBJECT, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("FOA_TYPE_START_ARRAY", FOA_TYPE_START_ARRAY, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("FOA_TYPE_END_OBJECT", FOA_TYPE_END_OBJECT, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("FOA_TYPE_END_ARRAY", FOA_TYPE_END_ARRAY, CONST_CS | CONST_PERSISTENT);

	REGISTER_LONG_CONSTANT("FOA_MODE_ESCAPE", FOA_MODE_ESCAPE, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("FOA_MODE_HASHES", FOA_MODE_HASHES, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("FOA_MODE_SETERR", FOA_MODE_SETERR, CONST_CS | CONST_PERSISTENT);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(foa)
{
	/* 
	 * Uncomment this line if you have INI entries:
	 * UNREGISTER_INI_ENTRIES();
	 */
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(foa)
{
	FOA_G(ptr) = emalloc(sizeof(struct libfoa));
	if (foa_init(FOA_G(ptr)) < 0) {
		return FAILURE;
	}
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */

/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(foa)
{
	foa_cleanup(FOA_G(ptr));
	efree(FOA_G(ptr));
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(foa)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "foa support", "enabled");
	php_info_print_table_end();

	/*
	 * Remove comment if you have entries in php.ini:
	 * DISPLAY_INI_ENTRIES();
	 */
}
/* }}} */

/* {{{ proto  foa_set_stream(resource stream)
   Set stream for next encode or decode operation. If decode, then the stream is used as input. If encode, then its used as output. */
PHP_FUNCTION(foa_set_stream)
{
	zval *resarg;
	php_stream *stream;
	FILE *fp = NULL;

#if ZEND_MODULE_API_NO > 20131226  /* PHP 7.x */
	ZEND_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_RESOURCE(resarg)
	ZEND_PARSE_PARAMETERS_END();
#else      /* PHP 5.6 */ 
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"r",
		&resarg) == FAILURE) {
		return;
	}
#endif

#if ZEND_MODULE_API_NO > 20131226  /* PHP 7.x */
	php_stream_from_zval(stream, resarg);
#else      /* PHP 5.6 */
	php_stream_from_zval(stream, &resarg);
#endif

	if (php_stream_cast(stream,
		PHP_STREAM_AS_STDIO,
		(void **) & fp,
		REPORT_ERRORS) == FAILURE) {
		return;
	}

	foa_set_stream(FOA_G(ptr), fp);
}
/* }}} */

/* {{{ proto  foa_set_buffer(string buffer)
   Set the buffer to decode entities from. */
PHP_FUNCTION(foa_set_buffer)
{
#if ZEND_MODULE_API_NO > 20131226  /* PHP 7.x */
	char *buffer;
	size_t buffer_len;

	ZEND_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_STRING(buffer, buffer_len)
	ZEND_PARSE_PARAMETERS_END();
#else      /* PHP 5.6 */
	char *buffer;
	int buffer_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"s",
		&buffer,
		&buffer_len) == FAILURE) {
		php_printf("BUFFER 2\n");
		return;
	}
#endif

	foa_set_buffer(FOA_G(ptr), buffer);
}
/* }}} */

/* {{{ proto  foa_set_mode(int mode, bool enable)
   Enable or disable mode. The mode is either FOA_MODE_ESCAPE, FOA_MODE_HASHES or FOA_MODE_SETERR. */
PHP_FUNCTION(foa_set_mode)
{
	long mode;
	zend_bool enable;

#if ZEND_MODULE_API_NO > 20131226  /* PHP 7.x */
	ZEND_PARSE_PARAMETERS_START(2, 2)
	Z_PARAM_LONG(mode)
	Z_PARAM_BOOL(enable)
	ZEND_PARSE_PARAMETERS_END();
#else     /* PHP 5.6 */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"lb",
		&mode,
		&enable) == FAILURE) {
		return;
	}
#endif

	foa_set_mode(FOA_G(ptr), mode, enable);
}
/* }}} */

/* {{{ proto int foa_get_mode(int mode)
   Get whether mode is currently enabled or disabled.  */
PHP_FUNCTION(foa_get_mode)
{
	long mode;
	int enable = 0;

#if ZEND_MODULE_API_NO > 20131226  /* PHP 7.x */
	ZEND_PARSE_PARAMETERS_START(1, 1)
	Z_PARAM_LONG(mode)
	ZEND_PARSE_PARAMETERS_END();
#else  /* PHP 5.6 */
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"l",
		&mode) == FAILURE) {
		return;
	}
#endif

	foa_get_mode(FOA_G(ptr), mode, &enable);
	RETURN_LONG((long) enable);
}
/* }}} */

/* {{{ proto string foa_encode(int type [, string name [, string data]])
   Encodes the entity defined by the parameters. The type argument is either one of FOA_TYPE_DATA_ENTITY, FOA_TYPE_START_OBJECT, FOA_TYPE_START_ARRAY, FOA_TYPE_END_OBJECT or FOA_TYPE_END_ARRAY. The name argument is used to name the entity, the data argument is only used together with FOA_TYPE_DATA_ENTITY. Returns the encoded entity string. */
PHP_FUNCTION(foa_encode)
{
	char *name = NULL;
	char *data = NULL;
	size_t name_len;
	size_t data_len;
	long type;

#if ZEND_MODULE_API_NO > 20131226  /* PHP 7.x */
	ZEND_PARSE_PARAMETERS_START(1, 3)
	Z_PARAM_LONG(type)
	Z_PARAM_OPTIONAL
	Z_PARAM_STRING(name, name_len)
	Z_PARAM_STRING(data, data_len)
	ZEND_PARSE_PARAMETERS_END();
#else
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
		"l|ss",
		&type,
		&name,
		&name_len,
		&data,
		&data_len) == FAILURE) {
		return;
	}
#endif

#if ZEND_MODULE_API_NO > 20131226  /* PHP 7.x */
	RETURN_STRING((char *) foa_write(FOA_G(ptr),
		0,
		(int) type,
		(const char *) name,
		(const char *) data));
#else      /* PHP 5.6 */
	RETURN_STRING((char *) foa_write(FOA_G(ptr),
		0,
		(int) type,
		(const char *) name,
		(const char *) data), 1);
#endif
}
/* }}} */

/* {{{ proto array foa_decode()
   Decodes next entity from the input source (either an stream set by foa_set_stream() or an input buffer set by foa_set_buffer()). */
PHP_FUNCTION(foa_decode)
{
#if ZEND_MODULE_API_NO >= 20180731  /* PHP 7.3 */
	ZEND_PARSE_PARAMETERS_NONE();
#elif ZEND_MODULE_API_NO > 20131226 /* PHP 7.1 */
	zend_parse_parameters_none();
#else /* PHP 5.6 */
	if (ZEND_NUM_ARGS() != 0) {
		WRONG_PARAM_COUNT;
	}
#endif

	const struct foa_entity *ent = foa_next(FOA_G(ptr));
	if (!ent) {
		RETURN_NULL();
	}

	array_init(return_value);

#if ZEND_MODULE_API_NO > 20131226  /* PHP 7.x */
	if (ent->name) {
		add_assoc_string(return_value, "name", (char *) ent->name);
	}
	if (ent->data) {
		add_assoc_string(return_value, "data", (char *) ent->data);
	}
#else      /* PHP 5.6 */
	if (ent->name) {
		add_assoc_string(return_value, "name", (char *) ent->name, 1);
	}
	if (ent->data) {
		add_assoc_string(return_value, "data", (char *) ent->data, 1);
	}
#endif

	add_assoc_long(return_value, "type", ent->type);
	add_assoc_long(return_value, "line", ent->line);
}
/* }}} */

/* {{{ proto bool foa_has_error()
   Return true if error is set. */
PHP_FUNCTION(foa_has_error)
{
#if ZEND_MODULE_API_NO >= 20180731  /* PHP 7.3 */
	ZEND_PARSE_PARAMETERS_NONE();
#elif ZEND_MODULE_API_NO > 20131226 /* PHP 7.1 */
	zend_parse_parameters_none();
#else /* PHP 5.6 */
	if (ZEND_NUM_ARGS() != 0) {
		WRONG_PARAM_COUNT;
	}
#endif

	if (foa_error_set(FOA_G(ptr))) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
}
/* }}} */

/* {{{ proto string foa_last_error()
   Get last error message. */
PHP_FUNCTION(foa_last_error)
{
#if ZEND_MODULE_API_NO >= 20180731  /* PHP 7.3 */
	ZEND_PARSE_PARAMETERS_NONE();
#elif ZEND_MODULE_API_NO > 20131226 /* PHP 7.1 */
	zend_parse_parameters_none();
#else /* PHP 5.6 */
	if (ZEND_NUM_ARGS() != 0) {
		WRONG_PARAM_COUNT;
	}
#endif

#if ZEND_MODULE_API_NO > 20131226  /* PHP 7.x */
	if (foa_last_error(FOA_G(ptr))) {
		RETURN_STRING((char *) foa_last_error(FOA_G(ptr)));
	} else {
		RETURN_NULL();
	}
#else      /* PHP 5.6 */
	if (foa_last_error(FOA_G(ptr))) {
		RETURN_STRING((char *) foa_last_error(FOA_G(ptr)), 1);
	} else {
		RETURN_NULL();
	}
#endif
}
/* }}} */

/* {{{ proto  foa_reset_error()
   Clear last error message. */
PHP_FUNCTION(foa_reset_error)
{
#if ZEND_MODULE_API_NO >= 20180731  /* PHP 7.3 */
	ZEND_PARSE_PARAMETERS_NONE();
#elif ZEND_MODULE_API_NO > 20131226 /* PHP 7.1 */
	zend_parse_parameters_none();
#else /* PHP 5.6 */
	if (ZEND_NUM_ARGS() != 0) {
		WRONG_PARAM_COUNT;
	}
#endif

	foa_reset_error(FOA_G(ptr));
}
/* }}} */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
