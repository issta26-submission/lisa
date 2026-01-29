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
//<ID> 1000
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char payload[] = "zlib API sequence payload exercising deflateSetHeader, gzwrite, gztell, gzdirect and gzclose";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong bound = compressBound((uLong)payload_len);
    Bytef *comp_buf = new Bytef[(bound ? (size_t)bound : 1)];
    z_stream dstr;
    memset(&dstr, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstr, 6, zlibVersion(), (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, (size_t)sizeof(gz_header));
    char namebuf[] = "testname";
    head.name = (Bytef *)namebuf;
    head.name_max = (uInt)(sizeof(namebuf) - 1);
    head.text = 1;
    int rc_deflate_set_header = deflateSetHeader(&dstr, &head);

    // step 2: Configure
    dstr.next_in = (Bytef *)src_buf;
    dstr.avail_in = (uInt)payload_len;
    dstr.next_out = comp_buf;
    dstr.avail_out = (uInt)(bound ? (uInt)bound : 1U);
    int rc_deflate = deflate(&dstr, 0);

    // step 3: Operate
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = gzwrite(gw, (voidpc)src_buf, (unsigned int)payload_len);
    off_t pos_after_write = gztell(gw);
    int direct_mode = gzdirect(gw);
    int rc_gzclose = gzclose(gw);

    // step 4: Validate & Cleanup
    int rc_deflate_end = deflateEnd(&dstr);
    delete [] comp_buf;
    (void)rc_deflate_init;
    (void)rc_deflate_set_header;
    (void)rc_deflate;
    (void)rc_gzwrite;
    (void)pos_after_write;
    (void)direct_mode;
    (void)rc_gzclose;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}