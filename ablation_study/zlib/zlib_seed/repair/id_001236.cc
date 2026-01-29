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
//<ID> 1236
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const Bytef source[] = "Payload for zlib API sequence: compress, inflate, header, and gz operations.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    const char *version = zlibVersion();
    z_stream def_strm;
    z_stream inf_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    memset(&inf_strm, 0, (int)sizeof(inf_strm));
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure and perform deflate (compression)
    uLong bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)(source_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)source_len);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 3: Initialize inflate, attach header, and perform inflate (decompression)
    int inf_init_ret = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, (int)sizeof(head));
    int hdr_ret = inflateGetHeader(&inf_strm, &head);
    uLong out_alloc = source_len + 256;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(out_buf, 0, (int)out_alloc);
    inf_strm.next_in = comp_buf;
    inf_strm.avail_in = (uInt)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_size);
    inf_strm.next_out = out_buf;
    inf_strm.avail_out = (uInt)(out_alloc > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)out_alloc);
    int inf_ret = inflate(&inf_strm, 4);
    uLong out_size = inf_strm.total_out;
    int inf_end_ret = inflateEnd(&inf_strm);

    // step 4: gzFile operations: write compressed data, rewind, and check direct mode, then cleanup
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");
    int gz_write_ret = gzf ? gzwrite(gzf, comp_buf, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_size)) : 0;
    int gz_rewind_ret = gzf ? gzrewind(gzf) : 0;
    int gz_direct_ret = gzf ? gzdirect(gzf) : 0;
    int gz_close_ret = gzf ? gzclose(gzf) : 0;
    free(comp_buf);
    free(out_buf);
    (void)version;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)def_end_ret;
    (void)inf_init_ret;
    (void)hdr_ret;
    (void)inf_ret;
    (void)out_size;
    (void)inf_end_ret;
    (void)gz_write_ret;
    (void)gz_rewind_ret;
    (void)gz_direct_ret;
    (void)gz_close_ret;
    // API sequence test completed successfully
    return 66;
}