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
//<ID> 146
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
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    z_stream dsrc;
    z_stream ddst;
    memset(&dsrc, 0, sizeof(z_stream));
    memset(&ddst, 0, sizeof(z_stream));
    const char *version = zlibVersion();

    // step 2: Compress the source to produce input for inflate
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);
    uLong compressed_size = (uLong)compLen;

    // step 3: Initialize a deflate stream and copy it (deflateCopy)
    int rc_def_init = deflateInit_(&dsrc, 6, version, (int)sizeof(z_stream));
    dsrc.next_in = compBuf;
    dsrc.avail_in = (uInt)compressed_size;
    int rc_def_copy = deflateCopy(&ddst, &dsrc);
    int rc_def_end_dst = deflateEnd(&ddst);
    int rc_def_end_src = deflateEnd(&dsrc);

    // step 4: Initialize inflate, set dictionary, validate and inflate, then cleanup
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compressed_size;
    Bytef dict_c[] = "example-dictionary";
    const Bytef *dictionary = (const Bytef *)dict_c;
    uInt dictLen = (uInt)(sizeof(dict_c) - 1);
    int rc_inf_set_dict = inflateSetDictionary(&istrm, dictionary, dictLen);
    int rc_inf_val = inflateValidate(&istrm, 1);
    Bytef *outBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf, 0, (size_t)sourceLen);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)sourceLen;
    int rc_inflate = inflate(&istrm, 0);
    int rc_inf_end = inflateEnd(&istrm);

    free(compBuf);
    free(outBuf);

    (void)rc_comp; (void)compressed_size; (void)rc_def_init; (void)rc_def_copy;
    (void)rc_def_end_dst; (void)rc_def_end_src; (void)rc_inf_init; (void)rc_inf_set_dict;
    (void)rc_inf_val; (void)rc_inflate; (void)rc_inf_end; (void)version;
    // API sequence test completed successfully
    return 66;
}