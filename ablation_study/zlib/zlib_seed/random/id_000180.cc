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
//<ID> 180
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize buffers and streams
    const char src_c[] = "zlib API sequence: compress2 -> uncompress -> inflateSetDictionary -> gzclearerr";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();

    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    memset(&istrm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup & compress (initialize deflate to compute bound, then compress2)
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp2 = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 3: Operate & Validate (initialize inflate, set dictionary, uncompress, write to gz and clear error)
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    const Bytef dict[] = "preset_dictionary";
    uInt dictLen = (uInt)(sizeof(dict) - 1);
    int rc_set_dict = inflateSetDictionary(&istrm, dict, dictLen);

    uLongf decompLen = (uLongf)(sourceLen + 64);
    Bytef *decompBuf = (Bytef *)malloc((size_t)decompLen);
    memset(decompBuf, 0, (size_t)decompLen);
    int rc_uncomp = uncompress(decompBuf, &decompLen, compBuf, (uLong)compLen);

    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = 0;
    if (gz) {
        rc_gz_write = gzwrite(gz, (voidpc)decompBuf, (unsigned int)(decompLen < (uLong)UINT_MAX ? decompLen : (uLong)UINT_MAX));
    }
    gzclearerr(gz);

    // step 4: Cleanup
    int rc_inf_end = inflateEnd(&istrm);
    int rc_def_end = deflateEnd(&dstrm);
    int rc_gz_close = gz ? gzclose(gz) : 0;
    free(compBuf);
    free(decompBuf);

    (void)rc_def_init;
    (void)rc_comp2;
    (void)rc_inf_init;
    (void)rc_set_dict;
    (void)rc_uncomp;
    (void)rc_gz_write;
    (void)rc_inf_end;
    (void)rc_def_end;
    (void)rc_gz_close;

    // API sequence test completed successfully
    return 66;
}