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
//<ID> 4
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    z_stream defstream;
    z_stream infstream;
    memset(&defstream, 0, sizeof(defstream));
    memset(&infstream, 0, sizeof(infstream));
    const char input_data[] = "Example data for zlib compression and API sequence test";
    uLong input_len = (uLong)(sizeof(input_data) - 1);
    const char *version = zlibVersion();

    // step 2: Initialize compressor and query configuration
    deflateInit_(&defstream, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&defstream, input_len);
    static unsigned char out_buffer[65536];
    uInt out_size = (bound > (uLong)sizeof(out_buffer)) ? (uInt)sizeof(out_buffer) : (uInt)bound;
    defstream.next_in = (Bytef *)input_data;
    defstream.avail_in = (uInt)input_len;
    defstream.next_out = (Bytef *)out_buffer;
    defstream.avail_out = out_size;
    deflateReset(&defstream);

    // step 3: Operate with gzFile APIs
    gzFile gz = gzopen("test_zlib_api_sequence.tmp", "wb+");
    gzbuffer(gz, 4096);
    off_t current_offset = gzoffset(gz);
    (void)current_offset;
    gzclose(gz);

    // step 4: Initialize inflater, validate, and cleanup
    inflateInit_(&infstream, version, (int)sizeof(z_stream));
    infstream.next_in = (Bytef *)out_buffer;
    infstream.avail_in = out_size;
    unsigned long codes_used = inflateCodesUsed(&infstream);
    (void)codes_used;
    deflateEnd(&defstream);
    inflateEnd(&infstream);

    // API sequence test completed successfully
    return 66;
}