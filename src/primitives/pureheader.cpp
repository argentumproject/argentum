// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "primitives/pureheader.h"

#include "hash.h"
#include "crypto/hashgroestl.h"
#include "crypto/lyra2/lyra2RE.h"
#include "crypto/hashargon2d.h"
#include "crypto/scrypt.h"
#include "crypto/yescrypt.h"
#include "crypto/lyra2/lyra2RE.h"
#include "utilstrencodings.h"

uint256 CPureBlockHeader::GetHash() const
{
    return SerializeHash(*this);
}

uint256 CPureBlockHeader::GetPoWHash(int algo, const Consensus::Params& consensusParams) const
{
    switch (algo)
    {
        case ALGO_SHA256D:
            return GetHash();
        case ALGO_SCRYPT:
        {
            uint256 thash;
            scrypt_1024_1_1_256(BEGIN(nVersion), BEGIN(thash));
            return thash;
        }
        case ALGO_LYRA2RE2:
            {
            uint256 thash;
            lyra2re2_hash(BEGIN(nVersion), BEGIN(thash));
            return thash;
            }
        case ALGO_GROESTL:
            return HashGroestl(BEGIN(nVersion), END(nNonce));
        case ALGO_ARGON2D:
            return HashArgon2d(BEGIN(nVersion), END(nNonce));
        case ALGO_YESCRYPT:
        {
            uint256 thash;
            yescrypt_hash(BEGIN(nVersion), BEGIN(thash));
            return thash;
        }
    }
    return GetHash();
}

void CPureBlockHeader::SetBaseVersion(int32_t nBaseVersion, int32_t nChainId)
{
    //assert(nBaseVersion >= 1 && nBaseVersion < VERSION_AUXPOW);
    assert(!IsAuxpow());
    nVersion = nBaseVersion | (nChainId * VERSION_CHAIN_START);
}

int GetAlgo(int nVersion)
{
    switch (nVersion & BLOCK_VERSION_ALGO)
    {
        case 1:
            return ALGO_SCRYPT;
        case BLOCK_VERSION_SHA256D:
            return ALGO_SHA256D;
        case BLOCK_VERSION_LYRA2RE2:
            return ALGO_LYRA2RE2;
        case BLOCK_VERSION_GROESTL:
            return ALGO_GROESTL;
        case BLOCK_VERSION_ARGON2D:
            return ALGO_ARGON2D;
        case BLOCK_VERSION_YESCRYPT:
            return ALGO_YESCRYPT;
    }
    return ALGO_SCRYPT;
}
