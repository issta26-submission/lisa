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
//<ID> 405
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare payload
    const char src[] = "zlib API sequence payload for compress -> inflateSyncPoint -> gzwrite -> gzputc";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();

    // step 2: Compress the payload one-shot
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 3: Initialize an inflate stream and query sync point without performing inflate
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    int rc_sync = inflateSyncPoint(&strm);
    int rc_inf_end = inflateEnd(&strm);

    // step 4: Write the compressed data to a gzip file and add a single character, then cleanup
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, comp, (unsigned int)compLen);
    int rc_gzputc = gzputc(gzf, '\n');
    int rc_gzclose = gzclose(gzf);
    free(comp);

    (void)version;
    (void)rc_comp;
    (void)rc_inf_init;
    (void)rc_sync;
    (void)rc_inf_end;
    (void)rc_gzwrite;
    (void)rc_gzputc;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}