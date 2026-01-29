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
//<ID> 181
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize buffers and streams
    const char src_c[] = "zlib API sequence payload for compress2 -> inflateSetDictionary -> gzwrite -> gzclearerr";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();

    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));

    // step 2: Initialize deflate stream and compute bound, allocate compression buffer, compress
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp2 = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 3: Initialize inflate stream, set dictionary (using prefix of source), write compressed data to gz and clear errors
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    const Bytef *dict = (const Bytef *)src_c;
    uInt dictLen = (uInt) (sourceLen > 8 ? 8u : (uInt)sourceLen);
    int rc_setdict = inflateSetDictionary(&istrm, dict, dictLen);

    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, compBuf, (unsigned int)compLen);
    gzclearerr(gz);
    int rc_gz_close = gzclose(gz);

    // step 4: Cleanup and finalize streams and buffers
    int rc_def_end = deflateEnd(&dstrm);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);

    (void)rc_def_init;
    (void)rc_comp2;
    (void)rc_inf_init;
    (void)rc_setdict;
    (void)rc_gz_write;
    (void)rc_gz_close;
    (void)rc_def_end;
    (void)rc_inf_end;

    // API sequence test completed successfully
    return 66;
}