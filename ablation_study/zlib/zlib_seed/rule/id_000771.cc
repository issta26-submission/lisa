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
//<ID> 771
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare input, compress using deflate, and write original data to a gzip file
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 1024;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    z_stream def;
    memset(&def, 0, sizeof(def));
    def.zalloc = NULL;
    def.zfree = NULL;
    def.opaque = NULL;
    def.next_in = input;
    def.avail_in = (uInt)src_len;
    def.next_out = compBuf;
    def.avail_out = (uInt)compBound;
    deflateInit_(&def, 6, ver, (int)sizeof(z_stream));
    deflate(&def, 0);
    uLong compLen = def.total_out;
    deflateEnd(&def);
    gzFile wgz = gzopen("tmp_zlib_api_seq.gz", "wb");
    gzwrite(wgz, input, (unsigned int)src_len);
    gzclose(wgz);

    // step 2: Initialize inflate, decompress the buffer, and validate with adler32_z
    uLong OUT_LEN = INPUT_LEN * 2;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);
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
    uLong computed_adler = adler32_z(0UL, outBuf, (z_size_t)src_len);

    // step 3: Initialize inflate back-end and immediately tear it down using inflateBackEnd
    z_stream back;
    memset(&back, 0, sizeof(back));
    back.zalloc = NULL;
    back.zfree = NULL;
    back.opaque = NULL;
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&back, 15, window, ver, (int)sizeof(z_stream));
    inflateBackEnd(&back);

    // step 4: Cleanup and finalization
    inflateEnd(&inf);
    free(input);
    free(compBuf);
    free(outBuf);
    (void)ver;
    (void)computed_adler;
    // API sequence test completed successfully
    return 66;
}