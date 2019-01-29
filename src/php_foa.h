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

#ifndef PHP_FOA_H
#define PHP_FOA_H

extern zend_module_entry foa_module_entry;
#define phpext_foa_ptr &foa_module_entry

#ifdef PHP_WIN32
#define PHP_FOA_API __declspec(dllexport)
#else
#define PHP_FOA_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(foa);
PHP_MSHUTDOWN_FUNCTION(foa);
PHP_RINIT_FUNCTION(foa);
PHP_RSHUTDOWN_FUNCTION(foa);
PHP_MINFO_FUNCTION(foa);

PHP_FUNCTION(foa_set_stream);
PHP_FUNCTION(foa_set_buffer);
PHP_FUNCTION(foa_set_mode);
PHP_FUNCTION(foa_get_mode);
PHP_FUNCTION(foa_encode);
PHP_FUNCTION(foa_decode);
PHP_FUNCTION(foa_has_error);
PHP_FUNCTION(foa_last_error);
PHP_FUNCTION(foa_reset_error);

/* 
 * Declare any global variables you may need between the BEGIN
 * and END macros here:     
 */
ZEND_BEGIN_MODULE_GLOBALS(foa)
	struct libfoa *ptr;
ZEND_END_MODULE_GLOBALS(foa)

/*
 * In every utility function you add that needs to use variables in 
 * php_foa_globals, call TSRMLS_FETCH(); after declaring other variables 
 * used by that function, or better yet, pass in TSRMLS_CC after the last 
 * function argument and declare your utility function with TSRMLS_DC after 
 * the last declared argument. Always refer to the globals in your function 
 * as FOA_G(variable). You are encouraged to rename these macros something 
 * shorter, see examples in any other php module directory.
 */

#ifdef ZTS
# define FOA_G(v) TSRMG(foa_globals_id, zend_foa_globals *, v)
#else
# define FOA_G(v) (foa_globals.v)
#endif

#endif	/* PHP_FOA_H */
