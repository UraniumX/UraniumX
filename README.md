UraniumX integration/staging tree
==================================

What is Uranium-X?
--------------------
UraniumX is a P2P digital asset that enables instant transfers to anyone, anywhere 
in the world.  UraniumX uses peer-to-peer technology to operate with no central 
authority: managing transactions and issuing coins are carried out collectively 
by the network. UraniumX is the name of the open source software which enables 
the use of this coin.

Blockchain
----------
- **Type** Proof-of-Work (PoW) mining
- **Max Supply**: 235,000 URX
- **Current Block Reward**: 0.25 URX, halving every few years
- **Block Time**: ~5 Minutes
- **Emission length**: ~70 years
- **First block** 1-APR-2018

Halving Table, URX Emissions
----------
nHeight --> Rewards
----------

1 - 23499 --> 1.0 URX
23500 - 46999 --> 0.5 URX
47000 - 57649 --> 0.25 URX
57650--> replacement funds 2118.25 (CVE bug, see Whitepaper)
57651 - 257239 --> 0.25 URX
257240 - 467479 --> 0.125 URX
467480 - 677719 --> 0.06250 URX
677720 - 887959 -->  0.03125 URX
...etc...
//Force block reward to zero when right shift is undefined.
    if (nHeight >= 7364760)
//Subsidy is cut in half every 23500 URX in the first phase, and then every 210240 blocks

Proof of Work
-------------
- **PoW Algorithm**: blocks 0-105000 was Argon2ad. After block 106500: modified yespowerurx (cpu-friendly, gpu-unfriendly)
- **Difficulty**: Uses Dark Gravity Wave and updates every block.

License
-------
UraniumX is released under the terms of the MIT license. See 
[COPYING](COPYING) for more information or see https://opensource.org/licenses/MIT.

Websites
-------
block explorer: scan.urx.zone

main site: urx.zone

Community/Help
-------
https://discord.gg/xFxtY78

