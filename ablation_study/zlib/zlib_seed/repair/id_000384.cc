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
//<ID> 384
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    Bytef src[] = "Example payload data to be compressed by deflate for gzwrite testing.";
    uLong src_len = (uLong)(sizeof(src) - 1);
    z_stream orig;
    z_stream copy;
    memset(&orig, 0, sizeof(orig));
    memset(&copy, 0, sizeof(copy));
    int ret_deflate_init;
    uLong bound;
    size_t alloc_size;
    Bytef *out1 = NULL;
    Bytef *out2 = NULL;
    unsigned int out1_len = 0;
    unsigned int out2_len = 0;
    gzFile gz_wr = (gzFile)0;
    int ret_gzwrite1 = 0;
    int ret_gzwrite2 = 0;
    int ret_gzclose = 0;
    int ret_deflate_copy = 0;
    int ret_deflate_end_orig = 0;
    int ret_deflate_end_copy = 0;

    // step 2: Setup / Configure
    ret_deflate_init = deflateInit_(&orig, 6, ver, (int)sizeof(z_stream));
    bound = deflateBound(&orig, src_len);
    alloc_size = (size_t)(bound ? bound : 1);
    out1 = (Bytef*)new unsigned char[alloc_size];
    out2 = (Bytef*)new unsigned char[alloc_size];
    orig.next_in = src;
    orig.avail_in = (uInt)src_len;
    orig.next_out = out1;
    orig.avail_out = (uInt)alloc_size;
    ret_deflate_copy = deflateCopy(&copy, &orig);
    copy.next_out = out2;
    copy.avail_out = (uInt)alloc_size;
    copy.next_in = orig.next_in;
    copy.avail_in = orig.avail_in;

    // step 3: Operate
    deflate(&orig, 0);
    deflate(&copy, 0);
    out1_len = (unsigned int)orig.total_out;
    out2_len = (unsigned int)copy.total_out;
    gz_wr = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    ret_gzwrite1 = gzwrite(gz_wr, (voidpc)out1, out1_len);
    ret_gzwrite2 = gzwrite(gz_wr, (voidpc)out2, out2_len);
    ret_gzclose = gzclose(gz_wr);

    // step 4: Validate / Cleanup
    ret_deflate_end_copy = deflateEnd(&copy);
    ret_deflate_end_orig = deflateEnd(&orig);
    delete [] (unsigned char*)out1;
    delete [] (unsigned char*)out2;
    (void)ver;
    (void)src_len;
    (void)ret_deflate_init;
    (void)bound;
    (void)alloc_size;
    (void)out1_len;
    (void)out2_len;
    (void)ret_gzwrite1;
    (void)ret_gzwrite2;
    (void)ret_gzclose;
    (void)ret_deflate_copy;
    (void)ret_deflate_end_orig;
    (void)ret_deflate_end_copy;
    // API sequence test completed successfully
    return 66;
}