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
//<ID> 136
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // A small sample input (use sizeof-1 to avoid needing strlen)
    const char input[] = "The quick brown fox jumps over the lazy dog";
    const uLong input_len = static_cast<uLong>(sizeof(input) - 1);

    // 1) Initialize deflate (compress) stream
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = Z_NULL;
    strm.avail_in = 0;
    strm.next_out = Z_NULL;
    strm.avail_out = 0;

    int ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return -1;
    }

    // 2) Compute an upper bound for compressed size and allocate output buffer
    uLong bound = deflateBound(&strm, input_len);
    unsigned char* comp = new (std::nothrow) unsigned char[bound];
    if (!comp) {
        deflateEnd(&strm);
        return -2;
    }

    // 3) Provide input and output buffers to the stream and compress
    strm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input));
    strm.avail_in = input_len;
    strm.next_out = reinterpret_cast<Bytef*>(comp);
    strm.avail_out = bound;

    ret = deflate(&strm, Z_FINISH);
    if (ret != Z_STREAM_END) {
        // Clean up on error
        deflateEnd(&strm);
        delete[] comp;
        return -3;
    }

    // compressed size is bound - remaining output space
    uLong comp_size = bound - strm.avail_out;

    // Finish deflate
    ret = deflateEnd(&strm);
    if (ret != Z_OK) {
        delete[] comp;
        return -4;
    }

    // 4) Initialize inflate (decompress) stream
    z_stream istrm;
    istrm.zalloc = Z_NULL;
    istrm.zfree = Z_NULL;
    istrm.opaque = Z_NULL;
    istrm.next_in = Z_NULL;
    istrm.avail_in = 0;
    istrm.next_out = Z_NULL;
    istrm.avail_out = 0;

    ret = inflateInit(&istrm);
    if (ret != Z_OK) {
        delete[] comp;
        return -5;
    }

    // Allocate buffer large enough for the uncompressed data (+1 for safety/terminator)
    unsigned char* decomp = new (std::nothrow) unsigned char[input_len + 1];
    if (!decomp) {
        inflateEnd(&istrm);
        delete[] comp;
        return -6;
    }

    // 5) Provide compressed data and output buffer and decompress
    istrm.next_in = reinterpret_cast<Bytef*>(comp);
    istrm.avail_in = comp_size;
    istrm.next_out = reinterpret_cast<Bytef*>(decomp);
    istrm.avail_out = input_len + 1;

    ret = inflate(&istrm, Z_FINISH);
    if (ret != Z_STREAM_END) {
        // Clean up on error
        inflateEnd(&istrm);
        delete[] comp;
        delete[] decomp;
        return -7;
    }

    // decompressed size
    uLong decomp_size = (input_len + 1) - istrm.avail_out;

    // Finish inflate
    ret = inflateEnd(&istrm);
    if (ret != Z_OK) {
        delete[] comp;
        delete[] decomp;
        return -8;
    }

    // 6) Verify decompressed data matches original
    if (decomp_size != input_len) {
        delete[] comp;
        delete[] decomp;
        return -9;
    }
    for (uLong i = 0; i < input_len; ++i) {
        if (decomp[i] != static_cast<unsigned char>(input[i])) {
            delete[] comp;
            delete[] decomp;
            return -10;
        }
    }

    // Clean up and return success code 66
    delete[] comp;
    delete[] decomp;
    return 66;
}