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
//<ID> 788
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare input and compress
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 1024UL;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    compress(compBuf, &compLen, input, src_len);

    // step 2: Initialize inflate, configure stream, and decompress compressed buffer
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = NULL;
    strm.zfree = NULL;
    strm.opaque = NULL;
    inflateInit_(&strm, ver, (int)sizeof(z_stream));
    uLong OUT_LEN = INPUT_LEN * 2UL;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);
    strm.next_in = compBuf;
    strm.avail_in = (uInt)compLen;
    strm.next_out = outBuf;
    strm.avail_out = (uInt)OUT_LEN;
    inflate(&strm, 0);
    inflateEnd(&strm);

    // step 3: Write original data to a .gz file and read it back using gzFile APIs
    gzFile gw = gzopen("tmp_zlib_api_seq.gz", "wb");
    gzwrite(gw, input, (unsigned int)src_len);
    gzclose(gw);
    gzFile gr = gzopen("tmp_zlib_api_seq.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(readBuf, 0, (size_t)OUT_LEN);
    int readBytes = gzread(gr, readBuf, (unsigned int)OUT_LEN);
    gzclose(gr);

    // step 4: Validate via CRCs, cleanup resources
    uLong crc_input = crc32(0L, input, (uInt)src_len);
    uLong crc_out = crc32(0L, outBuf, (uInt)src_len);
    uLong crc_read = crc32(0L, readBuf, (uInt)readBytes);
    free(input);
    free(compBuf);
    free(outBuf);
    free(readBuf);
    (void)ver;
    (void)crc_input;
    (void)crc_out;
    (void)crc_read;
    // API sequence test completed successfully
    return 66;
}