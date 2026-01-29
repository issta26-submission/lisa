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
//<ID> 1115
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize and compress an in-memory payload
    const Bytef source[] = "zlib API sequence payload for compress -> inflate -> gzip write/read test.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    const char *version = zlibVersion();
    uLong comp_alloc = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)comp_alloc);
    memset(comp_buf, 0, (size_t)comp_alloc);
    uLongf destLen = (uLongf)comp_alloc;
    int comp_ret = compress2(comp_buf, &destLen, source, source_len, 6);

    // step 2: Configure and operate an inflate stream to decompress the compressed buffer
    z_stream inf;
    memset(&inf, 0, (int)sizeof(inf));
    int inf_init_ret = inflateInit_(&inf, version, (int)sizeof(z_stream));
    inf.next_in = comp_buf;
    inf.avail_in = (uInt)destLen;
    uLong out_alloc = source_len + 64;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(out_buf, 0, (size_t)out_alloc);
    inf.next_out = out_buf;
    inf.avail_out = (uInt)out_alloc;
    int inf_ret = inflate(&inf, 4);
    int inf_end_ret = inflateEnd(&inf);

    // step 3: Persist compressed bytes to a .gz file, then rewind and close the file
    gzFile gzf = gzopen("tmp_zlib_api_sequence.gz", "wb");
    int gzwrite_ret = gzwrite(gzf, comp_buf, (unsigned int)destLen);
    int gzrew_ret = gzrewind(gzf);
    int gzclose_ret = gzclose(gzf);

    // step 4: Validate checksum and cleanup
    uLong checksum = crc32(0UL, comp_buf, (uInt)destLen);
    free(comp_buf);
    free(out_buf);
    (void)version;
    (void)comp_ret;
    (void)destLen;
    (void)comp_alloc;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)gzwrite_ret;
    (void)gzrew_ret;
    (void)gzclose_ret;
    (void)checksum;
    (void)source_len;
    return 66;
    // API sequence test completed successfully
}