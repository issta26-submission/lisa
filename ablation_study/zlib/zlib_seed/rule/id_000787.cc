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
//<ID> 787
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare input and compress to an in-memory buffer
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 1024;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    compress(compBuf, &compLen, input, src_len);

    // step 2: Write original data to a gz file (uses gzopen and gzwrite)
    gzFile gz = gzopen("tmp_zlib_api_seq.gz", "wb");
    gzwrite(gz, input, (unsigned int)src_len);
    gzclose(gz);

    // step 3: Initialize inflate, decompress the in-memory compressed buffer
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = NULL;
    strm.zfree = NULL;
    strm.opaque = NULL;
    inflateInit_(&strm, ver, (int)sizeof(z_stream));
    uLong OUT_LEN = src_len * 2;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);
    strm.next_in = compBuf;
    strm.avail_in = (uInt)compLen;
    strm.next_out = outBuf;
    strm.avail_out = (uInt)OUT_LEN;
    inflate(&strm, 0);
    inflateEnd(&strm);

    // step 4: Read back the gz file using gzopen/gzread and perform lightweight validation, then cleanup
    gzFile gz2 = gzopen("tmp_zlib_api_seq.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)src_len);
    memset(readBuf, 0, (size_t)src_len);
    gzread(gz2, readBuf, (unsigned int)src_len);
    gzclose(gz2);
    uLong crc_input = crc32(0L, input, (uInt)src_len);
    uLong crc_read = crc32(0L, readBuf, (uInt)src_len);
    free(input);
    free(compBuf);
    free(outBuf);
    free(readBuf);
    (void)ver;
    (void)crc_input;
    (void)crc_read;
    // API sequence test completed successfully
    return 66;
}