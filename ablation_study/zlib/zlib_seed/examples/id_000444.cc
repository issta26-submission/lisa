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
//<ID> 444
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and produce a compressed buffer with compress()
    const char src[] = "zlib API sequence payload: prepare -> deflateSetHeader -> uncompress -> gz IO -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Initialize a deflate stream configured for gzip output and attach a gz_header
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_deflate_init = deflateInit2_(&dstrm, 6, 8, 31, 8, 0, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, sizeof(gz_header));
    head.text = 1;
    head.time = (uLong)123456789U;
    head.xflags = 0;
    head.os = 255;
    head.name = (Bytef *)"test_name";
    head.name_max = 0;
    head.comment = (Bytef *)"test_comment";
    head.comm_max = 0;
    int rc_sethead = deflateSetHeader(&dstrm, &head);

    // step 3: Decompress the compressed buffer, write the decompressed data to a gz stream on fd 1, and clear errors
    uLong outCap = sourceLen + 64;
    Bytef *dest = (Bytef *)malloc((size_t)outCap);
    memset(dest, 0, (size_t)outCap);
    uLongf destLen = (uLongf)outCap;
    int rc_uncomp = uncompress(dest, &destLen, comp, (uLong)compLen);
    gzFile gz = gzdopen(1, "wb");
    int rc_gzwrite = gzwrite(gz, dest, (unsigned int)destLen);
    gzclearerr(gz);
    int rc_gzclose = gzclose(gz);

    // step 4: Cleanup resources and finalization
    int rc_deflate_end = deflateEnd(&dstrm);
    free(comp);
    free(dest);
    (void)version;
    (void)rc_comp;
    (void)rc_deflate_init;
    (void)rc_sethead;
    (void)rc_uncomp;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}