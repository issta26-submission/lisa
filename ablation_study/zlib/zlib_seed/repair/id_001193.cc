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
//<ID> 1193
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const Bytef source[] = "zlib sequence payload for deflate -> gzwrite -> gzungetc -> gzread demonstration.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream dstream;
    memset(&dstream, 0, (int)sizeof(dstream));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&dstream, 6, version, (int)sizeof(z_stream));

    // step 2: Configure deflate, prime bit buffer, and compress
    dstream.next_in = (Bytef *)source;
    dstream.avail_in = (uInt)(source_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)source_len);
    uLong bound = deflateBound(&dstream, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    dstream.next_out = comp_buf;
    dstream.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);
    int prime_ret = deflatePrime(&dstream, 3, 5);
    int def_ret = deflate(&dstream, 4);
    uLong comp_size = dstream.total_out;
    int def_end_ret = deflateEnd(&dstream);

    // step 3: Persist compressed data into a gzip file and close writer
    gzFile gw = gzopen("zlib_test_stream.gz", "wb");
    unsigned int write_len = (unsigned int)((comp_size > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (uInt)comp_size);
    int gw_write_ret = gzwrite(gw, comp_buf, write_len);
    int gw_close_ret = gzclose(gw);

    // step 4: Reopen gzip file for reading, push a byte back with gzungetc, read data, and close read side
    gzFile gr = gzopen("zlib_test_stream.gz", "rb");
    int ungetc_ret = gzungetc(0x41, gr);
    Bytef *read_buf = (Bytef *)malloc((size_t)(comp_size + 16));
    memset(read_buf, 0, (int)(comp_size + 16));
    unsigned int read_len = (unsigned int)(((comp_size + 16) > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (uInt)(comp_size + 16));
    int gzread_ret = gzread(gr, read_buf, read_len);
    int gr_close_r = gzclose_r(gr);

    free(comp_buf);
    free(read_buf);
    int remove_ret = remove("zlib_test_stream.gz");

    (void)version;
    (void)def_init_ret;
    (void)prime_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)gw_write_ret;
    (void)gw_close_ret;
    (void)ungetc_ret;
    (void)gzread_ret;
    (void)gr_close_r;
    (void)remove_ret;
    (void)write_len;
    (void)read_len;
    (void)source_len;
    (void)comp_size;
    // API sequence test completed successfully
    return 66;
}