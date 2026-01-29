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
//<ID> 140
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char src_c[] = "zlib API sequence payload: compress + deflate + deflateCopy -> inflate with dictionary -> validate";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    Bytef dict_c[] = "example-dictionary";
    const Bytef *dictionary = (const Bytef *)dict_c;
    uInt dictLen = (uInt)(sizeof(dict_c) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(sourceLen);
    Bytef *comp_by_compress = (Bytef *)malloc((size_t)bound);
    memset(comp_by_compress, 0, (size_t)bound);
    uLongf comp_by_compress_len = (uLongf)bound;
    Bytef *comp_deflate = (Bytef *)malloc((size_t)bound);
    memset(comp_deflate, 0, (size_t)bound);
    Bytef *outBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf, 0, (size_t)sourceLen);
    z_stream defstrm;
    z_stream defcopy;
    z_stream infstrm;
    memset(&defstrm, 0, sizeof(z_stream));
    memset(&defcopy, 0, sizeof(z_stream));
    memset(&infstrm, 0, sizeof(z_stream));

    // step 2: Setup (single-shot compress and start deflate, then deflateCopy and continue)
    int rc_comp = compress(comp_by_compress, &comp_by_compress_len, source, sourceLen);
    int rc_def_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    defstrm.next_in = (Bytef *)source;
    defstrm.avail_in = (uInt)sourceLen;
    defstrm.next_out = comp_deflate;
    defstrm.avail_out = (uInt)bound;
    int rc_def1 = deflate(&defstrm, 0);
    int rc_defcopy = deflateCopy(&defcopy, &defstrm);
    defcopy.next_out = comp_deflate + (size_t)defstrm.total_out;
    defcopy.avail_out = (uInt)(bound - defstrm.total_out);
    int rc_defcopy_cont = deflate(&defcopy, 0);
    uLong def_compressed_size = defstrm.total_out + defcopy.total_out;

    // step 3: Core operations (initialize inflate, set dictionary, validate, inflate)
    int rc_inf_init = inflateInit_(&infstrm, version, (int)sizeof(z_stream));
    infstrm.next_in = comp_deflate;
    infstrm.avail_in = (uInt)def_compressed_size;
    infstrm.next_out = outBuf;
    infstrm.avail_out = (uInt)sourceLen;
    int rc_inf_set_dict = inflateSetDictionary(&infstrm, dictionary, dictLen);
    int rc_inf_validate = inflateValidate(&infstrm, 0);
    int rc_inflate = inflate(&infstrm, 0);

    // step 4: Cleanup & finalization
    int rc_def_end = deflateEnd(&defstrm);
    int rc_defcopy_end = deflateEnd(&defcopy);
    int rc_inf_end = inflateEnd(&infstrm);
    free(comp_by_compress);
    free(comp_deflate);
    free(outBuf);
    (void)rc_comp; (void)rc_def_init; (void)rc_def1; (void)rc_defcopy; (void)rc_defcopy_cont;
    (void)def_compressed_size; (void)rc_inf_init; (void)rc_inf_set_dict; (void)rc_inf_validate; (void)rc_inflate;
    (void)rc_def_end; (void)rc_defcopy_end; (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}