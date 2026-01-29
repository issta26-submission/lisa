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
//<ID> 442
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    int deflate_init_ret = deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    unsigned char inflate_window[32768];
    memset(inflate_window, 0, sizeof(inflate_window));
    int inflate_back_init_ret = inflateBackInit_(&istrm, 15, inflate_window, ver, (int)sizeof(z_stream));

    // step 2: Configure
    Bytef source[] = "Example payload for zlib API sequence test.";
    uLong sourceLen = (uLong)(sizeof(source) - 1);
    uLong crc_src = crc32(0L, source, (uInt)sourceLen);
    uLong bound = deflateBound(&dstrm, sourceLen);
    uInt out_buf_size = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);
    Bytef *out_buf = (Bytef *)malloc((size_t)out_buf_size);
    memset(out_buf, 0, (size_t)out_buf_size);

    // step 3: Operate
    dstrm.next_in = source;
    dstrm.avail_in = (uInt)sourceLen;
    dstrm.next_out = out_buf;
    dstrm.avail_out = out_buf_size;
    int deflate_ret = deflate(&dstrm, 4); // 4 corresponds to Z_FINISH
    uLong compressed_len = dstrm.total_out;
    uLong crc_comp = crc32(0L, out_buf, (uInt)compressed_len);

    const char *filename = "test_zlib_api_sequence.tmp.gz";
    gzFile gz = gzopen(filename, "wb");
    int gzwrite_ret = gzwrite(gz, out_buf, (unsigned int)compressed_len);

    // step 4: Validate / Cleanup
    int gzclose_ret = gzclose(gz);
    int deflate_end_ret = deflateEnd(&dstrm);
    int inflate_back_end_ret = inflateBackEnd(&istrm);
    free(out_buf);
    (void)ver;
    (void)deflate_init_ret;
    (void)inflate_back_init_ret;
    (void)crc_src;
    (void)bound;
    (void)out_buf_size;
    (void)deflate_ret;
    (void)compressed_len;
    (void)crc_comp;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    (void)deflate_end_ret;
    (void)inflate_back_end_ret;
    // API sequence test completed successfully
    return 66;
}