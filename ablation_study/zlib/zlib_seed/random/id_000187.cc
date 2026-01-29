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
//<ID> 187
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize buffers and sources
    const char src_c[] = "zlib API sequence payload: compress2 -> deflateBound -> inflateSetDictionary -> gzclearerr";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();

    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));

    uLong bound = deflateBound(&dstrm, sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;

    // step 2: Setup & Compress
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 3: Initialize inflate, set dictionary, write compressed data to gz and clear gz errors
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int rc_inf_setdic = inflateSetDictionary(&istrm, source, (uInt)sourceLen);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_write = gz ? gzwrite(gz, compBuf, (unsigned int)compLen) : -1;
    gzclearerr(gz);
    int rc_gz_close = gz ? gzclose(gz) : -1;

    // step 4: Cleanup & finalization
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);

    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_inf_setdic;
    (void)rc_gz_write;
    (void)rc_gz_close;
    (void)rc_inf_end;
    (void)bound;
    (void)version;
    // API sequence test completed successfully
    return 66;
}