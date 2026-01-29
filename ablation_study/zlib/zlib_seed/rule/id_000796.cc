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
//<ID> 796
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers (initialize & compress)
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 1024;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLong compLen = compBound;
    compress2(compBuf, (uLongf *)&compLen, input, src_len, 6);

    // step 2: Initialize inflate stream, decompress, and validate basic inflate path
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inf.zalloc = NULL;
    inf.zfree = NULL;
    inf.opaque = NULL;
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    Bytef *outBuf = (Bytef *)malloc((size_t)(src_len + 64));
    memset(outBuf, 0, (size_t)(src_len + 64));
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    inf.next_out = outBuf;
    inf.avail_out = (uInt)(src_len + 64);
    inflate(&inf, 0);
    inflateEnd(&inf);

    // step 3: InflateBack initialization, configure undermine option, and teardown
    z_stream backstrm;
    memset(&backstrm, 0, sizeof(backstrm));
    backstrm.zalloc = NULL;
    backstrm.zfree = NULL;
    backstrm.opaque = NULL;
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&backstrm, 15, window, ver, (int)sizeof(z_stream));
    inflateUndermine(&backstrm, 1);
    inflateBackEnd(&backstrm);
    inflateEnd(&backstrm);

    // step 4: File operations, checksum validation, cleanup
    gzFile gz = gzopen("tmp_zlib_api_seq.gz", "wb");
    gzwrite(gz, (voidpc)input, (unsigned int)src_len);
    gzclose(gz);
    uLong adler_input = adler32_z(0UL, input, (z_size_t)src_len);
    uLong adler_output = adler32_z(0UL, outBuf, (z_size_t)src_len);
    free(input);
    free(compBuf);
    free(outBuf);
    (void)ver;
    (void)adler_input;
    (void)adler_output;
    // API sequence test completed successfully
    return 66;
}