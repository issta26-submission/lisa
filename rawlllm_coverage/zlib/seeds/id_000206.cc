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
//<ID> 206
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Include headers here so the function is self-contained when pasted into a file.
    #include <zlib.h>
    #include <vector>
    #include <string>
    #include <cstring>

    // Sample data to compress/decompress and operate on with zlib APIs
    std::string original;
    for (int i = 0; i < 50; ++i) original += "The quick brown fox jumps over the lazy dog. ";

    // Flags to track initialization for proper cleanup on error paths
    bool deflate_inited = false;
    bool inflate_inited = false;

    // 1) Use compress() / uncompress() convenience APIs
    {
        uLong srcLen = static_cast<uLong>(original.size());
        uLong bound = compressBound(srcLen);
        std::vector<Bytef> comp(bound);
        uLongf compLen = static_cast<uLongf>(bound);

        int ret = compress(comp.data(), &compLen,
                           reinterpret_cast<const Bytef*>(original.data()), srcLen);
        if (ret != Z_OK) return 1;

        std::vector<Bytef> uncomp(srcLen);
        uLongf unLen = static_cast<uLongf>(srcLen);
        ret = uncompress(uncomp.data(), &unLen, comp.data(), compLen);
        if (ret != Z_OK) return 2;
        if (unLen != srcLen) return 3;
        if (std::memcmp(uncomp.data(), original.data(), srcLen) != 0) return 4;
    }

    // 2) Use lower-level deflate (streaming) API
    std::vector<Bytef> deflated;
    uLong total_deflated = 0;
    {
        z_stream strm;
        std::memset(&strm, 0, sizeof(strm));
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;

        int ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
        if (ret != Z_OK) return 5;
        deflate_inited = true;

        uLong srcLen = static_cast<uLong>(original.size());
        strm.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(original.data()));
        strm.avail_in = static_cast<uInt>(srcLen); // zlib windows this is uInt, safe because our data is small

        // Start with compressBound sized buffer, grow if necessary
        uLong outBound = compressBound(srcLen);
        deflated.resize(outBound);
        strm.next_out = deflated.data();
        strm.avail_out = static_cast<uInt>(deflated.size());

        while (true) {
            ret = deflate(&strm, Z_FINISH);
            if (ret == Z_STREAM_END) {
                total_deflated = strm.total_out;
                break;
            }
            if (ret != Z_OK) {
                deflateEnd(&strm);
                deflate_inited = false;
                return 6;
            }
            // Need more output space: expand buffer
            size_t used = static_cast<size_t>(strm.total_out);
            deflated.resize(deflated.size() * 2);
            strm.next_out = deflated.data() + used;
            strm.avail_out = static_cast<uInt>(deflated.size() - used);
        }

        // Clean up deflate state
        ret = deflateEnd(&strm);
        deflate_inited = false;
        if (ret != Z_OK) return 7;
    }

    // Trim deflated vector to actual size
    if (total_deflated < deflated.size()) deflated.resize(static_cast<size_t>(total_deflated));

    // 3) Use inflate (streaming) API to recreate original
    std::vector<Bytef> inflated;
    {
        z_stream inf;
        std::memset(&inf, 0, sizeof(inf));
        inf.zalloc = Z_NULL;
        inf.zfree = Z_NULL;
        inf.opaque = Z_NULL;

        int ret = inflateInit(&inf);
        if (ret != Z_OK) return 8;
        inflate_inited = true;

        inf.next_in = deflated.data();
        inf.avail_in = static_cast<uInt>(deflated.size());

        // Start with expected original size, grow if necessary
        inflated.resize(original.size());
        inf.next_out = inflated.data();
        inf.avail_out = static_cast<uInt>(inflated.size());

        while (true) {
            ret = inflate(&inf, Z_FINISH);
            if (ret == Z_STREAM_END) break;
            if (ret == Z_OK) {
                // Need more output space
                size_t used = static_cast<size_t>(inf.total_out);
                inflated.resize(inflated.size() * 2);
                inf.next_out = inflated.data() + used;
                inf.avail_out = static_cast<uInt>(inflated.size() - used);
                continue;
            }
            // Some other error
            inflateEnd(&inf);
            inflate_inited = false;
            return 9;
        }

        // Finalize inflate
        ret = inflateEnd(&inf);
        inflate_inited = false;
        if (ret != Z_OK) return 10;

        // Trim to actual size
        inflated.resize(static_cast<size_t>(inf.total_out));
    }

    // Verify inflated content equals original
    if (inflated.size() != original.size()) return 11;
    if (std::memcmp(inflated.data(), original.data(), original.size()) != 0) return 12;

    // 4) Compute and compare checksums (crc32 and adler32)
    {
        uLong crc_orig = crc32(0L, Z_NULL, 0);
        crc_orig = crc32(crc_orig, reinterpret_cast<const Bytef*>(original.data()), static_cast<uInt>(original.size()));

        uLong crc_infl = crc32(0L, Z_NULL, 0);
        crc_infl = crc32(crc_infl, inflated.data(), static_cast<uInt>(inflated.size()));

        if (crc_orig != crc_infl) return 13;

        uLong ad_orig = adler32(0L, Z_NULL, 0);
        ad_orig = adler32(ad_orig, reinterpret_cast<const Bytef*>(original.data()), static_cast<uInt>(original.size()));

        uLong ad_infl = adler32(0L, Z_NULL, 0);
        ad_infl = adler32(ad_infl, inflated.data(), static_cast<uInt>(inflated.size()));

        if (ad_orig != ad_infl) return 14;
    }

    // If we get here, all zlib API sequences used succeeded
    return 66;
}