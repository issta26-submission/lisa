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
//<ID> 449
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

    // step 2: Initialize a deflate stream, configure a gzip header, apply header, and finish the stream
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, sizeof(gz_header));
    head.text = 1;
    head.time = (uLong)123456789;
    head.name = (Bytef *)"example_name";
    head.comment = (Bytef *)"example_comment";
    int rc_sethead = deflateSetHeader(&dstrm, &head);
    int rc_def_end = deflateEnd(&dstrm);

    // step 3: Uncompress the compressed buffer and write the result to a gz stream obtained via gzdopen, then clear errors
    uLongf destLen = sourceLen;
    Bytef *dest = (Bytef *)malloc((size_t)destLen);
    memset(dest, 0, (size_t)destLen);
    int rc_un = uncompress(dest, &destLen, comp, (uLong)compLen);
    gzFile gz = gzdopen(1, "wb");
    int rc_gzwrite = gzwrite(gz, (voidpc)dest, (unsigned int)destLen);
    gzclearerr(gz);
    int rc_gzclose = gzclose(gz);

    // step 4: Cleanup
    free(comp);
    free(dest);
    (void)version;
    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_sethead;
    (void)rc_def_end;
    (void)rc_un;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}