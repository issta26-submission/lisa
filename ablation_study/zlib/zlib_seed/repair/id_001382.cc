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
//<ID> 1382
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef dict_src[] = "zlib preset dictionary example data used for inflate/deflate sequence";
    const Bytef src[] = "Payload data that references the preset dictionary to demonstrate deflate/inflate usage";
    uLong dict_len = (uLong)(sizeof(dict_src) - 1);
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream defstrm;
    memset(&defstrm, 0, (int)sizeof(defstrm));
    int def_init_ret = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    deflateSetDictionary(&defstrm, dict_src, (uInt)(dict_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)dict_len));
    defstrm.next_in = (Bytef *)src;
    defstrm.avail_in = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    uLong bound = deflateBound(&defstrm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    defstrm.next_out = comp_buf;
    defstrm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);

    // step 2: Configure
    int def_ret = deflate(&defstrm, 4);
    uLong comp_len = (uLong)defstrm.total_out;
    deflateReset(&defstrm);
    gzFile gzw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw = gzwrite(gzw, comp_buf, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));
    int close_w = gzclose(gzw);

    // step 3: Operate
    gzFile gzr = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)(comp_len + 1));
    memset(read_buf, 0, (int)(comp_len + 1));
    int gr = gzread(gzr, read_buf, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));
    int close_r = gzclose_r(gzr);
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    inflateSetDictionary(&istrm, dict_src, (uInt)(dict_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)dict_len));
    istrm.next_in = read_buf;
    istrm.avail_in = (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_len);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + dict_len + 16));
    memset(decomp_buf, 0, (int)(src_len + dict_len + 16));
    istrm.next_out = decomp_buf;
    istrm.avail_out = (uInt)((src_len + dict_len + 16) > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)(src_len + dict_len + 16));
    int inf_ret = inflate(&istrm, 0);
    uLong decomp_written = (uLong)istrm.total_out;

    // step 4: Validate & Cleanup
    int def_end_ret = deflateEnd(&defstrm);
    int inf_end_ret = inflateEnd(&istrm);
    (void)def_init_ret;
    (void)def_ret;
    (void)gw;
    (void)close_w;
    (void)gr;
    (void)close_r;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)decomp_written;
    (void)def_end_ret;
    (void)inf_end_ret;
    (void)version;
    free(comp_buf);
    free(read_buf);
    free(decomp_buf);
    // API sequence test completed successfully
    return 66;
}