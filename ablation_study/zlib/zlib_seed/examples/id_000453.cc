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
//<ID> 453
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress into a buffer
    const char src[] = "zlib API sequence payload: compress -> validate stream -> crc -> gzwrite -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Initialize a z_stream, assign compressed input and run inflateValidate
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    strm.next_in = comp;
    strm.avail_in = (uInt)compLen;
    int rc_inf_valid = inflateValidate(&strm, 15);

    // step 3: Compute CRC of compressed data, open a gz file and write the original uncompressed data using gzfwrite
    uLong crc = crc32_z(0L, comp, (z_size_t)compLen);
    gzFile gf = gzopen64("zlib_api_sequence_out.gz", "wb");
    z_size_t written = gzfwrite((voidpc)source, (z_size_t)1, (z_size_t)sourceLen, gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup
    free(comp);
    (void)rc_comp;
    (void)rc_inf_valid;
    (void)crc;
    (void)written;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}