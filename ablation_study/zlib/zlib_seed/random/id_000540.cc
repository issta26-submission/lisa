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
//<ID> 540
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data, query zlib version, and initialize deflate stream
    const char src[] = "zlib_api_sequence_payload_simple_text_without_percent_signs";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure dictionary, prepare output buffer, and perform deflate
    const Bytef dict[] = { 'd', 'i', 'c', 't' };
    uInt dictLen = (uInt)(sizeof(dict));
    int rc_setdict = deflateSetDictionary(&dstrm, dict, dictLen);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    uLong outBound = deflateBound(&dstrm, sourceLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)outBound;
    int rc_deflate = deflate(&dstrm, 4); /* Z_FINISH == 4 */
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Open a gzFile, write text via gzprintf and a single character via gzputc, then close
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzprintf = gzprintf(gf, (const char *)src);
    int rc_gzputc = gzputc(gf, (int)src[0]);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup allocated buffers and finalize
    free(outBuf);
    (void)version;
    (void)rc_def_init;
    (void)rc_setdict;
    (void)rc_deflate;
    (void)rc_def_end;
    (void)rc_gzprintf;
    (void)rc_gzputc;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}