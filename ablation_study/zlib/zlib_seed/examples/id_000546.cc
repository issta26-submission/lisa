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
//<ID> 546
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data, version, initialize deflate stream and set dictionary
    const char src[] = "zlib API sequence payload: dictionary-assisted stream deflate -> gz IO -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    Bytef dict[] = "zlib_dictionary_example";
    uInt dictLen = (uInt)(sizeof(dict) - 1);
    int rc_setdict = deflateSetDictionary(&dstrm, dict, dictLen);

    // step 2: Configure stream buffers, perform deflate (one-shot with Z_FINISH), and finalize stream
    uLong bound = deflateBound(&dstrm, sourceLen);
    Bytef *out = (Bytef *)malloc((size_t)bound);
    memset(out, 0, (size_t)bound);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = out;
    dstrm.avail_out = (uInt)bound;
    int rc_deflate = deflate(&dstrm, 4); /* Z_FINISH == 4 */
    int rc_def_end = deflateEnd(&dstrm);
    uLong produced = dstrm.total_out;

    // step 3: Open gzFile, write metadata with gzprintf, write a single byte with gzputc, write compressed blob with gzwrite, then close
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzprintf = gzprintf(gf, "compressed_len=%lu\n", (unsigned long)produced);
    int rc_gzputc = gzputc(gf, (int)src[0]);
    int rc_gzwrite = gzwrite(gf, out, (unsigned int)produced);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup buffers and finalize
    free(out);
    (void)rc_def_init;
    (void)rc_setdict;
    (void)rc_deflate;
    (void)rc_def_end;
    (void)rc_gzprintf;
    (void)rc_gzputc;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)version;
    // API sequence test completed successfully
    return 66;
}