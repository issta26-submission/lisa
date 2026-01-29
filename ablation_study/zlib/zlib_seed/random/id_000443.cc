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
//<ID> 443
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress into a buffer
    const char src[] = "zlib API sequence payload: prepare -> deflateSetHeader -> compress -> uncompress -> gz IO -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress2(comp, &compLen, source, sourceLen, 6);

    // step 2: Initialize a deflate stream and attach a gzip header via deflateSetHeader (no actual deflation performed)
    z_stream def_strm;
    memset(&def_strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    gz_header head;
    head.text = 0;
    head.time = 0;
    head.xflags = 0;
    head.os = 255;
    head.extra = (Bytef *)0;
    head.extra_len = 0;
    head.extra_max = 0;
    head.name = (Bytef *)0;
    head.name_max = 0;
    head.comment = (Bytef *)0;
    head.comm_max = 0;
    head.hcrc = 0;
    head.done = 0;
    int rc_sethdr = deflateSetHeader(&def_strm, &head);
    int rc_def_end = deflateEnd(&def_strm);

    // step 3: Uncompress into destination buffer and write out via gzFile opened from fd 1, then clear errors
    Bytef *dest = (Bytef *)malloc((size_t)sourceLen);
    memset(dest, 0, (size_t)sourceLen);
    uLongf destLen = (uLongf)sourceLen;
    int rc_uncomp = uncompress(dest, &destLen, comp, (uLong)compLen);
    gzFile gf = gzdopen(1, "wb");
    int rc_gzwrite = gzwrite(gf, dest, (unsigned int)destLen);
    gzclearerr(gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup
    free(comp);
    free(dest);
    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_sethdr;
    (void)rc_def_end;
    (void)rc_uncomp;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)version;
    (void)sourceLen;
    (void)compLen;
    (void)destLen;
    // API sequence test completed successfully
    return 66;
}