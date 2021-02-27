// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include "chainparamsseeds.h"
#include "pow.h"
#include "arith_uint256.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=6be1ade2619d1402571996e436b726c8b0bd72f10fdcae10cff5acd369118626, pow=0000362a7af6bd3af8b7baaa957bccd2e892cdeb9f7a34bb6851044624c67bff, ver=0x00000001, hashPrevBlock=0000000000000000000000000000000000000000000000000000000000000000, hashMerkleRoot=a12b04a5138e9241d194dc72a5811e6c4468585d450e2548d71294444855d985, nTime=1524198159, nBits=1e7fffff, nNonce=261378, vtx=1)
 *   CTransaction(hash=a12b04a513, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(0000000000, 4294967295), coinbase 04ffff001d0104465572616e69756d582032302f417072696c2f3230313820526164696f61637469766520426c6f636b636861696e20446973636f766572656420696e2044656570205370616365)
 *     CScriptWitness()
 *     CTxOut(nValue=1.00000000, scriptPubKey=4104584ca4018adf8b6b26edb1d2ac)
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "UraniumX 20/April/2018 Radioactive Blockchain Discovered in Deep Space";
    const CScript genesisOutputScript = CScript() << ParseHex("04584ca4018adf8b6b26edb1d2ac869a3ad91f8b92c1d93a89bd7c2142a5f42332a3896625ca5acb660004d4a07d73ec46b456e494cca331e7f3cfb12e0af68538") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 23500;
        consensus.nSubsidyHalvingIntervalPhase2 = 210240;
        consensus.nSubsidyPhase2Height = consensus.nSubsidyHalvingInterval * 2;

        consensus.BIP34Height = 0;
        consensus.BIP34Hash   = uint256S("0x6be1ade2619d1402571996e436b726c8b0bd72f10fdcae10cff5acd369118626");
        consensus.BIP65Height = 0;
        consensus.BIP66Height = 0;
        consensus.powLimit    = uint256S("00007fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 7 * 24 * 60 * 60;  // 7 days
        consensus.nPowTargetSpacing  = 5 * 60;            // 5 minutes
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 3427;  // 85% of 2016 * 2
        consensus.nMinerConfirmationWindow = 2016 * 2;    // nPowTargetTimespan / nPowTargetSpacing

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1521417600; // March 19th, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1552953600; // March 19th, 2019

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1521417600; // March 19th, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1552953600; // March 19th, 2019

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork =  uint256S ("0x0000000000000000000000000000000000000000000000000000000353210638");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S ("0xb1978de9486663641d7ff53a121bbed16502b0cf0816fb1136efbb326bdbdeed"); // 1150

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xf9;
        pchMessageStart[3] = 0xd9;
        pchMessageStart[1] = 0xcd;
        pchMessageStart[2] = 0xc3;
        nDefaultPort       = 8235;
        nPruneAfterHeight  = 100000;

        genesis = CreateGenesisBlock(1524198159, 261378, 0x1e7fffff, 1, COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

        assert (consensus.hashGenesisBlock == uint256S("0x6be1ade2619d1402571996e436b726c8b0bd72f10fdcae10cff5acd369118626"));
        assert (genesis.GetHashArgon2d() == uint256S("0x0000362a7af6bd3af8b7baaa957bccd2e892cdeb9f7a34bb6851044624c67bff"));
        assert (genesis.hashMerkleRoot == uint256S("0xa12b04a5138e9241d194dc72a5811e6c4468585d450e2548d71294444855d985"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,68);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,115);
        base58Prefixes[SECRET_KEY]     = std::vector<unsigned char>(1,12);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));
        vSeeds.clear();
        vSeeds.emplace_back ("node.uranium-x.com", false);

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fMiningRequiresPeers = true;

        checkpointData = (CCheckpointData) {
            {
                { 500,     uint256S("0x9f1c1d57adfe288560f1ed74c794799866b4e2864a394361df93ff4548123e03") },
                { 1000,    uint256S("0x493dbf44cd716b9b015eae9b1076689d0083df1e1808d3c1dbca805fcdf6d34c") },
                { 57650,   uint256S("0x4a1bdbf299ede7138e9217ba44968eb29e9eb649b5bc378b6056f99ab6d44135") }
            }
        };

        chainTxData = ChainTxData {
            // Data as of 0xa90ca8e30fe99f653336f21c782c70b6fa0d86d53ac8fd076bba58f3481f693a.
            1524631863,  // * UNIX timestamp of last known number of transactions
            1163,        // * total number of transactions between genesis and that timestamp
                         //   (the tx=... number in the SetBestChain debug.log lines)
            0.003090017725683077 // * estimated number of transactions per second after that timestamp
        };
    }
};

