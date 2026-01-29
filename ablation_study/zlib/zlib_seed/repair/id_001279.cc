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
//<ID> 1279
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src_data[] = "zlib API sequence payload: tune deflater, set gzip header, deflate, write via gzopen64 and query position";
    uLong src_len = (uLong)(sizeof(src_data) - 1);
    const char *version = zlibVersion();
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure
    gz_header head;
    memset(&head, 0, (int)sizeof(head));
    head.text = 1;
    const char name_buf[] = "test_name";
    head.name = (Bytef *)malloc((size_t)(sizeof(name_buf)));
    memset(head.name, 0, (int)sizeof(name_buf));
    memcpy(head.name, name_buf, sizeof(name_buf));
    head.name_max = (uInt)(sizeof(name_buf));
    deflateSetHeader(&strm, &head);
    int tune_ret = deflateTune(&strm, 32, 64, 258, 4096);

    // step 3: Operate
    uLong bound = deflateBound(&strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    strm.next_in = (Bytef *)src_data;
    strm.avail_in = (uInt)src_len;
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;
    int def_ret = deflate(&strm, 4);
    uLong comp_size = strm.total_out;
    gzFile gzf = gzopen64("test_zlib_api_sequence.gz", "wb");
    int gz_write_ret = gzwrite(gzf, comp_buf, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_size));
    off64_t gz_pos = gztell64(gzf);

    // step 4: Validate and cleanup
    uLong ad1 = adler32(0UL, src_data, (uInt)src_len);
    uLong combined_adler = adler32_combine64(ad1, ad1, gz_pos);
    int def_end_ret = deflateEnd(&strm);
    int gzclose_ret = gzclose(gzf);
    free(comp_buf);
    free(head.name);
    (void)init_ret;
    (void)tune_ret;
    (void)def_ret;
    (void)bound;
    (void)gz_write_ret;
    (void)gz_pos;
    (void)ad1;
    (void)combined_adler;
    (void)def_end_ret;
    (void)gzclose_ret;
    (void)version;
    // API sequence test completed successfully
    return 66;
}