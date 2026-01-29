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
//<ID> 117
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
    const char src_c[] = "zlib API sequence payload: deflate with (empty) dictionary -> write gz -> inflate";
    Bytef *source = (Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    Bytef *outBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf, 0, (size_t)sourceLen);
    Bytef dict_bytes[1];
    dict_bytes[0] = 0;
    uInt dictLen = 0;
    const char *version = zlibVersion();

    // step 2: Setup (initialize deflate, set dictionary, perform deflate)
    int rc_def_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    int rc_set_dict = deflateSetDictionary(&defstrm, dict_bytes, dictLen);
    defstrm.next_in = source;
    defstrm.avail_in = (uInt)sourceLen;
    defstrm.next_out = compBuf;
    defstrm.avail_out = (uInt)bound;
    int rc_def = deflate(&defstrm, 0);
    uLong compProduced = defstrm.total_out;

    // step 3: Operate (write compressed data to gz, initialize inflate, validate and inflate)
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, compBuf, (unsigned int)compProduced);
    int rc_inf_init = inflateInit_(&infstrm, version, (int)sizeof(z_stream));
    infstrm.next_in = compBuf;
    infstrm.avail_in = (uInt)compProduced;
    infstrm.next_out = outBuf;
    infstrm.avail_out = (uInt)sourceLen;
    int rc_inf_validate = inflateValidate(&infstrm, 1);
    int rc_inf = inflate(&infstrm, 0);

    // step 4: Validate & Cleanup
    (void)rc_def_init; (void)rc_set_dict; (void)rc_def; (void)rc_gz_write;
    (void)rc_inf_init; (void)rc_inf_validate; (void)rc_inf;
    int rc_close = gzclose(gz);
    int rc_def_end = deflateEnd(&defstrm);
    int rc_inf_end = inflateEnd(&infstrm);
    free(compBuf);
    free(outBuf);
    (void)rc_close; (void)rc_def_end; (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}