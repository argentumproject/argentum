// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"
#include "primitives/pureheader.h"
#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include "arith_uint256.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"

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
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Edward Snowden says he's behind NSA leaks - CNN - June 10 2013";
    const CScript genesisOutputScript = CScript() << ParseHex("040184710fa589ad5023690c80f3a49c8f13f8d45b8c857fbcbc8bc4a8e4d3eb4b10f4d4604fa08dce601aaf0f470216fe1b51850b4acf21b179c45070ac7b03a9") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
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

        /*** Argentum Additional Chainparams ***/

        consensus.nPowTargetSpacingV1 = 32; // target time for block spacing across all algorithms
        consensus.nPowTargetSpacingV2 = 45; // new target time for block spacing across all algorithms
        consensus.nAveragingInterval = 10; // number of blocks to take the timespan of

        consensus.nStartAuxPow = 1825000; // Allow AuxPow blocks from this height
        consensus.nAuxpowChainId = 0x004A3; 
        consensus.fStrictChainId = false;

        //consensus.nBlockTimeWarpPreventStart1 = 740500; // block where time warp 1 prevention starts
        //consensus.nBlockTimeWarpPreventStart2 = 766000; // block where time warp 2 prevention starts
        //consensus.nBlockTimeWarpPreventStart3 = 1048320; // block where time warp 3 prevention starts
        //consensus.Phase2Timespan_Start = 1401000; // block where 60 second target time kicks in
        consensus.nBlockDiffAdjustV2 = 1635000; // block where difficulty adjust V2 starts (DGW)

        consensus.nMaxAdjustDown = 22; // 22% adjustment down
        consensus.nMaxAdjustUp = 14; // 14% adjustment up
        consensus.nMaxAdjustUpV2 = 12; // 12% adjustment up
        //consensus.nMaxAdjustUpV2 = 4; // 4% adjustment up
        consensus.nLocalDifficultyAdjustment = 12; // 12%     

        consensus.nBlockSequentialAlgoRuleStart1 = 1930000; // block where sequential algo rule starts
        consensus.nBlockSequentialAlgoRuleStart2 = 2032000; // block where sequential algo rule starts
        consensus.nBlockSequentialAlgoMaxCount1 = 3; // maximum sequential blocks of same algo
        consensus.nBlockSequentialAlgoMaxCount2 = 6; // maximum sequential blocks of same algo
        //consensus.nBlockSequentialAlgoMaxCount3 = 6; // maximum sequential blocks of same algo
        consensus.nMultiAlgoFork = 1930000; // Block where multi-algo difficulty adjustment and 45 second blocktime starts

        // consensus.nBlockAlgoWorkWeightStart = 142000; // block where algo work weighting starts
        // consensus.nBlockAlgoNormalisedWorkStart = 740000; // block where algo combined weight starts
        // consensus.nBlockAlgoNormalisedWorkDecayStart1 = 866000; // block where weight decay starts
        // consensus.nBlockAlgoNormalisedWorkDecayStart2 = 932000; // block where weight decay starts
        consensus.nGeoAvgWork_Start = 2387838;
        //consensus.nFork1MinBlock = 1764000; // minimum block height where fork 1 takes effect (algo switch, seq algo count change)
        consensus.nCoinbaseMaturityV2Start = 2387838;

        // Deployment of Legacy Blocks. Once activated, keeps v0.11 nodes on the same chain. Should be the first softfork.
        consensus.vDeployments[Consensus::DEPLOYMENT_LEGBIT].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_LEGBIT].nStartTime = 1516290600; // January 18th, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_LEGBIT].nTimeout = 1547826600; // January 18th, 2019

        /*** Upstream Chainparams ***/

        //consensus.nSubsidyHalvingInterval = 80640 * 12;
        consensus.BIP34Height = 1796159;
        consensus.BIP34Hash = uint256S("0x85a6966de131c03e41884bb5062abc20a14227cb6e164dc5a6f55db9164222ce");
        consensus.BIP65Height = 2387838; // 00000000000000254214c9f9fbdd39c07072b43df52f35a910509f4c5406c272  
        consensus.BIP66Height = 1796159; // 85a6966de131c03e41884bb5062abc20a14227cb6e164dc5a6f55db9164222ce   
        consensus.powLimit = ArithToUint256(~arith_uint256(0) >> 20);
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = consensus.nPowTargetSpacingV2; // Current value
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 20160; // 75% of 26880
        consensus.nMinerConfirmationWindow = 26880; // 2 weeks based on 1920 blocks per day.
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113. *Argentum: To be activated at a later date
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1521547200; // March 20th, 2018 12:00 GMT
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1552996800; // March 19th , 2019 12:00 GMT

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x43964064d8605d2bb89972f07919dc5b1a00457b3919eb6010003c8d0b242c93"); //2210002

        // BIP146 fork
        consensus.nBIP146Height = 2977000; // Around March 13th 2018

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xfb;
        pchMessageStart[1] = 0xc1;
        pchMessageStart[2] = 0xb8;
        pchMessageStart[3] = 0xdc;
        nDefaultPort = 13580;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1369199888, 12786092, 0x1e0ffff0, 1, 5 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        
        assert(consensus.hashGenesisBlock == uint256S("0x88c667bc63167685e4e4da058fffdfe8e007e5abffd6855de52ad59df7bb0bb2"));
        assert(genesis.hashMerkleRoot == uint256S("0xc7e4af4190d3eb28d4e61261fb1b47e13d7efd6d7279c75ae6a91c2987d43d53"));

        vSeeds.push_back(CDNSSeedData("", ""));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,23);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,151);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_COIN_TYPE] =  boost::assign::list_of(0x8000002D).convert_to_container<std::vector<unsigned char> >(); // Argentum BIP 44 index is 45

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (      0, uint256S("0x88c667bc63167685e4e4da058fffdfe8e007e5abffd6855de52ad59df7bb0bb2"))
            (      1, uint256S("0xdf0a20b6609b206448778428648f42a592dea10884a3fca1ce3a4c2ce12caf2f"))
            (      2, uint256S("0xa10fd82c25b5ddcbcb9077cf9a70b47502a449663655a5d9cec09c19556c6d43"))
            (  50000, uint256S("0xa8b62a4dd6c8aa28e2bf68c521e01da0638214f2def124a551f1a90df8402b6d"))
            ( 124415, uint256S("0x6d5a842bea984df8088f910d27f81af72ccfeef2bde21e100db036a51e17d6f8"))
            ( 224414, uint256S("0x2df7b26aea2376760533552baa54a61044d13f2c6f44ff017e855e9593aa0216"))
            ( 624415, uint256S("0x4651a604984d4494d09d1bb8c9f1c951e9bb214b5c930af4d0ad0bff7c513057"))
            ( 824404, uint256S("0x91456228171e75954e44e44b740ba075d179c11c31fc0d403843a582316a9e21"))
            (1024406, uint256S("0xf70a1dbf9c54f114690dfc7d6ac03366034e2c7a4dcd4fb71cf25f079c46c859"))
            (1224406, uint256S("0xa55d78df58fca4aec78e658614f004c2ad5930779e512447a0d96c5bb427cbe7"))
            (1424405, uint256S("0x820d255f359325f8c1792d776bef999bc8c54ab2cfb0d9f0b5d2281b759bccb3"))
            (1624406, uint256S("0xe58f3416b69542699b3e9662ea0a36318166753080b49e06e8b0a884efcb581b"))
            (1826401, uint256S("0x37c0d97ef3e0a24753b3e0eebe93588ad21b302f66c78b1046cbadeedd52c196"))
            (1924401, uint256S("0x2267dd4753f9e8fa088cdc4d257355b0a118ccd43a2f366c68de1aa260c4ba22"))
            (1934707, uint256S("0xe4527742cb236f24b355156213775e44da0e728ea453fae712fa672b18ce5f84"))
            (2050260, uint256S("0xbc55838e026131fab334f1c87de9fea3218d076f4de03dc19f94d4f12da7d6c1"))
            (2365716, uint256S("0x947827b4599512a675fc3789ac8778b3187277705ffda7b6e3cf6adaa31dac10"))
            (2387838, uint256S("0x00000000000000254214c9f9fbdd39c07072b43df52f35a910509f4c5406c272"))
            (2400814, uint256S("0x6b5b226b0f896a286c0768f51c128fa4d9be6bb985c264b19ecb180e8b81e4b4"))
            (2855151, uint256S("0x43964064d8605d2bb89972f07919dc5b1a00457b3919eb6010003c8d0b242c93"))
        };

        chainTxData = ChainTxData{
            // Data as of block 00000000000000000166d612d5595e2b1cd88d71d695fc580af64d8da8658c23 (height 2210002).
            1515539327, // * UNIX timestamp of last known number of transactions
            3140727,    // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            0.0347        // * estimated number of transactions per second after that timestamp
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";

        /*** Argentum Additional Chainparams ***/

        consensus.nPowTargetSpacingV1 = 32; // target time for block spacing across all algorithms
        consensus.nPowTargetSpacingV2 = 45; // new target time for block spacing across all algorithms
        consensus.nAveragingInterval = 10; // number of blocks to take the timespan of

        consensus.nStartAuxPow = 40;
        consensus.nAuxpowChainId = 0x004A3; 
        consensus.fStrictChainId = false;

        // consensus.nBlockTimeWarpPreventStart1 = 1000; // block where time warp 1 prevention starts
        // consensus.nBlockTimeWarpPreventStart2 = 1005; // block where time warp 2 prevention starts
        // consensus.nBlockTimeWarpPreventStart3 = 1010; // block where time warp 3 prevention starts
        // consensus.Phase2Timespan_Start = 150; // block where 60 second target time kicks in
        consensus.nBlockDiffAdjustV2 = 30; // block where difficulty adjust V2 starts

        consensus.nMaxAdjustDown = 22; // 4% adjustment down
        consensus.nMaxAdjustUp = 14; // 2% adjustment up
        // consensus.nMaxAdjustUpV2 = 4; // 4% adjustment up
        consensus.nLocalDifficultyAdjustment = 12; // 12%     

        consensus.nBlockSequentialAlgoRuleStart1 = 55; // block where sequential algo rule starts
        consensus.nBlockSequentialAlgoRuleStart2 = 60; // block where sequential algo rule starts
        consensus.nBlockSequentialAlgoMaxCount1 = 3; // maximum sequential blocks of same algo
        consensus.nBlockSequentialAlgoMaxCount2 = 6; // maximum sequential blocks of same algo
        // consensus.nBlockSequentialAlgoMaxCount3 = 6; // maximum sequential blocks of same algo

        // consensus.nBlockAlgoWorkWeightStart = 0; // block where algo work weighting starts
        // consensus.nBlockAlgoNormalisedWorkStart = 0; // block where algo combined weight starts
        // consensus.nBlockAlgoNormalisedWorkDecayStart1 = 0; // block where weight decay starts
        // consensus.nBlockAlgoNormalisedWorkDecayStart2 = 0; // block where weight decay starts
        consensus.nGeoAvgWork_Start = 95;
        consensus.nCoinbaseMaturityV2Start = 100;
        consensus.nMultiAlgoFork = 55;
        //consensus.nFork1MinBlock = 601; // minimum block height where fork 1 takes effect (algo switch, seq algo count change)

        // Deployment of Legacy Blocks. Once activated, keeps v0.11 nodes on the same chain. Should be the first softfork.
        consensus.vDeployments[Consensus::DEPLOYMENT_LEGBIT].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_LEGBIT].nStartTime = 1516290600; // January 18th, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_LEGBIT].nTimeout = 1547826600; // January 18th, 2019

        /*** Upstream Chainparams ***/

        //consensus.nSubsidyHalvingInterval = 80640 * 12;
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256S("0x6bee778b0f99ee7a02635bc7de7d2c28f8a844f8c3aa01cb19b02adb9a169461");
        consensus.BIP65Height = 40; // df5c7c64c02981a2b0407a5f0672e5cad03b06c36f0c07046cfe0336eee20dec
        consensus.BIP66Height = 1; // efd0b4a4cb4ef4e7554e0f4602afd079fdff0a4c52fad184781f90d9eb6c41a7
        consensus.powLimit = ArithToUint256(~arith_uint256(0) >> 16);
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = consensus.nPowTargetSpacingV2; // Current value
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 756; // 75% for testchains
        consensus.nMinerConfirmationWindow = 1008; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113. *Argentum: To be activated at a later date
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1737215400; // January 18th, 2025
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1768751400; // January 18th, 2026

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00"); // 40

        // BIP146 fork
        consensus.nBIP146Height = 100; 

        pchMessageStart[0] = 0xfc;
        pchMessageStart[1] = 0xc1;
        pchMessageStart[2] = 0xb7;
        pchMessageStart[3] = 0xdc;
        nDefaultPort = 13555;
        nPruneAfterHeight = 1000;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis = CreateGenesisBlock(1516763009, 295222, 0x1e0ffff0, 1, 5 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x6bee778b0f99ee7a02635bc7de7d2c28f8a844f8c3aa01cb19b02adb9a169461"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.push_back(CDNSSeedData("", ""));


        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,88);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,188);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;


        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (   0, uint256S("0x6bee778b0f99ee7a02635bc7de7d2c28f8a844f8c3aa01cb19b02adb9a169461"))
        };

        chainTxData = ChainTxData{
            // 
            1516763009,
            817,
            0.02
        };

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";

        /*** Argentum Additional Chainparams ***/

        consensus.nPowTargetSpacingV1 = 30; // target time for block spacing across all algorithms
        consensus.nPowTargetSpacingV2 = 60; // new target time for block spacing across all algorithms
        consensus.nAveragingInterval = 10; // number of blocks to take the timespan of

        consensus.nStartAuxPow = 150;
        consensus.nAuxpowChainId = 0x005A;
        consensus.fStrictChainId = false;

        // consensus.nBlockTimeWarpPreventStart1 = 1000; // block where time warp 1 prevention starts
        // consensus.nBlockTimeWarpPreventStart2 = 1005; // block where time warp 2 prevention starts
        // consensus.nBlockTimeWarpPreventStart3 = 1010; // block where time warp 3 prevention starts
        // consensus.Phase2Timespan_Start = 150; // block where 60 second target time kicks in
        consensus.nBlockDiffAdjustV2 = 150; // block where difficulty adjust V2 starts

        consensus.nMaxAdjustDown = 4; // 4% adjustment down
        consensus.nMaxAdjustUp = 2; // 2% adjustment up
        // consensus.nMaxAdjustUpV2 = 4; // 4% adjustment up

        consensus.nBlockSequentialAlgoRuleStart1 = 200; // block where sequential algo rule starts
        consensus.nBlockSequentialAlgoRuleStart2 = 250; // block where sequential algo rule starts
        consensus.nBlockSequentialAlgoMaxCount1 = 3; // maximum sequential blocks of same algo
        consensus.nBlockSequentialAlgoMaxCount2 = 6; // maximum sequential blocks of same algo
        // consensus.nBlockSequentialAlgoMaxCount3 = 6; // maximum sequential blocks of same algo

        // consensus.nBlockAlgoWorkWeightStart = 0; // block where algo work weighting starts
        // consensus.nBlockAlgoNormalisedWorkStart = 0; // block where algo combined weight starts
        // consensus.nBlockAlgoNormalisedWorkDecayStart1 = 0; // block where weight decay starts
        // consensus.nBlockAlgoNormalisedWorkDecayStart2 = 0; // block where weight decay starts
        consensus.nGeoAvgWork_Start = 0;
        //consensus.nFork1MinBlock = 601; // minimum block height where fork 1 takes effect (algo switch, seq algo count change)

        consensus.vDeployments[Consensus::DEPLOYMENT_LEGBIT].bit = 2;
        consensus.vDeployments[Consensus::DEPLOYMENT_LEGBIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_LEGBIT].nTimeout = 999999999999ULL;

        /*** Upstream Chainparams ***/

        //consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = consensus.nPowTargetSpacingV2; // Current value
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

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        // BIP146 fork is always on on regtest.
        consensus.nBIP146Height = 0;

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xdb;
        nDefaultPort = 18445;
        nPruneAfterHeight = 1000;

        // genesis = CreateGenesisBlock(1296688602, 4, 0x207fffff, 2, 1000 * COIN);
        // consensus.hashGenesisBlock = genesis.GetHash();
        // assert(consensus.hashGenesisBlock == uint256S("0x63b92987ddc93808aa33dddc80b3e52948bdfffaf2420bf4cd9c5137b54ea37c"));
        // assert(genesis.hashMerkleRoot == uint256S("0x3f75db3c18e92f46c21530dc1222e1fddf4ccebbf88e289a6c9dc787fd6469da"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0x63b92987ddc93808aa33dddc80b3e52948bdfffaf2420bf4cd9c5137b54ea37c"))
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
    }

    void UpdateBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

void UpdateRegtestBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    regTestParams.UpdateBIP9Parameters(d, nStartTime, nTimeout);
}
 
