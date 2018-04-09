Argentum integration/staging tree
=====================================

[![Build Status](https://travis-ci.org/argentumproject/argentum.svg?branch=master)](https://travis-ci.org/argentumproject/argentum)

http://www.argentum.io

What is Argentum?
----------------

Argentum is an experimental digital currency that enables instant payments to
anyone, anywhere in the world. Argentum uses peer-to-peer technology to operate
with no central authority: managing transactions and issuing money are carried
out collectively by the network. Argentum is the name of open source
software which enables the use of this currency.

License
-------

Argentum is a fork of Bitcoin Core and inherits the terms of the MIT license. See 
[COPYING](COPYING) for more information or see https://opensource.org/licenses/MIT.

Development Process
-------------------

The `master` branch is occasionally built and tested, but is not guaranteed to be
completely stable. [Tags](https://github.com/argentumproject/argentum/tags) are created
regularly to indicate new official, stable release versions of Argentum.

The contribution workflow is described in [CONTRIBUTING.md](CONTRIBUTING.md).

Building
--------
See files in the [doc](doc) directory for generic build instructions for Windows,
OSX and Unix.

## Argentum Version 4.14.4
- Speed up Initial Blockchain Download

## Argentum Version 4.14.3
- Adds 4 new mining algorithms (Activates at block 2,977,000)
	-Lyra2re2 (GPU)  
	-Myr-Groestl (GPU/ASIC)  
	-Argon2d (CPU)  
	-Yescrypt (CPU/GPU)  

## Argentum Version 4.14.2
- These are just a few of the hundreds of enhancements with this new release

- BIP112 (CheckSequenceVerify) soft fork
- BIP146 Argentum will hard fork at block 2,977,000 (around March 13th 2018) to implement BIP146
- Signature validation using libsecp256k1
- Direct headers announcement (BIP 130)
- Automatically use Tor hidden services
- Notifications through ZMQ
- BIP9 softfork deployment
- Linux ARM builds
- Compact Block support (BIP 152)
- Hierarchical Deterministic Key Generation (HD wallets)
- Substantial improvments to the client load time. 

## General
### Six different mining algorithms
- Scrypt AUXPoW (Merged Mining)
- SHA256D AUXPoW (Merged Mining)
- Lyra2re2 (GPU)
- Myr-Groestl (GPU)
- Argon2d (CPU)
- Yescrypt (CPU)

## Blocks
- Max Block size 10mb
- 45 Second block time
- 3 Argentums per block

## Currency Creation
- 64 million Argentums will be created

## Security
- 6 independent mining algorithms

## Mining Settings
Use this to set the algorithm to SHA256D for mining (add to argentum.conf or use -algo=)  

algo=sha256d 
algo=lyra2re2 or lyra2 
algo=groestl 
algo=argon2d or argon 
algo=yescrypt 

## Mining Software (for best results, download specialized pool software from your pool of choice when able)
### Lyra2re2 / Myriad-Groestl (GPU)
- AMD: - https://github.com/nicehash/sgminer/releases or https://github.com/tpruvot/sgminer/releases
- NVIDIA: - https://github.com/tpruvot/ccminer/releases
- You might be able to get a hashrate increase using this Myriad-Groestl kernel with SGMINER https://bitcointalk.org/index.php?topic=1211739.0

### Argon2d (CPU)
- https://github.com/unitusdev/unitus-cpuminer

### Yescrypt
- https://github.com/unitusdev/unitus-cpuminer
- https://github.com/nicehash/sgminer/releases
- https://github.com/koto-dev/cpuminer-yescrypt/releases

### P2Pool (All Algorithms)
- https://github.com/argentumproject/p2pool-argentum/

- Point your miner to:
- Argon2d 144.202.80.45:9552
- Myr-groestl 45.77.210.177:9553 
- Yescrypt 45.76.112.155:9554
- Lyra 45.76.240.8:9557

## Known pools (*with Argentum Payout*)
- https://prohashing.com (Scrypt)
- http://blockmunch.club/ (SHA256)
- http://www.zpool.ca/ (all but Argon2d)
- https://www.joinmycrypto.tk (Yescrypt)
- http://poolovich.pro/ (Myriad-Groestl)

## Pools without Argentum payout
- https://litecoinpool.org

## Bootstrap
- https://electrum-arg.org/bootstrap.dat  
- sha256 checksum 37020ed0abe02d7bf97a9dde7de0419d278f88866ff9d67419492d2be0dd32db

- https://electrum-arg.org/bootstrap.zip  

November 1, 2017
Block 2,723,638