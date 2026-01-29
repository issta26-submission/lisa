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
//<ID> 445
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress into a buffer
    const char src[] = "zlib API sequence payload: prepare -> compress -> set header -> uncompress -> gz IO -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Configure a z_stream and a gz_header and associate them via deflateSetHeader
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    gz_header head;
    memset(&head, 0, sizeof(gz_header));
    head.name = (Bytef *)source;
    head.name_max = (uInt)sourceLen;
    head.comment = (Bytef *)source;
    head.comm_max = (uInt)sourceLen;
    int rc_sethead = deflateSetHeader(&strm, &head);

    // step 3: Uncompress the compressed buffer and write the result to a gzFile obtained via gzdopen
    uLongf destLen = (uLongf)sourceLen;
    Bytef *dest = (Bytef *)malloc((size_t)destLen);
    memset(dest, 0, (size_t)destLen);
    int rc_uncomp = uncompress(dest, &destLen, comp, (uLong)compLen);

    gzFile gz = gzdopen(1, "wb");
    int rc_gzwrite = gzwrite(gz, (voidpc)dest, (unsigned int)destLen);
    gzclearerr(gz);

    // step 4: Cleanup resources
    int rc_gzclose = gzclose(gz);
    free(comp);
    free(dest);
    (void)rc_comp;
    (void)rc_sethead;
    (void)rc_uncomp;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)compLen;
    (void)destLen;
    (void)sourceLen;
    // API sequence test completed successfully
    return 66;
}