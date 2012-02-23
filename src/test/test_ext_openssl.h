/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010- Facebook, Inc. (http://www.facebook.com)         |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
*/

#ifndef __TEST_EXT_OPENSSL_H__
#define __TEST_EXT_OPENSSL_H__

// >>>>>> Generated by idl.php. Do NOT modify. <<<<<<

#include <test/test_cpp_ext.h>

///////////////////////////////////////////////////////////////////////////////

class TestExtOpenssl : public TestCppExt {
 public:
  virtual bool RunTests(const std::string &which);

  bool test_openssl_csr_export_to_file();
  bool test_openssl_csr_export();
  bool test_openssl_csr_get_public_key();
  bool test_openssl_csr_new();
  bool test_openssl_csr_sign();
  bool test_openssl_error_string();
  bool test_openssl_open();
  bool test_openssl_pkcs12_export_to_file();
  bool test_openssl_pkcs12_export();
  bool test_openssl_pkcs12_read();
  bool test_openssl_pkcs7_decrypt();
  bool test_openssl_pkcs7_encrypt();
  bool test_openssl_pkcs7_sign();
  bool test_openssl_pkcs7_verify();
  bool test_openssl_pkey_export_to_file();
  bool test_openssl_pkey_export();
  bool test_openssl_pkey_free();
  bool test_openssl_free_key();
  bool test_openssl_pkey_get_details();
  bool test_openssl_pkey_get_private();
  bool test_openssl_get_privatekey();
  bool test_openssl_pkey_get_public();
  bool test_openssl_get_publickey();
  bool test_openssl_pkey_new();
  bool test_openssl_private_decrypt();
  bool test_openssl_private_encrypt();
  bool test_openssl_public_decrypt();
  bool test_openssl_public_encrypt();
  bool test_openssl_seal();
  bool test_openssl_sign();
  bool test_openssl_verify();
  bool test_openssl_x509_check_private_key();
  bool test_openssl_x509_checkpurpose();
  bool test_openssl_x509_export_to_file();
  bool test_openssl_x509_export();
  bool test_openssl_x509_free();
  bool test_openssl_x509_parse();
  bool test_openssl_x509_read();
  bool test_openssl_encrypt();
  bool test_openssl_digest();
};

///////////////////////////////////////////////////////////////////////////////

#endif // __TEST_EXT_OPENSSL_H__
