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
//<ID> 1140
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize source data and deflate stream, compute bound and allocate buffers
    const Bytef source[] = "zlib API sequence fuzz driver payload exercising deflateBound/gzbuffer/inflateResetKeep/inflateEnd usage.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);

    // step 2: Compress source into comp_buf and finalize deflate
    dstrm.next_in = (Bytef *)source;
    dstrm.avail_in = (uInt)source_len;
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)bound;
    int def_ret = deflate(&dstrm, 4);
    uLong comp_size = dstrm.total_out;
    int def_end_ret = deflateEnd(&dstrm);

    // step 3: Persist original source into a .gz file using gzwrite with gzbuffer, then read it back
    gzFile gw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzbuf_w = gzbuffer(gw, 8192);
    int gzwrite_ret = gzwrite(gw, source, (unsigned int)source_len);
    int gzclose_w_ret = gzclose(gw);
    gzFile gr = gzopen("tmp_zlib_api_seq.gz", "rb");
    int gzbuf_r = gzbuffer(gr, 4096);
    uLong read_alloc = source_len + 128;
    Bytef *read_buf = (Bytef *)malloc((size_t)read_alloc);
    memset(read_buf, 0, (size_t)read_alloc);
    int gzread_ret = gzread(gr, read_buf, (unsigned int)read_alloc);
    int gzeof_ret = gzeof(gr);
    int gzclose_r_ret = gzclose(gr);

    // step 4: Initialize inflate stream, use inflateResetKeep, inflate compressed buffer into out_buf, cleanup
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    int inf_reset_ret = inflateResetKeep(&istrm);
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_size > (uLong)0xFFFFFFFFU ? 0xFFFFFFFFU : (uInt)comp_size);
    uLong out_alloc = source_len + 256;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(out_buf, 0, (size_t)out_alloc);
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)out_alloc;
    int inf_ret = inflate(&istrm, 4);
    uLong out_size = istrm.total_out;
    int inf_end_ret = inflateEnd(&istrm);

    uLong checksum = crc32(0UL, read_buf, (uInt)(gzread_ret > 0 ? (unsigned int)gzread_ret : 0U));

    free(comp_buf);
    free(read_buf);
    free(out_buf);

    (void)def_init_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)bound;
    (void)version;
    (void)gzbuf_w;
    (void)gzwrite_ret;
    (void)gzclose_w_ret;
    (void)gzbuf_r;
    (void)gzread_ret;
    (void)gzeof_ret;
    (void)gzclose_r_ret;
    (void)inf_init_ret;
    (void)inf_reset_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)comp_size;
    (void)out_size;
    (void)checksum;
    return 66; // API sequence test completed successfully
}