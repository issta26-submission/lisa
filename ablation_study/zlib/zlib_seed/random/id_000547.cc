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
//<ID> 547
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data, query zlib version, and initialize a deflate stream
    const char src[] = "zlib API sequence payload: initialize -> set dictionary -> deflate -> gz IO -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure the deflate stream with a small preset dictionary
    const Bytef *dict = source;
    uInt dictLen = (uInt)(sourceLen < 8 ? sourceLen : 8);
    int rc_setdict = deflateSetDictionary(&dstrm, dict, dictLen);

    // step 3: Operate - perform a one-shot deflate using the configured stream
    uLong bound = deflateBound(&dstrm, sourceLen);
    Bytef *out = (Bytef *)malloc((size_t)bound);
    memset(out, 0, (int)bound);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = out;
    dstrm.avail_out = (uInt)bound;
    int rc_deflate = deflate(&dstrm, 4); /* Z_FINISH == 4 */
    int rc_def_end = deflateEnd(&dstrm);
    uLong compressedSize = dstrm.total_out;

    // step 4: Validate (write a byte and a message) and cleanup: open a gzFile, write using gzputc/gzprintf, then close and free
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzputc = gzputc(gf, out[0]);
    int rc_gzprintf = gzprintf(gf, "Deflate completed\n");
    int rc_gzclose = gzclose(gf);
    free(out);
    (void)version;
    (void)rc_def_init;
    (void)rc_setdict;
    (void)rc_deflate;
    (void)rc_def_end;
    (void)compressedSize;
    (void)rc_gzputc;
    (void)rc_gzprintf;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}