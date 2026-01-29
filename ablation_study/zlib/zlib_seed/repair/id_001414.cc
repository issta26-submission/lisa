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
//<ID> 1414
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "Test data for zlib sequence using gzseek64 and inflateGetHeader";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLongf comp_destLen = (uLongf)bound;
    int comp_ret = compress2(comp_buf, &comp_destLen, src, src_len, 6);
    uLong comp_len = (uLong)comp_destLen;

    // step 2: Configure (write compressed data to .gz then reopen for reading)
    gzFile gwf = gzopen64("test_zlib_api_sequence.gz", "wb");
    unsigned int write_len = (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFUL : comp_len);
    int gw = gzwrite(gwf, comp_buf, write_len);
    int gwclose = gzclose(gwf);

    gzFile gr = gzopen64("test_zlib_api_sequence.gz", "rb");
    int grewind = gzrewind(gr);
    off64_t seekpos = gzseek64(gr, (off64_t)0, 0);
    uInt read_buf_size = (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFUL : comp_len);
    Bytef *read_buf = (Bytef *)malloc((size_t)read_buf_size);
    memset(read_buf, 0, (int)read_buf_size);
    int grd = gzread(gr, read_buf, read_buf_size);

    // step 3: Operate (inflate using read buffer and inspect gz header; also exercise deflateReset)
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int inf_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = read_buf;
    istrm.avail_in = (uInt)read_buf_size;
    uInt decomp_size = (uInt)((src_len + 16) > 0xFFFFFFFFUL ? 0xFFFFFFFFUL : src_len + 16);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)decomp_size);
    memset(decomp_buf, 0, (int)decomp_size);
    istrm.next_out = decomp_buf;
    istrm.avail_out = decomp_size;
    gz_header head;
    memset(&head, 0, (int)sizeof(head));
    int gethead = inflateGetHeader(&istrm, &head);
    int inf_ret = inflate(&istrm, 0);

    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    int def_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int def_reset = deflateReset(&dstrm);
    int def_end = deflateEnd(&dstrm);

    // step 4: Cleanup
    int inf_end = inflateEnd(&istrm);
    int grclose = gzclose(gr);
    free(comp_buf);
    free(read_buf);
    free(decomp_buf);
    (void)comp_ret;
    (void)comp_len;
    (void)gw;
    (void)gwclose;
    (void)grewind;
    (void)seekpos;
    (void)grd;
    (void)inf_init;
    (void)gethead;
    (void)inf_ret;
    (void)def_init;
    (void)def_reset;
    (void)def_end;
    (void)inf_end;
    (void)version;
    // API sequence test completed successfully
    return 66;
}