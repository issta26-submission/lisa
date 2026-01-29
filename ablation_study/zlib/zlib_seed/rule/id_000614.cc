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
//<ID> 614
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 128;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong bound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    compress(compBuf, &compLen, input, src_len);
    uLong split = (uLong)(src_len / 2);
    uLong crc1 = crc32(0L, input, (uInt)split);
    uLong crc2 = crc32(0L, input + split, (uInt)(src_len - split));
    uLong combined_crc = crc32_combine(crc1, crc2, (off_t)(src_len - split));

    // step 2: Initialize inflate stream and buffers, perform inflate operations
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    inflateInit_(&strm, ver, (int)sizeof(z_stream));
    strm.next_in = compBuf;
    strm.avail_in = (uInt)compLen;
    const uLong OUT_LEN = INPUT_LEN * 2;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);
    strm.next_out = outBuf;
    strm.avail_out = (uInt)OUT_LEN;
    inflateReset2(&strm, 15);
    inflate(&strm, 0);
    inflateSync(&strm);

    // step 3: Write original input to a gzip file
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, input, (unsigned int)src_len);
    gzclose(gz);

    // step 4: Cleanup
    inflateEnd(&strm);
    free(input);
    free(compBuf);
    free(outBuf);
    (void)ver;
    (void)combined_crc;
    return 66;
    // API sequence test completed successfully
}