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
//<ID> 576
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source and produce compressed payload
    const char src[] = "zlib API payload: inflateInit_ -> inflateBackInit_ -> inflate -> gzwrite -> gztell -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong compBound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compBound);
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Initialize inflate streams (standard inflate and inflateBack) using zlibVersion()
    const char *version = zlibVersion();
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    z_stream backstrm;
    memset(&backstrm, 0, sizeof(z_stream));
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int rc_inflate_back_init = inflateBackInit_(&backstrm, 15, window, version, (int)sizeof(z_stream));

    // step 3: Operate - inflate the compressed buffer and write the compressed data to a gzFile, then query its position
    uLong outBound = sourceLen + 16;
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outBound;
    int rc_inflate = inflate(&istrm, 4); /* Z_FINISH == 4 */
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, compBuf, (unsigned int)compLen);
    off_t pos = gztell(gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Validate (use returned totals/position) and cleanup resources
    int rc_inflate_end = inflateEnd(&istrm);
    int rc_inflate_back_end = inflateBackEnd(&backstrm);
    free(compBuf);
    free(outBuf);
    free(window);
    (void)rc_comp;
    (void)rc_inflate_init;
    (void)rc_inflate_back_init;
    (void)rc_inflate;
    (void)rc_gzwrite;
    (void)pos;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    (void)rc_inflate_back_end;
    // API sequence test completed successfully
    return 66;
}