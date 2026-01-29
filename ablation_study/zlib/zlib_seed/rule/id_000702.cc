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
//<ID> 702
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare input and compress to produce a compressed buffer
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 1024;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'Z', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    compress2(compBuf, &compLen, input, src_len, 6);

    // step 2: Initialize inflate stream and apply reset/prime operations
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    inflateInit_(&strm, ver, (int)sizeof(z_stream));
    strm.next_in = compBuf;
    strm.avail_in = (uInt)compLen;
    uLong OUT_LEN = INPUT_LEN * 2;
    Bytef *out = (Bytef *)malloc((size_t)OUT_LEN);
    memset(out, 0, (size_t)OUT_LEN);
    strm.next_out = out;
    strm.avail_out = (uInt)OUT_LEN;
    inflateResetKeep(&strm);
    inflatePrime(&strm, 1, 1);

    // step 3: Inflate, perform undermine, compute checksum and write one byte to gz
    inflate(&strm, 0);
    inflateUndermine(&strm, 1);
    uLong checksum = adler32(0UL, out, (uInt)strm.total_out);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gz, (int)out[0]);

    // step 4: Cleanup
    gzclose(gz);
    inflateEnd(&strm);
    free(input);
    free(compBuf);
    free(out);
    (void)ver;
    (void)checksum;
    // API sequence test completed successfully
    return 66;
}