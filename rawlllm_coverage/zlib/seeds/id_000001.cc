#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 1
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // local includes so the function is self-contained when pasted into a file
    #include <zlib.h>
    #include <cstring>
    #include <string>
    #include <vector>

    // Original data to compress
    const std::string original = 
        "The quick brown fox jumps over the lazy dog. "
        "This string is used to test zlib compression and decompression APIs.";

    // 1) Stream compression using deflateInit / deflate / deflateEnd
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    int ret = deflateInit(&defstream, Z_BEST_COMPRESSION);
    if (ret != Z_OK) return -1;

    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original.data()));
    defstream.avail_in = static_cast<uInt>(original.size());

    uLong bound = compressBound(static_cast<uLong>(original.size()));
    std::vector<unsigned char> compressed(bound);

    defstream.next_out = compressed.data();
    defstream.avail_out = static_cast<uInt>(compressed.size());

    // Use Z_FINISH to finish the stream in one call (buffer is sized by compressBound)
    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END && ret != Z_OK) {
        deflateEnd(&defstream);
        return -2;
    }
    size_t compressed_size = defstream.total_out;
    deflateEnd(&defstream);

    // Resize to actual compressed size
    compressed.resize(compressed_size);

    // 2) Stream decompression using inflateInit / inflate / inflateEnd
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    ret = inflateInit(&infstream);
    if (ret != Z_OK) return -3;

    infstream.next_in = compressed.data();
    infstream.avail_in = static_cast<uInt>(compressed.size());

    std::vector<unsigned char> decompressed(original.size()); // we know expected size
    infstream.next_out = decompressed.data();
    infstream.avail_out = static_cast<uInt>(decompressed.size());

    ret = inflate(&infstream, Z_FINISH);
    if (ret != Z_STREAM_END && ret != Z_OK) {
        inflateEnd(&infstream);
        return -4;
    }
    size_t decompressed_size = infstream.total_out;
    inflateEnd(&infstream);

    // Verify decompressed content matches original
    if (decompressed_size != original.size()) return -5;
    if (std::memcmp(decompressed.data(), original.data(), original.size()) != 0) return -6;

    // 3) Convenience API: compress2 and uncompress
    uLong destBound = compressBound(static_cast<uLong>(original.size()));
    std::vector<unsigned char> comp2(destBound);
    uLong comp2_size = destBound;
    ret = compress2(comp2.data(), &comp2_size,
                    reinterpret_cast<const Bytef*>(original.data()),
                    static_cast<uLong>(original.size()),
                    Z_BEST_SPEED);
    if (ret != Z_OK) return -7;
    comp2.resize(comp2_size);

    std::vector<unsigned char> decomp2(original.size());
    uLong decomp2_size = static_cast<uLong>(decomp2.size());
    ret = uncompress(decomp2.data(), &decomp2_size, comp2.data(), comp2_size);
    if (ret != Z_OK) return -8;
    if (decomp2_size != original.size()) return -9;
    if (std::memcmp(decomp2.data(), original.data(), original.size()) != 0) return -10;

    // 4) Checksum APIs: adler32 and crc32 to validate content integrity
    uLong adler_orig = adler32(0L, Z_NULL, 0);
    adler_orig = adler32(adler_orig, reinterpret_cast<const Bytef*>(original.data()),
                         static_cast<uInt>(original.size()));
    uLong adler_decomp = adler32(0L, Z_NULL, 0);
    adler_decomp = adler32(adler_decomp, decompressed.data(),
                           static_cast<uInt>(decompressed_size));
    if (adler_orig != adler_decomp) return -11;

    uLong crc_orig = crc32(0L, Z_NULL, 0);
    crc_orig = crc32(crc_orig, reinterpret_cast<const Bytef*>(original.data()),
                     static_cast<uInt>(original.size()));
    uLong crc_decomp = crc32(0L, Z_NULL, 0);
    crc_decomp = crc32(crc_decomp, decompressed.data(),
                       static_cast<uInt>(decompressed_size));
    if (crc_orig != crc_decomp) return -12;

    // All checks passed; return the required success value
    return 66;
}