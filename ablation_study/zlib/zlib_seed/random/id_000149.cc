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
//<ID> 149
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize buffers and source
    z_stream src_defstrm;
    z_stream copy_defstrm;
    z_stream infstrm;
    memset(&src_defstrm, 0, sizeof(z_stream));
    memset(&copy_defstrm, 0, sizeof(z_stream));
    memset(&infstrm, 0, sizeof(z_stream));
    const char src_c[] = "zlib API sequence payload: compress -> deflateInit_ -> deflateCopy -> inflate -> validate";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    Bytef dict_c[] = "example-dictionary";
    const Bytef *dictionary = (const Bytef *)dict_c;
    uInt dictLen = (uInt)(sizeof(dict_c) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    Bytef *outBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf, 0, (size_t)sourceLen);

    // step 2: Compress the source and prepare deflate streams (initialize source and copy)
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    int rc_def_init = deflateInit_(&src_defstrm, 6, version, (int)sizeof(z_stream));
    int rc_def_copy = deflateCopy(&copy_defstrm, &src_defstrm);

    // step 3: Initialize inflate, set dictionary, perform inflate, validate
    int rc_inf_init = inflateInit_(&infstrm, version, (int)sizeof(z_stream));
    infstrm.next_in = compBuf;
    infstrm.avail_in = (uInt)compLen;
    infstrm.next_out = outBuf;
    infstrm.avail_out = (uInt)sourceLen;
    int rc_inf_set_dict = inflateSetDictionary(&infstrm, dictionary, dictLen);
    int rc_inflate = inflate(&infstrm, 0);
    int rc_inf_validate = inflateValidate(&infstrm, 1);

    // step 4: Cleanup & finalize
    int rc_def_end_src = deflateEnd(&src_defstrm);
    int rc_def_end_copy = deflateEnd(&copy_defstrm);
    int rc_inf_end = inflateEnd(&infstrm);
    free(compBuf);
    free(outBuf);
    (void)rc_comp; (void)rc_def_init; (void)rc_def_copy; (void)rc_inf_init;
    (void)rc_inf_set_dict; (void)rc_inflate; (void)rc_inf_validate;
    (void)rc_def_end_src; (void)rc_def_end_copy; (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}