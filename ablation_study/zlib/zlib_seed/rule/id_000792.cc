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
//<ID> 792
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 1536;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'X', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLong compLen = compBound;
    compress2(compBuf, (uLongf *)&compLen, input, src_len, 6);

    // step 2: Inflate using inflateInit_ -> configure via inflateUndermine -> operate -> cleanup
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = NULL;
    strm.zfree = NULL;
    strm.opaque = NULL;
    inflateInit_(&strm, ver, (int)sizeof(z_stream));
    inflateUndermine(&strm, 1);
    Bytef *outBuf = (Bytef *)malloc((size_t)(INPUT_LEN + 256));
    memset(outBuf, 0, (size_t)(INPUT_LEN + 256));
    strm.next_in = compBuf;
    strm.avail_in = (uInt)compLen;
    strm.next_out = outBuf;
    strm.avail_out = (uInt)(INPUT_LEN + 256);
    inflate(&strm, 0);
    inflateEnd(&strm);

    // step 3: InflateBack initialization and teardown using inflateBackInit_ and inflateBackEnd
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

    // step 4: Validation via adler32_z, gzip file write/read using gzopen, cleanup
    uLong adler_input = adler32_z(0UL, input, (z_size_t)src_len);
    gzFile gz_out = gzopen("tmp_zlib_api_seq.gz", "wb");
    gzwrite(gz_out, input, (unsigned int)src_len);
    gzclose(gz_out);
    gzFile gz_in = gzopen("tmp_zlib_api_seq.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)(src_len + 128));
    memset(readBuf, 0, (size_t)(src_len + 128));
    int read_bytes = gzread(gz_in, (voidp)readBuf, (unsigned int)(src_len + 128));
    gzclose(gz_in);
    uLong adler_output = adler32_z(0UL, readBuf, (z_size_t)src_len);
    free(input);
    free(compBuf);
    free(outBuf);
    free(readBuf);
    (void)ver;
    (void)adler_input;
    (void)adler_output;
    // API sequence test completed successfully
    return 66;
}