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
#include "bignum.h"
#include "validation.h"

static const int64_t nTargetTimespan = 32 * 250; // Argentum: every 250 blocks
static const int64_t nTargetSpacing = 32; // Argentum: 32 sec
static const int64_t nInterval = nTargetTimespan / nTargetSpacing;

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, int algo, const Consensus::Params& params)
{
        if (pindexLast->nHeight >= params.nBIP146Height){
            return DigiSpeed(pindexLast, pblock, params, algo);
        }
        else if (pindexLast->nHeight >= params.nMultiAlgoFork)
        {
            return StabilX(pindexLast, pblock, params, algo);
        } 
        else if (pindexLast->nHeight >= params.nBlockDiffAdjustV2)
        {
            return DarkGravityWave3(pindexLast, pblock, params, algo);
        } 
        return GetNextWorkRequired_Legacy(pindexLast, pblock, params, algo);

}

unsigned int GetNextWorkRequired_Legacy(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params, int algo)
{
    const arith_uint256 nProofOfWorkLimit = UintToArith256(params.powLimit);

    // argentum difficulty adjustment protocol switch
    int nHeight = pindexLast->nHeight + 1;

    // Genesis block
    if (pindexLast == NULL)
    {
        if(fDebug)
        {
            LogPrintf("pindexLast is null. returning nProofOfWorkLimit\n");
        }
        return nProofOfWorkLimit.GetCompact();
    }

    // Only change once per interval
    if ((pindexLast->nHeight+1) % nInterval != 0)
    {
       /* // Special difficulty rule for testnet:
       // if (TestNet())
        //{
            // If the new block's timestamp is more than 2* 10 minutes
            // then allow mining of a min-difficulty block.
         //   if (pblock->nTime > pindexLast->nTime + nTargetSpacing*2)
           //     return nProofOfWorkLimit.GetCompact();
           // else
            {
                // Return the last non-special-min-difficulty-rules-block
                const CBlockIndex* pindex = pindexLast;
                while (pindex->pprev && pindex->nHeight % nInterval != 0 && pindex->nBits == nProofOfWorkLimit)
                    pindex = pindex->pprev;
                return pindex->nBits;
            }
        //}*/

        return pindexLast->nBits;
    }

    // argentum: This fixes an issue where a 51% attack can change difficulty at will.
    // Go back the full period unless it's the first retarget after genesis. Code courtesy of Art Forz
    int blockstogoback = nInterval-1;
    if ((pindexLast->nHeight+1) != nInterval)
        blockstogoback = nInterval;

    // Go back by what we want to be the last intervals worth of blocks
    const CBlockIndex* pindexFirst = pindexLast;
    for (int i = 0; pindexFirst && i < blockstogoback; i++)
        pindexFirst = pindexFirst->pprev;
    assert(pindexFirst);

    // Limit adjustment step
    int64_t nActualTimespan = pindexLast->GetBlockTime() - pindexFirst->GetBlockTime();

    if(nHeight >= 111500){ 
    if (nActualTimespan < ((nTargetTimespan*55)/73))
        nActualTimespan = ((nTargetTimespan*55)/73);
    if (nActualTimespan > ((nTargetTimespan*75)/60))
        nActualTimespan = ((nTargetTimespan*75)/60);
    }
    else if(nHeight >= 79000){ 
    if (nActualTimespan < nTargetTimespan/2)
        nActualTimespan = nTargetTimespan/2;
    if (nActualTimespan > nTargetTimespan*2)
        nActualTimespan = nTargetTimespan*2;
    }
    else if(nHeight > 10000){ 
    if (nActualTimespan < nTargetTimespan/4)
        nActualTimespan = nTargetTimespan/4;
    if (nActualTimespan > nTargetTimespan*4)
        nActualTimespan = nTargetTimespan*4;
    }
    else if(nHeight > 5000){ 
    if (nActualTimespan < nTargetTimespan/8)
        nActualTimespan = nTargetTimespan/8;
    if (nActualTimespan > nTargetTimespan*4)
        nActualTimespan = nTargetTimespan*4;
    }
    else{ 
    if (nActualTimespan < nTargetTimespan/16)
        nActualTimespan = nTargetTimespan/16;
    if (nActualTimespan > nTargetTimespan*4)
        nActualTimespan = nTargetTimespan*4;
    }

    // Retarget
    arith_uint256 bnNew;
    arith_uint256 bnOld;
    bnNew.SetCompact(pindexLast->nBits);
    bnOld = bnNew;
    bnNew *= nActualTimespan;
    bnNew /= nTargetTimespan;

    if (bnNew > nProofOfWorkLimit)
        bnNew = nProofOfWorkLimit;

    return bnNew.GetCompact();
}

