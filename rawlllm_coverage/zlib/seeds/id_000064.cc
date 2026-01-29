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
//<ID> 64
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // This function demonstrates common zlib APIs:
    // - deflateInit / deflate / deflateEnd (streaming compression)
    // - inflateInit / inflate / inflateEnd (streaming decompression)
    // - compress2 / uncompress (convenience single-call APIs)
    // - crc32 (checksum verification)
    //
    // On success the function returns 66. Any zlib error or mismatch returns a non-66 value.

    // Include headers needed for zlib types and C++ containers/strings
    #include <zlib.h>
    #include <string>
    #include <vector>
    #include <cstring>

    using std::string;
    using std::vector;

    // Sample input to compress
    const string input = "The quick brown fox jumps over the lazy dog. "
                         "This is a test of zlib APIs. "
                         "Repeat this phrase to increase size. "
                         "The quick brown fox jumps over the lazy dog. "
                         "The quick brown fox jumps over the lazy dog.";

    // 1) Compute CRC32 of original data
    uLong crc_orig = crc32(0L, Z_NULL, 0);
    crc_orig = crc32(crc_orig,
                     reinterpret_cast<const Bytef*>(input.data()),
                     static_cast<uInt>(input.size()));

    // 2) Streaming compression using deflateInit / deflate / deflateEnd
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    if (deflateInit(&defstream, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return -1;
    }

    // Allocate output buffer using compressBound as a safe upper bound
    uLong compBound = compressBound(static_cast<uLong>(input.size()));
    vector<Bytef> compBuf(static_cast<size_t>(compBound));
    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    defstream.avail_in = static_cast<uInt>(input.size());
    defstream.next_out = compBuf.data();
    defstream.avail_out = static_cast<uInt>(compBuf.size());

    int defret = deflate(&defstream, Z_FINISH);
    if (defret != Z_STREAM_END) {
        deflateEnd(&defstream);
        return -2;
    }
    uLong compressed_size = defstream.total_out;
    if (deflateEnd(&defstream) != Z_OK) {
        return -3;
    }

    // 3) Streaming decompression using inflateInit / inflate / inflateEnd
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    if (inflateInit(&infstream) != Z_OK) {
        return -4;
    }

    vector<Bytef> decompBuf(input.size()); // we expect the original size
    infstream.next_in = compBuf.data();
    infstream.avail_in = static_cast<uInt>(compressed_size);
    infstream.next_out = decompBuf.data();
    infstream.avail_out = static_cast<uInt>(decompBuf.size());

    int infret = inflate(&infstream, Z_FINISH);
    if (infret != Z_STREAM_END) {
        inflateEnd(&infstream);
        return -5;
    }
    uLong decompressed_size = infstream.total_out;
    if (inflateEnd(&infstream) != Z_OK) {
        return -6;
    }

    // Validate sizes and content
    if (decompressed_size != input.size()) {
        return -7;
    }
    if (std::memcmp(decompBuf.data(), input.data(), input.size()) != 0) {
        return -8;
    }

    // 4) Use compress2 / uncompress convenience APIs to compress and decompress again
    uLong comp2_len = compressBound(static_cast<uLong>(input.size()));
    vector<Bytef> comp2Buf(static_cast<size_t>(comp2_len));
    int c2 = compress2(comp2Buf.data(), &comp2_len,
                       reinterpret_cast<const Bytef*>(input.data()),
                       static_cast<uLong>(input.size()),
                       Z_BEST_COMPRESSION);
    if (c2 != Z_OK) {
        return -9;
    }

    vector<Bytef> decomp2Buf(input.size());
    uLong decomp2_len = static_cast<uLong>(decomp2Buf.size());
    int ures = uncompress(decomp2Buf.data(), &decomp2_len,
                          comp2Buf.data(), comp2_len);
    if (ures != Z_OK) {
        return -10;
    }
    if (decomp2_len != input.size()) {
        return -11;
    }
    if (std::memcmp(decomp2Buf.data(), input.data(), input.size()) != 0) {
        return -12;
    }

    // 5) Verify CRC32 of decompressed data matches original CRC
    uLong crc_decompressed = crc32(0L, Z_NULL, 0);
    crc_decompressed = crc32(crc_decompressed,
                             reinterpret_cast<const Bytef*>(decompBuf.data()),
                             static_cast<uInt>(decompressed_size));
    if (crc_decompressed != crc_orig) {
        return -13;
    }

    // All zlib operations succeeded and data matched
    return 66;
}