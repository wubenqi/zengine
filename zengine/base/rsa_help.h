// Copyright (C) 2012 by wubenqi
// Distributable under the terms of either the Apache License (Version 2.0) or 
// the GNU Lesser General Public License, as specified in the COPYING file.
//
// By: wubenqi<wubenqi@gmail.com>
//

#ifndef BASE_RSA_HELPER_H_
#define BASE_RSA_HELPER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string>

namespace RSAHelper {

const int gRSAKeyBufLen = 1024;
const int gRSAGenerateKeyBits = 512;

int GenerateKey(char pubKey[], int& pubKeyLen, char priKey[], int& priKeyLen);
std::string Base64encode(char pubKey[], int pubKeyLen, bool useSafeSet);
int Base64decode(const std::string& strPubKey, char pubKey[], int& pubKeyLen);
std::string EncryptPassword(char pubKey[], int& pubKeyLen, char password[]);
std::string DecryptPassword(char priKey[], int priKeyLen, std::string& base64Password);

}

#endif