unsigned int DarkGravityWave3(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params, int algo) 
{
    /* current difficulty formula, darkcoin - DarkGravity v3, written by Evan Duffield - evan@darkcoin.io */
    const arith_uint256 nProofOfWorkLimit = UintToArith256(params.powLimit);

    const CBlockIndex *BlockLastSolved = pindexLast;
    const CBlockIndex *BlockReading = pindexLast;
    const CBlockHeader *BlockCreating = pblock;
    BlockCreating = BlockCreating;
    int64_t nActualTimespan = 0;
    int64_t LastBlockTime = 0;
    int64_t PastBlocksMin = 24;
    int64_t PastBlocksMax = 24;
    int64_t CountBlocks = 0;
    arith_uint256 PastDifficultyAverage;
    arith_uint256 PastDifficultyAveragePrev;

    if (BlockLastSolved == NULL || BlockLastSolved->nHeight == 0 || BlockLastSolved->nHeight < PastBlocksMin) { 
        return nProofOfWorkLimit.GetCompact(); 
    }
        
    for (unsigned int i = 1; BlockReading && BlockReading->nHeight > 0; i++) {
        if (PastBlocksMax > 0 && i > PastBlocksMax) { break; }
        CountBlocks++;

        if(CountBlocks <= PastBlocksMin) {
            if (CountBlocks == 1) { PastDifficultyAverage.SetCompact(BlockReading->nBits); }
            else { PastDifficultyAverage = ((PastDifficultyAveragePrev * CountBlocks)+(arith_uint256().SetCompact(BlockReading->nBits))) / (CountBlocks+1); }
            PastDifficultyAveragePrev = PastDifficultyAverage;
        }

        if(LastBlockTime > 0){
            int64_t Diff = (LastBlockTime - BlockReading->GetBlockTime());
            nActualTimespan += Diff;
        }
        LastBlockTime = BlockReading->GetBlockTime();      

        if (BlockReading->pprev == NULL) { assert(BlockReading); break; }
        BlockReading = BlockReading->pprev;
    }
    
    arith_uint256 bnNew(PastDifficultyAverage);

    int64_t nTargetTimespan = CountBlocks*nTargetSpacing;

    if (nActualTimespan < nTargetTimespan/3)
        nActualTimespan = nTargetTimespan/3;
    if (nActualTimespan > nTargetTimespan*3)
        nActualTimespan = nTargetTimespan*3;

    // Retarget
    bnNew *= nActualTimespan;
    bnNew /= nTargetTimespan;

    if (bnNew > nProofOfWorkLimit){
        bnNew = nProofOfWorkLimit;
    }
     
    return bnNew.GetCompact();
}

