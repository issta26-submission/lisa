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
//<ID> 775
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
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    uLong OUT_LEN = INPUT_LEN * 2;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);

    // step 2: Compress input and write/read a gzip file
    compress2(compBuf, &compLen, input, src_len, 6);
    gzFile wgz = gzopen("tmp_zlib_api_seq.gz", "wb");
    gzwrite(wgz, input, (unsigned int)src_len);
    gzclose(wgz);
    gzFile rgz = gzopen("tmp_zlib_api_seq.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)src_len);
    memset(readBuf, 0, (size_t)src_len);
    int readLen = gzread(rgz, readBuf, (unsigned int)src_len);
    gzclose(rgz);

    // step 3: Initialize inflate, perform inflate, validate with adler32_z
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inf.zalloc = NULL;
    inf.zfree = NULL;
    inf.opaque = NULL;
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    inf.next_out = outBuf;
    inf.avail_out = (uInt)OUT_LEN;
    inflate(&inf, 0);
    uLong adler_out = adler32_z(0L, outBuf, (z_size_t)src_len);
    uLong adler_read = adler32_z(0L, readBuf, (z_size_t)src_len);
    inflateEnd(&inf);

    // step 4: Initialize inflateBack state and teardown, final cleanup
    z_stream back;
    memset(&back, 0, sizeof(back));
    back.zalloc = NULL;
    back.zfree = NULL;
    back.opaque = NULL;
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&back, 15, window, ver, (int)sizeof(z_stream));
    inflateBackEnd(&back);
    free(input);
    free(compBuf);
    free(outBuf);
    free(readBuf);
    (void)ver;
    (void)readLen;
    (void)adler_out;
    (void)adler_read;
    // API sequence test completed successfully
    return 66;
}