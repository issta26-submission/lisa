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
//<ID> 440
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress into a buffer
    const char src[] = "zlib API sequence payload: prepare -> deflateSetHeader -> uncompress -> gz IO -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Initialize a deflate stream configured for gzip and set a gzip header
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit2_(&dstrm, 6, 8, 31, 8, 0, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, sizeof(gz_header));
    head.name = NULL;
    head.comment = NULL;
    head.hcrc = 0;
    head.done = 0;
    int rc_seth = deflateSetHeader(&dstrm, &head);

    // step 3: Uncompress the compressed buffer and write the result to a gz stream opened from fd 1
    uLongf destLen = sourceLen;
    Bytef *dest = (Bytef *)malloc((size_t)destLen);
    memset(dest, 0, (size_t)destLen);
    int rc_un = uncompress(dest, &destLen, comp, (uLong)compLen);
    gzFile gz = gzdopen(1, "wb");
    gzclearerr(gz);
    int rc_gzwrite = gzwrite(gz, (voidpc)dest, (unsigned int)destLen);
    int rc_gzclose = gzclose(gz);

    // step 4: Cleanup resources and end the deflate stream
    int rc_def_end = deflateEnd(&dstrm);
    free(comp);
    free(dest);
    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_seth;
    (void)rc_un;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_def_end;
    (void)version;
    (void)sourceLen;
    (void)bound;
    (void)compLen;
    (void)destLen;
    (void)head;
    // API sequence test completed successfully
    return 66;
}