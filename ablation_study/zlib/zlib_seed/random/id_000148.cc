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
//<ID> 148
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize
    const char src_c[] = "zlib API sequence payload: compress -> deflate -> deflateCopy -> inflate -> validate";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    Bytef dict_c[] = "example-dictionary";
    const Bytef *dictionary = (const Bytef *)dict_c;
    uInt dictLen = (uInt)(sizeof(dict_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    z_stream dsrc;
    z_stream ddest;
    z_stream istrm;
    memset(&dsrc, 0, sizeof(z_stream));
    memset(&ddest, 0, sizeof(z_stream));
    memset(&istrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();

    // step 2: Setup (compress source, initialize deflate, deflate once, copy deflate state)
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    Bytef *defOut = (Bytef *)malloc((size_t)bound);
    memset(defOut, 0, (size_t)bound);
    int rc_def_init = deflateInit_(&dsrc, 6, version, (int)sizeof(z_stream));
    dsrc.next_in = (Bytef *)source;
    dsrc.avail_in = (uInt)sourceLen;
    dsrc.next_out = defOut;
    dsrc.avail_out = (uInt)bound;
    int rc_def = deflate(&dsrc, 0);
    int rc_defcopy = deflateCopy(&ddest, &dsrc);

    // step 3: Core operations (initialize inflate, set dictionary, inflate, validate)
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    Bytef *outBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf, 0, (size_t)sourceLen);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)sourceLen;
    int rc_inf_set_dict = inflateSetDictionary(&istrm, dictionary, dictLen);
    int rc_inflate = inflate(&istrm, 0);
    int rc_inf_validate = inflateValidate(&istrm, 0);

    // step 4: Cleanup & finalize
    int rc_def_end_src = deflateEnd(&dsrc);
    int rc_def_end_dest = deflateEnd(&ddest);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(defOut);
    free(outBuf);
    (void)rc_comp; (void)rc_def_init; (void)rc_def; (void)rc_defcopy;
    (void)rc_inf_init; (void)rc_inf_set_dict; (void)rc_inflate; (void)rc_inf_validate;
    (void)rc_def_end_src; (void)rc_def_end_dest; (void)rc_inf_end; (void)bound; (void)compLen;
    // API sequence test completed successfully
    return 66;
}