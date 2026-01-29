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
//<ID> 497
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

// zlib.h is implicitly available as per instructions, no explicit include needed.

int test_zlib_api_sequence() {
    // Step 1: Declarations
    z_stream inf_strm;
    z_stream def_strm; // Declared for deflateEnd API
    gzFile gz_file = Z_NULL;
    const char *test_filename = "zlib_api_gztell_test.gz";
    const char *file_content = "This is the first line of content.\nAnd this is the second line for more data.\n";
    char read_buffer[128];
    off64_t current_file_offset_64 = 0;
    uLong crc_val1 = 0;
    uLong crc_val2 = 0;
    off_t combined_len_for_crc = 0; // Type for crc32_combine's length parameter

    // Step 2: Setup
    // Initialize inflate stream for inflateInit_, inflateUndermine, and inflateEnd
    memset(&inf_strm, 0, sizeof(inf_strm));
    inf_strm.zalloc = Z_NULL;
    inf_strm.zfree = Z_NULL;
    inf_strm.opaque = Z_NULL;
    inflateInit_(&inf_strm, ZLIB_VERSION, (int)sizeof(z_stream));

    // Initialize deflate stream for deflateEnd
    memset(&def_strm, 0, sizeof(def_strm));
    def_strm.zalloc = Z_NULL;
    def_strm.zfree = Z_NULL;
    def_strm.opaque = Z_NULL;
    // Using default compression level for initialization
    deflateInit_(&def_strm, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, (int)sizeof(z_stream));

    // Open a gzipped file for writing, write some content, and close it
    gz_file = gzopen(test_filename, "wb");
    gzwrite(gz_file, file_content, (unsigned int)strlen(file_content));
    gzclose(gz_file);

    // Reopen the gzipped file for reading
    gz_file = gzopen(test_filename, "rb");

    // Step 3: Core operations (gzFile and inflate stream)
    // Read some content to advance the file pointer
    gzgets(gz_file, read_buffer, sizeof(read_buffer));

    // Get the current file offset using gztell64
    current_file_offset_64 = gztell64(gz_file);

    // Check for end-of-file status (should be false after partial read)
    gzeof(gz_file);

    // Enable undermining for the inflate stream
    inflateUndermine(&inf_strm, 1);

    // Step 4: Checksum operations
    // Calculate first CRC value
    crc_val1 = crc32(0L, (const Bytef*)"sample data one", 15);
    // Calculate second CRC value
    crc_val2 = crc32(0L, (const Bytef*)"sample data two", 15);

    // Combine the two CRC values using the offset obtained from gztell64
    // Cast off64_t to off_t as crc32_combine expects off_t.
    // For typical test file sizes, this cast is safe.
    combined_len_for_crc = (off_t)current_file_offset_64;
    crc_val1 = crc32_combine(crc_val1, crc_val2, combined_len_for_crc);

    // Step 5: Edge-case scenarios
    // Call gzeof immediately after opening (before any read operation).
    // This requires closing and reopening the file or seeking. Let's just call it again
    // after a minimal read to test its state change.
    gzread(gz_file, read_buffer, 1); // Read a single byte
    gzeof(gz_file); // Should still be false unless file is extremely small

    // Disable undermining for the inflate stream (another valid call)
    inflateUndermine(&inf_strm, 0);

    // Call crc32_combine with zero length and zero initial CRC (an edge case for combination)
    crc_val1 = crc32_combine(crc_val1, 0L, 0);

    // Step 6: Cleanup
    inflateEnd(&inf_strm);
    deflateEnd(&def_strm); // Clean up the deflate stream
    gzclose(gz_file);
    remove(test_filename); // Remove the temporary file

    printf("API sequence test completed successfully\n");

    return 66;
}