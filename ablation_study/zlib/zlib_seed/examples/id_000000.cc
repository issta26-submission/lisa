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
    z_stream def_strm;
    z_stream inf_strm;
    unsigned char compbuf[512];
    unsigned char decompbuf[512];
    const char src[] = "Hello zlib fuzz test data - ensure some content to compress.";
    uInt src_len = (uInt)(sizeof(src) - 1);
    uInt compressed_len = 0;
    gzFile gf;
    char msg[128];

    // step 2: Initialize and configure deflate (Initialize → Configure)
    memset(&def_strm, 0, sizeof(def_strm));
    def_strm.zalloc = 0;
    def_strm.zfree = 0;
    def_strm.opaque = 0;
    def_strm.next_in = (Bytef *)src;
    def_strm.avail_in = src_len;
    def_strm.next_out = compbuf;
    def_strm.avail_out = (uInt)sizeof(compbuf);
    deflateInit_(&def_strm, 6, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Operate → Validate
    deflate(&def_strm, Z_FINISH);
    compressed_len = (uInt)def_strm.total_out;
    deflateEnd(&def_strm);

    memset(&inf_strm, 0, sizeof(inf_strm));
    inf_strm.zalloc = 0;
    inf_strm.zfree = 0;
    inf_strm.opaque = 0;
    inflateInit_(&inf_strm, zlibVersion(), (int)sizeof(z_stream));
    inf_strm.next_in = compbuf;
    inf_strm.avail_in = compressed_len;
    inf_strm.next_out = decompbuf;
    inf_strm.avail_out = (uInt)sizeof(decompbuf);
    inflateSyncPoint(&inf_strm);
    inflateBackEnd(&inf_strm);

    // step 4: Cleanup (and use gzprintf)
    gf = gzopen("test_zlib_api_sequence.gz", "wb");
    sprintf(msg, "Compressed %u bytes\n", compressed_len);
    gzprintf(gf, msg);
    gzclose(gf);
    inflateEnd(&inf_strm);

    // API sequence test completed successfully
    return 66;
}