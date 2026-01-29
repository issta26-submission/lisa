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
//<ID> 144
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize
    const char src_c[] = "zlib API sequence payload: compress -> deflateCopy -> inflateSetDictionary -> validate";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    Bytef dict_c[] = "example-dictionary";
    const Bytef *dictionary = (const Bytef *)dict_c;
    uInt dictLen = (uInt)(sizeof(dict_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(compBuf, &compLen, source, sourceLen);

    // step 2: Configure deflate streams and copy
    z_stream orig_def;
    z_stream copy_def;
    memset(&orig_def, 0, sizeof(z_stream));
    memset(&copy_def, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&orig_def, 6, version, (int)sizeof(z_stream));
    int rc_def_copy = deflateCopy(&copy_def, &orig_def);

    // step 3: Operate - inflate with dictionary and validate
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    Bytef *outBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf, 0, (size_t)sourceLen);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)sourceLen;
    int rc_inf_set_dict = inflateSetDictionary(&istrm, dictionary, dictLen);
    int rc_inflate = inflate(&istrm, 0);
    int rc_inf_validate = inflateValidate(&istrm, 1);

    // step 4: Cleanup
    int rc_def_end1 = deflateEnd(&orig_def);
    int rc_def_end2 = deflateEnd(&copy_def);
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);
    free(outBuf);
    (void)rc_comp; (void)rc_def_init; (void)rc_def_copy; (void)rc_inf_init;
    (void)rc_inf_set_dict; (void)rc_inflate; (void)rc_inf_validate;
    (void)rc_def_end1; (void)rc_def_end2; (void)rc_inf_end;
    (void)compLen; (void)bound; (void)version; (void)sourceLen; (void)dictLen;
    // API sequence test completed successfully
    return 66;
}