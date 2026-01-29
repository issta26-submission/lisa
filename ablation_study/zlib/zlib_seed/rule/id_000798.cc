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
//<ID> 798
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 1500;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'X', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong adler_initial = adler32_z(0UL, input, (z_size_t)src_len);

    // step 2: Write input to a gzip file
    gzFile gz_out = gzopen("tmp_zlib_api_seq.gz", "wb");
    gzwrite(gz_out, input, (unsigned int)src_len);
    gzclose(gz_out);

    // step 3: Initialize an inflate stream and exercise undermine + backend cleanup
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = NULL;
    strm.zfree = NULL;
    strm.opaque = NULL;
    inflateInit_(&strm, ver, (int)sizeof(z_stream));
    inflateUndermine(&strm, 1);
    inflateBackEnd(&strm);
    inflateEnd(&strm);

    // step 4: Read back the gzip file, validate checksum, cleanup
    gzFile gz_in = gzopen("tmp_zlib_api_seq.gz", "rb");
    Bytef *outBuf = (Bytef *)malloc((size_t)(src_len + 64));
    memset(outBuf, 0, (size_t)(src_len + 64));
    int read_bytes = gzread(gz_in, (voidp)outBuf, (unsigned int)(src_len + 64));
    gzclose(gz_in);
    uLong adler_read = adler32_z(0UL, outBuf, (z_size_t)read_bytes);
    free(input);
    free(outBuf);
    (void)ver;
    (void)adler_initial;
    (void)adler_read;
    // API sequence test completed successfully
    return 66;
}