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
//<ID> 608
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    const char * ver = zlibVersion();
    const uLong INPUT_LEN = 128;
    Bytef * input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'Z', (size_t)INPUT_LEN);
    uLong inputLen = (uLong)INPUT_LEN;
    uLongf destLen = compressBound(inputLen);
    Bytef * dest = (Bytef *)malloc((size_t)destLen);
    memset(dest, 0, (size_t)destLen);

    // step 2: Initialize inflate stream & compress data
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    compress(dest, &destLen, input, inputLen);

    // step 3: Compute checksums, combine them, write compressed data to gz file
    uLong crc1 = crc32(0L, input, (uInt)inputLen);
    uLong crc2 = crc32(0L, dest, (uInt)destLen);
    uLong combined = crc32_combine(crc1, crc2, (off_t)destLen);
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, dest, (unsigned int)destLen);
    gzclose_w(gz);

    // step 4: Cleanup
    inflateEnd(&inf);
    free(input);
    free(dest);
    (void)ver;
    (void)combined;
    // API sequence test completed successfully
    return 66;
}