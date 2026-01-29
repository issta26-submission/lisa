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
//<ID> 1244
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char source_text[] = "Hello_zlib";
    const uLong source_len = (uLong)(sizeof(source_text) - 1);
    const Bytef dict[] = "example_dict";
    const uInt dict_len = (uInt)(sizeof(dict) - 1);
    const char *version = zlibVersion();
    Bytef *src = new Bytef[(size_t)source_len];
    memset(src, 0, (size_t)source_len);
    memcpy(src, source_text, (size_t)source_len);
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup (initialize deflate stream and compute output bound)
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    uLong out_bound = deflateBound(&dstrm, source_len);
    Bytef *out_buf = new Bytef[(size_t)out_bound];
    memset(out_buf, 0, (size_t)out_bound);
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)source_len;
    dstrm.next_out = out_buf;
    dstrm.avail_out = (uInt)out_bound;

    // step 3: Operate (prepare an inflate stream and set its dictionary)
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = out_buf;
    istrm.avail_in = 0; // no compressed data actually produced in this synthetic flow
    istrm.next_out = src;
    istrm.avail_out = (uInt)source_len;
    int rc_inflate_set_dict = inflateSetDictionary(&istrm, dict, dict_len);

    // step 4: Validate and cleanup
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] src;
    delete [] out_buf;
    (void)version;
    (void)rc_deflate_init;
    (void)out_bound;
    (void)rc_inflate_init;
    (void)rc_inflate_set_dict;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}