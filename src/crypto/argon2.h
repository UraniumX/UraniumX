// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <cstdint>

int Argon2Init();
void Argon2Deinit();
void Argon2dHash(const char* input, const char* output, const int64_t nTime);
