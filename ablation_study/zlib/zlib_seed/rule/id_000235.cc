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
//<ID> 235
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream def_src;
    z_stream def_dst;
    memset(&def_src, 0, sizeof(def_src));
    memset(&def_dst, 0, sizeof(def_dst));
    unsigned char input[512];
    memset(input, 'Z', sizeof(input));
    uLong inputLen = (uLong)sizeof(input);
    uLong crc = crc32(0L, (const Bytef *)input, (uInt)inputLen);
    uLong adl = adler32(0L, (const Bytef *)input, (uInt)inputLen);

    // step 2: Setup deflate source and produce first compressed buffer
    deflateInit_(&def_src, 6, zlibVersion(), (int)sizeof(z_stream));
    uLong bound1 = deflateBound(&def_src, inputLen);
    Bytef *comp1 = (Bytef *)malloc((size_t)bound1);
    memset(comp1, 0, (size_t)bound1);
    def_src.next_in = (Bytef *)input;
    def_src.avail_in = (uInt)inputLen;
    def_src.next_out = comp1;
    def_src.avail_out = (uInt)bound1;
    deflate(&def_src, 0);

    // step 3: Copy deflate state and continue compression into a second buffer
    memset(&def_dst, 0, sizeof(def_dst));
    deflateCopy(&def_dst, &def_src);
    uLong bound2 = deflateBound(&def_dst, inputLen);
    Bytef *comp2 = (Bytef *)malloc((size_t)bound2);
    memset(comp2, 0, (size_t)bound2);
    def_dst.next_in = (Bytef *)input;
    def_dst.avail_in = (uInt)inputLen;
    def_dst.next_out = comp2;
    def_dst.avail_out = (uInt)bound2;
    deflate(&def_dst, 0);

    // step 4: Persist compressed data via gz API and cleanup
    gzFile gf = gzopen("test_zlib_api_sequence.gz", "wb");
    gzwrite(gf, comp2, (unsigned int)def_dst.total_out);
    gzclose(gf);
    deflateEnd(&def_dst);
    deflateEnd(&def_src);
    free(comp1);
    free(comp2);

    // API sequence test completed successfully
    return 66;
}