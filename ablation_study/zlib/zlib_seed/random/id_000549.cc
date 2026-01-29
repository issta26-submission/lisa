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
//<ID> 549
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare data, query version, allocate buffers and initialize deflate stream
    const char src[] = "zlib API sequence payload: initialize -> set dictionary -> deflate -> gz IO -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong outBound = deflateBound(&dstrm, sourceLen);
    Bytef *outBuf = (Bytef *)malloc((size_t)outBound);
    memset(outBuf, 0, (size_t)outBound);

    // step 2: Configure deflate with a small dictionary and prepare stream pointers
    const Bytef *dictionary = source; /* use the start of the source as dictionary */
    uInt dictLen = (uInt)4; /* choose a small, fixed dictionary length */
    int rc_set_dict = deflateSetDictionary(&dstrm, dictionary, dictLen);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = outBuf;
    dstrm.avail_out = (uInt)outBound;

    // step 3: Perform deflate and write some results to a gzFile using gzprintf and gzputc
    int rc_def = deflate(&dstrm, 4); /* Z_FINISH == 4 */
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzprintf = gzprintf(gf, "original data header\n");
    int rc_gzputc = gzputc(gf, (int)(outBuf[0])); /* write first byte of compressed output */
    int rc_gzclose_res = gzclose(gf);

    // step 4: Cleanup deflate stream and free buffers
    int rc_def_end = deflateEnd(&dstrm);
    free(outBuf);
    (void)version;
    (void)rc_def_init;
    (void)outBound;
    (void)rc_set_dict;
    (void)rc_def;
    (void)rc_gzprintf;
    (void)rc_gzputc;
    (void)rc_gzclose_res;
    (void)rc_def_end;
    // API sequence test completed successfully
    return 66;
}