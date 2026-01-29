#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 973
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json_text[] = "{\"name\":\"tester\",\"values\":[1,2,3],\"active\":true}";
    size_t json_len = sizeof(json_text); // include terminating NUL so ParseWithLengthOpts sees null-terminated buffer
    const char *parse_end = NULL;
    cJSON *root = NULL;
    cJSON *vals = NULL;
    cJSON *first = NULL;
    char *printed_unformatted = NULL;
    char *buffer = NULL;
    int buffer_len = 128;
    cJSON_bool prealloc_ok = 0;
    const char *version = NULL;
    double first_val = 0.0;

    // step 2: Setup / Configure
    version = cJSON_Version();
    root = cJSON_ParseWithLengthOpts(json_text, json_len, &parse_end, 1);
    vals = cJSON_GetObjectItem(root, "values");
    first = cJSON_GetArrayItem(vals, 0);
    first_val = cJSON_GetNumberValue(first);

    // step 3: Operate / Validate
    printed_unformatted = cJSON_PrintUnformatted(root);
    buffer = (char *)cJSON_malloc((size_t)buffer_len);
    memset(buffer, 0, (size_t)buffer_len);
    prealloc_ok = cJSON_PrintPreallocated(root, buffer, buffer_len, 0);
    buffer[0] = version[0];
    buffer[1] = printed_unformatted[0];
    buffer[2] = (char)('0' + ((int)first_val % 10));

    // step 4: Cleanup
    cJSON_free(printed_unformatted);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}