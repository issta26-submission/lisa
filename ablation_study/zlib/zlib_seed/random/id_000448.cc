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
//<ID> 448
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress into a buffer
    const char src[] = "zlib API sequence payload: prepare -> configure -> uncompress -> gz IO -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Initialize a deflate stream and configure a gzip header for it
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, sizeof(gz_header));
    head.text = 1;
    head.time = (uLong)123456789U;
    int rc_sethead = deflateSetHeader(&dstrm, &head);

    // step 3: Uncompress the previously produced compressed buffer into a destination buffer
    uLongf destLen = (uLongf)sourceLen;
    Bytef *dest = (Bytef *)malloc((size_t)sourceLen);
    memset(dest, 0, (size_t)sourceLen);
    int rc_un = uncompress(dest, &destLen, comp, (uLong)compLen);

    // step 4: Open a gzFile via file descriptor, clear any errors, write the uncompressed data, then cleanup
    gzFile gz = gzdopen(1, "wb");
    gzclearerr(gz);
    int rc_gzwrite = gzwrite(gz, (voidpc)dest, (unsigned int)destLen);
    int rc_gzclose = gzclose(gz);

    int rc_def_end = deflateEnd(&dstrm);
    free(comp);
    free(dest);
    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_sethead;
    (void)rc_un;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_def_end;
    (void)version;
    // API sequence test completed successfully
    return 66;
}