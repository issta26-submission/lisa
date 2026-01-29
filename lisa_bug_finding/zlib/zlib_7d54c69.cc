#include <cstddef>
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
#include <stdio.h>
#include <assert.h>

int test_zlib_api_sequence() {
    // step 1: Declarations
    size_t raw_sz = 16u * 1024u * 1024u;           // 16 MB
    Bytef* raw = (Bytef*)malloc(raw_sz);
    uLongf comp_bound = compressBound((uLong)raw_sz);
    Bytef* comp = (Bytef*)malloc(comp_bound);
    size_t out_cap = 1u * 1024u * 1024u;           // 1 MB output buffer
    Bytef* out = (Bytef*)malloc(out_cap);
    uLongf comp_sz = comp_bound;
    uLongf dest_len = (uLongf)UINT_MAX + 10u;     
    int rc = Z_OK;

    // step 2: Setup
    assert(raw != NULL && "raw buffer allocation failed");
    assert(comp != NULL && "comp buffer allocation failed");
    assert(out != NULL && "out buffer allocation failed");
    memset(raw, 'A', raw_sz);
    memset(comp, 0, comp_bound);
    memset(out, 0, out_cap);

    assert(raw_sz > 0);
    assert(((unsigned char*)raw)[0] == 'A');

    rc = compress(comp, &comp_sz, raw, (uLong)raw_sz);
    assert(rc == Z_OK && "compress() failed unexpectedly");
    assert(comp_sz > 0 && comp_sz <= comp_bound && "compressed size invalid");
    assert(memcmp(raw, comp, (size_t)comp_sz) != 0 && "compressed data should differ from raw");

    // step 3: Operate
    rc = uncompress(out, &dest_len, comp, (uLong)comp_sz);
    assert(rc == Z_DATA_ERROR);
    assert(dest_len > 0 && dest_len > (uLongf)out_cap);

    // step 4: Validate & Cleanup
    size_t nonzero = 0;
    for (size_t i = 0; i < 128 && i < out_cap; ++i)
        nonzero += (out[i] != 0);
    assert(nonzero > 0 && "output buffer remains all zero");

    assert(out != comp && out != raw && comp != raw);

    uLong crc_raw = crc32(0L, raw, (uInt)raw_sz);
    uLong crc_out = crc32(0L, out, (uInt)dest_len);

    assert(crc_raw != crc_out && "CRC mismatch between raw and decompressed output");
    free(raw);

    printf("API sequence test completed successfully\n");
    return 66;
}

int main() {
    int result = test_zlib_api_sequence();
    if (result != 66) {
        std::cerr << "API sequence test failed with error code: " << result << std::endl;
        exit(100);
    }
    return 0;
}
