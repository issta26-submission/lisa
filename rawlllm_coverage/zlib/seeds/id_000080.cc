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
//<ID> 80
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // This function demonstrates a correct sequence of zlib APIs:
    //  - initialize a deflate stream, compress data (with dynamic output growth),
    //  - finalize deflate, initialize an inflate stream, decompress the data
    //  - verify decompressed bytes equal the original,
    //  - clean up and return 66 on success.
    //
    // It uses only zlib APIs (deflateInit/deflate/deflateEnd and
    // inflateInit/inflate/inflateEnd) and plain C allocations so it
    // compiles if zlib.h is included in the translation unit.

    // Example input
    const char input_literal[] = "The quick brown fox jumps over the lazy dog. 1234567890";
    const size_t input_len = sizeof(input_literal) - 1; // exclude terminating NUL

    int ret;
    z_stream defstream;
    z_stream infstream;
    unsigned char *comp = nullptr;
    size_t comp_capacity = 0;
    unsigned char *decomp = nullptr;
    size_t decomp_capacity = 0;
    bool deflate_inited = false;
    bool inflate_inited = false;

    // --- Initialize deflate stream ---
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;
    defstream.next_in = Z_NULL;
    defstream.avail_in = 0;
    ret = deflateInit(&defstream, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        return -1;
    }
    deflate_inited = true;

    // Prepare input for deflate
    defstream.next_in = (Bytef*)input_literal;
    defstream.avail_in = (uInt)input_len;

    // Start with an output buffer sized using compressBound (safe upper bound)
    comp_capacity = compressBound((uLong)input_len);
    comp = (unsigned char*)malloc(comp_capacity);
    if (!comp) {
        deflateEnd(&defstream);
        return -2;
    }
    defstream.next_out = comp;
    defstream.avail_out = (uInt)comp_capacity;

    // Deflate in a loop, growing the output buffer if needed
    int flush = Z_FINISH;
    while (true) {
        ret = deflate(&defstream, flush);
        if (ret == Z_STREAM_ERROR) {
            // internal error
            free(comp);
            deflateEnd(&defstream);
            return -3;
        }

        if (ret == Z_STREAM_END) {
            // finished compression
            break;
        }

        // if output buffer exhausted and stream not finished, grow it
        if (defstream.avail_out == 0) {
            size_t used = comp_capacity; // all used
            size_t new_capacity = comp_capacity * 2;
            unsigned char *tmp = (unsigned char*)realloc(comp, new_capacity);
            if (!tmp) {
                free(comp);
                deflateEnd(&defstream);
                return -4;
            }
            comp = tmp;
            // update pointers and capacities in stream
            defstream.next_out = comp + used;
            defstream.avail_out = (uInt)(new_capacity - used);
            comp_capacity = new_capacity;
            // continue compression
            continue;
        }

        // Unexpected return (should not generally happen), treat as error
        if (ret != Z_OK) {
            free(comp);
            deflateEnd(&defstream);
            return -5;
        }
    }

    // get compressed size
    size_t comp_size = comp_capacity - defstream.avail_out;

    // finalize deflate
    deflateEnd(&defstream);
    deflate_inited = false;

    // --- Initialize inflate stream ---
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;
    infstream.next_in = Z_NULL;
    infstream.avail_in = 0;
    ret = inflateInit(&infstream);
    if (ret != Z_OK) {
        free(comp);
        return -6;
    }
    inflate_inited = true;

    // Provide compressed data as input
    infstream.next_in = comp;
    // avail_in is uInt; ensure comp_size fits into uInt
    if (comp_size > (size_t)UINT_MAX) {
        // unrealistic for this test input, but handle gracefully
        free(comp);
        inflateEnd(&infstream);
        return -7;
    }
    infstream.avail_in = (uInt)comp_size;

    // Prepare output buffer to hold decompressed data; initially same as input length + 1
    decomp_capacity = input_len + 1;
    decomp = (unsigned char*)malloc(decomp_capacity);
    if (!decomp) {
        free(comp);
        inflateEnd(&infstream);
        return -8;
    }
    infstream.next_out = decomp;
    if (decomp_capacity > (size_t)UINT_MAX) { // avoid overflow for avail_out
        free(comp);
        free(decomp);
        inflateEnd(&infstream);
        return -9;
    }
    infstream.avail_out = (uInt)decomp_capacity;

    // Inflate in a loop, growing output buffer if needed
    while (true) {
        ret = inflate(&infstream, Z_NO_FLUSH);
        if (ret == Z_STREAM_END) {
            // finished decompression
            break;
        }
        if (ret == Z_NEED_DICT || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
            free(comp);
            free(decomp);
            inflateEnd(&infstream);
            return -10;
        }

        if (ret == Z_OK && infstream.avail_out == 0) {
            // need to grow output buffer
            size_t used = decomp_capacity;
            size_t new_capacity = decomp_capacity * 2;
            unsigned char *tmp = (unsigned char*)realloc(decomp, new_capacity);
            if (!tmp) {
                free(comp);
                free(decomp);
                inflateEnd(&infstream);
                return -11;
            }
            decomp = tmp;
            infstream.next_out = decomp + used;
            infstream.avail_out = (uInt)(new_capacity - used);
            decomp_capacity = new_capacity;
            continue;
        }

        // other unexpected ret values
        if (ret != Z_OK) {
            free(comp);
            free(decomp);
            inflateEnd(&infstream);
            return -12;
        }
    }

    // determine decompressed size
    size_t decomp_size = decomp_capacity - infstream.avail_out;

    // Finalize inflate
    inflateEnd(&infstream);
    inflate_inited = false;

    // --- Verify decompressed data matches original ---
    if (decomp_size != input_len) {
        free(comp);
        free(decomp);
        return -13;
    }
    // byte-wise compare
    for (size_t i = 0; i < input_len; ++i) {
        if (decomp[i] != (unsigned char)input_literal[i]) {
            free(comp);
            free(decomp);
            return -14;
        }
    }

    // Cleanup
    free(comp);
    free(decomp);

    // Success: return 66 as requested
    return 66;
}