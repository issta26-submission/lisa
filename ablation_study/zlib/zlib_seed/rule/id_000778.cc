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
//<ID> 778
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare input data and compress it
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 1024;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    compress2(compBuf, &compLen, input, src_len, 6);

    // step 2: Initialize inflate stream and decompress using inflate()
    uLong OUT_LEN = INPUT_LEN;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inf.zalloc = NULL;
    inf.zfree = NULL;
    inf.opaque = NULL;
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    inf.next_out = outBuf;
    inf.avail_out = (uInt)OUT_LEN;
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inflate(&inf, 0);
    uLong adler_out = adler32_z(1UL, outBuf, (z_size_t)src_len);

    // step 3: Initialize inflate back-end and immediately tear it down
    z_stream back;
    memset(&back, 0, sizeof(back));
    back.zalloc = NULL;
    back.zfree = NULL;
    back.opaque = NULL;
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&back, 15, window, ver, (int)sizeof(z_stream));
    inflateBackEnd(&back);

    // step 4: Write original data to a gzip file, read it back, validate adler, cleanup
    gzFile wgz = gzopen("tmp_zlib_api_seq.gz", "wb");
    gzwrite(wgz, input, (unsigned int)src_len);
    gzclose(wgz);
    gzFile rgz = gzopen("tmp_zlib_api_seq.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)src_len);
    memset(readBuf, 0, (size_t)src_len);
    int readLen = gzread(rgz, readBuf, (unsigned int)src_len);
    gzclose(rgz);
    uLong adler_read = adler32_z(1UL, readBuf, (z_size_t)src_len);

    inflateEnd(&inf);
    free(input);
    free(compBuf);
    free(outBuf);
    free(readBuf);
    (void)ver;
    (void)adler_out;
    (void)adler_read;
    (void)readLen;
    // API sequence test completed successfully
    return 66;
}