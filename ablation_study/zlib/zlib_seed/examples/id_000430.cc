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
//<ID> 430
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    const char src[] = "zlib API sequence payload: prepare -> compress -> inflateInit2_ -> inflate -> gzprintf -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init2 = inflateInit2_(&strm, 15, version, (int)sizeof(z_stream));
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    uLong outBound = sourceLen + 64;
    Bytef *dest = (Bytef *)malloc((size_t)outBound);
    memset(dest, 0, (size_t)outBound);
    strm.next_out = dest;
    strm.avail_out = (uInt)outBound;

    int rc_inflate = inflate(&strm, 0);
    int rc_inf_end = inflateEnd(&strm);
    uLong outProduced = strm.total_out;
    uLong ad_source = adler32(0L, source, (uInt)sourceLen);
    uLong ad_dest = adler32(0L, dest, (uInt)outProduced);
    uLong flags = zlibCompileFlags();
    char msg[128];
    sprintf(msg, "zlib flags=%lu ad_src=%lu ad_out=%lu\n", flags, ad_source, ad_dest);
    gzFile gz = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int rc_gzprintf = gzprintf(gz, msg);
    int rc_gzclose = gzclose(gz);

    free(comp);
    free(dest);
    (void)rc_comp;
    (void)rc_inf_init2;
    (void)rc_inflate;
    (void)rc_inf_end;
    (void)outProduced;
    (void)ad_source;
    (void)ad_dest;
    (void)flags;
    (void)rc_gzprintf;
    (void)rc_gzclose;
    return 66;
}