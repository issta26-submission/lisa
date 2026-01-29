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
//<ID> 1413
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "Zlib API sequence: compress -> deflateReset -> write gz -> seek64 -> read -> inflateGetHeader";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf destLen = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &destLen, src, src_len, 6);
    uLong comp_len = (uLong)destLen;

    // step 2: Configure
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int def_reset_ret = deflateReset(&dstrm);
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, (int)sizeof(head));
    int inf_get_head_ret = inflateGetHeader(&istrm, &head);

    // step 3: Operate
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gwbytes = gzwrite(gw, comp_buf, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));
    int gwclose = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    off64_t seek_ret = gzseek64(gr, (off64_t)0, 0);
    int rewind_ret = gzrewind(gr);
    Bytef *read_buf = (Bytef *)malloc((size_t)comp_len + 1);
    memset(read_buf, 0, (int)(comp_len + 1));
    int grbytes = gzread(gr, read_buf, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));

    // step 4: Validate & Cleanup
    int def_end_ret = deflateEnd(&dstrm);
    int inf_end_ret = inflateEnd(&istrm);
    int grclose = gzclose(gr);
    (void)comp_ret;
    (void)def_init_ret;
    (void)def_reset_ret;
    (void)inf_init_ret;
    (void)inf_get_head_ret;
    (void)gwbytes;
    (void)gwclose;
    (void)seek_ret;
    (void)rewind_ret;
    (void)grbytes;
    (void)def_end_ret;
    (void)inf_end_ret;
    (void)grclose;
    (void)version;
    free(comp_buf);
    free(read_buf);
    // API sequence test completed successfully
    return 66;
}