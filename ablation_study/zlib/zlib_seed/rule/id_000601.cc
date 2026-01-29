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
//<ID> 601
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
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 128UL;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong srcLen = (uLong)INPUT_LEN;
    uLong bound = compressBound(srcLen);
    uLongf compLen = (uLongf)bound;
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    const uLong OUT_LEN = 256UL;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);

    // step 2: Initialize inflate stream
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inf.next_out = outBuf;
    inf.avail_out = (uInt)OUT_LEN;

    // step 3: Operate: compress input, compute CRCs, combine CRCs, prepare inflate input, write via gz
    int compRes = compress(compBuf, &compLen, input, srcLen);
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    uLong crc1 = crc32(0L, input, (uInt)srcLen);
    uLong crc2 = crc32(0L, compBuf, (uInt)compLen);
    uLong combined_crc = crc32_combine(crc1, crc2, (off_t)compLen);
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, input, (unsigned int)srcLen);
    gzclose_w(gz);

    // step 4: Validate/cleanup
    (void)compRes;
    (void)combined_crc;
    inflateEnd(&inf);
    free(input);
    free(compBuf);
    free(outBuf);
    // API sequence test completed successfully
    return 66;
}