/**
 * Testnet (v1)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 23500;
        consensus.nSubsidyHalvingIntervalPhase2 = 210240;
        consensus.nSubsidyPhase2Height = consensus.nSubsidyHalvingInterval * 2;

        consensus.BIP34Height = 0;
        consensus.BIP34Hash   = uint256S("0xdae1dec7263541b45186f60c7eaf951bc05d62dc1fa25c7b3861baa0199a16e8");
        consensus.BIP65Height = 0;
        consensus.BIP66Height = 0;
        consensus.powLimit    = uint256S("0x000fffff00000000000000000000000000000000000000000000000000000000");
        consensus.nPowTargetTimespan = 7 * 24 * 60 * 60;  // 7 days
        consensus.nPowTargetSpacing  = 5 * 60;            // 5 minutes
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 3024;  // 75% of 2016 * 2
        consensus.nMinerConfirmationWindow = 2016 * 2;    // nPowTargetTimespan / nPowTargetSpacing

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1521417600; // March 19th, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1552953600; // March 19th, 2019

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1521417600; // March 19th, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1552953600; // March 19th, 2019

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork =  uint256S ("0x0000000000000000000000000000000000000000000000000000000000000000");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S ("0x0000000000000000000000000000000000000000000000000000000000000000");

        pchMessageStart[0] = 0x0b;
        pchMessageStart[1] = 0x20;
        pchMessageStart[2] = 0x18;
        pchMessageStart[3] = 0x07;
        nDefaultPort       = 18235;
        nPruneAfterHeight  = 1000;

        genesis = CreateGenesisBlock(1557009671, 2743, 0x1f0fffff, 1, COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

        assert (consensus.hashGenesisBlock == uint256S("0x6589ebb12bf31c219738eddb2f892f0ea4ae1ba7e24f55718fbcbdf30eba1b03"));
        assert (genesis.GetHashYespower() == uint256S("0x000023e17dc44b9ce12b4e8b30076d241a70b0137183f2a011603965deea73ea"));

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));
        vSeeds.emplace_back ("155.138.148.16", false);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,112);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,227);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,199);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fMiningRequiresPeers = true;

        checkpointData = (CCheckpointData) {
            {
            }
        };

        chainTxData = ChainTxData {
            /* data as of:  */
            0,
            0,
            0.0
        };
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 35;
        consensus.nSubsidyHalvingIntervalPhase2 = 150;
        consensus.nSubsidyPhase2Height = 2 * consensus.nSubsidyHalvingInterval;

        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 0; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 0; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("0fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 7 * 24 * 60 * 60;  // 7 days
        consensus.nPowTargetSpacing  = 5 * 60;            // 5 minutes
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork  = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x101336b273a9626bed8541eea8c716e5527ac5805681f41cf6e36d153ca7636f");

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xce;
        pchMessageStart[2] = 0xc4;
        pchMessageStart[3] = 0xda;
        nDefaultPort       = 18236;
        nPruneAfterHeight  = 1000;

        genesis = CreateGenesisBlock(1524198906, 17, 0x200fffff, 1, COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

        //assert (consensus.hashGenesisBlock == uint256S("0x101336b273a9626bed8541eea8c716e5527ac5805681f41cf6e36d153ca7636f"));
        //assert (genesis.GetHashArgon2d() == uint256S("0x05716ab92b3b919209e1fe46b7a602e772df20df516aabc7f682e5e1d1d6be0f"));
        //assert (genesis.hashMerkleRoot == uint256S("0xa12b04a5138e9241d194dc72a5811e6c4468585d450e2548d71294444855d985"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fMiningRequiresPeers = false;

        checkpointData = (CCheckpointData) {
            {
                // {0, uint256S("00")},
            }
        };

        chainTxData = ChainTxData {
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,112);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,227);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,199);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}
