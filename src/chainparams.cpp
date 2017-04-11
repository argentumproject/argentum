// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#include "arith_uint256.h"
#include "tinyformat.h"
#include "chainparams.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;

#include "chainparamsseeds.h"

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
        //consensus.nSubsidyHalvingInterval = 80640 * 12;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.powLimit = ArithToUint256(~arith_uint256(0) >> 20);
        consensus.nPowTargetTimespan = 10 * 90; // 15 mins
        consensus.nPowTargetSpacingV1 = 32; // target time for block spacing across all algorithms
        consensus.nPowTargetSpacingV2 = 45; // new target time for block spacing across all algorithms
        consensus.nAveragingInterval = 10; // number of blocks to take the timespan of
        consensus.fPowAllowMinDifficultyBlocks = false;
        
        // Allow AuxPow blocks from this height
        consensus.nStartAuxPow = 1825000; // 
        consensus.nAuxpowChainId = 0x004A3; 
        consensus.fStrictChainId = true;
        consensus.nLocalDifficultyAdjustment = 12; // 12%
        consensus.nMaxAdjustDown = 22; //22% adjustment down
        consensus.nMaxAdjustUp = 14; // 14% adjustment up
        //consensus.nMaxAdjustUpV2 = 4; // 4% adjustment up     
        
        consensus.nBlockSequentialAlgoRuleStart1 = 1930000; // block where sequential algo rule starts
        consensus.nBlockSequentialAlgoRuleStart2 = 2032000; // block where sequential algo rule starts
        consensus.nBlockSequentialAlgoMaxCount1 = 3; // maximum sequential blocks of same algo
        consensus.nBlockSequentialAlgoMaxCount2 = 6; // maximum sequential blocks of same algo
        
        //consensus.nBlockAlgoWorkWeightStart = 142000; // block where algo work weighting starts
        //consensus.nBlockAlgoNormalisedWorkStart = 740000; // block where algo combined weight starts
        //consensus.nBlockAlgoNormalisedWorkDecayStart1 = 866000; // block where weight decay starts
        //consensus.nBlockAlgoNormalisedWorkDecayStart2 = 932000; // block where weight decay starts
        //consensus.nGeoAvgWork_Start = 4000000; // TODO
        consensus.nDGW3StartBlock = 1635000; 
        consensus.nMultiAlgoFork = 1930000; // Block where multi-algo difficulty adjustment and 45 second blocktime starts
        consensus.nCoinbaseMaturityV2Start = 2422000;

        /** 
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xfb;
        pchMessageStart[1] = 0xc1;
        pchMessageStart[2] = 0xb8;
        pchMessageStart[3] = 0xdc;
        vAlertPubKey = ParseHex("04d4da7a5dae4db797d9b0644d57a5cd50e05a70f36091cd62e2fc41c98ded06340be5a43a35e185690cd9cde5d72da8f6d065b499b06f51dcfba14aad859f443a");
        nDefaultPort = 13580;
        nMinerThreads = 0;
        nMaxTipAge = 24 * 60 * 60; // Maximum tip age in seconds to consider node in initial block download
        nPruneAfterHeight = 100000;

        /**
         * Build the genesis block. Note that the output of its generation
         * transaction cannot be spent since it did not originally exist in the
         * database.
         *
         * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
         *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
         *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
         *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
         *   vMerkleTree: 4a5e1e
         */
        const char* pszTimestamp = "Edward Snowden says he's behind NSA leaks - CNN - June 10 2013";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 5 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("040184710fa589ad5023690c80f3a49c8f13f8d45b8c857fbcbc8bc4a8e4d3eb4b10f4d4604fa08dce601aaf0f470216fe1b51850b4acf21b179c45070ac7b03a9") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock.SetNull();
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = 1369199888;
        genesis.nBits    = 0x1e0ffff0;
        genesis.nNonce   = 12786092;


        consensus.hashGenesisBlock = genesis.GetHash();
        
        assert(consensus.hashGenesisBlock == uint256S("0x88c667bc63167685e4e4da058fffdfe8e007e5abffd6855de52ad59df7bb0bb2"));
        assert(genesis.hashMerkleRoot == uint256S("0xc7e4af4190d3eb28d4e61261fb1b47e13d7efd6d7279c75ae6a91c2987d43d53"));

        //vSeeds.push_back(CDNSSeedData("x.x", "x.x"));


        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,23);   // 0x17
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);    // 0x5
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,151);  // 0x97
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_COIN_TYPE] =   boost::assign::list_of(0x8000002D).convert_to_container<std::vector<unsigned char> >(); // Argentum BIP 44 index is 45

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (Checkpoints::CCheckpointData) {
            boost::assign::map_list_of
            (        0, uint256S("0x88c667bc63167685e4e4da058fffdfe8e007e5abffd6855de52ad59df7bb0bb2"))
            (        1, uint256S("0xdf0a20b6609b206448778428648f42a592dea10884a3fca1ce3a4c2ce12caf2f"))
            (        2, uint256S("0xa10fd82c25b5ddcbcb9077cf9a70b47502a449663655a5d9cec09c19556c6d43"))
            (    50000, uint256S("0xa8b62a4dd6c8aa28e2bf68c521e01da0638214f2def124a551f1a90df8402b6d"))
            (   124415, uint256S("0x6d5a842bea984df8088f910d27f81af72ccfeef2bde21e100db036a51e17d6f8"))
            (   224414, uint256S("0x2df7b26aea2376760533552baa54a61044d13f2c6f44ff017e855e9593aa0216"))
            (   624415, uint256S("0x4651a604984d4494d09d1bb8c9f1c951e9bb214b5c930af4d0ad0bff7c513057"))
            (   824404, uint256S("0x91456228171e75954e44e44b740ba075d179c11c31fc0d403843a582316a9e21"))
            (  1024406, uint256S("0xf70a1dbf9c54f114690dfc7d6ac03366034e2c7a4dcd4fb71cf25f079c46c859"))
            (  1224406, uint256S("0xa55d78df58fca4aec78e658614f004c2ad5930779e512447a0d96c5bb427cbe7"))
            (  1424405, uint256S("0x820d255f359325f8c1792d776bef999bc8c54ab2cfb0d9f0b5d2281b759bccb3"))
            (  1624406, uint256S("0xe58f3416b69542699b3e9662ea0a36318166753080b49e06e8b0a884efcb581b"))
            (  1826401, uint256S("0x37c0d97ef3e0a24753b3e0eebe93588ad21b302f66c78b1046cbadeedd52c196"))
            (  1924401, uint256S("0x2267dd4753f9e8fa088cdc4d257355b0a118ccd43a2f366c68de1aa260c4ba22"))
            (  1934707, uint256S("0xe4527742cb236f24b355156213775e44da0e728ea453fae712fa672b18ce5f84"))
            (  2050260, uint256S("0xbc55838e026131fab334f1c87de9fea3218d076f4de03dc19f94d4f12da7d6c1")),
        1477330596, // * UNIX timestamp of last checkpoint block
        2263089,    // * total number of transactions between genesis and last checkpoint
                    //   (the tx=... number in the SetBestChain debug.log lines)
        2000        // * estimated number of transactions per day after checkpoint
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 100;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.powLimit = ArithToUint256(~arith_uint256(0) >> 16);
        
        consensus.nStartAuxPow = 100;
        consensus.nAuxpowChainId = 0x004A3; 
        consensus.fStrictChainId = false;
        
        //consensus.nBlockTimeWarpPreventStart1 = 740500; // block where time warp 1 prevention starts
        //consensus.nBlockTimeWarpPreventStart2 = 766000; // block where time warp 2 prevention starts
        //consensus.nBlockTimeWarpPreventStart3 = 1048320; // block where time warp 3 prevention starts
        //consensus.Phase2Timespan_Start = 150; // block where 45 second target time kicks in
        //consensus.nBlockDiffAdjustV2 = 150; // block where difficulty adjust V2 starts
        
        consensus.nMaxAdjustDown = 22; // 22% adjustment down
        consensus.nMaxAdjustUp = 14; // 14% adjustment up
        //consensus.nMaxAdjustUpV2 = 4; // 4% adjustment up     
        
        consensus.nBlockSequentialAlgoRuleStart1 = 100; // block where sequential algo rule starts
        consensus.nBlockSequentialAlgoRuleStart2 = 150; // block where sequential algo rule starts
        consensus.nBlockSequentialAlgoMaxCount1 = 3; // maximum sequential blocks of same algo
        consensus.nBlockSequentialAlgoMaxCount2 = 6; // maximums equential blocks of same algo
        //consensus.nBlockSequentialAlgoMaxCount3 = 6; // maximum sequential blocks of same algo
        
        //consensus.nBlockAlgoWorkWeightStart = 0; // block where algo work weighting starts
        //consensus.nBlockAlgoNormalisedWorkStart = 0; // block where algo combined weight starts
        //consensus.nBlockAlgoNormalisedWorkDecayStart1 = 0; // block where weight decay starts
        //consensus.nBlockAlgoNormalisedWorkDecayStart2 = 0; // block where weight decay starts
        //consensus.nGeoAvgWork_Start = 150;
        consensus.nDGW3StartBlock = 50; 
        consensus.nMultiAlgoFork = 150; // Block where multi-algo difficulty adjustment and 45 second blocktime starts
        consensus.nCoinbaseMaturityV2Start = 200;

        pchMessageStart[0] = 0xfc;
        pchMessageStart[1] = 0xc1;
        pchMessageStart[2] = 0xb7;
        pchMessageStart[3] = 0xdc;
        vAlertPubKey = ParseHex("042756726da3c7ef515d89212ee1705023d14be389e25fe15611585661b9a20021908b2b80a3c7200a0139dd2b26946606aab0eef9aa7689a6dc2c7eee237fa834");
        nDefaultPort = 40555;
        nMinerThreads = 0;
        nMaxTipAge = 0x7fffffff;
        nPruneAfterHeight = 1000;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 123456;
        genesis.nNonce = 123456;
        consensus.hashGenesisBlock = genesis.GetHash();
        
        //assert(consensus.hashGenesisBlock == uint256S("0x0000017ce2a79c8bddafbbe47c004aa92b20678c354b34085f62b762084b9788"));
// TODO
        vFixedSeeds.clear();
        vSeeds.clear();
        //vSeeds.push_back(CDNSSeedData("x.com", "x.com"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111); // 0x6F
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196); // 0xC4
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239); // 0xEF
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        /*checkpointData = (Checkpoints::CCheckpointData) { TODO
            boost::assign::map_list_of
            ( 0, uint256S("0x")),
            0, // * UNIX timestamp of last checkpoint block
            0, // * total number of transactions between genesis and last checkpoint
            0  // * estimated number of transactions per day after checkpoint
        }; */

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.powLimit = ArithToUint256(~arith_uint256(0) >> 1);
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nMinerThreads = 1;
        nMaxTipAge = 24 * 60 * 60;
        genesis.nTime = 1296688602;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 2;
        consensus.hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 18444;
        
        // TODO assert(consensus.hashGenesisBlock == uint256S("0x3d2160a3b5dc4a9d62e7e66a295f70313ac808440ef7400d6c0772171ce973a5"));
        nPruneAfterHeight = 1000;

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (Checkpoints::CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0x3d2160a3b5dc4a9d62e7e66a295f70313ac808440ef7400d6c0772171ce973a5")),
            0,
            0,
            0
        };
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams &Params(CBaseChainParams::Network network) {
    switch (network) {
        case CBaseChainParams::MAIN:
            return mainParams;
        case CBaseChainParams::TESTNET:
            return testNetParams;
        case CBaseChainParams::REGTEST:
            return regTestParams;
        default:
            assert(false && "Unimplemented network");
            return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network) {
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
