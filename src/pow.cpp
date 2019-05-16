// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "pow.h"

#include "arith_uint256.h"
#include "chain.h"
#include "chainparams.h"
#include "primitives/block.h"
#include "uint256.h"
#include "util.h"
#include "validation.h"

unsigned int static DarkGravityWave(const CBlockIndex* pindexLast, const Consensus::Params& params) {
    /* current difficulty formula, dash - DarkGravity v3, written by Evan Duffield - evan@dashpay.io */
    const CBlockIndex *pBlockLastSolved = pindexLast;
    const CBlockIndex *pBlockReading = pindexLast;
    int64_t nActualTimespan = 0;
    int64_t nLastBlockTime  = 0;
    const int64_t nPastBlocksMin = 10;
    const int64_t nPastBlocksMax = 12;
    int64_t nCountBlocks = 0;
    arith_uint256 bnPastDifficultyAverage;
    arith_uint256 bnPastDifficultyAveragePrev;
    
    const auto bnPowLimit = UintToArith256 (params.powLimit);

    if (pindexLast->nHeight > 57646 && pindexLast->nHeight < 57651)
        return bnPowLimit.GetCompact();

    if (pBlockLastSolved == nullptr || pBlockLastSolved->nHeight < nPastBlocksMin || params.fPowNoRetargeting)
        return bnPowLimit.GetCompact();

    for (unsigned int i = 1; pBlockReading && pBlockReading->nHeight > 0; i++)
    {
        if (nPastBlocksMax > 0 && i > nPastBlocksMax) { break; }
        nCountBlocks++;

        if (nCountBlocks <= nPastBlocksMin) {
            if (nCountBlocks == 1) {
                bnPastDifficultyAverage.SetCompact (pBlockReading->nBits); 
            } else { 
                bnPastDifficultyAverage = ((bnPastDifficultyAveragePrev * nCountBlocks) + (arith_uint256().SetCompact(pBlockReading->nBits))) / (nCountBlocks + 1); 
            }
            bnPastDifficultyAveragePrev = bnPastDifficultyAverage;
        }

        if (nLastBlockTime > 0) {
            int64_t Diff = (nLastBlockTime - pBlockReading->GetBlockTime());
            nActualTimespan += Diff;
        }

        nLastBlockTime = pBlockReading->GetBlockTime();

        if (pBlockReading->pprev == nullptr) { 
            assert(pBlockReading); 
            break; 
        }

        pBlockReading = pBlockReading->pprev;
    }

    arith_uint256 bnNew(bnPastDifficultyAverage);

    int64_t _nTargetTimespan = nCountBlocks * params.nPowTargetSpacing;

    if (nActualTimespan < _nTargetTimespan / 3)
        nActualTimespan = _nTargetTimespan / 3;
    if (nActualTimespan > _nTargetTimespan * 3)
        nActualTimespan = _nTargetTimespan * 3;

    // Retarget
    bnNew *= nActualTimespan;
    bnNew /= _nTargetTimespan;

    if (bnNew > bnPowLimit) {
        bnNew = bnPowLimit;
    }

    return bnNew.GetCompact();
}

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    assert(pindexLast != nullptr);

    // as argon2d/yespower are entirely different algorithms, we place
    //        a period of 10 blocks between them which is minimum difficulty
    if ((pindexLast->nHeight >= nYesPowerFork-5) &&
        (pindexLast->nHeight <= nYesPowerFork+5))
        return UintToArith256(params.powLimit).GetCompact();

    return DarkGravityWave (pindexLast, params);
}

unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
    if (params.fPowNoRetargeting)
        return pindexLast->nBits;

    // Limit adjustment step
    int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
    if (nActualTimespan < params.nPowTargetTimespan/4)
        nActualTimespan = params.nPowTargetTimespan/4;
    if (nActualTimespan > params.nPowTargetTimespan*4)
        nActualTimespan = params.nPowTargetTimespan*4;

    // Retarget
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    arith_uint256 bnNew;
    bnNew.SetCompact(pindexLast->nBits);
    bnNew *= nActualTimespan;
    bnNew /= params.nPowTargetTimespan;

    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    return bnNew.GetCompact();
}

bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params& params)
{
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(params.powLimit))
        return false;

    // Check proof of work matches claimed amount
    if (UintToArith256(hash) > bnTarget)
        return false;

    return true;
}
