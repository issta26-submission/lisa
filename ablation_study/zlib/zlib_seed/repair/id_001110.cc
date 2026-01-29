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
//<ID> 1110
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize source buffer and compress into an output buffer
    const Bytef source[] = "zlib API sequence fuzz driver payload for compress -> inflate -> gz file operations.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    uLong bound = compressBound(source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, source, source_len, 6);

    // step 2: Initialize inflate stream and prepare output buffer
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    const char *version = zlibVersion();
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_len;
    uLong out_alloc = source_len + 128;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(out_buf, 0, (size_t)out_alloc);
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)out_alloc;

    // step 3: Perform inflate and then end the inflate stream
    int inf_ret = inflate(&istrm, 4);
    uLong out_size = istrm.total_out;
    int inf_end_ret = inflateEnd(&istrm);

    // step 4: Persist compressed bytes to a .gz file, rewind, close, validate and cleanup
    gzFile gz = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gz, comp_buf, (unsigned int)comp_len);
    int gzrewind_ret = gzrewind(gz);
    int gzclose_ret = gzclose(gz);
    uLong checksum = crc32(0UL, comp_buf, (uInt)comp_len);
    free(comp_buf);
    free(out_buf);
    (void)comp_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)out_size;
    (void)inf_end_ret;
    (void)gzwrite_ret;
    (void)gzrewind_ret;
    (void)gzclose_ret;
    (void)checksum;
    (void)version;
    (void)bound;
    (void)source_len;
    return 66;
    // API sequence test completed successfully
}