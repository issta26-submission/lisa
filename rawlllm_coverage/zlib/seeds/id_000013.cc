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
//<ID> 13
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Sample input
    const std::string src = "The quick brown fox jumps over the lazy dog. "
                            "This is a test string for zlib API sequence. "
                            "We will compress and decompress it using multiple zlib APIs.";

    // 1) High-level compress() / uncompress()
    uLong srcLen = static_cast<uLong>(src.size());
    uLongf compBound = compressBound(srcLen);
    std::vector<Bytef> compBuf(compBound);
    uLongf compLen = compBound;

    int zerr = compress(compBuf.data(), &compLen,
                        reinterpret_cast<const Bytef*>(src.data()), srcLen);
    if (zerr != Z_OK) return 1; // compress failed

    // Decompress with uncompress()
    std::vector<Bytef> decompBuf(srcLen + 1);
    uLongf decompLen = srcLen;
    zerr = uncompress(decompBuf.data(), &decompLen,
                      compBuf.data(), compLen);
    if (zerr != Z_OK) return 2; // uncompress failed

    // Verify decompressed data matches original
    if (decompLen != srcLen) return 3;
    if (std::memcmp(decompBuf.data(), src.data(), srcLen) != 0) return 4;

    // 2) Streaming deflate (z_stream + deflateInit/deflate/deflateEnd)
    z_stream def;
    std::memset(&def, 0, sizeof(def));
    def.zalloc = Z_NULL;
    def.zfree = Z_NULL;
    def.opaque = Z_NULL;
    if (deflateInit(&def, Z_DEFAULT_COMPRESSION) != Z_OK) return 5;

    def.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(src.data()));
    def.avail_in = srcLen;

    const size_t CHUNK = 128;
    std::vector<Bytef> outBuf(CHUNK);
    std::vector<Bytef> streamComp; // will hold streaming compressed output

    int ret;
    do {
        def.next_out = outBuf.data();
        def.avail_out = static_cast<uInt>(CHUNK);
        // Provide entire input and finish
        ret = deflate(&def, Z_FINISH);
        if (ret != Z_OK && ret != Z_STREAM_END) {
            deflateEnd(&def);
            return 6; // deflate failed
        }
        size_t have = CHUNK - def.avail_out;
        if (have > 0) {
            streamComp.insert(streamComp.end(), outBuf.data(), outBuf.data() + have);
        }
    } while (ret != Z_STREAM_END);

    deflateEnd(&def);

    // 3) Streaming inflate (z_stream + inflateInit/inflate/inflateEnd)
    z_stream inf;
    std::memset(&inf, 0, sizeof(inf));
    inf.zalloc = Z_NULL;
    inf.zfree = Z_NULL;
    inf.opaque = Z_NULL;
    if (inflateInit(&inf) != Z_OK) return 7;

    inf.next_in = streamComp.data();
    inf.avail_in = static_cast<uInt>(streamComp.size());

    std::vector<Bytef> streamDecomp; // will hold streaming decompressed output

    do {
        inf.next_out = outBuf.data();
        inf.avail_out = static_cast<uInt>(CHUNK);
        ret = inflate(&inf, Z_NO_FLUSH);
        if (ret == Z_NEED_DICT || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
            inflateEnd(&inf);
            return 8; // inflate failed
        }
        size_t have = CHUNK - inf.avail_out;
        if (have > 0) {
            streamDecomp.insert(streamDecomp.end(), outBuf.data(), outBuf.data() + have);
        }
    } while (ret != Z_STREAM_END);

    inflateEnd(&inf);

    // Verify streaming decompressed data matches original
    if (streamDecomp.size() != srcLen) return 9;
    if (std::memcmp(streamDecomp.data(), src.data(), srcLen) != 0) return 10;

    // 4) CRC32 sanity check (crc32 API)
    uLong crc_src = crc32(0L, Z_NULL, 0);
    crc_src = crc32(crc_src, reinterpret_cast<const Bytef*>(src.data()), srcLen);
    uLong crc_decomp = crc32(0L, Z_NULL, 0);
    crc_decomp = crc32(crc_decomp, streamDecomp.data(), static_cast<uInt>(streamDecomp.size()));
    if (crc_src != crc_decomp) return 11;

    // If we've reached here, all API sequences succeeded
    return 66;
}