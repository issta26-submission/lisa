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
//<ID> 450
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress into a buffer
    const char src[] = "zlib API fuzzer payload: prepare -> compress -> write -> inflateInit_ -> validate -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);
    uLong crc_src = crc32_z(0UL, source, (z_size_t)sourceLen);

    // step 2: Open a gz file via gzopen64 and write the compressed buffer with gzfwrite
    gzFile gz = gzopen64("zlib_fuzz_out.gz", "wb");
    z_size_t wrote = gzfwrite((voidpc)comp, (z_size_t)1, (z_size_t)compLen, gz);
    int rc_gzclose = gzclose(gz);

    // step 3: Initialize an inflate stream, attach compressed buffer, and call inflateValidate
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_inf_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    int rc_validate = inflateValidate(&strm, 1);
    int rc_inf_end = inflateEnd(&strm);

    // step 4: Cleanup
    free(comp);
    (void)rc_comp;
    (void)crc_src;
    (void)wrote;
    (void)rc_gzclose;
    (void)rc_inf_init;
    (void)rc_validate;
    (void)rc_inf_end;
    (void)version;
    (void)sourceLen;
    (void)bound;
    (void)source;
    // API sequence test completed successfully
    return 66;
}