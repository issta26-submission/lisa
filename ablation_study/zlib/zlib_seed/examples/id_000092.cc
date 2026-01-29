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
//<ID> 92
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
    const char src_c[] = "Sample payload for zlib API sequence: compress -> gzwrite -> inflate";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    Bytef *decompBuf = (Bytef *)malloc((size_t)sourceLen + 16);
    memset(decompBuf, 0, (size_t)sourceLen + 16);
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    const char *version = zlibVersion();

    // step 2: Setup (compress and open gz)
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzbuf = gzbuffer(gz, 8192);
    int rc_gzwrite = gzwrite(gz, compBuf, (unsigned int)compLen);
    off64_t pos64 = gzoffset64(gz);

    // step 3: Operate (inflate the compressed buffer and initialize/teardown inflateBack)
    infstrm.next_in = compBuf;
    infstrm.avail_in = (uInt)compLen;
    infstrm.next_out = decompBuf;
    infstrm.avail_out = (uInt)(sourceLen + 16);
    int rc_inf_init = inflateInit_(&infstrm, version, (int)sizeof(z_stream));
    int rc_inf = inflate(&infstrm, 0);
    int rc_inf_end = inflateEnd(&infstrm);
    int rc_infback_init = inflateBackInit_(&infbackstrm, 15, window, version, (int)sizeof(z_stream));
    int rc_infback_end = inflateBackEnd(&infbackstrm);

    // step 4: Validate & Cleanup
    (void)rc_comp; (void)rc_gzbuf; (void)rc_gzwrite; (void)pos64;
    (void)rc_inf_init; (void)rc_inf; (void)rc_inf_end; (void)rc_infback_init; (void)rc_infback_end;
    gzclose(gz);
    free(compBuf);
    free(decompBuf);
    free(window);
    // API sequence test completed successfully
    return 66;
}