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
//<ID> 94
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream infstrm;
    z_stream infbackstrm;
    memset(&infstrm, 0, sizeof(z_stream));
    memset(&infbackstrm, 0, sizeof(z_stream));
    const char src_c[] = "Zlib API sequence payload: compress, write gz, inspect offset, inflate cleanup";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    uLongf compLen = (uLongf)bound;
    Bytef *decompBuf = (Bytef *)malloc((size_t)(sourceLen + 16));
    memset(compBuf, 0, (size_t)bound);
    memset(decompBuf, 0, (size_t)(sourceLen + 16));
    const char *version = zlibVersion();

    // step 2: Setup (initialize inflate and inflateBack contexts and window)
    infstrm.next_in = compBuf;
    infstrm.avail_in = 0;
    infstrm.next_out = decompBuf;
    infstrm.avail_out = (uInt)(sourceLen + 16);
    int rc_inf_init = inflateInit_(&infstrm, version, (int)sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_infback_init = inflateBackInit_(&infbackstrm, 15, window, version, (int)sizeof(z_stream));

    // step 3: Operate (compress source, write compressed data to gz, get offset, perform inflate)
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    infstrm.next_in = compBuf;
    infstrm.avail_in = (uInt)compLen;
    int rc_inflate = inflate(&infstrm, 0);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_buf = gzbuffer(gz, 8192);
    int rc_gz_write = gzwrite(gz, compBuf, (unsigned int)compLen);
    off64_t pos64 = gzoffset64(gz);
    int rc_gz_close = gzclose(gz);
    int rc_infback_end = inflateBackEnd(&infbackstrm);

    // step 4: Validate & Cleanup
    (void)rc_inf_init; (void)rc_infback_init; (void)rc_comp; (void)rc_inflate; (void)rc_buf; (void)rc_gz_write;
    (void)pos64; (void)rc_gz_close; (void)rc_infback_end;
    inflateEnd(&infstrm);
    free(compBuf);
    free(decompBuf);
    free(window);
    // API sequence test completed successfully
    return 66;
}