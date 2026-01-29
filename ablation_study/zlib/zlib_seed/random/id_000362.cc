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
//<ID> 362
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and prepare payload, version, and buffers
    const char payload[] = "zlib API sequence payload: compress2 + crc32_z + deflate -> gzwrite -> gzclose";
    const Bytef *src = (const Bytef *)payload;
    uLong srcLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(srcLen);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;

    // step 2: Compress with compress2 and compute CRC of original data
    int rc_comp = compress2(comp_buf, &comp_len, src, srcLen, 6);
    uLong crc = crc32_z(0UL, src, (z_size_t)srcLen);

    // step 3: Initialize a deflate stream, provide input and execute a deflate call
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(z_stream));
    int rc_def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)src;
    dstrm.avail_in = (uInt)srcLen;
    Bytef *def_out = (Bytef *)malloc((size_t)bound);
    memset(def_out, 0, (size_t)bound);
    dstrm.next_out = def_out;
    dstrm.avail_out = (uInt)bound;
    int rc_def = deflate(&dstrm, 0);

    // step 4: Write compressed output produced by compress2 to a gz file, close and cleanup
    const char *fname = "test_zlib_api_sequence.gz";
    gzFile gzf = gzopen(fname, "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)comp_buf, (unsigned int)comp_len);
    int rc_gzclose = gzclose(gzf);
    int rc_def_end = deflateEnd(&dstrm);
    free(comp_buf);
    free(def_out);

    (void)version;
    (void)rc_comp;
    (void)crc;
    (void)rc_def_init;
    (void)rc_def;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_def_end;

    // API sequence test completed successfully
    return 66;
}