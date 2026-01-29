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
//<ID> 572
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    /* step 1: Prepare source data and produce a zlib-compressed buffer */
    const char src[] = "zlib API payload: prepare -> compress -> inflate -> gz write -> validate";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    /* step 2: Initialize a standard inflate stream and perform an inflate of the compressed buffer */
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    Bytef *outBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf, 0, (size_t)sourceLen);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)sourceLen;
    int rc_inflate = inflate(&istrm, Z_FINISH);

    /* step 3: Initialize an inflateBack stream (provide a window) and immediately finalize it to exercise that API */
    z_stream backstrm;
    memset(&backstrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_inflate_back_init = inflateBackInit_(&backstrm, 15, window, version, (int)sizeof(z_stream));
    int rc_inflate_back_end = inflateBackEnd(&backstrm);

    /* step 4: Persist the compressed buffer to a gzFile, validate file position via gztell, and cleanup */
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, compBuf, (unsigned int)compLen);
    off_t pos_after_write = gztell(gf);
    int rc_gzclose = gzclose(gf);

    int rc_inflate_end = inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);
    free(window);

    (void)rc_comp;
    (void)rc_inflate_init;
    (void)rc_inflate;
    (void)rc_inflate_back_init;
    (void)rc_inflate_back_end;
    (void)rc_gzwrite;
    (void)pos_after_write;
    (void)rc_gzclose;
    (void)rc_inflate_end;

    return 66;
}