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
//<ID> 658
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    double initial_values[3] = {1.1, 2.2, 3.3};
    cJSON *array = cJSON_CreateDoubleArray(initial_values, 3);
    cJSON_AddItemToObject(root, "values", array);

    // step 2: Configure
    int initial_count = cJSON_GetArraySize(array);
    double replacement_values[2] = {9.9, 8.8};
    cJSON *replacement_array = cJSON_CreateDoubleArray(replacement_values, 2);
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "values", replacement_array);
    (void)replaced;

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t buf_len = 0;
    {
        const char *p = snapshot;
        while (*p) { buf_len++; p++; } /* sizing without using strlen */
    }
    char *buffer = (char *)cJSON_malloc(buf_len + 1);
    memset(buffer, 0, buf_len + 1);
    {
        const char *s = snapshot;
        char *d = buffer;
        while (*s) { *d++ = *s++; }
        *d = '\0';
    }
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_values = cJSON_GetObjectItem(parsed, "values");
    int parsed_count = cJSON_GetArraySize(parsed_values);
    (void)version;
    (void)initial_count;
    (void)parsed_count;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}