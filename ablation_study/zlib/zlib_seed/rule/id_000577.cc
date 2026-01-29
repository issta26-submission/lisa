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
//<ID> 577
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char * filename = "tmp_zlib_api_sequence.gz";
    unsigned char input[256];
    memset(input, 'A', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong bound = compressBound(inputLen);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    Bytef * outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);
    uLong flags = zlibCompileFlags();

    // step 2: Compress the input into compBuf
    compress(compBuf, &compLen, (const Bytef *)input, inputLen);

    // step 3: Write compressed data to a gz file, query position, compute CRC combine
    gzFile gz = gzopen64(filename, "wb");
    gzwrite(gz, compBuf, (unsigned int)compLen);
    off64_t pos64 = gztell64(gz);
    uLong crc1 = crc32(0L, (const Bytef *)input, (uInt)inputLen);
    uLong crc2 = crc32(0L, (const Bytef *)"Z", (uInt)1);
    uLong combined = crc32_combine(crc1, crc2, (off_t)inputLen);

    // step 4: Decompress to validate and cleanup
    uLongf outLen = (uLongf)inputLen;
    uncompress(outBuf, &outLen, compBuf, (uLong)compLen);
    gzclose(gz);
    free(compBuf);
    free(outBuf);

    // API sequence test completed successfully
    (void)flags;
    (void)pos64;
    (void)combined;
    return 66;
}