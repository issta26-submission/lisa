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
//<ID> 98
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Original data to compress
    const char *orig = "The quick brown fox jumps over the lazy dog. zlib API usage test.";
    const size_t orig_len = std::strlen(orig);

    // 1) One-shot compress with compress2()
    uLong sourceLen = static_cast<uLong>(orig_len);
    uLongf compBound = compressBound(sourceLen);
    std::vector<Bytef> compBuf(compBound);

    uLongf compSize1 = compBound;
    int rc = compress2(compBuf.data(), &compSize1, reinterpret_cast<const Bytef*>(orig), sourceLen, Z_BEST_COMPRESSION);
    if (rc != Z_OK) {
        return -1; // compress2 failed
    }

    // 2) One-shot uncompress with uncompress()
    std::vector<Bytef> decompBuf1(sourceLen + 1); // +1 for safety / potential terminator
    uLongf decompSize1 = static_cast<uLongf>(sourceLen);
    rc = uncompress(decompBuf1.data(), &decompSize1, compBuf.data(), compSize1);
    if (rc != Z_OK || decompSize1 != sourceLen) {
        return -2; // uncompress failed or size mismatch
    }
    // Verify content
    if (std::memcmp(decompBuf1.data(), orig, sourceLen) != 0) {
        return -3; // decompressed content mismatch
    }

    // 3) Streaming deflate (compress) with z_stream and deflateInit/deflate/deflateEnd
    z_stream defstrm;
    std::memset(&defstrm, 0, sizeof(defstrm));
    rc = deflateInit(&defstrm, Z_DEFAULT_COMPRESSION);
    if (rc != Z_OK) {
        return -4; // deflateInit failed
    }

    // Prepare input and output for streaming deflate
    defstrm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(orig));
    defstrm.avail_in = static_cast<uInt>(sourceLen); // safe because sourceLen is small
    uLongf streamCompBound = compressBound(sourceLen);
    std::vector<Bytef> compStreamBuf(streamCompBound);
    defstrm.next_out = compStreamBuf.data();
    defstrm.avail_out = static_cast<uInt>(streamCompBound);

    // Perform deflate with Z_FINISH to get the entire compressed stream
    rc = deflate(&defstrm, Z_FINISH);
    if (rc != Z_STREAM_END) {
        deflateEnd(&defstrm);
        return -5; // deflate didn't finish properly
    }
    // Calculate produced size
    uLongf compSize2 = streamCompBound - defstrm.avail_out;
    deflateEnd(&defstrm);

    // 4) Demonstrate deflateReset + deflateParams (re-init cycle)
    z_stream defstrm2;
    std::memset(&defstrm2, 0, sizeof(defstrm2));
    rc = deflateInit(&defstrm2, Z_BEST_SPEED);
    if (rc != Z_OK) {
        return -6; // deflateInit second instance failed
    }
    // change parameters mid-stream (for demonstration; stream is empty currently)
    rc = deflateParams(&defstrm2, Z_BEST_COMPRESSION, Z_DEFAULT_STRATEGY);
    if (rc != Z_OK) {
        deflateEnd(&defstrm2);
        return -7; // deflateParams failed
    }
    // reset and then end
    rc = deflateReset(&defstrm2);
    if (rc != Z_OK) {
        deflateEnd(&defstrm2);
        return -8; // deflateReset failed
    }
    deflateEnd(&defstrm2);

    // 5) Streaming inflate (decompress) with z_stream and inflateInit/inflate/inflateEnd
    z_stream infstrm;
    std::memset(&infstrm, 0, sizeof(infstrm));
    rc = inflateInit(&infstrm);
    if (rc != Z_OK) {
        return -9; // inflateInit failed
    }

    infstrm.next_in = compStreamBuf.data();
    infstrm.avail_in = static_cast<uInt>(compSize2);
    std::vector<Bytef> decompStreamBuf(sourceLen + 1);
    infstrm.next_out = decompStreamBuf.data();
    infstrm.avail_out = static_cast<uInt>(sourceLen + 1);

    // Use inflate in a loop in case multiple calls are necessary (here one should suffice)
    while (true) {
        rc = inflate(&infstrm, Z_FINISH);
        if (rc == Z_STREAM_END) {
            break; // finished successfully
        } else if (rc == Z_OK) {
            // not finished yet, but for our small input this path is unlikely
            if (infstrm.avail_out == 0) {
                // output buffer too small
                inflateEnd(&infstrm);
                return -10;
            }
            continue;
        } else {
            inflateEnd(&infstrm);
            return -11; // inflate error
        }
    }

    // Determine decompressed size
    uLongf decompSize2 = (sourceLen + 1) - infstrm.avail_out;
    inflateEnd(&infstrm);

    if (decompSize2 != sourceLen) {
        return -12; // decompressed size mismatch
    }
    if (std::memcmp(decompStreamBuf.data(), orig, sourceLen) != 0) {
        return -13; // streaming decompressed content mismatch
    }

    // 6) Additional convenience API: compressBound was used earlier.
    //    Show compress() wrapper (simpler) as another API demonstration.
    std::vector<Bytef> compBufSimple(compBound);
    uLongf compSize3 = compBound;
    rc = compress(compBufSimple.data(), &compSize3, reinterpret_cast<const Bytef*>(orig), sourceLen);
    if (rc != Z_OK) {
        return -14; // compress failed
    }
    // quick uncompress check
    std::vector<Bytef> decompCheck(sourceLen + 1);
    uLongf decompSize3 = static_cast<uLongf>(sourceLen);
    rc = uncompress(decompCheck.data(), &decompSize3, compBufSimple.data(), compSize3);
    if (rc != Z_OK || decompSize3 != sourceLen) {
        return -15; // uncompress after compress failed
    }

    // All zlib API sequences completed and verified.
    return 66;
}