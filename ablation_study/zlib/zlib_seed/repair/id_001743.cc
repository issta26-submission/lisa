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
//<ID> 1743
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source, CRC, and deflate stream)
    const Bytef src[] = "zlib API sequence test: deflate -> gzwrite -> gzread -> inflate -> inflateGetDictionary -> crc32";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *ver = zlibVersion();
    uLong crc_before = crc32(0UL, src, (uInt)src_len);
    z_stream defstrm;
    memset(&defstrm, 0, (int)sizeof(defstrm));
    int def_init_ret = deflateInit_(&defstrm, 6, ver, (int)sizeof(z_stream));
    uLong bound = compressBound(src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);

    // step 2: Configure & Operate (perform deflate)
    defstrm.next_in = (Bytef *)src;
    defstrm.avail_in = (uInt)src_len;
    defstrm.next_out = comp_buf;
    defstrm.avail_out = (uInt)bound;
    int def_ret = deflate(&defstrm, 4);
    uLong comp_len = defstrm.total_out;
    int def_end_ret = deflateEnd(&defstrm);

    // step 3: Write compressed data to gzFile and read it back
    gzFile gzf_w = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int gw_ret = gzwrite(gzf_w, comp_buf, (unsigned int)comp_len);
    int gf_ret = gzflush(gzf_w, 2);
    int gzclose_w = gzclose(gzf_w);
    gzFile gzf_r = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)(comp_len + 16));
    memset(read_buf, 0, (int)(comp_len + 16));
    int read_bytes = gzread(gzf_r, read_buf, (unsigned int)comp_len);
    int gzclose_r_read = gzclose(gzf_r);

    // step 4: Inflate, retrieve dictionary, compute CRC, and cleanup
    z_stream infstrm;
    memset(&infstrm, 0, (int)sizeof(infstrm));
    int inf_init_ret = inflateInit_(&infstrm, ver, (int)sizeof(z_stream));
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(decomp_buf, 0, (int)(src_len + 16));
    infstrm.next_in = read_buf;
    infstrm.avail_in = (uInt)read_bytes;
    infstrm.next_out = decomp_buf;
    infstrm.avail_out = (uInt)(src_len + 16);
    int inf_ret = inflate(&infstrm, 0);
    Bytef dict_buf[64];
    memset(dict_buf, 0, (int)sizeof(dict_buf));
    uInt dict_len = (uInt)sizeof(dict_buf);
    int dict_ret = inflateGetDictionary(&infstrm, dict_buf, &dict_len);
    uLong crc_after = crc32(0UL, decomp_buf, (uInt)src_len);
    int inf_end_ret = inflateEnd(&infstrm);

    free(comp_buf);
    free(read_buf);
    free(decomp_buf);
    (void)ver;
    (void)crc_before;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)def_end_ret;
    (void)gw_ret;
    (void)gf_ret;
    (void)gzclose_w;
    (void)read_bytes;
    (void)gzclose_r_read;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)dict_ret;
    (void)dict_len;
    (void)crc_after;
    (void)inf_end_ret;
    // API sequence test completed successfully
    return 66;
}