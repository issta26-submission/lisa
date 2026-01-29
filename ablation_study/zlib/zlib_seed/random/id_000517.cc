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
//<ID> 517
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and buffers
    const char src[] = "zlib API sequence payload: prepare -> deflate -> gzwrite -> uncompress -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong outBound = compressBound(sourceLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    uLongf destProvision = (uLongf)(sourceLen + 256);
    Bytef *dest = (Bytef *)malloc((size_t)destProvision);
    memset(dest, 0, (size_t)destProvision);

    // step 2: Initialize deflate stream and perform compression
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)outBound;
    int rc_def = deflate(&dstrm, 0);
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Write compressed output to a gzFile using gzwrite
    gzFile gf = gzopen("test_zlib_api_sequence_out.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)outBuf, (unsigned int)dstrm.total_out);
    int rc_gzclose = gzclose(gf);

    // step 4: Uncompress the produced compressed buffer to validate and then cleanup
    uLongf uncompressedLen = destProvision;
    int rc_uncomp = uncompress(dest, &uncompressedLen, outBuf, (uLong)dstrm.total_out);
    const char *errstr = zError(rc_uncomp);

    free(outBuf);
    free(dest);
    (void)rc_def_init;
    (void)rc_def;
    (void)rc_def_end;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_uncomp;
    (void)errstr;
    (void)version;
    // API sequence test completed successfully
    return 66;
}