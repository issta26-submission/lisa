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
//<ID> 770
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare input and compress it
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

    // step 2: Write original data into a gzip file and close it
    gzFile gz = gzopen("tmp_zlib_api_seq.gz", "wb");
    gzwrite(gz, input, (unsigned int)src_len);
    gzclose(gz);

    // step 3: Inflate compressed buffer and compute Adler-32 of output
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
    uLong out_adler = adler32_z(0UL, outBuf, (z_size_t)src_len);

    // step 4: Initialize a back-inflate stream and tear it down, then cleanup
    z_stream backStrm;
    memset(&backStrm, 0, sizeof(backStrm));
    backStrm.zalloc = NULL;
    backStrm.zfree = NULL;
    backStrm.opaque = NULL;
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&backStrm, 15, window, ver, (int)sizeof(z_stream));
    inflateBackEnd(&backStrm);
    inflateEnd(&inf);

    free(input);
    free(compBuf);
    free(outBuf);
    (void)ver;
    (void)compBound;
    (void)out_adler;
    // API sequence test completed successfully
    return 66;
}