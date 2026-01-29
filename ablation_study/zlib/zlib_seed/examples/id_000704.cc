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
//<ID> 704
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, initialize a deflate stream, compute bound and compress
    const char src[] = "zlib API sequence test payload for inflateBackInit_, deflateBound, inflateEnd and gzclose";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong compAlloc = deflateBound(&dstrm, sourceLen);
    Bytef *compBuf = (Bytef *)malloc((size_t)compAlloc);
    memset(compBuf, 0, (size_t)compAlloc);
    uLongf compLen = (uLongf)compAlloc;
    int rc_comp = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Initialize inflate-back stream with a working window (no actual inflateBack calls)
    z_stream istrm;
    memset(&istrm, 0, sizeof(z_stream));
    int windowBits = 15;
    unsigned char *window = (unsigned char *)malloc((size_t)(1u << windowBits));
    memset(window, 0, (size_t)(1u << windowBits));
    int rc_ibinit = inflateBackInit_(&istrm, windowBits, window, version, (int)sizeof(z_stream));

    // step 3: Operate — write the compressed buffer to a gzFile and flush
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)compBuf, (unsigned int)compLen);
    int rc_gzflush = gzflush(gf, 0);
    int rc_gzclose = gzclose(gf);

    // step 4: Validate and cleanup — end inflate and deflate streams and free memory
    int rc_inf_end = inflateEnd(&istrm);
    int rc_def_end = deflateEnd(&dstrm);
    free(compBuf);
    free(window);
    (void)rc_def_init;
    (void)compAlloc;
    (void)rc_comp;
    (void)rc_ibinit;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzclose;
    (void)rc_inf_end;
    (void)rc_def_end;
    // API sequence test completed successfully
    return 66;
}