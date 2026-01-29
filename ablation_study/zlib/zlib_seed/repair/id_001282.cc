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
//<ID> 1282
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize and compress source data into an in-memory buffer
    const Bytef src[] = "zlib API sequence payload: compress to memory, write to gz, seek, read via inflate and query codes used";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(src_len);
    uLongf comp_len_f = (uLongf)bound;
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    int comp_ret = compress2(comp_buf, &comp_len_f, src, src_len, 6);
    uLong comp_len = (uLong)comp_len_f;

    // step 2: Write compressed data to a gz file, seek, and close the writer
    gzFile gzf_w = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    int gz_write_ret = gzwrite(gzf_w, comp_buf, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));
    off_t pos_after_write = gzseek(gzf_w, (off_t)0, 0);
    int gzclose_w_ret = gzclose_w(gzf_w);

    // step 3: Open the gz file for reading, seek to start, then inflate the in-memory compressed buffer and query inflateCodesUsed
    gzFile gzf_r = gzopen("test_zlib_api_sequence.tmp.gz", "rb");
    off_t pos_after_open = gzseek(gzf_r, (off_t)0, 0);
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    const char *version = zlibVersion();
    int init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    Bytef *out_buf = (Bytef *)malloc((size_t)src_len);
    memset(out_buf, 0, (int)src_len);
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_len);
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    int inf_ret = inflate(&istrm, 4);
    unsigned long codes_used = inflateCodesUsed(&istrm);

    // step 4: Cleanup resources and close the reader
    int inf_end_ret = inflateEnd(&istrm);
    int gzclose_r_ret = gzclose_r(gzf_r);
    free(comp_buf);
    free(out_buf);
    (void)comp_ret;
    (void)gz_write_ret;
    (void)pos_after_write;
    (void)gzclose_w_ret;
    (void)pos_after_open;
    (void)init_ret;
    (void)inf_ret;
    (void)codes_used;
    (void)inf_end_ret;
    (void)gzclose_r_ret;
    (void)version;
    // API sequence test completed successfully
    return 66;
}