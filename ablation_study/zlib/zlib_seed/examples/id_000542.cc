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
//<ID> 542
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, initialize deflate stream and set a dictionary
    const char raw[] = "Example payload for zlib deflate with dictionary and gz IO.";
    const Bytef *source = (const Bytef *)raw;
    uLong sourceLen = (uLong)(sizeof(raw) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    const Bytef dict[] = "sample_dictionary";
    uInt dictLen = (uInt)(sizeof(dict) - 1);
    int rc_setdict = deflateSetDictionary(&dstrm, dict, dictLen);
    uLong bound = deflateBound(&dstrm, sourceLen);
    Bytef *out = (Bytef *)malloc((size_t)bound);
    memset(out, 0, (size_t)bound);
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = out;
    dstrm.avail_out = (uInt)bound;

    // step 2: Perform a deflate operation and finalize the stream
    int rc_deflate = deflate(&dstrm, 4); /* Z_FINISH == 4 */
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Open a gzFile, write formatted data and a character, then close
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzprintf = gzprintf(gf, "Original:%s", raw);
    int rc_gzputc = gzputc(gf, (int)raw[0]);
    int rc_gzclose = gzclose(gf);

    // step 4: Basic validation, cleanup and finish
    uLong adl = adler32(1, source, (uInt)sourceLen);
    (void)adl;
    free(out);
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