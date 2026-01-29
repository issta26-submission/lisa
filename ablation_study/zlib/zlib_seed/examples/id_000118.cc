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
//<ID> 118
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
    const char *version = zlibVersion();
    const char src_c[] = "Example payload for zlib sequence";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    int rc_def_init = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    Bytef dict[] = "example-dictionary";
    uInt dictLen = (uInt)(sizeof(dict) - 1);
    int rc_def_dict = deflateSetDictionary(&defstrm, dict, dictLen);

    // step 2: Configure & prepare buffers and compress
    uLong bound = compressBound(sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, compBuf, (unsigned int)compLen);

    // step 3: Initialize inflate stream and validate, then attempt inflate
    int rc_inf_init = inflateInit_(&infstrm, version, (int)sizeof(z_stream));
    infstrm.next_in = compBuf;
    infstrm.avail_in = (uInt)compLen;
    Bytef *outBuf = (Bytef *)malloc((size_t)sourceLen);
    memset(outBuf, 0, (size_t)sourceLen);
    infstrm.next_out = outBuf;
    infstrm.avail_out = (uInt)sourceLen;
    int rc_validate = inflateValidate(&infstrm, 1);
    int rc_inf = inflate(&infstrm, 0);

    // step 4: Validate & Cleanup
    (void)rc_def_init; (void)rc_def_dict; (void)rc_comp; (void)rc_gz_write;
    (void)rc_inf_init; (void)rc_validate; (void)rc_inf;
    int rc_close = gzclose(gz);
    int rc_def_end = deflateEnd(&defstrm);
    int rc_inf_end = inflateEnd(&infstrm);
    free(compBuf);
    free(outBuf);
    (void)rc_close; (void)rc_def_end; (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}