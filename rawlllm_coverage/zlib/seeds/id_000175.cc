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
//<ID> 175
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Prepare sample input data
    std::string input;
    for (int i = 0; i < 100; ++i) {
        input += "The quick brown fox jumps over the lazy dog. ";
    }
    uLong orig_len = static_cast<uLong>(input.size());

    // 1) Compute CRC32 and Adler-32 checksums
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(input.data()), static_cast<uInt>(input.size()));

    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, reinterpret_cast<const Bytef*>(input.data()), static_cast<uInt>(input.size()));

    // 2) compress() / uncompress() simple usage
    uLong bound = compressBound(orig_len);
    std::vector<Bytef> compBuf(bound);
    uLong compLen = bound;
    int ret = compress(compBuf.data(), &compLen,
                       reinterpret_cast<const Bytef*>(input.data()), orig_len);
    if (ret != Z_OK) return 1;

    std::vector<Bytef> decompBuf(orig_len);
    uLong decompLen = orig_len;
    ret = uncompress(decompBuf.data(), &decompLen, compBuf.data(), compLen);
    if (ret != Z_OK) return 2;
    if (decompLen != orig_len) return 3;
    if (std::memcmp(decompBuf.data(), input.data(), orig_len) != 0) return 4;

    // 3) compress2() with a different compression level
    std::vector<Bytef> comp2Buf(bound);
    uLong comp2Len = bound;
    ret = compress2(comp2Buf.data(), &comp2Len,
                    reinterpret_cast<const Bytef*>(input.data()), orig_len,
                    Z_BEST_COMPRESSION);
    if (ret != Z_OK) return 5;
    // sanity check: uncompress comp2Buf
    std::vector<Bytef> decomp2Buf(orig_len);
    uLong decomp2Len = orig_len;
    ret = uncompress(decomp2Buf.data(), &decomp2Len, comp2Buf.data(), comp2Len);
    if (ret != Z_OK) return 6;
    if (decomp2Len != orig_len) return 7;
    if (std::memcmp(decomp2Buf.data(), input.data(), orig_len) != 0) return 8;

    // 4) deflate (streaming) and then inflate (streaming)
    // Prepare a streaming deflate
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return 9;

    // allocate output buffer for streaming deflate
    uLong streamBound = compressBound(orig_len);
    std::vector<Bytef> streamCompBuf(streamBound);
    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    defstream.avail_in = static_cast<uInt>(input.size());
    defstream.next_out = streamCompBuf.data();
    defstream.avail_out = static_cast<uInt>(streamCompBuf.size());

    // deflate with Z_FINISH, loop until stream end
    while (true) {
        ret = deflate(&defstream, Z_FINISH);
        if (ret == Z_STREAM_END) break;
        if (ret != Z_OK) {
            deflateEnd(&defstream);
            return 10;
        }
        // if more output needed, enlarge buffer (unlikely given compressBound)
        if (defstream.avail_out == 0) {
            size_t oldSize = streamCompBuf.size();
            streamCompBuf.resize(oldSize * 2);
            defstream.next_out = streamCompBuf.data() + oldSize;
            defstream.avail_out = static_cast<uInt>(oldSize);
        }
    }
    uLong streamCompLen = static_cast<uLong>(streamCompBuf.size()) - defstream.avail_out;
    ret = deflateEnd(&defstream);
    if (ret != Z_OK) return 11;

    // Now inflate the streaming-compressed data
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    ret = inflateInit(&infstream);
    if (ret != Z_OK) return 12;

    std::vector<Bytef> streamDecompBuf(orig_len);
    infstream.next_in = streamCompBuf.data();
    infstream.avail_in = static_cast<uInt>(streamCompLen);
    infstream.next_out = streamDecompBuf.data();
    infstream.avail_out = static_cast<uInt>(streamDecompBuf.size());

    while (true) {
        ret = inflate(&infstream, Z_FINISH);
        if (ret == Z_STREAM_END) break;
        if (ret == Z_OK) {
            // if more output needed, enlarge buffer
            if (infstream.avail_out == 0) {
                size_t oldSize = streamDecompBuf.size();
                streamDecompBuf.resize(oldSize * 2);
                infstream.next_out = streamDecompBuf.data() + oldSize;
                infstream.avail_out = static_cast<uInt>(oldSize);
                continue;
            }
            // otherwise continue calling inflate
            continue;
        }
        // any other return value is an error
        inflateEnd(&infstream);
        return 13;
    }
    uLong streamDecompLen = static_cast<uLong>(streamDecompBuf.size()) - infstream.avail_out;
    ret = inflateEnd(&infstream);
    if (ret != Z_OK) return 14;

    if (streamDecompLen != orig_len) return 15;
    if (std::memcmp(streamDecompBuf.data(), input.data(), orig_len) != 0) return 16;

    // 5) Use deflateReset and re-deflate same data into a new buffer to exercise reset API
    std::memset(&defstream, 0, sizeof(defstream));
    ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return 17;
    // first compression
    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    defstream.avail_in = static_cast<uInt>(input.size());
    std::vector<Bytef> resetCompBuf(compressBound(orig_len));
    defstream.next_out = resetCompBuf.data();
    defstream.avail_out = static_cast<uInt>(resetCompBuf.size());
    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&defstream);
        return 18;
    }
    ret = deflateEnd(&defstream);
    if (ret != Z_OK) return 19;

    // re-init and compress again
    std::memset(&defstream, 0, sizeof(defstream));
    ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return 20;
    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    defstream.avail_in = static_cast<uInt>(input.size());
    defstream.next_out = resetCompBuf.data();
    defstream.avail_out = static_cast<uInt>(resetCompBuf.size());
    ret = deflate(&defstream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&defstream);
        return 21;
    }
    ret = deflateEnd(&defstream);
    if (ret != Z_OK) return 22;

    // Final verification: recompute checksums on decompressed data to match initial values
    uLong final_crc = crc32(0L, Z_NULL, 0);
    final_crc = crc32(final_crc, reinterpret_cast<const Bytef*>(streamDecompBuf.data()), static_cast<uInt>(streamDecompLen));
    if (final_crc != crc) return 23;

    uLong final_adl = adler32(0L, Z_NULL, 0);
    final_adl = adler32(final_adl, reinterpret_cast<const Bytef*>(streamDecompBuf.data()), static_cast<uInt>(streamDecompLen));
    if (final_adl != adl) return 24;

    // All zlib API sequences succeeded
    return 66;
}