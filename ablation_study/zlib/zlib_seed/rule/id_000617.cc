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
//<ID> 617
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 256;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong bound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    uLongf compLen = (uLongf)bound;
    const uLong OUT_LEN = 512;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);

    // step 2: Compress the input into compBuf
    compress(compBuf, &compLen, input, src_len);

    // step 3: Initialize inflate stream, adjust window and sync, then inflate
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    inflateInit_(&strm, ver, (int)sizeof(z_stream));
    strm.next_in = compBuf;
    strm.avail_in = (uInt)compLen;
    strm.next_out = outBuf;
    strm.avail_out = (uInt)OUT_LEN;
    inflateReset2(&strm, 15);
    inflateSync(&strm);
    inflate(&strm, 0);

    // step 4: Compute CRCs, write original data to a gz file, cleanup
    uLong half = (uLong)(src_len / 2);
    uLong crc1 = crc32(0L, input, (uInt)half);
    uLong crc2 = crc32(0L, input + half, (uInt)(src_len - half));
    uLong combined_crc = crc32_combine(crc1, crc2, (off_t)(src_len - half));
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, input, (unsigned int)src_len);
    gzclose(gz);
    inflateEnd(&strm);
    free(input);
    free(compBuf);
    free(outBuf);
    (void)ver;
    (void)combined_crc;
    // API sequence test completed successfully
    return 66;
}