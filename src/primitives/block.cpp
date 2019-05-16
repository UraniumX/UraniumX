// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "primitives/block.h"
#include "hash.h"
#include "tinyformat.h"
#include "utilstrencodings.h"
#include "crypto/argon2.h"
#include "crypto/common.h"
#include "yespower/yespower.h"
#include "streams.h"
#include "pow.h"

uint256 CBlockHeader::GetHash() const
{
    return SerializeHash (*this);
}

uint256 CBlockHeader::GetHashArgon2d() const
{
    uint256 hash;
    Argon2dHash (BEGIN(nVersion), BEGIN(hash), nTime);
    return hash;
}

uint256 CBlockHeader::GetHashYespower() const
{
    uint256 thash;
    CDataStream ss(SER_NETWORK, PROTOCOL_VERSION);
    ss << *this;
    yespower_params_t yespower_1_0_uraniumx = {
        .version = YESPOWER_1_0,
        .N = 2048,
        .r = 32,
        .pers = (const uint8_t *)"UraniumX",
        .perslen = 8 
    };
    if (yespower_tls( (unsigned char *)&ss[0], ss.size(), &yespower_1_0_uraniumx, (yespower_binary_t *)&thash) ) {
        abort();
    }
    return thash;
}

std::string CBlock::ToString() const
{
    std::stringstream s;
    s << strprintf("CBlock(hash=%s, argon2dpow=%s, yespowerpow=%s, ver=0x%08x, hashPrevBlock=%s, hashMerkleRoot=%s, nTime=%u, nBits=%08x, nNonce=%u, vtx=%u)\n",
        GetHash().ToString(),
        GetHashArgon2d().ToString(),
        GetHashYespower().ToString(),
        nVersion,
        hashPrevBlock.ToString(),
        hashMerkleRoot.ToString(),
        nTime, nBits, nNonce,
        vtx.size());
    for (const auto& tx : vtx) {
        s << "  " << tx->ToString() << "\n";
    }
    return s.str();
}
