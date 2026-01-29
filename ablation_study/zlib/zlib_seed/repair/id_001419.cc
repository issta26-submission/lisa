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
//<ID> 1419
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source data and write a gzip file)
    const Bytef src[] = "Zlib API sequence test data: gz write, seek64, read, rewind, inflate header, deflate reset";
    uLong src_len = (uLong)(sizeof(src) - 1);
    gzFile gzw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw_ret = gzwrite(gzw, src, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    int gzc_ret = gzclose(gzw);

    // step 2: Configure (initialize deflate and inflate streams and request header handling)
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int def_reset_ret = deflateReset(&dstrm);
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, (int)sizeof(head));
    int inf_get_head_ret = inflateGetHeader(&istrm, &head);

    // step 3: Operate (open for reading, seek with gzseek64, read into buffer, then rewind)
    gzFile gzr = gzopen("test_zlib_api_sequence.gz", "rb");
    off64_t seek_ret = gzseek64(gzr, (off64_t)1, 0); // 0 == SEEK_SET
    unsigned int read_buf_size = 256;
    Bytef *read_buf = (Bytef *)malloc((size_t)read_buf_size);
    memset(read_buf, 0, (int)read_buf_size);
    int gzread_ret = gzread(gzr, (voidp)read_buf, (unsigned int)(read_buf_size - 1));
    int g_rewind_ret = gzrewind(gzr);
    istrm.next_in = read_buf;
    istrm.avail_in = (uInt)(gzread_ret > 0 ? (unsigned int)gzread_ret : 0u);

    // step 4: Validate & Cleanup (end streams, close file, free buffers)
    int def_end_ret = deflateEnd(&dstrm);
    int inf_end_ret = inflateEnd(&istrm);
    int gzc2_ret = gzclose(gzr);
    (void)gw_ret;
    (void)gzc_ret;
    (void)def_init_ret;
    (void)def_reset_ret;
    (void)inf_init_ret;
    (void)inf_get_head_ret;
    (void)seek_ret;
    (void)gzread_ret;
    (void)g_rewind_ret;
    (void)def_end_ret;
    (void)inf_end_ret;
    (void)gzc2_ret;
    (void)version;
    free(read_buf);
    // API sequence test completed successfully
    return 66;
}