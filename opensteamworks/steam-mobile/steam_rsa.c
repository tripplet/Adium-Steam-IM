/*
 https://steamcommunity.com/mobilelogin/getrsakey?username=<steamusername>
 {"success":true,"publickey_mod":"pubkeyhex","publickey_exp":"pubkeyhex","timestamp":"165685150000"}
 
 
 https://steamcommunity.com/mobilelogin/dologin/
 password=<base64rsaencryptedpwd>&username=<steamusername>&emailauth=&captchagid=-1&captcha_text=&emailsteamid=&rsatimestamp=165685150000&remember_login=true&donotcache=1368831657863
 
 */

#include "polarssl/config.h"
#include "polarssl/rsa.h"
#include "polarssl/entropy.h"
#include "polarssl/ctr_drbg.h"

gchar* steam_encrypt_password(const gchar *modulus_str, const gchar *exponent_str, const gchar *password)
{
  rsa_context rsa;
  entropy_context entropy;
  ctr_drbg_context ctr_drbg;
  
  // Init entropy context
  entropy_init(&entropy);
  int ret = ctr_drbg_init(&ctr_drbg, entropy_func, &entropy, NULL, 0);
  
  if (ret != 0) {
    purple_debug_error("steam", "RSA init failed, error=%d\n", ret);
    return NULL;
  }
  
  // Init polarssl rsa
  rsa_init(&rsa, RSA_PKCS_V15, 0);
  
  // Read modulus
  ret = mpi_read_string(&rsa.N, 16, modulus_str);
  if (ret != 0) {
    purple_debug_error("steam", "modulus parsing failed, error=%d\n", ret);
    return NULL;
  }
  
  // Read exponent
  ret = mpi_read_string(&rsa.E, 16, exponent_str);
  if (ret != 0) {
    purple_debug_error("steam", "exponent parsing failed, error=%d\n", ret);
    return NULL;
  }
  
  // Set RSA key length
  rsa.len = ( mpi_msb( &rsa.N ) + 7 ) >> 3;
  
  // Allocate space for encrypted password
  unsigned char *encrypted_password = malloc(sizeof(char) * rsa.len);
  
  ret = rsa_pkcs1_encrypt(&rsa, ctr_drbg_random, &ctr_drbg, RSA_PUBLIC, strlen(password), (unsigned char*)password, encrypted_password);
  
  if (ret != 0) {
    purple_debug_error("steam", "password encryption failed, error=%d\n", ret);
    free(encrypted_password);
    return NULL;
  }
  
  gchar *output = purple_base64_encode(encrypted_password, (int)rsa.len);
  free(encrypted_password);
  
  return output;
}