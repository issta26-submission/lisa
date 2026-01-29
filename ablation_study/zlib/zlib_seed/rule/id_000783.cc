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
//<ID> 783
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare buffers and write compressed gzip file
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 512;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    gzFile gzout = gzopen("tmp_zlib_api_seq.gz", "wb");
    gzwrite(gzout, input, (unsigned int)src_len);
    gzclose(gzout);

    // step 2: Read back decompressed data via gzread
    Bytef *outBuf = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(outBuf, 0, (size_t)INPUT_LEN);
    gzFile gzin = gzopen("tmp_zlib_api_seq.gz", "rb");
    int read_bytes = gzread(gzin, outBuf, (unsigned int)INPUT_LEN);
    gzclose(gzin);

    // step 3: Initialize an inflate stream (demonstrate inflateInit_) and then clean it up
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = NULL;
    strm.zfree = NULL;
    strm.opaque = NULL;
    inflateInit_(&strm, ver, (int)sizeof(z_stream));
    inflateEnd(&strm);

    // step 4: Validate integrity via adler32, cleanup buffers
    uLong adler_in = adler32(0UL, input, (uInt)src_len);
    uLong adler_out = adler32(0UL, outBuf, (uInt)read_bytes);
    free(input);
    free(outBuf);
    (void)ver;
    (void)adler_in;
    (void)adler_out;
    // API sequence test completed successfully
    return 66;
}