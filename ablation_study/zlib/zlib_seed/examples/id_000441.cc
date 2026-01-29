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
//<ID> 441
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress into a buffer
    const char src[] = "zlib API sequence payload: init -> header -> compress -> uncompress -> gz IO -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Initialize a deflate stream and attach a gzip header via deflateSetHeader
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, sizeof(gz_header));
    Bytef *name_buf = (Bytef *)malloc(32);
    memset(name_buf, 0, 32);
    memcpy(name_buf, "test_name", 9);
    head.name = name_buf;
    head.name_max = 32;
    int rc_def_set_head = deflateSetHeader(&dstrm, &head);
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Uncompress the compressed buffer and write the result to a gz stream from fd 1
    uLongf destCap = sourceLen + 16;
    Bytef *dest = (Bytef *)malloc((size_t)destCap);
    memset(dest, 0, (size_t)destCap);
    int rc_un = uncompress(dest, &destCap, comp, (uLong)compLen);
    gzFile gz = gzdopen(1, "wb");
    gzclearerr(gz);
    int rc_gzwrite = gzwrite(gz, (voidpc)dest, (unsigned int)destCap);
    int rc_gzclose = gzclose(gz);

    // step 4: Cleanup resources
    free(comp);
    free(dest);
    free(name_buf);
    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_def_set_head;
    (void)rc_def_end;
    (void)rc_un;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)version;
    // API sequence test completed successfully
    return 66;
}