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
//<ID> 116
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream defstrm;
    z_stream infstrm;
    memset(&defstrm, 0, sizeof(z_stream));
    memset(&infstrm, 0, sizeof(z_stream));
    const char src_c[] = "zlib API payload: deflateInit -> deflateSetDictionary -> compress -> inflateValidate -> inflate";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    const char *version = zlibVersion();

    // step 2: Setup (compress data and open gz file for recording compressed bytes)
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, compBuf, (unsigned int)compLen);

    // step 3: Core operations (initialize deflate, set dictionary, init inflate, validate and inflate)
    int rc_def_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    uInt dictLen = (uInt)((sourceLen > 16) ? 16 : sourceLen);
    Bytef *dict = (Bytef *)malloc((size_t)dictLen);
    memset(dict, 0, (size_t)dictLen);
    memcpy(dict, source, (size_t)dictLen);
    int rc_def_dict = deflateSetDictionary(&defstrm, dict, dictLen);
    int rc_inf_init = inflateInit_(&infstrm, version, (int)sizeof(z_stream));
    Bytef *outBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf, 0, (size_t)sourceLen);
    infstrm.next_in = compBuf;
    infstrm.avail_in = (uInt)compLen;
    infstrm.next_out = outBuf;
    infstrm.avail_out = (uInt)sourceLen;
    int rc_val = inflateValidate(&infstrm, 1);
    int rc_inf = inflate(&infstrm, 0);

    // step 4: Validate & Cleanup
    (void)rc_comp; (void)rc_gz_write; (void)rc_def_init; (void)rc_def_dict;
    (void)rc_inf_init; (void)rc_val; (void)rc_inf;
    int rc_close = gzclose(gz);
    int rc_def_end = deflateEnd(&defstrm);
    int rc_inf_end = inflateEnd(&infstrm);
    free(compBuf);
    free(dict);
    free(outBuf);
    (void)rc_close; (void)rc_def_end; (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}