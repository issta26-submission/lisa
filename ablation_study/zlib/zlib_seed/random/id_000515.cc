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
//<ID> 515
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress it with compress2
    const char src[] = "zlib API sequence payload: prepare -> deflateInit_ -> gzwrite -> uncompress -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)compBound);
    memset(comp, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress2(comp, &compLen, source, sourceLen, 6);

    // step 2: Initialize a deflate stream (configuration) and query a bound for potential further operations
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong defBound = deflateBound(&dstrm, (uLong)compLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)defBound);
    memset(outBuf, 0, (size_t)defBound);

    // step 3: Operate: write the original uncompressed payload into a gzFile using gzwrite
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)source, (unsigned int)sourceLen);
    int rc_gzclose = gzclose(gf);

    // step 4: Validate decompression using uncompress and obtain any textual error via zError, then cleanup
    uLongf destLen = sourceLen + 16;
    Bytef *dest = (Bytef *)malloc((size_t)destLen);
    memset(dest, 0, (size_t)destLen);
    int rc_uncomp = uncompress(dest, &destLen, comp, (uLong)compLen);
    const char *err_msg = zError(rc_uncomp);
    int rc_def_end = deflateEnd(&dstrm);
    free(comp);
    free(outBuf);
    free(dest);
    (void)rc_comp;
    (void)rc_def_init;
    (void)defBound;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_uncomp;
    (void)rc_def_end;
    (void)err_msg;
    (void)version;
    // API sequence test completed successfully
    return 66;
}