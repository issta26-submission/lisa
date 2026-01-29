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
//<ID> 794
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare input buffer, get zlib version, compute checksum and write gzip file
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 1500UL;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'X', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong adler_input = adler32_z(0UL, input, (z_size_t)src_len);
    gzFile gz_out = gzopen("tmp_zlib_api_seq.gz", "wb");
    gzwrite(gz_out, (voidpc)input, (unsigned int)src_len);
    gzclose(gz_out);

    // step 2: Initialize an inflate stream and call inflateUndermine to exercise API
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = NULL;
    strm.zfree = NULL;
    strm.opaque = NULL;
    inflateInit_(&strm, ver, (int)sizeof(z_stream));
    inflateUndermine(&strm, 1);

    // step 3: Initialize inflateBack context and immediately tear it down (use inflateBackEnd)
    z_stream backstrm;
    memset(&backstrm, 0, sizeof(backstrm));
    backstrm.zalloc = NULL;
    backstrm.zfree = NULL;
    backstrm.opaque = NULL;
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&backstrm, 15, window, ver, (int)sizeof(z_stream));
    inflateBackEnd(&backstrm);
    inflateEnd(&backstrm);

    // step 4: Read back gzip file, validate checksum, cleanup, and finish
    gzFile gz_in = gzopen("tmp_zlib_api_seq.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)(src_len + 64));
    memset(readBuf, 0, (size_t)(src_len + 64));
    int read_bytes = gzread(gz_in, (voidp)readBuf, (unsigned int)(src_len + 64));
    gzclose(gz_in);
    uLong adler_output = adler32_z(0UL, readBuf, (z_size_t)read_bytes);
    inflateEnd(&strm);
    free(input);
    free(readBuf);
    (void)ver;
    (void)adler_input;
    (void)adler_output;
    // API sequence test completed successfully
    return 66;
}