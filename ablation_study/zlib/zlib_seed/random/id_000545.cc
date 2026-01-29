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
//<ID> 545
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data and initialize deflate stream
    const char src[] = "zlib API sequence payload: initialize -> set dictionary -> deflate -> gz IO -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    const char dict[] = "common_dictionary";
    uInt dictLen = (uInt)(sizeof(dict) - 1);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure deflate with a preset dictionary and prepare output buffer
    int rc_setdict = deflateSetDictionary(&dstrm, (const Bytef *)dict, dictLen);
    uLong bound = deflateBound(&dstrm, sourceLen);
    Bytef *out = (Bytef *)malloc((size_t)bound);
    memset(out, 0, (size_t)bound);

    // step 3: Operate - perform a streaming deflate using the configured stream
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = out;
    dstrm.avail_out = (uInt)bound;
    int rc_deflate = deflate(&dstrm, 4); /* Z_FINISH == 4 */
    int rc_def_end = deflateEnd(&dstrm);

    // step 4: Validate-like IO using gzFile APIs and cleanup resources
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzprintf1 = gzprintf(gf, "original: ");
    int rc_gzputc1 = gzputc(gf, (int)src[0]);
    int rc_gzputc2 = gzputc(gf, '\n');
    int rc_gzprintf2 = gzprintf(gf, "deflated\n");
    int rc_gzclose = gzclose(gf);
    free(out);
    (void)version;
    (void)rc_def_init;
    (void)rc_setdict;
    (void)rc_deflate;
    (void)rc_def_end;
    (void)rc_gzprintf1;
    (void)rc_gzputc1;
    (void)rc_gzputc2;
    (void)rc_gzprintf2;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}