#include "base/rsa_help.h"

#include <string.h>

#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/x509.h>
#include <cassert>

using std::string;

namespace RSAHelper {

int GenerateKey(char pubKey[], int& pubKeyLen, char priKey[], int& priKeyLen)
{
    RSA* rsa = NULL;
    int ret = -1;
    rsa = RSA_generate_key(gRSAGenerateKeyBits, RSA_F4, NULL, NULL);
    if (!rsa)
    {
        printf("RSA_generate_key() failed");
        return -1;
    }

    //创建BIO
    BIO *PubBio = BIO_new(BIO_s_mem());
    BIO *PriBio = BIO_new(BIO_s_mem());
    if (!PubBio || !PriBio)
    {
        printf("BIO_new() failed");
        if (PubBio) BIO_free_all(PubBio);
        if (PriBio) BIO_free_all(PriBio);
        if (rsa) RSA_free(rsa);
        return -1;
    }

    //生成公钥
    ret = i2d_RSAPublicKey_bio(PubBio, rsa);
    if (ret < 0)
    {
        printf("i2d_RSAPublicKey_bio() failed");
        if (PubBio) BIO_free_all(PubBio);
        if (PriBio) BIO_free_all(PriBio);
        if (rsa) RSA_free(rsa);
        return -1;
    }

    //生成私钥
    ret = i2d_RSAPrivateKey_bio(PriBio, rsa);
    if (ret < 0)
    {
        printf("i2d_RSAPrivateKey_bio() failed");
        if (PubBio) BIO_free_all(PubBio);
        if (PriBio) BIO_free_all(PriBio);
        if (rsa) RSA_free(rsa);
        return -1;
    }

    //获取公钥
    pubKeyLen = BIO_read(PubBio, pubKey, gRSAKeyBufLen);
    if (pubKeyLen < 0)
    {
        printf("BIO_read() PubKey PubKeyLen = %d.", pubKeyLen);

        if(PubBio) BIO_free_all(PubBio);
        if(PriBio) BIO_free_all(PriBio);
        if(rsa) RSA_free(rsa);
        return -1;
    }

    //获取私钥
    priKeyLen = BIO_read(PriBio, priKey, gRSAKeyBufLen);
    if (priKeyLen < 0)
    {
        printf("BIO_read() PriKey PriKeyLen = %d\n", priKeyLen);

        if (PubBio) BIO_free_all(PubBio);
        if (PriBio) BIO_free_all(PriBio);
        if (rsa) RSA_free(rsa);
        return -1;;
    }

    BIO_free_all(PubBio);
    BIO_free_all(PriBio);
    RSA_free(rsa);

    return 0;
}

string Base64encode(char pubKey[], int pubKeyLen, bool useSafeSet)
{
   // see RFC 3548
   static unsigned char codeCharUnsafe[] =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
   static unsigned char codeCharSafe[] =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.";

   unsigned char* codeChar = useSafeSet ? codeCharSafe : codeCharUnsafe;

   int srcLength = pubKeyLen;
   unsigned int dstLimitLength = srcLength*4/3 + 1 + 2; // +2 for the == chars
   unsigned char * dstData = new unsigned char[dstLimitLength];
   unsigned int dstIndex = 0;

   const char * p = static_cast<const char *>( pubKey );

   for(int index=0;index<srcLength;index+=3)
   {
      unsigned char codeBits = (p[index] & 0xfc)>>2;

      assert(codeBits < 64);
      dstData[dstIndex++] = codeChar[codeBits]; // c0 output
      assert(dstIndex <= dstLimitLength);

      // do second codeBits
      codeBits = ((p[index]&0x3)<<4);
      if (index+1 < srcLength)
      {
         codeBits |= ((p[index+1]&0xf0)>>4);
      }
      assert(codeBits < 64);
      dstData[dstIndex++] = codeChar[codeBits]; // c1 output
      assert(dstIndex <= dstLimitLength);

      if (index+1 >= srcLength)
      {
         dstData[dstIndex++] = codeChar[64];
         assert(dstIndex <= dstLimitLength);
         dstData[dstIndex++] = codeChar[64];
         assert(dstIndex <= dstLimitLength);
         break; // encoded d0 only
      }

      // do third codeBits
      codeBits = ((p[index+1]&0xf)<<2);
      if (index+2 < srcLength)
      {
         codeBits |= ((p[index+2]&0xc0)>>6);
      }
      assert(codeBits < 64);
      dstData[dstIndex++] = codeChar[codeBits]; // c2 output
      assert(dstIndex <= dstLimitLength);

      if (index+2 >= srcLength)
      {
         dstData[dstIndex++] = codeChar[64];
         assert(dstIndex <= dstLimitLength);
         break; // encoded d0 d1 only
      }

      // do fourth codeBits
      codeBits = ((p[index+2]&0x3f));
      assert(codeBits < 64);
      dstData[dstIndex++] = codeChar[codeBits]; // c3 output
      assert(dstIndex <= dstLimitLength);
      // outputed all d0,d1, and d2
   }

   return string(reinterpret_cast<char*>(dstData), dstIndex);
}

int Base64decode(const string& strPubKey, char pubKey[], int& pubKeyLen)
{
   // see RFC 3548
   // this will decode normal and URL safe alphabet
   static signed char base64Lookup[128] =
      {
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
         -1,-1,-1,62,-1,62,-2,63,52,53,
         54,55,56,57,58,59,60,61,-1,-1,
         -1,-2,-1,-1,-1,0, 1, 2, 3, 4,
         5, 6, 7, 8, 9, 10,11,12,13,14,
         15,16,17,18,19,20,21,22,23,24,
         25,-1,-1,-1,-1,63,-1,26,27,28,
         29,30,31,32,33,34,35,36,37,38,
         39,40,41,42,43,44,45,46,47,48,
         49,50,51,-1,-1,-1,-1,-1
      };

   int wc=0;
   int val=0;
   //Data bin;
   //bin.reserve( size()*3/4 );
   pubKeyLen = 0;

   for( unsigned int i=0; i<strPubKey.size(); i++ )
   {
      unsigned int x = strPubKey[i] & 0x7F;
      char c1,c2,c3;

      int v =  base64Lookup[x];

      if ( v >= 0 )
      {
         val = val << 6;
         val |= v;
         wc++;

         if ( wc == 4 )
         {
            c3 = char( val & 0xFF ); val = val >> 8;
            c2 = char( val & 0xFF ); val = val >> 8;
            c1 = char( val & 0xFF ); val = val >> 8;

            pubKey[pubKeyLen++] = c1;
            pubKey[pubKeyLen++] = c2;
            pubKey[pubKeyLen++] = c3;

            wc=0;
            val=0;
         }
      }
      if ( base64Lookup[x] == -2 )
      {
         if (wc==2) val = val<<12;
         if (wc==3) val = val<<6;

         c3 = char( val & 0xFF ); val = val >> 8;
         c2 = char( val & 0xFF ); val = val >> 8;
         c1 = char( val & 0xFF ); val = val >> 8;

         unsigned int xNext = strPubKey[i] & 0x7F;
         if ( (i+1<strPubKey.size() ) && ( base64Lookup[xNext] == -2 ))
         {
            pubKey[pubKeyLen++] = c1;
            i++;
         }
         else
         {
            pubKey[pubKeyLen++] = c1;
            pubKey[pubKeyLen++] = c2;
         }

         break;
      }
   }

   return 0;
}

string EncryptPassword(char pubKey[], int& pubKeyLen, char password[])
{
    BIO *DeBio = BIO_new(BIO_s_mem());
    if (!DeBio)
    {
        if (DeBio) BIO_free_all(DeBio);

        printf("encryptPassword::APP_ERROR:BIO_new()\n");
        return "";
    }

    int ret=-1;
    ret = BIO_write(DeBio, pubKey, pubKeyLen);
    if (ret < 0)
    {
        if (DeBio) BIO_free_all(DeBio);

        printf("encryptPassword BIO_write error\n");
        return "";
    }

    RSA* enRsa = d2i_RSAPublicKey_bio(DeBio, NULL);
    if (!enRsa)
    {
        if(DeBio) BIO_free_all(DeBio);
        if(enRsa) RSA_free(enRsa);

        printf("encryptPassword d2i_RSAPublicKey_bio error\n");
        return "";
    }

    unsigned char EnBuf[1024];
    int EnBufLen = 0;
    EnBufLen = RSA_public_encrypt(strlen(password), (unsigned char*)password, EnBuf, enRsa, RSA_PKCS1_PADDING);
    if (EnBufLen <= 0)
    {
        if(DeBio) BIO_free_all(DeBio);
        if(enRsa) RSA_free(enRsa);
        return "";
    }

    if(DeBio) BIO_free_all(DeBio);
    if(enRsa) RSA_free(enRsa);

    printf("EnBufLen=%d\n", EnBufLen);
    string strPasswd = Base64encode((char*)EnBuf, EnBufLen, false);
    return strPasswd;
}

string DecryptPassword(char priKey[], int priKeyLen, string& base64Password)
{
    char dePassword[gRSAKeyBufLen];
    int dePasswordLen = 0;
    Base64decode(base64Password, dePassword, dePasswordLen);
    printf("dePasswordLen=%d\n", dePasswordLen);

    BIO *DeBio = BIO_new(BIO_s_mem());
    if(!DeBio)
    {
        if (DeBio) BIO_free_all(DeBio);

        printf("decryptPassword::APP_ERROR:BIO_new()\n");
        return "";
    }

    int ret=-1;
    ret = BIO_write(DeBio, priKey, priKeyLen);
    if (ret < 0)
    {
        if (DeBio) BIO_free_all(DeBio);

        printf("APP_ERROR:rsa导入私钥BIO_write()失败");
        return "";
    }

    RSA* DeRsa = d2i_RSAPrivateKey_bio(DeBio, NULL);
    if(!DeRsa)
    {
        if(DeBio) BIO_free_all(DeBio);
        if(DeRsa) RSA_free(DeRsa);

        printf("APP_ERROR:rsa导入私钥d2i_RSAPrivateKey_bio()失败");
        return "";
    }

    unsigned char RsaDeStr[128]={'\0'};
    int DeStrLen=sizeof(RsaDeStr)-1;
    DeStrLen = RSA_private_decrypt(dePasswordLen, reinterpret_cast<unsigned char*>(dePassword), RsaDeStr, DeRsa, RSA_PKCS1_PADDING);
    if (DeStrLen < 0)
    {
        if(DeBio) BIO_free_all(DeBio);
        if(DeRsa) RSA_free(DeRsa);

        printf("APP_ERROR:rsa私钥解密失败");

        return "";
    }

    BIO_free_all(DeBio);
    RSA_free(DeRsa);

    return string(reinterpret_cast<char*>(RsaDeStr), DeStrLen);
}

#if 0
int main(int argc, char **argv)
{
    char pubKey[gRSAKeyBufLen];
    int pubKeyLen;
    char priKey[gRSAKeyBufLen];
    int priKeyLen;

    if (generateKey(pubKey, pubKeyLen, priKey, priKeyLen))
    {
        printf("generateKey failed.");
        return 0;
    }

    printf("generateKey pubKeyLen=%lu, priKeyLen=%lu.\n", pubKeyLen, priKeyLen);
    //printf("pubKey=%s\n", pubKey);
    string strPubKey = base64encode(pubKey, pubKeyLen, true);
    printf("base64 strPubKeylen=%d, pubKey=%s\n", strPubKey.size(), strPubKey.c_str());

    char dePubKey[gRSAKeyBufLen];
    int dePubKeyLen = 0;
    base64decode(strPubKey, dePubKey, dePubKeyLen);
    if (dePubKeyLen != pubKeyLen || memcmp(pubKey, dePubKey, pubKeyLen))
    {
        printf("decode error, dePubKeyLen=%d, pubKeyLen=%d\n", dePubKeyLen, pubKeyLen);
    }
    else
    {
        printf("base64decode decode success\n");
    }

    char password[] = "avonacoabcdef";

    string base64RSAPasswd = encryptPassword(pubKey, pubKeyLen, password);
    printf("encrypt success: %s\n", base64RSAPasswd.c_str());

    string strPasswd = decryptPassword(priKey, priKeyLen, base64RSAPasswd);
    printf("decode success: %s\n", strPasswd.c_str());

    return 0;
}
#endif


}
