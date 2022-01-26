## FNV hash history

The basis of the **FNV** hash algorithm was taken from an idea sent as reviewer comments to the IEEE POSIX P1003.2 committee by [Glenn Fowler](http://www.research.att.com/~gsf/) and [Phong Vo](http://www.research.att.com/info/kpv/) back in 1991. In a subsequent ballot round: [Landon Curt Noll](http://www.isthe.com/chongo/index.html) improved on their algorithm. Some people tried this hash and found that it worked rather well. In an EMail message to Landon, they named it the `**Fowler/Noll/Vo**'' or **FNV** hash.`

FNV hash算法 的基础取自 1991年 由 [Glenn Fowler](http://www.research.att.com/~gsf/) and [Phong Vo](http://www.research.att.com/info/kpv/)  作为审稿人意见 ， 提交给  IEEE POSIX P1003.2 委员会 的一个想法。在随后的一轮投票中， [Landon Curt Noll](http://www.isthe.com/chongo/index.html)  改进了他们的算法。有些人尝试了这个哈希，发现它工作得很好。 他们将其命名为 `**Fowler/Noll/Vo**'' or **FNV** hash`


**FNV** hashes are designed to be fast while maintaining a low collision rate. The **FNV** speed allows one to quickly hash lots of data while maintaining a reasonable collision rate. The high dispersion of the **FNV** hashes makes them well suited for hashing nearly identical strings such as URLs, hostnames, filenames, text, IP addresses, etc.

The IETF has an informational draft on [The FNV Non-Cryptographic Hash Algorithm](http://tools.ietf.org/html/draft-eastlake-fnv-03)


FNV哈希值 设计为 快速、同时保持低的冲突，FNV 哈希值的高分散性 使它非常适合对 几乎相同的字符串如  URL、主机名、文件名、文本、IP地址等 进行hash处理

IETF有一个关于[FNV非加密哈希算法](http://tools.ietf.org/html/draft-eastlake-fnv-03)的信息草案。

The **FNV** hash is in wide spread use:

-   [calc](http://www.isthe.com/chongo/tech/comp/calc/index.html)
-   Domain Name Servers(DNS)
-   mdbm key/value data lookup functions
-   Database indexing hashes
-   major web search / indexing engines
-   high performance EMail servers
-   Netnews history file Message-ID lookup functions
-   Anti-spam filters
-   NFS implementations (e.g., [FreeBSD 4.3](http://www.freebsd.org/releases/4.3R/notes.html), IRIX, Linux (NFS v4))
-   [Cohesia MASS project](http://www.cohesia.com/) server collision avoidance
-   spellchecker programmed in Ada 95
-   [flatassembler](http://flatassembler.net/)'s open source x86 assembler - [user-defined symbol hashtree](http://board.flatassembler.net/viewtopic.php?t=854)
-   [PowerBASIC](http://www.isthe.com/chongo/tech/comp/fnv/#PowerBASIC) inline assembly routine
-   text based referenced resources for video games on the PS2, Gamecube and XBOX
-   non-cryptographic file fingerprints
-   [FRET](http://fret.sourceforge.net/) - a tool to identify file data structures / helps to understand file formats
-   used to in the process of computing Unique IDs in DASM (DTN Applications for Symbian Mobile-phones)
-   Used by Microsoft in their hash_map implementation for VC++ 2005
-   Used in an implementation of [libketama](http://www.last.fm/user/RJ/journal/2007/04/10/392555/) for use in items such as [memcache](http://pecl.php.net/package/memcache).
-   Used in the realpath cache in [PHP 5.x](http://www.php.net/) (php-5.2.3/TSRM/tsrm_virtual_cwd.c).
-   Used to [improve the fragment cache](http://www.slideshare.net/Eweaver/improving-running-components-at-twitter) at [twitter](http://twitter.com/) (see slide 31).
-   Used in the [BSD IDE project](http://sourceforge.net/projects/fasmlab/)
-   Used in the [deliantra game server](http://www.deliantra.net/) for it's shared string implementation
-   Used to improve [Leprechaun](http://www.sanmayce.com/Downloads/), an extremely fast word list creator
-   Favored as a hash for IPv6 Flow Labels in a [University of Auckland Computer Science Technical Report (2012-002)](https://researchspace.auckland.ac.nz/bitstream/handle/2292/13240/flowhashRep.pdf) of March 2012
-   Used in the speed-sensitive guts of [twistylists](http://twistylists.blogspot.com/), an open-source structured namespace manager

The core of the **FNV-1** hash algorithm is as follows:
```c
hash = offset_basis
for each _octet_of_data_ to be hashed
 hash = hash * FNV_prime
 hash = hash xor _octet_of_data_
return hash
```

**NOTE:** We recommend that you use the [FNV-1a alternative algorithm](http://www.isthe.com/chongo/tech/comp/fnv/#FNV-1a) instead of the **FNV-1** hash where possible.

## Parameters of the FNV-1/FNV-1a hash

The **FNV-1** hash parameters are as follows:
- hash值是 一个n位的 无符合整形。n是 hash的长度
- The multiplication is performed modulo 2**n** where **n** is the bit length of **hash**.
    
-   The xor is performed on the low order octet (8 bits) of **hash**.
    
-   The **_FNV_prime_** is dependent on **n**, the size of the hash:
    
    > 32 bit **_FNV_prime_** = 224 + 28 + 0x93 = 16777619  
    >   
    > 64 bit **_FNV_prime_** = 240 + 28 + 0xb3 = 1099511628211  
    >   
    > 128 bit **_FNV_prime_** = 288 + 28 + 0x3b = 309485009821345068724781371  
    >   
    > 256 bit **_FNV_prime_** = 2168 + 28 + 0x63 = 374144419156711147060143317175368453031918731002211  
    >   
    > 512 bit **_FNV_prime_** = 2344 + 28 + 0x57 =  
    > 35835915874844867368919076489095108449946327955754392558399825615420669938882575  
    > 126094039892345713852759  
    >   
    > 1024 bit **_FNV_prime_** = 2680 + 28 + 0x8d =  
    > 50164565101131186554345988110352789550307653454047907443030175238311120551081474  
    > 51509157692220295382716162651878526895249385292291816524375083746691371804094271  
    > 873160484737966720260389217684476157468082573
    
    Part of the magic of **FNV** is the selection of the **_FNV_prime_** for a given sized unsigned integer. Some primes do hash better than other primes for a given integer size.