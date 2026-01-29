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
//<ID> 1488
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source, compute bound, compress)
    const Bytef src[] = "zlib API sequence: compute deflateBound, compress, write gzip, read gzip, inflate compressed buffer";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream tmp;
    memset(&tmp, 0, (int)sizeof(tmp));
    uLong bound = deflateBound(&tmp, src_len);
    uLongf comp_len = (uLongf)bound;
    Bytef * comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);

    // step 2: Configure (write original data into a gzip file using gzopen64)
    gzFile gw = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gw_write_ret = gzwrite(gw, (void *)src, (unsigned int)((src_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)src_len));
    int gw_close_ret = gzclose(gw);

    // step 3: Operate (open with gzopen64, seek with gzseek64, read via gzread, initialize inflate and decompress compressed buffer)
    gzFile gr = gzopen64("test_zlib_api_sequence.gz", "rb");
    off64_t seek_ret = gzseek64(gr, (off64_t)0, 0);
    unsigned int read_buf_size = (unsigned int)((src_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)src_len);
    Bytef * read_buf = (Bytef *)malloc((size_t)read_buf_size);
    memset(read_buf, 0, (int)read_buf_size);
    int gz_read_ret = gzread(gr, (void *)read_buf, read_buf_size);
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    const char * version = zlibVersion();
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)((comp_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)comp_len);
    Bytef * out_buf = (Bytef *)malloc((size_t)src_len);
    memset(out_buf, 0, (int)src_len);
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)((src_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (unsigned int)src_len);
    int inf_ret = inflate(&istrm, 0);
    int inf_end_ret = inflateEnd(&istrm);

    // step 4: Validate & Cleanup (close file, free buffers and tidy up)
    int gr_close_ret = gzclose(gr);
    free(comp_buf);
    free(read_buf);
    free(out_buf);
    (void)comp_ret;
    (void)bound;
    (void)comp_len;
    (void)gw_write_ret;
    (void)gw_close_ret;
    (void)seek_ret;
    (void)gz_read_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)gr_close_ret;
    (void)version;
    // API sequence test completed successfully
    return 66;
}