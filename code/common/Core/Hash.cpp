#include "Hash.h"


namespace FHash
{
    static const uint64_t crc64_tab[256] = {
        UINT64_C(0x0000000000000000), UINT64_C(0x42f0e1eba9ea3693),
        UINT64_C(0x85e1c3d753d46d26), UINT64_C(0xc711223cfa3e5bb5),
        UINT64_C(0x493366450e42ecdf), UINT64_C(0x0bc387aea7a8da4c),
        UINT64_C(0xccd2a5925d9681f9), UINT64_C(0x8e224479f47cb76a),
        UINT64_C(0x9266cc8a1c85d9be), UINT64_C(0xd0962d61b56fef2d),
        UINT64_C(0x17870f5d4f51b498), UINT64_C(0x5577eeb6e6bb820b),
        UINT64_C(0xdb55aacf12c73561), UINT64_C(0x99a54b24bb2d03f2),
        UINT64_C(0x5eb4691841135847), UINT64_C(0x1c4488f3e8f96ed4),
        UINT64_C(0x663d78ff90e185ef), UINT64_C(0x24cd9914390bb37c),
        UINT64_C(0xe3dcbb28c335e8c9), UINT64_C(0xa12c5ac36adfde5a),
        UINT64_C(0x2f0e1eba9ea36930), UINT64_C(0x6dfeff5137495fa3),
        UINT64_C(0xaaefdd6dcd770416), UINT64_C(0xe81f3c86649d3285),
        UINT64_C(0xf45bb4758c645c51), UINT64_C(0xb6ab559e258e6ac2),
        UINT64_C(0x71ba77a2dfb03177), UINT64_C(0x334a9649765a07e4),
        UINT64_C(0xbd68d2308226b08e), UINT64_C(0xff9833db2bcc861d),
        UINT64_C(0x388911e7d1f2dda8), UINT64_C(0x7a79f00c7818eb3b),
        UINT64_C(0xcc7af1ff21c30bde), UINT64_C(0x8e8a101488293d4d),
        UINT64_C(0x499b3228721766f8), UINT64_C(0x0b6bd3c3dbfd506b),
        UINT64_C(0x854997ba2f81e701), UINT64_C(0xc7b97651866bd192),
        UINT64_C(0x00a8546d7c558a27), UINT64_C(0x4258b586d5bfbcb4),
        UINT64_C(0x5e1c3d753d46d260), UINT64_C(0x1cecdc9e94ace4f3),
        UINT64_C(0xdbfdfea26e92bf46), UINT64_C(0x990d1f49c77889d5),
        UINT64_C(0x172f5b3033043ebf), UINT64_C(0x55dfbadb9aee082c),
        UINT64_C(0x92ce98e760d05399), UINT64_C(0xd03e790cc93a650a),
        UINT64_C(0xaa478900b1228e31), UINT64_C(0xe8b768eb18c8b8a2),
        UINT64_C(0x2fa64ad7e2f6e317), UINT64_C(0x6d56ab3c4b1cd584),
        UINT64_C(0xe374ef45bf6062ee), UINT64_C(0xa1840eae168a547d),
        UINT64_C(0x66952c92ecb40fc8), UINT64_C(0x2465cd79455e395b),
        UINT64_C(0x3821458aada7578f), UINT64_C(0x7ad1a461044d611c),
        UINT64_C(0xbdc0865dfe733aa9), UINT64_C(0xff3067b657990c3a),
        UINT64_C(0x711223cfa3e5bb50), UINT64_C(0x33e2c2240a0f8dc3),
        UINT64_C(0xf4f3e018f031d676), UINT64_C(0xb60301f359dbe0e5),
        UINT64_C(0xda050215ea6c212f), UINT64_C(0x98f5e3fe438617bc),
        UINT64_C(0x5fe4c1c2b9b84c09), UINT64_C(0x1d14202910527a9a),
        UINT64_C(0x93366450e42ecdf0), UINT64_C(0xd1c685bb4dc4fb63),
        UINT64_C(0x16d7a787b7faa0d6), UINT64_C(0x5427466c1e109645),
        UINT64_C(0x4863ce9ff6e9f891), UINT64_C(0x0a932f745f03ce02),
        UINT64_C(0xcd820d48a53d95b7), UINT64_C(0x8f72eca30cd7a324),
        UINT64_C(0x0150a8daf8ab144e), UINT64_C(0x43a04931514122dd),
        UINT64_C(0x84b16b0dab7f7968), UINT64_C(0xc6418ae602954ffb),
        UINT64_C(0xbc387aea7a8da4c0), UINT64_C(0xfec89b01d3679253),
        UINT64_C(0x39d9b93d2959c9e6), UINT64_C(0x7b2958d680b3ff75),
        UINT64_C(0xf50b1caf74cf481f), UINT64_C(0xb7fbfd44dd257e8c),
        UINT64_C(0x70eadf78271b2539), UINT64_C(0x321a3e938ef113aa),
        UINT64_C(0x2e5eb66066087d7e), UINT64_C(0x6cae578bcfe24bed),
        UINT64_C(0xabbf75b735dc1058), UINT64_C(0xe94f945c9c3626cb),
        UINT64_C(0x676dd025684a91a1), UINT64_C(0x259d31cec1a0a732),
        UINT64_C(0xe28c13f23b9efc87), UINT64_C(0xa07cf2199274ca14),
        UINT64_C(0x167ff3eacbaf2af1), UINT64_C(0x548f120162451c62),
        UINT64_C(0x939e303d987b47d7), UINT64_C(0xd16ed1d631917144),
        UINT64_C(0x5f4c95afc5edc62e), UINT64_C(0x1dbc74446c07f0bd),
        UINT64_C(0xdaad56789639ab08), UINT64_C(0x985db7933fd39d9b),
        UINT64_C(0x84193f60d72af34f), UINT64_C(0xc6e9de8b7ec0c5dc),
        UINT64_C(0x01f8fcb784fe9e69), UINT64_C(0x43081d5c2d14a8fa),
        UINT64_C(0xcd2a5925d9681f90), UINT64_C(0x8fdab8ce70822903),
        UINT64_C(0x48cb9af28abc72b6), UINT64_C(0x0a3b7b1923564425),
        UINT64_C(0x70428b155b4eaf1e), UINT64_C(0x32b26afef2a4998d),
        UINT64_C(0xf5a348c2089ac238), UINT64_C(0xb753a929a170f4ab),
        UINT64_C(0x3971ed50550c43c1), UINT64_C(0x7b810cbbfce67552),
        UINT64_C(0xbc902e8706d82ee7), UINT64_C(0xfe60cf6caf321874),
        UINT64_C(0xe224479f47cb76a0), UINT64_C(0xa0d4a674ee214033),
        UINT64_C(0x67c58448141f1b86), UINT64_C(0x253565a3bdf52d15),
        UINT64_C(0xab1721da49899a7f), UINT64_C(0xe9e7c031e063acec),
        UINT64_C(0x2ef6e20d1a5df759), UINT64_C(0x6c0603e6b3b7c1ca),
        UINT64_C(0xf6fae5c07d3274cd), UINT64_C(0xb40a042bd4d8425e),
        UINT64_C(0x731b26172ee619eb), UINT64_C(0x31ebc7fc870c2f78),
        UINT64_C(0xbfc9838573709812), UINT64_C(0xfd39626eda9aae81),
        UINT64_C(0x3a28405220a4f534), UINT64_C(0x78d8a1b9894ec3a7),
        UINT64_C(0x649c294a61b7ad73), UINT64_C(0x266cc8a1c85d9be0),
        UINT64_C(0xe17dea9d3263c055), UINT64_C(0xa38d0b769b89f6c6),
        UINT64_C(0x2daf4f0f6ff541ac), UINT64_C(0x6f5faee4c61f773f),
        UINT64_C(0xa84e8cd83c212c8a), UINT64_C(0xeabe6d3395cb1a19),
        UINT64_C(0x90c79d3fedd3f122), UINT64_C(0xd2377cd44439c7b1),
        UINT64_C(0x15265ee8be079c04), UINT64_C(0x57d6bf0317edaa97),
        UINT64_C(0xd9f4fb7ae3911dfd), UINT64_C(0x9b041a914a7b2b6e),
        UINT64_C(0x5c1538adb04570db), UINT64_C(0x1ee5d94619af4648),
        UINT64_C(0x02a151b5f156289c), UINT64_C(0x4051b05e58bc1e0f),
        UINT64_C(0x87409262a28245ba), UINT64_C(0xc5b073890b687329),
        UINT64_C(0x4b9237f0ff14c443), UINT64_C(0x0962d61b56fef2d0),
        UINT64_C(0xce73f427acc0a965), UINT64_C(0x8c8315cc052a9ff6),
        UINT64_C(0x3a80143f5cf17f13), UINT64_C(0x7870f5d4f51b4980),
        UINT64_C(0xbf61d7e80f251235), UINT64_C(0xfd913603a6cf24a6),
        UINT64_C(0x73b3727a52b393cc), UINT64_C(0x31439391fb59a55f),
        UINT64_C(0xf652b1ad0167feea), UINT64_C(0xb4a25046a88dc879),
        UINT64_C(0xa8e6d8b54074a6ad), UINT64_C(0xea16395ee99e903e),
        UINT64_C(0x2d071b6213a0cb8b), UINT64_C(0x6ff7fa89ba4afd18),
        UINT64_C(0xe1d5bef04e364a72), UINT64_C(0xa3255f1be7dc7ce1),
        UINT64_C(0x64347d271de22754), UINT64_C(0x26c49cccb40811c7),
        UINT64_C(0x5cbd6cc0cc10fafc), UINT64_C(0x1e4d8d2b65facc6f),
        UINT64_C(0xd95caf179fc497da), UINT64_C(0x9bac4efc362ea149),
        UINT64_C(0x158e0a85c2521623), UINT64_C(0x577eeb6e6bb820b0),
        UINT64_C(0x906fc95291867b05), UINT64_C(0xd29f28b9386c4d96),
        UINT64_C(0xcedba04ad0952342), UINT64_C(0x8c2b41a1797f15d1),
        UINT64_C(0x4b3a639d83414e64), UINT64_C(0x09ca82762aab78f7),
        UINT64_C(0x87e8c60fded7cf9d), UINT64_C(0xc51827e4773df90e),
        UINT64_C(0x020905d88d03a2bb), UINT64_C(0x40f9e43324e99428),
        UINT64_C(0x2cffe7d5975e55e2), UINT64_C(0x6e0f063e3eb46371),
        UINT64_C(0xa91e2402c48a38c4), UINT64_C(0xebeec5e96d600e57),
        UINT64_C(0x65cc8190991cb93d), UINT64_C(0x273c607b30f68fae),
        UINT64_C(0xe02d4247cac8d41b), UINT64_C(0xa2dda3ac6322e288),
        UINT64_C(0xbe992b5f8bdb8c5c), UINT64_C(0xfc69cab42231bacf),
        UINT64_C(0x3b78e888d80fe17a), UINT64_C(0x7988096371e5d7e9),
        UINT64_C(0xf7aa4d1a85996083), UINT64_C(0xb55aacf12c735610),
        UINT64_C(0x724b8ecdd64d0da5), UINT64_C(0x30bb6f267fa73b36),
        UINT64_C(0x4ac29f2a07bfd00d), UINT64_C(0x08327ec1ae55e69e),
        UINT64_C(0xcf235cfd546bbd2b), UINT64_C(0x8dd3bd16fd818bb8),
        UINT64_C(0x03f1f96f09fd3cd2), UINT64_C(0x41011884a0170a41),
        UINT64_C(0x86103ab85a2951f4), UINT64_C(0xc4e0db53f3c36767),
        UINT64_C(0xd8a453a01b3a09b3), UINT64_C(0x9a54b24bb2d03f20),
        UINT64_C(0x5d45907748ee6495), UINT64_C(0x1fb5719ce1045206),
        UINT64_C(0x919735e51578e56c), UINT64_C(0xd367d40ebc92d3ff),
        UINT64_C(0x1476f63246ac884a), UINT64_C(0x568617d9ef46bed9),
        UINT64_C(0xe085162ab69d5e3c), UINT64_C(0xa275f7c11f7768af),
        UINT64_C(0x6564d5fde549331a), UINT64_C(0x279434164ca30589),
        UINT64_C(0xa9b6706fb8dfb2e3), UINT64_C(0xeb46918411358470),
        UINT64_C(0x2c57b3b8eb0bdfc5), UINT64_C(0x6ea7525342e1e956),
        UINT64_C(0x72e3daa0aa188782), UINT64_C(0x30133b4b03f2b111),
        UINT64_C(0xf7021977f9cceaa4), UINT64_C(0xb5f2f89c5026dc37),
        UINT64_C(0x3bd0bce5a45a6b5d), UINT64_C(0x79205d0e0db05dce),
        UINT64_C(0xbe317f32f78e067b), UINT64_C(0xfcc19ed95e6430e8),
        UINT64_C(0x86b86ed5267cdbd3), UINT64_C(0xc4488f3e8f96ed40),
        UINT64_C(0x0359ad0275a8b6f5), UINT64_C(0x41a94ce9dc428066),
        UINT64_C(0xcf8b0890283e370c), UINT64_C(0x8d7be97b81d4019f),
        UINT64_C(0x4a6acb477bea5a2a), UINT64_C(0x089a2aacd2006cb9),
        UINT64_C(0x14dea25f3af9026d), UINT64_C(0x562e43b4931334fe),
        UINT64_C(0x913f6188692d6f4b), UINT64_C(0xd3cf8063c0c759d8),
        UINT64_C(0x5dedc41a34bbeeb2), UINT64_C(0x1f1d25f19d51d821),
        UINT64_C(0xd80c07cd676f8394), UINT64_C(0x9afce626ce85b507)
    };

    // ECMA-182
    uint64_t Crc64(const unsigned char* acpData, std::size_t aSize)
    {
        auto crc = ~(0ULL);

        for (size_t j = 0; j < aSize; ++j)
        {
            const uint8_t index = acpData[j] ^ (crc >> 56);
            const auto lookup = crc64_tab[index];
            crc <<= 8;
            crc ^= lookup;
        }
        return ~crc;
    }

    uint64_t FNV1a64(const char* aText, uint64_t aSeed)
    {
        constexpr uint64_t prime = 0x100000001b3;

        uint64_t hash = aSeed;
        while (aText && *aText)
        {
            hash ^= *aText;
            hash *= prime;

            aText++;
        }

        return hash;
    }
    }
