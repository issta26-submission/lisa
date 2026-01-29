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
//<ID> 1231
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const Bytef source[] = "Payload for zlib API sequence demonstrating deflate, inflate, header retrieval and gz writing";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    z_stream inf_strm;
    gz_header gzhead;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    memset(&gzhead, 0, (int)sizeof(gzhead));
    const char *version = zlibVersion();

    // step 2: Initialize and configure streams, compute bounds and allocate buffers
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)(source_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)source_len);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);
    uLong out_alloc = source_len + 512;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(out_buf, 0, (int)out_alloc);

    // step 3: Core operations — compress, initialize inflate, get header, inflate, write to gz, rewind and direct check
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_size);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)(out_alloc > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)out_alloc);
    int get_head_ret = inflateGetHeader(&inf_strm, &gzhead);
    int inf_ret = inflate(&inf_strm, 4);
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gz_write_ret = gzwrite(gzf, comp_buf, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_size));
    int gz_rewind_ret = gzrewind(gzf);
    int gz_direct_ret = gzdirect(gzf);

    // step 4: Cleanup
    int def_end_ret = deflateEnd(&def_strm);
    int inf_end_ret = inflateEnd(&inf_strm);
    int gzclose_ret = gzclose(gzf);
    free(comp_buf);
    free(out_buf);
    free(gzhead.extra);
    free(gzhead.name);
    free(gzhead.comment);
    (void)version;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_size;
    (void)inf_init_ret;
    (void)get_head_ret;
    (void)inf_ret;
    (void)gz_write_ret;
    (void)gz_rewind_ret;
    (void)gz_direct_ret;
    (void)def_end_ret;
    (void)inf_end_ret;
    (void)gzclose_ret;
    // API sequence test completed successfully
    return 66;
}