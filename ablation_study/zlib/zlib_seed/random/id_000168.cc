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
//<ID> 168
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize buffers and source
    const char src_c[] = "zlib payload for roundtrip compression and CRC";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c)); /* include terminating NUL */
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    uLongf destLen = (uLongf)sourceLen;
    Bytef *destBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(destBuf, 0, (size_t)sourceLen);

    // step 2: Configure & Compress
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 3: Operate: Uncompress -> CRC -> write gz
    int rc_uncomp = uncompress(destBuf, &destLen, compBuf, compLen);
    uLong crc = crc32_z(0UL, destBuf, (z_size_t)destLen);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_puts = gzputs(gz, (const char *)destBuf);
    int rc_gz_close = gzclose(gz);

    // step 4: Validate & Cleanup
    z_stream backstrm;
    memset(&backstrm, 0, sizeof(z_stream));
    unsigned char window[32768];
    memset(window, 0, sizeof(window));
    int rc_inflateback_init = inflateBackInit_(&backstrm, 15, window, version, (int)sizeof(z_stream));
    int rc_inflateback_end = inflateBackEnd(&backstrm);
    free(compBuf);
    free(destBuf);
    (void)rc_comp; (void)rc_uncomp; (void)crc; (void)rc_gz_puts; (void)rc_gz_close;
    (void)rc_inflateback_init; (void)rc_inflateback_end;
    // API sequence test completed successfully
    return 66;
}