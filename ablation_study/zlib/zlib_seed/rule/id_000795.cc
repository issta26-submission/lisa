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
//<ID> 795
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 1024UL;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong adler_input = adler32_z(0UL, input, (z_size_t)src_len);

    // step 2: Write input to a gzip file using gzopen/gzwrite and close
    gzFile gz_out = gzopen("tmp_zlib_api_seq.gz", "wb");
    gzwrite(gz_out, (voidpc)input, (unsigned int)src_len);
    gzclose(gz_out);

    // step 3: Read back the gzip file, validate via adler32_z, and use inflate APIs
    gzFile gz_in = gzopen("tmp_zlib_api_seq.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)(src_len + 128));
    memset(readBuf, 0, (size_t)(src_len + 128));
    int read_bytes = gzread(gz_in, (voidp)readBuf, (unsigned int)(src_len + 128));
    gzclose(gz_in);
    uLong adler_output = adler32_z(0UL, readBuf, (z_size_t)read_bytes);

    // step 4: Initialize inflate stream, call inflateUndermine, initialize inflateBack and cleanup
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = NULL;
    strm.zfree = NULL;
    strm.opaque = NULL;
    inflateInit_(&strm, ver, (int)sizeof(z_stream));
    inflateUndermine(&strm, 1);
    inflateEnd(&strm);

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

    free(input);
    free(readBuf);
    (void)adler_input;
    (void)adler_output;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}