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
//<ID> 139
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Sample input data
    std::string input;
    for (int i = 0; i < 64; ++i) {
        input += "The quick brown fox jumps over the lazy dog. ";
    }
    uLong sourceLen = static_cast<uLong>(input.size());

    // 1) Compress using deflate APIs
    z_stream cstrm;
    std::memset(&cstrm, 0, sizeof(cstrm));
    int ret = deflateInit(&cstrm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return -1;
    bool deflateInitialized = true;

    // Pre-allocate output buffer using compressBound (safe upper bound)
    uLong bound = compressBound(sourceLen);
    std::vector<unsigned char> compBuf(bound);
    cstrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    cstrm.avail_in = static_cast<uInt>(sourceLen);
    cstrm.next_out = compBuf.data();
    cstrm.avail_out = static_cast<uInt>(compBuf.size());

    // deflate until stream ends (handle possible reallocation if needed)
    while (true) {
        ret = deflate(&cstrm, Z_FINISH);
        if (ret == Z_STREAM_END) break;
        if (ret == Z_OK) {
            // need more output space; enlarge buffer
            size_t oldSize = compBuf.size();
            compBuf.resize(oldSize * 2);
            cstrm.next_out = compBuf.data() + oldSize;
            cstrm.avail_out = static_cast<uInt>(oldSize);
            continue;
        }
        // error
        deflateEnd(&cstrm);
        return -2;
    }

    // compressed size
    size_t compSize = static_cast<size_t>(cstrm.total_out);
    // finalize deflate
    deflateEnd(&cstrm);
    deflateInitialized = false;

    // Shrink to actual size
    compBuf.resize(compSize);

    // 2) Decompress using inflate APIs
    z_stream dstrm;
    std::memset(&dstrm, 0, sizeof(dstrm));
    ret = inflateInit(&dstrm);
    if (ret != Z_OK) return -3;
    bool inflateInitialized = true;

    std::vector<unsigned char> decompBuf;
    decompBuf.resize(sourceLen); // start with expected size

    dstrm.next_in = compBuf.data();
    dstrm.avail_in = static_cast<uInt>(compSize);
    dstrm.next_out = decompBuf.data();
    dstrm.avail_out = static_cast<uInt>(decompBuf.size());

    while (true) {
        ret = inflate(&dstrm, Z_NO_FLUSH);
        if (ret == Z_STREAM_END) break;
        if (ret == Z_OK) {
            if (dstrm.avail_out == 0) {
                // output buffer full but not finished; expand
                size_t oldSize = decompBuf.size();
                decompBuf.resize(oldSize * 2);
                dstrm.next_out = decompBuf.data() + oldSize;
                dstrm.avail_out = static_cast<uInt>(oldSize);
                continue;
            }
            // If we got Z_OK and still have avail_out, we may need more input.
            if (dstrm.avail_in == 0) {
                // No more input but not finished -> error
                inflateEnd(&dstrm);
                return -4;
            }
            continue;
        }
        // error
        inflateEnd(&dstrm);
        return -5;
    }

    // decompressed size
    size_t decompSize = static_cast<size_t>(dstrm.total_out);
    // finalize inflate
    inflateEnd(&dstrm);
    inflateInitialized = false;

    // Resize to actual decompressed size
    decompBuf.resize(decompSize);

    // 3) Verify decompressed data matches original
    if (decompSize != input.size()) return -6;
    if (std::memcmp(decompBuf.data(), input.data(), input.size()) != 0) return -7;

    // If we reach here, sequence succeeded
    return 66;
}