unsigned int StabilX(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params, int algo)
{
    const arith_uint256 nProofOfWorkLimit = UintToArith256(params.powLimit);

    int64_t nTargetSpacingPerAlgo = params.nPowTargetSpacingV2 * NUM_ALGOS; // 90 Seconds (NUM_ALGOS * 45 seconds)
    int64_t nAveragingTargetTimespan = params.nAveragingInterval * nTargetSpacingPerAlgo; // 10 * 90 = 900s, 15 minutes
    int64_t nMinActualTimespan = nAveragingTargetTimespan * (100 - params.nMaxAdjustUp) / 100;
    int64_t nMaxActualTimespan = nAveragingTargetTimespan * (100 + params.nMaxAdjustDown) / 100;
    
    // Genesis block
    if (pindexLast == NULL)
        return nProofOfWorkLimit.GetCompact();
    
    const CBlockIndex* pindexPrev = GetLastBlockIndexForAlgo(pindexLast, algo);
    
    // find first block in averaging interval
    // Go back by what we want to be nAveragingInterval blocks per algo
    const CBlockIndex* pindexFirst = pindexLast;
    for (int i = 0; pindexFirst && i < NUM_ALGOS*params.nAveragingInterval; i++)
    {
        pindexFirst = pindexFirst->pprev;
    }
    const CBlockIndex* pindexPrevAlgo = GetLastBlockIndexForAlgo(pindexLast, algo);
    if (pindexPrevAlgo == NULL || pindexFirst == NULL)
    {
        if(fDebug)
        {
            LogPrintf("StabilX(Algo=%d): not enough blocks available, using default POW limit\n");
        }
        return nProofOfWorkLimit.GetCompact(); // not enough blocks available
    }

    // Limit adjustment step
    // Use medians to prevent time-warp attacks
    int64_t nActualTimespan = pindexLast->GetMedianTimePast() - pindexFirst->GetMedianTimePast();
    nActualTimespan = nAveragingTargetTimespan + (nActualTimespan - nAveragingTargetTimespan)/6;
    if(fDebug)
    {
        LogPrintf("StabilX(Algo=%d): nActualTimespan = %d before bounds (%d - %d)\n", algo, nActualTimespan, nMinActualTimespan, nMaxActualTimespan);
    }
    if (nActualTimespan < nMinActualTimespan)
        nActualTimespan = nMinActualTimespan;
    if (nActualTimespan > nMaxActualTimespan)
        nActualTimespan = nMaxActualTimespan;
    if(fDebug)
    {
        LogPrintf("StabilX(Algo=%d): nActualTimespan = %d after bounds (%d - %d)\n", algo, nActualTimespan, nMinActualTimespan, nMaxActualTimespan);
    }
    
    // Global retarget
    arith_uint256 bnNew;
    arith_uint256 bnOld;
    bnNew.SetCompact(pindexPrevAlgo->nBits);
    bnOld = bnNew;
    bnNew *= nActualTimespan;
    bnNew /= nAveragingTargetTimespan;

    // Per-algo retarget
    int nAdjustments = pindexPrevAlgo->nHeight - pindexLast->nHeight + NUM_ALGOS - 1;
    if (nAdjustments > 0)
    {
        for (int i = 0; i < nAdjustments; i++)
        {
            bnNew *= 100;
            bnNew /= (100 + params.nLocalDifficultyAdjustment);
        }
    }
    else if (nAdjustments < 0)
    {
        for (int i = 0; i < -nAdjustments; i++)
        {
            bnNew *= (100 + params.nLocalDifficultyAdjustment);
            bnNew /= 100;
        }
    }

    if (bnNew > nProofOfWorkLimit)
    {
        if(fDebug)
        {
            LogPrintf("StabilX(Algo=%d): Adjusted target large than limit, so is now POW limit\n", algo);
        }
        bnNew = nProofOfWorkLimit;
    }

    return bnNew.GetCompact();
}

