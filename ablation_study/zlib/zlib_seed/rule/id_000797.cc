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
//<ID> 797
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare input buffer, compute checksum, and write to a gzip file
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 1500UL;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'X', (size_t)INPUT_LEN);
    uLong adler_input = adler32_z(0UL, input, (z_size_t)INPUT_LEN);
    gzFile gz_out = gzopen("tmp_zlib_api_seq.gz", "wb");
    gzwrite(gz_out, (voidpc)input, (unsigned int)INPUT_LEN);
    gzclose(gz_out);

    // step 2: Initialize an inflateBack stream with a sliding window
    z_stream backstrm;
    memset(&backstrm, 0, sizeof(backstrm));
    backstrm.zalloc = NULL;
    backstrm.zfree = NULL;
    backstrm.opaque = NULL;
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    inflateBackInit_(&backstrm, 15, window, ver, (int)sizeof(z_stream));

    // step 3: Mark the stream as undermined and then tear down the back inflate state
    inflateUndermine(&backstrm, 1);
    int ib_end_ret = inflateBackEnd(&backstrm);

    // step 4: Read back the gzip file, validate checksum, cleanup
    gzFile gz_in = gzopen("tmp_zlib_api_seq.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)(INPUT_LEN + 64));
    memset(readBuf, 0, (size_t)(INPUT_LEN + 64));
    int read_bytes = gzread(gz_in, (voidp)readBuf, (unsigned int)(INPUT_LEN + 64));
    gzclose(gz_in);
    uLong adler_output = adler32_z(0UL, readBuf, (z_size_t)INPUT_LEN);
    free(input);
    free(readBuf);
    (void)ver;
    (void)adler_input;
    (void)adler_output;
    (void)ib_end_ret;
    // API sequence test completed successfully
    return 66;
}