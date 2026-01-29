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
//<ID> 799
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 2048;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    gzFile gz_out = gzopen("tmp_zlib_api_seq.gz", "wb");
    gzwrite(gz_out, (voidpc)input, (unsigned int)INPUT_LEN);
    gzclose(gz_out);

    // step 2: Initialize inflateBack stream
    z_stream backstrm;
    memset(&backstrm, 0, sizeof(backstrm));
    backstrm.zalloc = NULL;
    backstrm.zfree = NULL;
    backstrm.opaque = NULL;
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&backstrm, 15, window, ver, (int)sizeof(z_stream));

    // step 3: Use inflateUndermine, read back gzip file, compute checksums
    inflateUndermine(&backstrm, 1);
    gzFile gz_in = gzopen("tmp_zlib_api_seq.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)(INPUT_LEN + 128));
    memset(readBuf, 0, (size_t)(INPUT_LEN + 128));
    int read_bytes = gzread(gz_in, (voidp)readBuf, (unsigned int)(INPUT_LEN + 128));
    uLong adler_input = adler32_z(0UL, input, (z_size_t)INPUT_LEN);
    uLong adler_output = adler32_z(0UL, readBuf, (z_size_t)read_bytes);
    gzclose(gz_in);

    // step 4: Cleanup inflateBack and free resources
    inflateBackEnd(&backstrm);
    free(input);
    free(readBuf);
    (void)ver;
    (void)adler_input;
    (void)adler_output;
    // API sequence test completed successfully
    return 66;
}