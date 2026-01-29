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
//<ID> 142
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize
    z_stream def_src;
    z_stream def_dst;
    z_stream inf_strm;
    memset(&def_src, 0, sizeof(z_stream));
    memset(&def_dst, 0, sizeof(z_stream));
    memset(&inf_strm, 0, sizeof(z_stream));
    const char src_c[] = "zlib API sequence payload: compress + deflateCopy -> inflate with dictionary -> validate";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    Bytef dict_c[] = "example-dictionary";
    const Bytef *dictionary = (const Bytef *)dict_c;
    uInt dictLen = (uInt)(sizeof(dict_c) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf_def = (Bytef *)malloc((size_t)bound);
    memset(compBuf_def, 0, (size_t)bound);
    Bytef *compBuf_cmp = (Bytef *)malloc((size_t)bound);
    memset(compBuf_cmp, 0, (size_t)bound);
    Bytef *outBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf, 0, (size_t)sourceLen);

    // step 2: Setup (initialize deflate source and produce some state, then copy)
    int rc_def_init = deflateInit_(&def_src, 6, version, (int)sizeof(z_stream));
    def_src.next_in = (Bytef *)source;
    def_src.avail_in = (uInt)sourceLen;
    def_src.next_out = compBuf_def;
    def_src.avail_out = (uInt)bound;
    int rc_def_call = deflate(&def_src, 0);
    uLong def_compressed_size = def_src.total_out;
    int rc_def_copy = deflateCopy(&def_dst, &def_src);

    // step 3: Core operations (use compress convenience API and then inflate with dictionary)
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(compBuf_cmp, &compLen, source, sourceLen);
    inf_strm.next_in = compBuf_cmp;
    inf_strm.avail_in = (uInt)compLen;
    inf_strm.next_out = outBuf;
    inf_strm.avail_out = (uInt)sourceLen;
    int rc_inf_init = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    int rc_inf_set_dict = inflateSetDictionary(&inf_strm, dictionary, dictLen);
    int rc_inflate = inflate(&inf_strm, 0);
    int rc_inf_validate = inflateValidate(&inf_strm, 1);

    // step 4: Cleanup & finalize
    int rc_def_end_src = deflateEnd(&def_src);
    int rc_def_end_dst = deflateEnd(&def_dst);
    int rc_inf_end = inflateEnd(&inf_strm);
    free(compBuf_def);
    free(compBuf_cmp);
    free(outBuf);

    (void)rc_def_init; (void)rc_def_call; (void)def_compressed_size; (void)rc_def_copy;
    (void)rc_comp; (void)rc_inf_init; (void)rc_inf_set_dict; (void)rc_inflate; (void)rc_inf_validate;
    (void)rc_def_end_src; (void)rc_def_end_dst; (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}