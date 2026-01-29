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
//<ID> 779
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 2048;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'Z', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLong compLen = compBound;
    compress(compBuf, (uLongf *)&compLen, input, src_len);

    // step 2: Inflate using inflate API (initialize -> configure -> operate)
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inf.zalloc = NULL;
    inf.zfree = NULL;
    inf.opaque = NULL;
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    Bytef *outBuf = (Bytef *)malloc((size_t)(INPUT_LEN * 2));
    memset(outBuf, 0, (size_t)(INPUT_LEN * 2));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    inf.next_out = outBuf;
    inf.avail_out = (uInt)(INPUT_LEN * 2);
    inflate(&inf, 0);
    inflateEnd(&inf);

    // step 3: InflateBack path with callbacks and file write/read using gzFile
    z_stream backstrm;
    memset(&backstrm, 0, sizeof(backstrm));
    backstrm.zalloc = NULL;
    backstrm.zfree = NULL;
    backstrm.opaque = NULL;
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&backstrm, 15, window, ver, (int)sizeof(z_stream));
    struct InDesc { const unsigned char *data; uInt len; uInt pos; };
    struct OutDesc { unsigned char *buf; uInt len; uInt pos; };
    InDesc in_desc = { compBuf, (uInt)compLen, 0u };
    OutDesc out_desc = { outBuf, (uInt)(INPUT_LEN * 2), 0u };
    auto in_cb = [](void *d, unsigned char **buf) -> unsigned int {
        InDesc *desc = (InDesc *)d;
        unsigned int remaining = desc->len - desc->pos;
        *buf = (unsigned char *)(desc->data + desc->pos);
        desc->pos = desc->len;
        return remaining;
    };
    auto out_cb = [](void *d, unsigned char *buf, unsigned int len) -> int {
        OutDesc *od = (OutDesc *)d;
        memcpy(od->buf + od->pos, buf, (size_t)len);
        od->pos += len;
        return 0;
    };
    inflateBack(&backstrm, (in_func)in_cb, (void *)&in_desc, (out_func)out_cb, (void *)&out_desc);

    // step 4: Validation via adler32_z, file operations, teardown and cleanup
    uLong adler_input = adler32_z(0UL, input, (z_size_t)src_len);
    uLong adler_out = adler32_z(0UL, outBuf, (z_size_t)src_len);
    gzFile gz = gzopen("tmp_zlib_api_seq.gz", "wb");
    gzwrite(gz, input, (unsigned int)src_len);
    gzclose(gz);
    inflateBackEnd(&backstrm);
    inflateEnd(&backstrm);
    free(input);
    free(compBuf);
    free(outBuf);
    (void)ver;
    (void)adler_input;
    (void)adler_out;
    // API sequence test completed successfully
    return 66;
}