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
//<ID> 1871
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize & Prepare data
    const Bytef src[] = "zlib API sequence test: deflate -> gzopen/gzwrite -> gzungetc -> inflateMark";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream defstrm;
    memset(&defstrm, 0, (int)sizeof(defstrm));
    const char *ver = zlibVersion();
    int def_init_ret = deflateInit_(&defstrm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure compressor and buffers
    defstrm.next_in = (Bytef *)src;
    defstrm.avail_in = (uInt)src_len;
    uLong bound = deflateBound(&defstrm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    defstrm.next_out = comp_buf;
    defstrm.avail_out = (uInt)bound;
    int def_ret = deflate(&defstrm, 4);
    uLong comp_len = defstrm.total_out;

    // step 3: Operate - write compressed data to a gzip file and manipulate gz file
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gz_wr = gzwrite(gzf, comp_buf, (unsigned int)(comp_len));
    int gz_unret = gzungetc('Z', gzf);

    // step 4: Inflate mark & Cleanup
    z_stream infstrm;
    memset(&infstrm, 0, (int)sizeof(infstrm));
    int inf_init_ret = inflateInit_(&infstrm, ver, (int)sizeof(z_stream));
    infstrm.next_in = comp_buf;
    infstrm.avail_in = (uInt)comp_len;
    long mark = inflateMark(&infstrm);
    int def_end_ret = deflateEnd(&defstrm);
    int inf_end_ret = inflateEnd(&infstrm);
    int gz_close_ret = gzclose(gzf);
    free(comp_buf);
    (void)def_init_ret;
    (void)def_ret;
    (void)bound;
    (void)gz_wr;
    (void)gz_unret;
    (void)inf_init_ret;
    (void)mark;
    (void)def_end_ret;
    (void)inf_end_ret;
    (void)gz_close_ret;
    (void)ver;
    // API sequence test completed successfully
    return 66;
}