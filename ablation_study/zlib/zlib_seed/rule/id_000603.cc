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
//<ID> 603
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    z_stream infStrm;
    memset(&infStrm, 0, sizeof(infStrm));
    const char *ver = zlibVersion();
    uLong inputLen = (uLong)64;
    Bytef *input = (Bytef *)malloc((size_t)inputLen);
    memset(input, 'B', (size_t)inputLen);
    uLong bound = compressBound(inputLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    Bytef *outBuf = (Bytef *)malloc((size_t)inputLen);
    memset(outBuf, 0, (size_t)inputLen);

    // step 2: Initialize / configure
    compress(compBuf, &compLen, input, inputLen);
    inflateInit_(&infStrm, ver, (int)sizeof(z_stream));
    infStrm.next_in = compBuf;
    infStrm.avail_in = (uInt)compLen;
    infStrm.next_out = outBuf;
    infStrm.avail_out = (uInt)inputLen;

    // step 3: Operate (CRC combine, write compressed data to gz)
    uLong half = inputLen / 2;
    uLong crc1 = crc32(0L, input, (uInt)half);
    uLong crc2 = crc32(0L, input + half, (uInt)(inputLen - half));
    uLong combined = crc32_combine(crc1, crc2, (off_t)(inputLen - half));
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, compBuf, (unsigned int)compLen);
    gzclose_w(gz);

    // step 4: Cleanup & validate
    (void)combined;
    inflateEnd(&infStrm);
    free(input);
    free(compBuf);
    free(outBuf);
    // API sequence test completed successfully
    return 66;
}