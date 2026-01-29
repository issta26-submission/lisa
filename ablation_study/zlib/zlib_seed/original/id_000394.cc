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
//<ID> 394
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_zlib_api_sequence() {
    // Step 1: Declarations
    z_stream def_strm;
    z_stream inf_strm;
    gzFile gz_file = Z_NULL;
    const char *temp_filename = "zlib_fuzz_temp_file.gz";
    int ret; // For API return values

    // Buffers for uncompress
    Bytef invalid_compressed_source[50]; // Source data for uncompress, intentionally invalid
    uLong source_len = sizeof(invalid_compressed_source);
    Bytef uncompressed_dest_buffer[100]; // Destination buffer for uncompress
    uLongf uncompressed_dest_len = sizeof(uncompressed_dest_buffer); // Length for uncompress

    Bytef tiny_dest_buffer[5]; // Very small destination buffer for uncompress edge case
    uLongf tiny_dest_len = sizeof(tiny_dest_buffer); // Length for tiny buffer uncompress

    uLong compile_flags_result; // To store result of zlibCompileFlags

    // Step 2: Setup and Initialization
    // Initialize deflate stream
    memset(&def_strm, 0, sizeof(def_strm));
    def_strm.zalloc = Z_NULL;
    def_strm.zfree = Z_NULL;
    def_strm.opaque = Z_NULL;
    ret = deflateInit_(&def_strm, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, (int)sizeof(z_stream));

    // Initialize inflate stream (needed for inflateUndermine)
    memset(&inf_strm, 0, sizeof(inf_strm));
    inf_strm.zalloc = Z_NULL;
    inf_strm.zfree = Z_NULL;
    inf_strm.opaque = Z_NULL;
    ret = inflateInit_(&inf_strm, ZLIB_VERSION, (int)sizeof(z_stream));

    // Prepare invalid source data for uncompress (fills with non-zlib data)
    memset(invalid_compressed_source, 'Z', sizeof(invalid_compressed_source));
    // Prepare destination buffers
    memset(uncompressed_dest_buffer, 0, sizeof(uncompressed_dest_buffer));
    memset(tiny_dest_buffer, 0, sizeof(tiny_dest_buffer));

    // Step 3: Stream Configuration and Compile Flags
    // Prime the deflate stream with 6 bits and value 0x1A
    ret = deflatePrime(&def_strm, 6, 0x1A);

    // Undermine the inflate stream by 4 bytes
    ret = inflateUndermine(&inf_strm, 4);

    // Retrieve zlib compile flags
    compile_flags_result = zlibCompileFlags();

    // Step 4: Uncompression Operations (Edge Cases)
    // Attempt to uncompress invalid source data.
    // This call is expected to return an error (e.g., Z_DATA_ERROR) due to malformed input.
    ret = uncompress(uncompressed_dest_buffer, &uncompressed_dest_len,
                     invalid_compressed_source, source_len);

    // Edge case: Attempt to uncompress invalid data into a very small destination buffer.
    // This tests buffer handling combined with invalid input.
    ret = uncompress(tiny_dest_buffer, &tiny_dest_len,
                     invalid_compressed_source, source_len);

    // Step 5: GZ File Operations
    // Open a temporary gzipped file for writing, then immediately close it.
    // This provides a valid gzFile handle for gzclose.
    gz_file = gzopen(temp_filename, "wb");
    // Close the gzFile. gzclose handles Z_NULL gracefully if gzopen failed.
    ret = gzclose(gz_file);

    // Step 6: Cleanup
    // End the deflate stream
    ret = deflateEnd(&def_strm);
    // End the inflate stream
    ret = inflateEnd(&inf_strm);
    // Remove the temporary file created during the test
    remove(temp_filename);

    printf("API sequence test completed successfully\n");

    return 66;
}