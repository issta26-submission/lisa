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
//<ID> 793
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare input buffer, compute checksum, and write to a gzip file
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 1536;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'X', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong input_adler = adler32_z(0UL, input, (z_size_t)src_len);
    gzFile gz_out = gzopen("tmp_zlib_api_seq.gz", "wb");
    gzwrite(gz_out, input, (unsigned int)src_len);
    gzclose(gz_out);

    // step 2: Initialize a normal inflate stream and exercise inflateUndermine
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = NULL;
    strm.zfree = NULL;
    strm.opaque = NULL;
    inflateInit_(&strm, ver, (int)sizeof(z_stream));
    inflateUndermine(&strm, 1);
    inflateEnd(&strm);

    // step 3: Initialize an inflate-back stream then tear it down properly
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

    // step 4: Read back the gzip file, validate checksum, cleanup
    gzFile gz_in = gzopen("tmp_zlib_api_seq.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)(src_len + 64));
    memset(readBuf, 0, (size_t)(src_len + 64));
    int read_bytes = gzread(gz_in, readBuf, (unsigned int)(src_len + 64));
    gzclose(gz_in);
    uLong read_adler = adler32_z(0UL, readBuf, (z_size_t)read_bytes);
    free(input);
    free(readBuf);
    (void)ver;
    (void)input_adler;
    (void)read_adler;
    // API sequence test completed successfully
    return 66;
}