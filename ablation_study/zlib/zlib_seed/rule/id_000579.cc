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
//<ID> 579
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    Bytef *input = (Bytef *)malloc((size_t)256);
    memset(input, 'A', 256);
    uLong inputLen = (uLong)256;
    uLong bound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Compress the input into compBuf
    compress(compBuf, &compLen, (const Bytef *)input, inputLen);

    // step 3: Write compressed data to a gz file and query its 64-bit tell position
    gzFile gz = gzopen64("tmp_test_zlib.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compLen);
    off64_t filePos = gztell64(gz);
    (void)filePos;
    gzflush(gz, Z_FINISH);
    gzclose(gz);

    // step 4: Compute CRCs, combine them, query compile flags, and cleanup
    uLong crcA = crc32(0L, input, (uInt)inputLen);
    uLong crcB = crc32(0L, compBuf, (uInt)compLen);
    uLong combinedCrc = crc32_combine(crcA, crcB, (off_t)inputLen);
    (void)combinedCrc;
    uLong flags = zlibCompileFlags();
    (void)flags;
    free(input);
    free(compBuf);

    // API sequence test completed successfully
    return 66;
}