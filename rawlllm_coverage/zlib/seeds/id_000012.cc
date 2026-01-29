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
//<ID> 12
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // Small test input
    const char src[] = "The quick brown fox jumps over the lazy dog";
    uLong src_len = (uLong)(sizeof(src) - 1); // exclude terminating NUL

    // Pointers for dynamic buffers (initialized to nullptr to allow safe cleanup)
    Bytef* comp = nullptr;
    Bytef* un = nullptr;
    Bytef* stream_out = nullptr;
    Bytef* stream_decompressed = nullptr;

    int ret = Z_OK;

    // --- 1) compress / uncompress using convenience APIs ---
    // Determine bound and allocate
    uLong bound = compressBound(src_len);
    comp = new Bytef[bound];

    uLongf comp_len = (uLongf)bound;
    ret = compress(comp, &comp_len, (const Bytef*)src, src_len);
    if (ret != Z_OK) {
        delete[] comp;
        return -1;
    }

    // Decompress with known original size
    un = new Bytef[src_len + 1]; // +1 so we can NUL-terminate for easy checks
    uLongf un_len = (uLongf)src_len;
    ret = uncompress(un, &un_len, comp, comp_len);
    if (ret != Z_OK) {
        delete[] comp;
        delete[] un;
        return -2;
    }
    // verify decompressed content matches original
    if (un_len != src_len) {
        delete[] comp;
        delete[] un;
        return -3;
    }
    for (uLong i = 0; i < src_len; ++i) {
        if (un[i] != (Bytef)src[i]) {
            delete[] comp;
            delete[] un;
            return -4;
        }
    }

    // --- 2) checksums: adler32 and crc32 ---
    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, (const Bytef*)src, (uInt)src_len);
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, (const Bytef*)src, (uInt)src_len);
    // (We don't assert particular values here, just exercise the APIs.)

    // --- 3) deflate / inflate using z_stream APIs ---
    // Initialize deflate stream
    z_stream dstrm = {0};
    ret = deflateInit(&dstrm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        delete[] comp;
        delete[] un;
        return -5;
    }

    // Provide input
    dstrm.next_in = (Bytef*)src;
    dstrm.avail_in = (uInt)src_len; // safe for small src

    // Allocate output buffer sized by compressBound (should be enough)
    stream_out = new Bytef[bound];
    dstrm.next_out = stream_out;
    dstrm.avail_out = (uInt)bound;

    // Perform deflate (single-call, Z_FINISH)
    ret = deflate(&dstrm, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&dstrm);
        delete[] comp;
        delete[] un;
        delete[] stream_out;
        return -6;
    }
    uLongf deflated_size = (uLongf)(bound - dstrm.avail_out);
    deflateEnd(&dstrm);

    // Initialize inflate stream
    z_stream istrm = {0};
    ret = inflateInit(&istrm);
    if (ret != Z_OK) {
        delete[] comp;
        delete[] un;
        delete[] stream_out;
        return -7;
    }

    istrm.next_in = stream_out;
    istrm.avail_in = (uInt)deflated_size;

    // Allocate buffer to receive inflated data
    stream_decompressed = new Bytef[src_len + 1];
    istrm.next_out = stream_decompressed;
    istrm.avail_out = (uInt)(src_len + 1);

    // Perform inflate
    ret = inflate(&istrm, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&istrm);
        delete[] comp;
        delete[] un;
        delete[] stream_out;
        delete[] stream_decompressed;
        return -8;
    }
    uLongf inflated_size = (uLongf)((src_len + 1) - istrm.avail_out);
    inflateEnd(&istrm);

    // Verify inflated data matches original
    if (inflated_size != src_len) {
        delete[] comp;
        delete[] un;
        delete[] stream_out;
        delete[] stream_decompressed;
        return -9;
    }
    for (uLong i = 0; i < src_len; ++i) {
        if (stream_decompressed[i] != (Bytef)src[i]) {
            delete[] comp;
            delete[] un;
            delete[] stream_out;
            delete[] stream_decompressed;
            return -10;
        }
    }

    // Cleanup
    delete[] comp;
    delete[] un;
    delete[] stream_out;
    delete[] stream_decompressed;

    // If we reach here, all zlib API sequences succeeded
    return 66;
}