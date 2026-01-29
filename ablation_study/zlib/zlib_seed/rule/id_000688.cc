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
//<ID> 688
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 512;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'Z', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    compress2(compBuf, &compLen, input, src_len, 6);

    // step 2: Initialize inflate stream and operate
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    Bytef *outBuf = (Bytef *)malloc((size_t)src_len);
    memset(outBuf, 0, (size_t)src_len);
    inf.next_out = outBuf;
    inf.avail_out = (uInt)src_len;
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inflateSync(&inf);
    inflate(&inf, 0);
    inflateEnd(&inf);

    // step 3: Validate with adler32_z and write output via gz
    uLong checksum = adler32_z(0UL, outBuf, (z_size_t)src_len);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, outBuf, (unsigned int)src_len);
    gzclose(gz);

    // step 4: Cleanup
    free(input);
    free(compBuf);
    free(outBuf);
    (void)ver;
    (void)compBound;
    (void)compLen;
    (void)checksum;
    // API sequence test completed successfully
    return 66;
}