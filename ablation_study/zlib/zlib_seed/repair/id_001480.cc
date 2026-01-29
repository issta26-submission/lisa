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
//<ID> 1480
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare data, streams and compute bound)
    const Bytef src[] = "zlib sequence test: compress with compress2, write with gzopen64, read with gzseek64, init inflate";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream dstrm;
    z_stream istrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    memset(&istrm, 0, (int)sizeof(istrm));
    const char * version = zlibVersion();
    int dinit_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, src_len);
    Bytef * comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);

    // step 2: Configure (write compressed data to a gz file using gzopen64)
    gzFile gw = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gw_write_ret = gzwrite(gw, (void *)comp_buf, (unsigned int)((comp_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)comp_len));
    int gw_close_ret = gzclose(gw);

    // step 3: Operate (open with gzopen64, seek with gzseek64, read and initialize inflate)
    gzFile gr = gzopen64("test_zlib_api_sequence.gz", "rb");
    off64_t seek_ret = gzseek64(gr, (off64_t)0, 0);
    unsigned int read_buf_size = (unsigned int)((bound > 4096UL) ? bound : 4096UL);
    Bytef * read_buf = (Bytef *)malloc((size_t)read_buf_size);
    memset(read_buf, 0, (int)read_buf_size);
    int read_ret = gzread(gr, (void *)read_buf, read_buf_size);
    int iinit_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    Bytef * out_buf = (Bytef *)malloc((size_t)(src_len + 8));
    memset(out_buf, 0, (int)(src_len + 8));
    uLongf out_len = src_len;
    int un_ret = uncompress(out_buf, &out_len, comp_buf, comp_len);

    // step 4: Validate & Cleanup (close files, end streams, free buffers)
    int gr_close_ret = gzclose(gr);
    int def_end_ret = deflateEnd(&dstrm);
    int inf_end_ret = inflateEnd(&istrm);
    free(comp_buf);
    free(read_buf);
    free(out_buf);
    (void)dinit_ret;
    (void)bound;
    (void)comp_len;
    (void)comp_ret;
    (void)gw_write_ret;
    (void)gw_close_ret;
    (void)seek_ret;
    (void)read_ret;
    (void)iinit_ret;
    (void)un_ret;
    (void)gr_close_ret;
    (void)def_end_ret;
    (void)inf_end_ret;
    (void)version;
    // API sequence test completed successfully
    return 66;
}