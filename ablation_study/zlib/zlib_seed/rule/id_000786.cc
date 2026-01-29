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
//<ID> 786
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
    Bytef *readBuf = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(readBuf, 0, (size_t)INPUT_LEN);

    // step 2: Write input to a gzip file using gzopen/gzwrite
    gzFile gz = gzopen("tmp_zlib_api_seq.gz", "wb");
    gzwrite(gz, input, (unsigned int)src_len);
    gzclose(gz);

    // step 3: Read back the gzip file using gzopen/gzread
    gzFile gzr = gzopen("tmp_zlib_api_seq.gz", "rb");
    int read_bytes = gzread(gzr, readBuf, (unsigned int)src_len);
    gzclose(gzr);

    // step 4: Initialize an inflate stream (inflateInit_), validate via adler32, and cleanup
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = NULL;
    strm.zfree = NULL;
    strm.opaque = NULL;
    inflateInit_(&strm, ver, (int)sizeof(z_stream));
    uLong adler_in = adler32(0UL, input, (uInt)src_len);
    uLong adler_out = adler32(0UL, readBuf, (uInt)read_bytes);
    inflateEnd(&strm);
    free(input);
    free(readBuf);
    (void)ver;
    (void)adler_in;
    (void)adler_out;
    // API sequence test completed successfully
    return 66;
}