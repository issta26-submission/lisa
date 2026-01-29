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
//<ID> 3
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream deflate_stream;
    z_stream inflate_stream;
    memset(&deflate_stream, 0, sizeof(deflate_stream));
    memset(&inflate_stream, 0, sizeof(inflate_stream));
    const unsigned char input_data[] = "Hello zlib API sequence test";
    uInt input_len = (uInt)(sizeof(input_data) - 1);
    uLong bound = 0;
    Bytef *comp_buf = 0;
    gzFile gzf = gzopen("test_zlib_api_sequence.gz", "wb");

    // step 2: Setup / Initialize streams and allocate buffers
    deflateInit_(&deflate_stream, 6, zlibVersion(), (int)sizeof(deflate_stream));
    bound = deflateBound(&deflate_stream, (uLong)input_len);
    comp_buf = (Bytef *)malloc(bound ? bound : 1);
    deflate_stream.next_in = (Bytef *)input_data;
    deflate_stream.avail_in = input_len;
    deflate_stream.next_out = comp_buf;
    deflate_stream.avail_out = (uInt)bound;

    // step 3: Operate / Validate
    deflate(&deflate_stream, 4); // Z_FINISH == 4
    uLong compressed_size = deflate_stream.total_out;
    deflateEnd(&deflate_stream);
    inflateInit_(&inflate_stream, zlibVersion(), (int)sizeof(inflate_stream));
    inflateSyncPoint(&inflate_stream);
    inflateBackEnd(&inflate_stream);
    gzprintf(gzf, "compressed=%lu\n", compressed_size);

    // step 4: Cleanup
    gzclose(gzf);
    free(comp_buf);

    // API sequence test completed successfully
    return 66;
}