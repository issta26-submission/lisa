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
//<ID> 680
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 1024;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'Z', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong adler = adler32_z(0L, input, (z_size_t)INPUT_LEN);
    uLong compBound = compressBound(src_len);
    uLongf compLen = (uLongf)compBound;
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    compress2(compBuf, &compLen, input, src_len, 6);

    // step 2: Initialize inflate stream and prepare output buffer
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    Bytef *decompBuf = (Bytef *)malloc((size_t)(INPUT_LEN * 2));
    memset(decompBuf, 0, (size_t)(INPUT_LEN * 2));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    inf.next_out = decompBuf;
    inf.avail_out = (uInt)(INPUT_LEN * 2);
    inflateInit_(&inf, ver, (int)sizeof(z_stream));

    // step 3: Operate: inflate, attempt sync (resync if needed), capture output size
    inflate(&inf, 0);
    inflateSync(&inf);
    uLong decompressed = inf.total_out;

    // step 4: Validate by writing decompressed data to a gzip file, cleanup resources
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, decompBuf, (unsigned int)decompressed);
    gzclose(gz);
    inflateEnd(&inf);
    free(input);
    free(compBuf);
    free(decompBuf);
    (void)ver;
    (void)adler;
    (void)compLen;
    (void)src_len;
    // API sequence test completed successfully
    return 66;
}