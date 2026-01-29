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
//<ID> 609
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 100;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'Z', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong bound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Compress the input into compBuf
    compress(compBuf, &compLen, input, src_len);

    // step 3: Initialize inflate stream, attempt inflate, compute CRCs and write original to gz64
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    const uLong OUT_LEN = 200;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);
    inf.next_out = outBuf;
    inf.avail_out = (uInt)OUT_LEN;
    inflate(&inf, 0);

    uLong crc1 = crc32(0L, input, (uInt)40);
    uLong crc2 = crc32(0L, input + 40, (uInt)(INPUT_LEN - 40));
    uLong combined_crc = crc32_combine(crc1, crc2, (off_t)(INPUT_LEN - 40));

    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, input, (unsigned int)INPUT_LEN);
    gzclose_w(gz);

    // step 4: Cleanup
    inflateEnd(&inf);
    free(input);
    free(compBuf);
    free(outBuf);

    // API sequence test completed successfully
    (void)ver;
    (void)combined_crc;
    return 66;
}