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
//<ID> 89
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // A small input buffer (use sizeof - 1 to avoid needing strlen)
    const char input[] = "The quick brown fox jumps over the lazy dog";
    uLong src_len = (uLong)(sizeof(input) - 1); // length of data without terminating NUL

    // --- 1) Deflate (stream API) ---
    // Allocate an output buffer using compressBound to be safe
    uLong bound = compressBound(src_len);
    unsigned char* comp = new unsigned char[bound];
    if (!comp) return -1;

    z_stream dstrm;
    // Zero the structure
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = Z_NULL;
    dstrm.zfree = Z_NULL;
    dstrm.opaque = Z_NULL;
    dstrm.next_in = (Bytef*)input;
    dstrm.avail_in = (uInt)src_len; // safe for small input
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;

    int ret = deflateInit(&dstrm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        delete[] comp;
        return -2;
    }

    // Run deflate with Z_FINISH; loop while returns Z_OK
    do {
        ret = deflate(&dstrm, Z_FINISH);
    } while (ret == Z_OK);

    if (ret != Z_STREAM_END) {
        deflateEnd(&dstrm);
        delete[] comp;
        return -3;
    }

    uLong comp_size = dstrm.total_out;
    deflateEnd(&dstrm);

    // --- 2) Inflate (stream API) ---
    unsigned char* decomp = new unsigned char[src_len + 16]; // a bit of headroom
    if (!decomp) { delete[] comp; return -4; }

    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = Z_NULL;
    istrm.zfree = Z_NULL;
    istrm.opaque = Z_NULL;
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)(src_len + 16);

    ret = inflateInit(&istrm);
    if (ret != Z_OK) {
        delete[] comp;
        delete[] decomp;
        return -5;
    }

    // Loop until stream end
    do {
        ret = inflate(&istrm, Z_NO_FLUSH);
    } while (ret == Z_OK);

    if (ret != Z_STREAM_END) {
        inflateEnd(&istrm);
        delete[] comp;
        delete[] decomp;
        return -6;
    }

    uLong decomp_size = istrm.total_out;
    inflateEnd(&istrm);

    // Verify decompressed size and contents match original
    if (decomp_size != src_len || memcmp(decomp, input, src_len) != 0) {
        delete[] comp;
        delete[] decomp;
        return -7;
    }

    // --- 3) Convenience API: compress / uncompress ---
    uLongf comp2_capacity = (uLongf)compressBound(src_len);
    unsigned char* comp2 = new unsigned char[comp2_capacity];
    if (!comp2) { delete[] comp; delete[] decomp; return -8; }
    uLongf comp2_size = comp2_capacity;

    ret = compress(comp2, &comp2_size, (const Bytef*)input, src_len);
    if (ret != Z_OK) {
        delete[] comp;
        delete[] decomp;
        delete[] comp2;
        return -9;
    }

    unsigned char* decomp2 = new unsigned char[src_len + 1];
    if (!decomp2) { delete[] comp; delete[] decomp; delete[] comp2; return -10; }
    uLongf decomp2_size = (uLongf)src_len;

    ret = uncompress(decomp2, &decomp2_size, comp2, comp2_size);
    if (ret != Z_OK) {
        delete[] comp;
        delete[] decomp;
        delete[] comp2;
        delete[] decomp2;
        return -11;
    }

    if (decomp2_size != src_len || memcmp(decomp2, input, src_len) != 0) {
        delete[] comp;
        delete[] decomp;
        delete[] comp2;
        delete[] decomp2;
        return -12;
    }

    // --- 4) Checksums: crc32 and adler32 ---
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, (const Bytef*)input, src_len);

    uLong adl = adler32(0L, Z_NULL, 0);
    adl = adler32(adl, (const Bytef*)input, src_len);

    // Basic sanity: checksums should be non-zero for this non-empty input
    if (crc == 0 || adl == 0) {
        delete[] comp;
        delete[] decomp;
        delete[] comp2;
        delete[] decomp2;
        return -13;
    }

    // Clean up
    delete[] comp;
    delete[] decomp;
    delete[] comp2;
    delete[] decomp2;

    // If we reached here, all zlib API sequences succeeded
    return 66;
}