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
//<ID> 29
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // The function uses the zlib API to:
    // 1) compress data using the streaming deflate API (deflateInit/deflate/deflateEnd)
    // 2) decompress that data using the streaming inflate API (inflateInit/inflate/inflateEnd)
    // 3) additionally use the convenience API (compress/uncompress) to verify both paths
    //
    // Returns 66 on success, other values on failure.

    // Include headers here so this function remains self-contained when pasted alone.
    // (Including inside a function is allowed; preprocessing happens before compilation.)
    #include <zlib.h>
    #include <string>
    #include <vector>
    #include <cstring>

    // Sample input data
    const std::string original = "The quick brown fox jumps over the lazy dog. "
                                 "zlib test data repeated. "
                                 "The quick brown fox jumps over the lazy dog. "
                                 "zlib test data repeated.";

    // ---------- Streaming deflate (compress) ----------
    z_stream defs;
    std::memset(&defs, 0, sizeof(defs));
    int ret = deflateInit(&defs, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return 1;
    }

    // Prepare buffers
    std::vector<unsigned char> compBuf(256); // start small and grow if needed
    defs.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(original.data()));
    // zlib expects avail_in as uInt; safe for our small sample
    defs.avail_in = static_cast<uInt>(original.size());
    defs.next_out = compBuf.data();
    defs.avail_out = static_cast<uInt>(compBuf.size());

    // Run deflate until stream end
    while (true) {
        ret = deflate(&defs, Z_FINISH);
        if (ret == Z_STREAM_END) {
            break; // finished successfully
        }
        if (ret == Z_OK && defs.avail_out == 0) {
            // Need more output space: grow buffer
            size_t oldSize = compBuf.size();
            compBuf.resize(oldSize * 2);
            defs.next_out = compBuf.data() + oldSize;
            defs.avail_out = static_cast<uInt>(compBuf.size() - oldSize);
            continue;
        }
        // Unexpected error
        deflateEnd(&defs);
        return 2;
    }

    // Compressed size is total buffer size minus remaining available
    size_t compSize = compBuf.size() - defs.avail_out;
    ret = deflateEnd(&defs);
    if (ret != Z_OK) return 3;

    // ---------- Streaming inflate (decompress) ----------
    z_stream infs;
    std::memset(&infs, 0, sizeof(infs));
    ret = inflateInit(&infs);
    if (ret != Z_OK) {
        return 4;
    }

    std::vector<unsigned char> outBuf(original.size() * 2); // should be enough; grow if needed
    infs.next_in = compBuf.data();
    infs.avail_in = static_cast<uInt>(compSize);
    infs.next_out = outBuf.data();
    infs.avail_out = static_cast<uInt>(outBuf.size());

    while (true) {
        ret = inflate(&infs, Z_NO_FLUSH);
        if (ret == Z_STREAM_END) {
            break; // finished successfully
        }
        if (ret == Z_OK && infs.avail_out == 0) {
            // Need more output space
            size_t oldSize = outBuf.size();
            outBuf.resize(oldSize * 2);
            infs.next_out = outBuf.data() + oldSize;
            infs.avail_out = static_cast<uInt>(outBuf.size() - oldSize);
            continue;
        }
        // Unexpected error
        inflateEnd(&infs);
        return 5;
    }

    size_t outSize = outBuf.size() - infs.avail_out;
    ret = inflateEnd(&infs);
    if (ret != Z_OK) return 6;

    // Validate decompressed data matches original
    if (outSize != original.size()) return 7;
    if (std::memcmp(outBuf.data(), original.data(), original.size()) != 0) return 8;

    // ---------- Convenience API: compress/uncompress ----------
    // compress()
    std::vector<unsigned char> comp2(256);
    uLongf comp2Len = static_cast<uLongf>(comp2.size());
    ret = compress(comp2.data(), &comp2Len,
                   reinterpret_cast<const Bytef*>(original.data()),
                   static_cast<uLong>(original.size()));
    if (ret == Z_BUF_ERROR) {
        // enlarge and retry
        comp2.resize(original.size()); // worst-case naive
        comp2Len = static_cast<uLongf>(comp2.size());
        ret = compress(comp2.data(), &comp2Len,
                       reinterpret_cast<const Bytef*>(original.data()),
                       static_cast<uLong>(original.size()));
    }
    if (ret != Z_OK) return 9;

    // uncompress()
    std::vector<unsigned char> un2(original.size());
    uLongf un2Len = static_cast<uLongf>(un2.size());
    ret = uncompress(un2.data(), &un2Len, comp2.data(), comp2Len);
    if (ret != Z_OK) return 10;
    if (un2Len != original.size()) return 11;
    if (std::memcmp(un2.data(), original.data(), original.size()) != 0) return 12;

    // If we reached here, both streaming and convenience APIs worked and data matched.
    return 66;
}