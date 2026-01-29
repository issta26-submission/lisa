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
//<ID> 1481
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source data and compute deflate bound)
    const Bytef src[] = "zlib sequence: compute deflate bound, compress, write gz via gzopen64/gzseek64, read back and inflate";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    uLong bound = deflateBound(&dstrm, src_len);

    // step 2: Configure (compress into a buffer using compress2)
    uLongf comp_len = (uLongf)bound;
    Bytef * comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);

    // step 3: Operate (write compressed data to a gzip file using gzopen64 and gzseek64, then read it back)
    gzFile gw = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gw_write_ret = gzwrite(gw, (void *)comp_buf, (unsigned int)((comp_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)comp_len));
    off64_t seek_ret = gzseek64(gw, (off64_t)0, 0);
    int gw_close_ret = gzclose(gw);
    gzFile gr = gzopen64("test_zlib_api_sequence.gz", "rb");
    unsigned int read_buf_size = (unsigned int)((bound > 4096UL) ? bound : 4096UL);
    Bytef * read_buf = (Bytef *)malloc((size_t)read_buf_size);
    memset(read_buf, 0, (int)read_buf_size);
    int read_ret = gzread(gr, (void *)read_buf, read_buf_size);

    // step 4: Validate & Cleanup (initialize inflate stream, point to read buffer, attempt a single inflate call, cleanup)
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    const char * version = zlibVersion();
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = read_buf;
    istrm.avail_in = (uInt)((read_ret > 0) ? (unsigned int)read_ret : 0u);
    uLong out_len = src_len + 64;
    Bytef * out_buf = (Bytef *)malloc((size_t)out_len);
    memset(out_buf, 0, (int)out_len);
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)((out_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)out_len);
    int inflate_ret = inflate(&istrm, 0);
    int inf_end_ret = inflateEnd(&istrm);
    int gr_close_ret = gzclose_r(gr);
    free(comp_buf);
    free(read_buf);
    free(out_buf);
    (void)bound;
    (void)comp_ret;
    (void)comp_len;
    (void)gw_write_ret;
    (void)seek_ret;
    (void)gw_close_ret;
    (void)read_ret;
    (void)version;
    (void)inf_init_ret;
    (void)inflate_ret;
    (void)inf_end_ret;
    (void)gr_close_ret;
    // API sequence test completed successfully
    return 66;
}