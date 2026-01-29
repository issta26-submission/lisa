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
//<ID> 145
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize
    const char src_c[] = "zlib API sequence payload: compress -> deflateCopy -> inflate with dictionary -> validate";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    Bytef dict_c[] = "example-dictionary";
    const Bytef *dictionary = (const Bytef *)dict_c;
    uInt dictLen = (uInt)(sizeof(dict_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    Bytef *outBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf, 0, (size_t)sourceLen);
    uLongf outLen = (uLongf)sourceLen;
    z_stream dsrc;
    z_stream dcopy;
    z_stream istrm;
    memset(&dsrc, 0, sizeof(z_stream));
    memset(&dcopy, 0, sizeof(z_stream));
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();

    // step 2: Setup (initialize deflate source and make a copy of its state)
    int rc_def_init = deflateInit_(&dsrc, 6, version, (int)sizeof(z_stream));
    dsrc.next_in = (Bytef *)source;
    dsrc.avail_in = (uInt)sourceLen;
    int rc_def_copy = deflateCopy(&dcopy, &dsrc);

    // step 3: Core operations (compress with compress(), then inflate using inflateSetDictionary and validate)
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    uLong compressed_size = (uLong)compLen;
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compressed_size;
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)outLen;
    int rc_inf_set_dict = inflateSetDictionary(&istrm, dictionary, dictLen);
    int rc_inf_val = inflateValidate(&istrm, 1);
    int rc_inflate = inflate(&istrm, 0);

    // step 4: Cleanup & finalize
    int rc_def_end_src = deflateEnd(&dsrc);
    int rc_def_end_copy = deflateEnd(&dcopy);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);
    (void)rc_def_init; (void)rc_def_copy; (void)rc_comp; (void)compressed_size;
    (void)rc_inf_init; (void)rc_inf_set_dict; (void)rc_inf_val; (void)rc_inflate;
    (void)rc_def_end_src; (void)rc_def_end_copy; (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}