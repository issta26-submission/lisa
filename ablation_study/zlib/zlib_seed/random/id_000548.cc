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
//<ID> 548
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data, dictionary and output buffers
    const char src[] = "zlib API sequence payload: dictionary seeded deflate -> gz I/O -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uInt dictLen = (uInt)(sourceLen > 4 ? 4U : (uInt)sourceLen);
    Bytef *dict = (Bytef *)malloc((size_t)dictLen);
    memcpy(dict, source, (size_t)dictLen);

    // step 2: Initialize and configure deflate stream with a dictionary
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int rc_set_dict = deflateSetDictionary(&dstrm, dict, dictLen);

    // step 3: Operate - compress the source using deflate with dictionary and finalize stream
    uLong bound = deflateBound(&dstrm, sourceLen);
    Bytef *out = (Bytef *)malloc((size_t)bound);
    memset(out, 0, (size_t)bound);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = out;
    dstrm.avail_out = (uInt)bound;
    int rc_deflate = deflate(&dstrm, 4); /* Z_FINISH == 4 */
    int rc_def_end = deflateEnd(&dstrm);

    // step 4: Validate-ish by writing original data to a gz file, then cleanup
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzprintf = gzprintf(gf, "%s", src);
    int rc_gzputc = gzputc(gf, (int)'.');
    int rc_gzclose = gzclose(gf);
    free(dict);
    free(out);
    (void)version;
    (void)rc_def_init;
    (void)rc_set_dict;
    (void)rc_deflate;
    (void)rc_def_end;
    (void)rc_gzprintf;
    (void)rc_gzputc;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}