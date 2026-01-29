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
//<ID> 0
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream defstrm;
    z_stream infstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    memset(&infstrm, 0, sizeof(infstrm));
    unsigned char input_data[] = "hello zlib";
    uLong input_len = (uLong)(sizeof(input_data) - 1);
    unsigned char comp_buf[512];
    unsigned char decomp_buf[512];
    uLongf comp_buf_len = (uLongf)sizeof(comp_buf);
    uLongf decomp_buf_len = (uLongf)sizeof(decomp_buf);
    gzFile gzf = gzopen("test_zlib_api.gz", "wb+");

    // step 2: Setup (configure file buffering and record initial offset)
    gzbuffer(gzf, 4096);
    off_t initial_offset = gzoffset(gzf);

    // step 3: Operate (compress then write, then decompress and inspect codes used)
    defstrm.next_in = input_data;
    defstrm.avail_in = (uInt)input_len;
    defstrm.next_out = comp_buf;
    defstrm.avail_out = (uInt)comp_buf_len;
    deflateInit_(&defstrm, 6, zlibVersion(), (int)sizeof(z_stream));
    deflate(&defstrm, Z_FINISH);
    uLong compressed_used = defstrm.total_out;
    deflateEnd(&defstrm);

    gzwrite(gzf, comp_buf, (unsigned int)compressed_used);
    off_t after_write_offset = gzoffset(gzf);

    infstrm.next_in = comp_buf;
    infstrm.avail_in = (uInt)compressed_used;
    infstrm.next_out = decomp_buf;
    infstrm.avail_out = (uInt)decomp_buf_len;
    inflateInit_(&infstrm, zlibVersion(), (int)sizeof(z_stream));
    inflate(&infstrm, Z_FINISH);
    unsigned long codes_used = inflateCodesUsed(&infstrm);
    inflateEnd(&infstrm);

    // step 4: Cleanup
    gzclose(gzf);
    (void)initial_offset;
    (void)after_write_offset;
    (void)codes_used;

    // API sequence test completed successfully
    return 66;
}