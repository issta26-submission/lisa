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
//<ID> 604
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const unsigned int SRC_LEN = 128;
    Bytef *src = (Bytef *)malloc((size_t)SRC_LEN);
    memset(src, 'X', (size_t)SRC_LEN);
    uLong srcLen = (uLong)SRC_LEN;
    uLong bound = compressBound(srcLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    z_stream inf;
    memset(&inf, 0, (int)sizeof(inf));
    Bytef *dec = (Bytef *)malloc((size_t)srcLen);
    memset(dec, 0, (size_t)srcLen);

    // step 2: Setup / Configure compression and inflater
    compress(comp, &compLen, src, srcLen);
    inflateInit_(&inf, ver, (int)sizeof(z_stream));
    inf.next_in = comp;
    inf.avail_in = (uInt)compLen;
    inf.next_out = dec;
    inf.avail_out = (uInt)srcLen;

    // step 3: Core operations (inflate, checksum combine, write gz with 64-bit API)
    inflate(&inf, 0);
    uLong crc1 = crc32(0L, src, (uInt)srcLen);
    uLong crc2 = crc32(0L, dec, (uInt)srcLen);
    uLong combined = crc32_combine(crc1, crc2, (off_t)srcLen);
    (void)combined;
    gzFile gz = gzopen64("test_zlib_api_sequence.gz", "wb");
    gzwrite(gz, comp, (unsigned int)compLen);
    gzclose_w(gz);

    // step 4: Validate / Cleanup
    inflateEnd(&inf);
    free(src);
    free(comp);
    free(dec);
    // API sequence test completed successfully
    return 66;
}