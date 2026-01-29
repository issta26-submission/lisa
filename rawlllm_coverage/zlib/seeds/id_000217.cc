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
//<ID> 217
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Required headers for zlib usage and standard containers.
    // It's permissible to place #include directives here; preprocessors run before compilation.
    #include <zlib.h>
    #include <string>
    #include <vector>
    #include <cstring>

    // Prepare sample data (small enough to safely cast sizes to zlib's uInt where needed).
    std::string src = "The quick brown fox jumped over the lazy dog. ";
    // Make it a bit larger / repetitive to show compression effect.
    for (int i = 0; i < 8; ++i) src += src;

    uLong srcLen = static_cast<uLong>(src.size());

    // 1) Query zlib version and compile flags (non-essential for logic, but demonstrates the API).
    const char* ver = zlibVersion();
    (void)ver; // silence unused-variable warnings in contexts where printing isn't desired
    unsigned long flags = zlibCompileFlags();
    (void)flags;

    // 2) Use compressBound + compress2 to compress the buffer.
    uLongf compBound = compressBound(srcLen);
    std::vector<Bytef> compBuf(compBound);
    uLongf compLen = compBound;
    int ret = compress2(compBuf.data(), &compLen,
                        reinterpret_cast<const Bytef*>(src.data()), srcLen,
                        Z_BEST_COMPRESSION);
    if (ret != Z_OK) return -1; // compression failed
    compBuf.resize(compLen); // trim to actual compressed size

    // 3) Compute checksums (crc32 and adler32) on original data.
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, reinterpret_cast<const Bytef*>(src.data()), srcLen);
    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, reinterpret_cast<const Bytef*>(src.data()), srcLen);
    (void)crc; (void)adl;

    // 4) Uncompress using uncompress API and verify equality.
    std::vector<Bytef> uncompBuf(srcLen);
    uLongf uncomprLen = srcLen;
    ret = uncompress(uncompBuf.data(), &uncomprLen, compBuf.data(), compLen);
    if (ret != Z_OK || uncomprLen != srcLen) return -2; // decompression failed
    if (std::memcmp(uncompBuf.data(), src.data(), srcLen) != 0) return -3; // data mismatch

    // 5) Demonstrate streaming API: deflate (stream) then inflate (stream).
    // DEFLate
    z_stream defstream;
    std::memset(&defstream, 0, sizeof(defstream));
    ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return -4;

    // Prepare output buffer large enough.
    uLongf defOutBound = compressBound(srcLen);
    std::vector<Bytef> def_out(defOutBound);
    defstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(src.data()));
    // Note: avail_in is uInt; we ensure src is reasonably small so casting is safe.
    defstream.avail_in = static_cast<uInt>(srcLen);
    defstream.next_out = def_out.data();
    defstream.avail_out = static_cast<uInt>(def_out.size());

    // deflate in a loop until stream ends
    int def_ret;
    do {
        def_ret = deflate(&defstream, Z_FINISH);
        if (def_ret == Z_OK && defstream.avail_out == 0) {
            // Needs more output space (not likely for small input but handle generically).
            size_t oldSize = def_out.size();
            def_out.resize(oldSize * 2);
            defstream.next_out = def_out.data() + oldSize;
            defstream.avail_out = static_cast<uInt>(oldSize);
        } else if (def_ret != Z_OK && def_ret != Z_STREAM_END) {
            deflateEnd(&defstream);
            return -5; // deflate failed
        }
    } while (def_ret != Z_STREAM_END);

    uLongf deflatedSize = static_cast<uLongf>(def_out.size() - defstream.avail_out);
    deflateEnd(&defstream);
    def_out.resize(deflatedSize);

    // INFLate
    z_stream infstream;
    std::memset(&infstream, 0, sizeof(infstream));
    ret = inflateInit(&infstream);
    if (ret != Z_OK) return -6;

    std::vector<Bytef> inf_out(srcLen); // we expect original size
    infstream.next_in = def_out.data();
    infstream.avail_in = static_cast<uInt>(deflatedSize);
    infstream.next_out = inf_out.data();
    infstream.avail_out = static_cast<uInt>(inf_out.size());

    int inf_ret;
    do {
        inf_ret = inflate(&infstream, Z_FINISH);
        if (inf_ret == Z_OK && infstream.avail_out == 0) {
            // Need more output space (should not happen here)
            size_t old = inf_out.size();
            inf_out.resize(old * 2);
            infstream.next_out = inf_out.data() + old;
            infstream.avail_out = static_cast<uInt>(old);
        } else if (inf_ret != Z_OK && inf_ret != Z_STREAM_END) {
            inflateEnd(&infstream);
            return -7; // inflate failed
        }
    } while (inf_ret != Z_STREAM_END);

    uLongf inflatedSize = static_cast<uLongf>(inf_out.size() - infstream.avail_out);
    inflateEnd(&infstream);
    if (inflatedSize != srcLen) return -8;
    if (std::memcmp(inf_out.data(), src.data(), srcLen) != 0) return -9; // mismatch

    // If all checks passed, return the requested success value.
    return 66;
}