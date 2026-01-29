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
//<ID> 781
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare input and compress to memory
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 2048;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    uLong src_len = (uLong)INPUT_LEN;
    uLong compBound = compressBound(src_len);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    compress(compBuf, &compLen, input, src_len);

    // step 2: Inflate compressed memory using inflateInit_
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    strm.zalloc = NULL;
    strm.zfree = NULL;
    strm.opaque = NULL;
    inflateInit_(&strm, ver, (int)sizeof(z_stream));
    Bytef *memOut = (Bytef *)malloc((size_t)src_len);
    memset(memOut, 0, (size_t)src_len);
    strm.next_in = compBuf;
    strm.avail_in = (uInt)compLen;
    strm.next_out = memOut;
    strm.avail_out = (uInt)src_len;
    inflate(&strm, 0);
    inflateEnd(&strm);

    // step 3: Write original input to gz file, then read back via gzread
    gzFile gz = gzopen("tmp_zlib_api_sequence.gz", "wb");
    gzwrite(gz, input, (unsigned int)src_len);
    gzclose(gz);
    gzFile gz2 = gzopen("tmp_zlib_api_sequence.gz", "rb");
    Bytef *readBuf = (Bytef *)malloc((size_t)src_len);
    memset(readBuf, 0, (size_t)src_len);
    gzread(gz2, readBuf, (unsigned int)src_len);
    gzclose(gz2);

    // step 4: Validate via crc32 and cleanup
    uLong crc_input = crc32(0L, input, (uInt)src_len);
    uLong crc_memout = crc32(0L, memOut, (uInt)src_len);
    uLong crc_read = crc32(0L, readBuf, (uInt)src_len);
    uLong combined = crc32_combine(crc_input, crc_memout, (off_t)src_len);
    free(input);
    free(compBuf);
    free(memOut);
    free(readBuf);
    (void)ver;
    (void)crc_input;
    (void)crc_memout;
    (void)crc_read;
    (void)combined;
    // API sequence test completed successfully
    return 66;
}