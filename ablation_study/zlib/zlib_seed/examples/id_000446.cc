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
//<ID> 446
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress into a buffer
    const char src[] = "zlib API sequence payload: prepare -> deflateSetHeader -> uncompress -> gzdopen/gzwrite -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress2(comp, &compLen, source, sourceLen, 6);

    // step 2: Initialize deflate stream and attach a gz_header via deflateSetHeader
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_def_init = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, sizeof(gz_header));
    char *name = (char *)malloc(32);
    memset(name, 0, 32);
    memcpy(name, "zlib_test_name", 14);
    head.name = (Bytef *)name;
    head.name_max = 32;
    head.comment = (Bytef *)NULL;
    head.comment = (Bytef *)malloc(1);
    memset(head.comment, 0, 1);
    head.comm_max = 1;
    int rc_sethdr = deflateSetHeader(&strm, &head);

    // step 3: Uncompress compressed buffer and write result to a gz stream opened from fd 1
    uLongf destCap = sourceLen * 2;
    Bytef *dest = (Bytef *)malloc((size_t)destCap);
    memset(dest, 0, (size_t)destCap);
    int rc_uncomp = uncompress(dest, &destCap, comp, (uLong)compLen);
    gzFile gf = gzdopen(1, "wb");
    int rc_gzwrite = gzwrite(gf, dest, (unsigned int)destCap);
    gzclearerr(gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup
    int rc_def_end = deflateEnd(&strm);
    free(comp);
    free(dest);
    free(name);
    free(head.comment);
    (void)rc_comp;
    (void)rc_def_init;
    (void)rc_sethdr;
    (void)rc_uncomp;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_def_end;
    (void)version;
    (void)compLen;
    (void)destCap;
    (void)sourceLen;
    // API sequence test completed successfully
    return 66;
}