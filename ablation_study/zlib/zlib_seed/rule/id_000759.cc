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
//<ID> 759
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
    uLong OUT_LEN = INPUT_LEN * 2;
    Bytef *outBuf = (Bytef *)malloc((size_t)OUT_LEN);
    memset(outBuf, 0, (size_t)OUT_LEN);

    // step 2: Initialize and run deflate to produce compressed data
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

    // step 3: Write original data into a gzip file and read it back using gzread
    gzFile wgz = gzopen("tmp_zlib_api_seq.gz", "wb");
    gzwrite(wgz, input, (unsigned int)src_len);
    gzclose(wgz);
    gzFile rgz = gzopen("tmp_zlib_api_seq.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)src_len);
    memset(readBuf, 0, (size_t)src_len);
    int readLen = gzread(rgz, readBuf, (unsigned int)src_len);
    gzclose(rgz);

    // step 4: Initialize inflate, adjust window bits, inflate compressed buffer, retrieve dictionary, validate and cleanup
    z_stream inf;
    memset(&inf, 0, sizeof(inf));
    inf.zalloc = NULL;
    inf.zfree = NULL;
    inf.opaque = NULL;
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inflateReset2(&inf, 15);
    inf.next_in = compBuf;
    inf.avail_in = (uInt)compLen;
    inf.next_out = outBuf;
    inf.avail_out = (uInt)OUT_LEN;
    inflate(&inf, 0);
    Bytef dictBuf[64];
    memset(dictBuf, 0, sizeof(dictBuf));
    uInt dictLen = (uInt)sizeof(dictBuf);
    inflateGetDictionary(&inf, dictBuf, &dictLen);
    uLong crc_out = crc32(0L, outBuf, (uInt)src_len);
    uLong crc_read = crc32(0L, readBuf, (uInt)src_len);
    uLong combined_crc = crc32_combine(crc_out, crc_read, (off_t)src_len);
    inflateEnd(&inf);

    free(input);
    free(compBuf);
    free(outBuf);
    free(readBuf);
    (void)ver;
    (void)readLen;
    (void)dictLen;
    (void)combined_crc;
    // API sequence test completed successfully
    return 66;
}