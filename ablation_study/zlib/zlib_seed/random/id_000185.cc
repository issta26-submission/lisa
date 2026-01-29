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
//<ID> 185
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & initialize buffers and source
    const char src_c[] = "zlib API sequence payload: demonstrate compress2 -> deflateBound -> inflateSetDictionary -> gzclearerr";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    const char *version = zlibVersion();

    // step 2: Initialize deflate stream, compute bound, allocate and compress
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)bound);
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Initialize inflate stream, set dictionary, associate compressed data and use gz APIs
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    uInt dictLength = (uInt)sourceLen;
    int rc_inf_setdict = inflateSetDictionary(&istrm, source, dictLength);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gz_puts = gzputs(gz, "Compressed payload written\n");
    gzclearerr(gz);
    int rc_gz_close = gzclose(gz);

    // step 4: Cleanup & finalize
    int rc_inf_end = inflateEnd(&istrm);
    free(compBuf);

    (void)rc_def_init;
    (void)bound;
    (void)compLen;
    (void)rc_comp;
    (void)rc_def_end;
    (void)rc_inf_init;
    (void)rc_inf_setdict;
    (void)rc_gz_puts;
    (void)rc_gz_close;
    (void)rc_inf_end;

    // API sequence test completed successfully
    return 66;
}