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
//<ID> 102
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream infstrm;
    z_stream copystrm;
    memset(&infstrm, 0, sizeof(z_stream));
    memset(&copystrm, 0, sizeof(z_stream));
    const char src_c[] = "Hello zlib API gzwrite gzputc inflateCopy test";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    const char *version = zlibVersion();

    // step 2: Setup (compress data, open gz file, initialize inflate)
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write1 = gzwrite(gz, compBuf, (unsigned int)compLen);
    int rc_gz_putc1 = gzputc(gz, '\n');
    int rc_inf_init = inflateInit_(&infstrm, version, (int)sizeof(z_stream));
    Bytef *outBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf, 0, (size_t)sourceLen);
    infstrm.next_in = compBuf;
    infstrm.avail_in = (uInt)compLen;
    infstrm.next_out = outBuf;
    infstrm.avail_out = (uInt)sourceLen;

    // step 3: Core operations (copy inflate state, inflate, write decompressed to gz)
    int rc_copy = inflateCopy(&copystrm, &infstrm);
    int rc_gz_putc2 = gzputc(gz, 'A');
    int rc_inf = inflate(&infstrm, 0);
    unsigned int wrote_decomp = (unsigned int)(infstrm.total_out & 0xFFFFFFFFUL);
    int rc_gz_write2 = gzwrite(gz, outBuf, wrote_decomp);

    // step 4: Cleanup & finalize
    (void)rc_comp; (void)rc_gz_write1; (void)rc_gz_putc1; (void)rc_inf_init; (void)rc_copy; (void)rc_gz_putc2; (void)rc_inf; (void)rc_gz_write2;
    gzclose(gz);
    inflateEnd(&infstrm);
    inflateEnd(&copystrm);
    free(compBuf);
    free(outBuf);
    // API sequence test completed successfully
    return 66;
}