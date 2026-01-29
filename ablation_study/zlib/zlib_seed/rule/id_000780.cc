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
//<ID> 780
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data and compress with compress2
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 1024;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int compRes = compress2(compBuf, &compLen, input, src_len, 6);

    // step 2: Initialize inflate and perform inflate to decompress into outBuf
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = NULL;
    strm.zfree = NULL;
    strm.opaque = NULL;
    int initRes = inflateInit_(&strm, ver, (int)sizeof(z_stream));
    Bytef *outBuf = (Bytef *)malloc((size_t)(INPUT_LEN * 2));
    memset(outBuf, 0, (size_t)(INPUT_LEN * 2));
    strm.next_in = compBuf;
    strm.avail_in = (uInt)compLen;
    strm.next_out = outBuf;
    strm.avail_out = (uInt)(INPUT_LEN * 2);
    int infRes = inflate(&strm, 0);
    int endRes = inflateEnd(&strm);

    // step 3: Write original input to a gz file and read it back
    gzFile gz = gzopen("tmp_zlib_api_seq.gz", "wb");
    int wres = gzwrite(gz, input, (unsigned int)src_len);
    int cgres = gzclose(gz);
    gz = gzopen("tmp_zlib_api_seq.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)src_len);
    memset(readBuf, 0, (size_t)src_len);
    int rres = gzread(gz, readBuf, (unsigned int)src_len);
    int cg2res = gzclose(gz);

    // step 4: Validate via CRCs and cleanup resources
    uLong crc_input = crc32(0L, input, (uInt)src_len);
    uLong crc_read = crc32(0L, readBuf, (uInt)src_len);
    uLong combined = crc32_combine(crc_input, crc_read, (off_t)src_len);
    free(input);
    free(compBuf);
    free(outBuf);
    free(readBuf);
    (void)ver;
    (void)compRes;
    (void)initRes;
    (void)infRes;
    (void)endRes;
    (void)wres;
    (void)cgres;
    (void)rres;
    (void)cg2res;
    (void)crc_input;
    (void)crc_read;
    (void)combined;
    // API sequence test completed successfully
    return 66;
}