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
//<ID> 182
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize buffers and streams
    const char src_c[] = "zlib API sequence payload: using compress2 -> write gz -> inflateSetDictionary -> deflateBound";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();

    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Configure (compress and prepare inflate stream with a dictionary)
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    Bytef dict[] = "example_dictionary";
    uInt dictLen = (uInt)(sizeof(dict) - 1);
    int rc_inf_setdict = inflateSetDictionary(&istrm, dict, dictLen);

    // step 3: Operate (write compressed data to a gzip file and clear any gz errors)
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gzwrite(gz, compBuf, (unsigned int)compLen);
    gzclearerr(gz);
    int rc_gz_close = gzclose(gz);

    // step 4: Cleanup & finalize
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);

    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inf_setdict;
    (void)rc_gz_write;
    (void)rc_gz_close;
    (void)rc_inf_end;

    // API sequence test completed successfully
    return 66;
}