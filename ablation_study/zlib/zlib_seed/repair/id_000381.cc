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
//<ID> 381
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    z_stream src;
    z_stream dest;
    memset(&src, 0, sizeof(src));
    memset(&dest, 0, sizeof(dest));
    Bytef input[] = "Sample payload for zlib deflate and gzwrite.";
    uLong in_len = (uLong)(sizeof(input) - 1);
    unsigned char gz_payload[128];
    unsigned int gz_payload_len = (unsigned int)sizeof(gz_payload);
    memset(gz_payload, 0, sizeof(gz_payload));
    memcpy(gz_payload, input, (size_t)((in_len < (uLong)gz_payload_len) ? in_len : (uLong)gz_payload_len));

    // step 2: Setup / Configure
    int init_ret = deflateInit_(&src, 6, ver, (int)sizeof(z_stream));
    uLong bound = deflateBound(&src, in_len);
    unsigned char outbuf[512];
    memset(outbuf, 0, sizeof(outbuf));
    src.next_in = input;
    src.avail_in = (uInt)in_len;
    src.next_out = outbuf;
    src.avail_out = (uInt)sizeof(outbuf);

    // step 3: Operate - copy deflate state and write data to a .gz file
    int copy_ret = deflateCopy(&dest, &src);
    gzFile gz = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    unsigned int write_len = (unsigned int)((in_len < (uLong)gz_payload_len) ? in_len : (uLong)gz_payload_len);
    int gzwrite_ret = gzwrite(gz, (voidpc)gz_payload, write_len);
    int gzclose_ret = gzclose(gz);
    int end_dest_ret = deflateEnd(&dest);
    int end_src_ret = deflateEnd(&src);

    // step 4: Validate / Cleanup
    (void)ver;
    (void)in_len;
    (void)init_ret;
    (void)bound;
    (void)outbuf;
    (void)copy_ret;
    (void)gz_payload_len;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    (void)end_dest_ret;
    (void)end_src_ret;
    // API sequence test completed successfully
    return 66;
}