unsigned int DigiSpeed(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params, int algo)
{
    unsigned int npowWorkLimit = UintToArith256(params.powLimit).GetCompact();

    int64_t nTargetSpacingPerAlgo = params.nPowTargetSpacingV2 * NUM_ALGOS2; // 270 Seconds (NUM_ALGOS2 * 45 seconds)
    int64_t nAveragingTargetTimespan = params.nAveragingInterval * nTargetSpacingPerAlgo; // 10 * 270 = 2700s, 45 minutes
    int64_t nMinActualTimespan = nAveragingTargetTimespan * (100 - params.nMaxAdjustUpV2) / 100;
    int64_t nMaxActualTimespan = nAveragingTargetTimespan * (100 + params.nMaxAdjustDown) / 100;

    // Genesis block
    if (pindexLast == NULL)
        return npowWorkLimit;

    // find first block in averaging interval
    // Go back by what we want to be nAveragingInterval blocks per algo
    const CBlockIndex* pindexFirst = pindexLast;
    for (int i = 0; pindexFirst && i < NUM_ALGOS2*params.nAveragingInterval; i++)
    {
        pindexFirst = pindexFirst->pprev;
    }

    const CBlockIndex* pindexPrevAlgo = GetLastBlockIndexForAlgo(pindexLast, algo);
    if (pindexPrevAlgo == NULL || pindexFirst == NULL)
    {
        return npowWorkLimit;
    }

    // Limit adjustment step
    // Use medians to prevent time-warp attacks
    int64_t nActualTimespan = pindexLast-> GetMedianTimePast() - pindexFirst->GetMedianTimePast();
    nActualTimespan = nAveragingTargetTimespan + (nActualTimespan - nAveragingTargetTimespan)/4;
    if (fDebug)
    {
        LogPrintf("DigiSpeed(Algo=%d): nActualTimespan = %d before bounds   %d   %d\n", algo, nActualTimespan, pindexLast->GetMedianTimePast(), pindexFirst->GetMedianTimePast());
    }
    if (nActualTimespan < nMinActualTimespan)
        nActualTimespan = nMinActualTimespan;
    if (nActualTimespan > nMaxActualTimespan)
        nActualTimespan = nMaxActualTimespan;
    if(fDebug)
    {
        LogPrintf("DigiSpeed(Algo=%d): nActualTimespan = %d after bounds (%d - %d)\n", algo, nActualTimespan, nMinActualTimespan, nMaxActualTimespan);
    }
    //Global retarget
    arith_uint256 bnNew;
    bnNew.SetCompact(pindexPrevAlgo->nBits);

    bnNew *= nActualTimespan;
    bnNew /= nAveragingTargetTimespan;

    //Per-algo retarget
    int nAdjustments = pindexPrevAlgo->nHeight + NUM_ALGOS2 - 1 - pindexLast->nHeight;
    if (nAdjustments > 0)
    {
        for (int i = 0; i < nAdjustments; i++)
        {
            bnNew *= 100;
            bnNew /= (100 + params.nLocalDifficultyAdjustment);
        }
    }
    else if (nAdjustments < 0)//make it easier
    {
        for (int i = 0; i < -nAdjustments; i++)
        {
            bnNew *= (100 + params.nLocalDifficultyAdjustment);
            bnNew /= 100;
        }
    }

    if (bnNew > UintToArith256(params.powLimit))
    {
        bnNew = UintToArith256(params.powLimit);
    }

    return bnNew.GetCompact();
}

bool CheckProofOfWork(uint256 hash, int algo, unsigned int nBits, const Consensus::Params& params)
{
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    int nHeight = chainActive.Height();

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(params.powLimit))
        return error("CheckProofOfWork(): nBits below minimum work");

    // Check proof of work matches claimed amount
    if (nHeight > params.nCoinbaseMaturityV2Start){
        if (UintToArith256(hash) > bnTarget)
            return error("CheckProofOfWork(): hash doesn't match nBits");}

        /*if (UintToArith256(hash) > bnTarget)
            return error("CheckProofOfWork(): hash doesn't match nBits");*/

    return true;
}

bool CheckProofOfWorkB(uint256 hash, int algo, unsigned int nBits, const Consensus::Params& params)
{
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    //int nHeight = chainActive.Height();

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(params.powLimit))
        return error("CheckProofOfWork(): nBits below minimum work");

    // // Check proof of work matches claimed amount
    // if (nHeight > params.nCoinbaseMaturityV2Start){
    //     if (UintToArith256(hash) > bnTarget)
    //         return error("CheckProofOfWork(): hash doesn't match nBits");}

    //     if (UintToArith256(hash) > bnTarget)
    //         return error("CheckProofOfWork(): hash doesn't match nBits");

    return true;
}
