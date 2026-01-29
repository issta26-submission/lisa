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
//<ID> 147
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize
    const char src_c[] = "zlib API sequence payload: compress -> deflate -> deflateCopy -> inflate with dictionary -> validate";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    Bytef dict_c[] = "example-dictionary";
    const Bytef *dictionary = (const Bytef *)dict_c;
    uInt dictLen = (uInt)(sizeof(dict_c) - 1);
    const char *version = zlibVersion();

    uLong bound_compress = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound_compress);
    memset(compBuf, 0, (size_t)bound_compress);
    uLongf compLen = (uLongf)bound_compress;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    z_stream dstrm;
    z_stream dstrm_copy;
    z_stream istrm;
    memset(&dstrm, 0, sizeof(z_stream));
    memset(&dstrm_copy, 0, sizeof(z_stream));
    memset(&istrm, 0, sizeof(z_stream));

    // step 2: Setup (initialize deflate, perform deflate, copy deflate state)
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong bound_deflate = deflateBound(&dstrm, sourceLen);
    Bytef *compBuf_def = (Bytef *)malloc((size_t)bound_deflate);
    memset(compBuf_def, 0, (size_t)bound_deflate);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = compBuf_def;
    dstrm.avail_out = (uInt)bound_deflate;
    int rc_deflate = deflate(&dstrm, 0);
    int rc_deflate_copy = deflateCopy(&dstrm_copy, &dstrm);

    // step 3: Core operations (initialize inflate, set dictionary, validate, inflate)
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    Bytef *outBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf, 0, (size_t)sourceLen);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)sourceLen;
    int rc_inf_set_dict = inflateSetDictionary(&istrm, dictionary, dictLen);
    int rc_inf_validate = inflateValidate(&istrm, 1);
    int rc_inflate = inflate(&istrm, 0);

    // step 4: Cleanup
    int rc_def_end = deflateEnd(&dstrm);
    int rc_def_copy_end = deflateEnd(&dstrm_copy);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(compBuf_def);
    free(outBuf);

    (void)rc_comp; (void)rc_def_init; (void)bound_deflate; (void)rc_deflate; (void)rc_deflate_copy;
    (void)rc_inf_init; (void)rc_inf_set_dict; (void)rc_inf_validate; (void)rc_inflate;
    (void)rc_def_end; (void)rc_def_copy_end; (void)rc_inf_end; (void)compLen; (void)sourceLen; (void)dictionary; (void)dictLen;
    // API sequence test completed successfully
    return 66;
}