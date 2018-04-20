// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <cstdlib>
#include "argon2/include/argon2.h"
#include "argon2.h"
#include "uint256.h"

static const char* POW_SECRET = "f412a69fdc6d8ee6663f796b2e7ea53a52b9532a641b2f9cb2a7860108dc4c03";
static const size_t INPUT_BYTES = 80;   // Size of a block header in bytes.
static const size_t OUTPUT_BYTES = 32;  // Size of output needed for a 256-bit hash
static uint8_t* pArgon2Ad = nullptr;    // Pre allocated memory
static size_t nArgon2AdLen = 0;         // Pre allocated memory size
static const int64_t INIT_TIME = 1523321554; // 04/10/2018 @ 12:52am (UTC)

static size_t Argon2FactorN(const int64_t nTime)
{
    assert(nTime >= 0);
    static const size_t offset = 9;

    // These have an extra 512k added from the memory cost parameter
    // of argon 2
    static const int64_t nTimes[] = {
        0,          //                             512KB
        1618876800, // 04/20/2021 @ 12:00am (UTC)  1MB
        1713571200, // 04/20/2024 @ 12:00am (UTC)  2MB
        1808179200  // 04/20/2027 @ 12:00am (UTC)  4MB
    };
    
    size_t nFactor = 0;
    for (nFactor = 0; nFactor < 3; ++nFactor)
        if (nTime >= nTimes[nFactor] && nTime < nTimes[nFactor+1])
            return nFactor + offset;

    return nFactor + offset;
}

static uint32_t GetArgon2AdSize(const int64_t nTime)
{
    const auto factor = Argon2FactorN(nTime);
    return 1024 * (1 << factor);
}

static void UpdateArgon2AdValues()
{
    assert (pArgon2Ad != nullptr);
    for (int i = 0; i < nArgon2AdLen; ++i)
        pArgon2Ad[i] = static_cast<uint8_t>(i < 256 ? i : i % 256);
}

static void EnsureArgon2MemoryAllocated(const int64_t nTime)
{
    const auto nSize = GetArgon2AdSize(nTime);
    if (nSize > nArgon2AdLen)
    {
        if (nullptr != pArgon2Ad)
            std::free (pArgon2Ad);
        pArgon2Ad = (uint8_t*) std::malloc(nSize);
        if (pArgon2Ad == nullptr)
            throw std::runtime_error("Could not allocate memory for argon2");
        nArgon2AdLen = nSize;
        UpdateArgon2AdValues();
    }
}

int Argon2Init()
{
    EnsureArgon2MemoryAllocated(INIT_TIME);
    return static_cast<int>(nArgon2AdLen);
}

void Argon2Deinit()
{    
    if (nullptr != pArgon2Ad)
    {
        std::free(pArgon2Ad);
        pArgon2Ad = nullptr;
    }
}

void Argon2dHash (const char* input, const char* output, const int64_t nTime)
{
    EnsureArgon2MemoryAllocated (nTime);
    argon2_context ctx;

    ctx.version         = ARGON2_VERSION_13;
    ctx.flags           = ARGON2_DEFAULT_FLAGS;

    ctx.out             = (uint8_t*) output;
    ctx.outlen          = OUTPUT_BYTES;
    ctx.pwd             = (uint8_t*)input;
    ctx.pwdlen          = INPUT_BYTES - 40;
    ctx.salt            = ((uint8_t*) input) + 40;
    ctx.saltlen         = 40;
    
    ctx.secret          = (uint8_t*) POW_SECRET;
    ctx.secretlen       = strlen (POW_SECRET);
    ctx.ad              = pArgon2Ad;
    ctx.adlen           = GetArgon2AdSize (nTime);

    ctx.m_cost          = 512;
    ctx.t_cost          = 1;
    ctx.lanes           = 2;
    ctx.threads         = 1;

    ctx.allocate_cbk    = nullptr;
    ctx.free_cbk        = nullptr;

    assert (ctx.adlen > 0 && ctx.ad != nullptr);
    const int result = argon2_ctx (&ctx, Argon2_d);
    assert (result == ARGON2_OK);
}
