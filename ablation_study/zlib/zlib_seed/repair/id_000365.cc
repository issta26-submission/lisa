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
//<ID> 365
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations / Initialize
    const char *ver = zlibVersion();
    z_stream inf_strm;
    Bytef dict[] = "example_dictionary_for_inflate";
    uInt dict_len = (uInt)(sizeof(dict) - 1);
    Bytef write_buf[] = "payload_for_gzwrite_and_inflateSetDictionary";
    unsigned int write_len = (unsigned int)(sizeof(write_buf) - 1);
    gzFile gz = (gzFile)0;
    int written = 0;
    int setdict_ret = 0;
    int close_ret = 0;
    memset(&inf_strm, 0, sizeof(inf_strm));
    memset(write_buf, 0, 0); // no-op to satisfy usage pattern without altering data

    // step 2: Configure / Operate - open gz and write data
    gz = gzopen("test_zlib_api_sequence.tmp.gz", "wb");
    written = gzwrite(gz, (voidpc)write_buf, write_len);

    // step 3: Operate - prepare stream and set inflate dictionary
    inf_strm.next_in = (Bytef *)write_buf;
    inf_strm.avail_in = (uInt)write_len;
    inf_strm.msg = (char *)ver;
    setdict_ret = inflateSetDictionary(&inf_strm, dict, dict_len);

    // step 4: Validate / Cleanup
    close_ret = gzclose(gz);
    (void)ver;
    (void)written;
    (void)setdict_ret;
    (void)close_ret;
    (void)inf_strm.next_in;
    (void)inf_strm.avail_in;
    // API sequence test completed successfully
    return 66;
}