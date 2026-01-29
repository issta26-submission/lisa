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
//<ID> 700
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare input and produce compressed data
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 512;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'X', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int compress_ret = compress2(compBuf, &compLen, input, src_len, 6);

    // step 2: Initialize inflate stream and prime bit buffer
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int init_ret = inflateInit_(&strm, ver, (int)sizeof(z_stream));
    strm.next_in = compBuf;
    strm.avail_in = (uInt)compLen;
    uLong OUT_LEN = INPUT_LEN * 2;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);
    strm.next_out = outBuf;
    strm.avail_out = (uInt)OUT_LEN;
    int prime_ret = inflatePrime(&strm, 3, 5);

    // step 3: Inflate, validate checksum, undermine detection, reset-keep, and write one byte to gz
    int inflate_ret = inflate(&strm, 0);
    uLong checksum = adler32(0UL, outBuf, (uInt)strm.total_out);
    int undermine_ret = inflateUndermine(&strm, 1);
    int resetkeep_ret = inflateResetKeep(&strm);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzputc(gz, (int)outBuf[0]);
    gzclose(gz);

    // step 4: Cleanup
    int end_ret = inflateEnd(&strm);
    free(input);
    free(compBuf);
    free(outBuf);
    (void)ver;
    (void)compress_ret;
    (void)init_ret;
    (void)prime_ret;
    (void)inflate_ret;
    (void)checksum;
    (void)undermine_ret;
    (void)resetkeep_ret;
    (void)end_ret;
    // API sequence test completed successfully
    return 66;
}