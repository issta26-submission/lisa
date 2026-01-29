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
//<ID> 186
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char src_c[] = "zlib API sequence payload: use deflateBound -> compress2 -> inflateSetDictionary -> gzclearerr";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Configure & Compress
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 3: Initialize inflate, set dictionary, open gz and clear errors
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uInt dictLen = (uInt)sourceLen;
    int rc_inf_setdict = inflateSetDictionary(&istrm, source, dictLen);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzclearerr(gz);

    // step 4: Cleanup
    int rc_def_end = deflateEnd(&dstrm);
    int rc_inf_end = inflateEnd(&istrm);
    int rc_gz_close = gzclose(gz);
    free(compBuf);
    (void)rc_def_init;
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inf_setdict;
    (void)rc_def_end;
    (void)rc_inf_end;
    (void)rc_gz_close;
    (void)compLen;
    (void)bound;
    (void)version;
    (void)sourceLen;

    // API sequence test completed successfully
    return 66;
}