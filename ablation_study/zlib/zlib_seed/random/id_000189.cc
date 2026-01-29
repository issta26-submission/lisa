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
//<ID> 189
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize sources
    const char src_c[] = "zlib API sequence payload: compress2 -> uncompress -> inflateSetDictionary -> gzclearerr";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();

    // step 2: Configure (initialize deflate stream to obtain bounds, then compress2)
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp2 = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 3: Operate & Validate (initialize inflate, set dictionary, uncompress, use gzclearerr)
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    const Bytef *dict_ptr = source;
    uInt dict_len = (uInt)4;
    int rc_inf_setdict = inflateSetDictionary(&istrm, dict_ptr, dict_len);
    uLongf decompLen = (uLongf)(sourceLen + 64);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompLen);
    memset(decompBuf, 0, (size_t)decompLen);
    int rc_uncomp = uncompress(decompBuf, &decompLen, compBuf, (uLong)compLen);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzclearerr(gz);
    int rc_gz_write = gzwrite(gz, (voidpc)decompBuf, (unsigned int)decompLen);
    int rc_gz_close = gzclose(gz);

    // step 4: Cleanup
    int rc_def_end = deflateEnd(&dstrm);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(decompBuf);

    (void)rc_def_init;
    (void)rc_comp2;
    (void)rc_inf_init;
    (void)rc_inf_setdict;
    (void)rc_uncomp;
    (void)rc_gz_write;
    (void)rc_gz_close;
    (void)rc_def_end;
    (void)rc_inf_end;

    // API sequence test completed successfully
    return 66;
}