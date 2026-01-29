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
//<ID> 188
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize buffers and sources
    const char src_c[] = "zlib payload for API sequence test: compress2 -> deflateBound -> inflateSetDictionary -> gzclearerr";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();

    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure: compute bound and compress with compress2
    uLong bound = deflateBound(&dstrm, sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp2 = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 3: Operate & Validate: initialize inflate, set dictionary, use gz and clear its error
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    const Bytef *dict = source;
    uInt dictLen = (uInt)16;
    int rc_inf_setdict = inflateSetDictionary(&istrm, dict, dictLen);

    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzclearerr(gz);
    int rc_gz_close = gzclose(gz);

    // step 4: Cleanup
    int rc_def_end = deflateEnd(&dstrm);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);

    (void)rc_def_init;
    (void)rc_def_end;
    (void)bound;
    (void)rc_comp2;
    (void)compLen;
    (void)rc_inf_init;
    (void)rc_inf_setdict;
    (void)rc_gz_close;
    (void)rc_inf_end;
    (void)version;
    (void)source;
    (void)sourceLen;
    (void)dict;
    (void)dictLen;

    // API sequence test completed successfully
    return 66;
}