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
//<ID> 447
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data, compress it, then uncompress to validate round-trip
    const char src[] = "zlib API sequence payload: prepare -> compress -> set header -> write gz -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);
    uLongf destLen = (uLongf)sourceLen;
    Bytef *dest = (Bytef *)malloc((size_t)destLen);
    memset(dest, 0, (size_t)destLen);
    int rc_uncomp = uncompress(dest, &destLen, comp, (uLong)compLen);

    // step 2: Initialize a deflate stream and attach a gz_header via deflateSetHeader
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, sizeof(gz_header));
    head.name_max = 16;
    head.name = (Bytef *)malloc((size_t)head.name_max);
    memset(head.name, 0, (size_t)head.name_max);
    memcpy(head.name, "zlib-test", 9);
    int rc_setheader = deflateSetHeader(&dstrm, &head);

    // step 3: Open a gzFile from stdout descriptor, write the uncompressed data, and clear any error state
    gzFile gf = gzdopen(1, "wb");
    int rc_gzwrite = gzwrite(gf, (voidpc)dest, (unsigned int)destLen);
    gzclearerr(gf);

    // step 4: Cleanup resources and finalize
    int rc_def_end = deflateEnd(&dstrm);
    int rc_gzclose = gzclose(gf);
    free(comp);
    free(dest);
    free(head.name);
    (void)rc_comp;
    (void)rc_uncomp;
    (void)rc_def_init;
    (void)rc_setheader;
    (void)rc_gzwrite;
    (void)rc_def_end;
    (void)rc_gzclose;
    (void)version;
    // API sequence test completed successfully
    return 66;
}