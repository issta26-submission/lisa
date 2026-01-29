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
//<ID> 900
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib dictionary and params test payload to exercise deflate/inflate sequence";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    Bytef dict_buf[] = "example_dictionary_data_for_zlib";
    const uInt dict_len = (uInt)(sizeof(dict_buf) - 1);

    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));
    int rc_deflate_init = deflateInit_(&dstrm, 6, zlibVersion(), (int)sizeof(z_stream));

    // step 2: Configure (adjust params and prepare input)
    int rc_deflate_params = deflateParams(&dstrm, 5, 0);
    dstrm.next_in = (Bytef *)src_buf;
    dstrm.avail_in = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);

    uLong comp_bound = deflateBound(&dstrm, payload_len);
    uInt comp_buf_size = (uInt)(comp_bound <= (uLong)~0u ? comp_bound : (uLong)~0u);
    Bytef *comp_buf = new Bytef[(comp_buf_size ? comp_buf_size : 1)];
    dstrm.next_out = comp_buf;
    dstrm.avail_out = comp_buf_size;

    // step 3: Operate (compress then initialize inflate and set dictionary and decompress)
    int rc_deflate = deflate(&dstrm, 4);
    uLong comp_size = (uLong)comp_buf_size - (uLong)dstrm.avail_out;

    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));

    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_size <= (uLong)~0u ? comp_size : (uLong)~0u);
    Bytef *out_buf = new Bytef[(payload_len ? payload_len : 1)];
    uInt out_buf_size = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    istrm.next_out = out_buf;
    istrm.avail_out = out_buf_size;

    int rc_inflate_setdict = inflateSetDictionary(&istrm, dict_buf, dict_len);
    int rc_inflate = inflate(&istrm, 4);
    int rc_inflate_validate = inflateValidate(&istrm, 0);

    // step 4: Cleanup and finalization
    int rc_deflate_end = deflateEnd(&dstrm);
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] comp_buf;
    delete [] out_buf;
    (void)rc_deflate_init;
    (void)rc_deflate_params;
    (void)rc_deflate;
    (void)comp_size;
    (void)rc_inflate_init;
    (void)rc_inflate_setdict;
    (void)rc_inflate;
    (void)rc_inflate_validate;
    (void)rc_deflate_end;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}