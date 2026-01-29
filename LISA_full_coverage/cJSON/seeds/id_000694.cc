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
//<ID> 694
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *names_arr[] = { "Alice", "Bob", "Carol" };
    cJSON *names = cJSON_CreateStringArray(names_arr, 3);
    cJSON_AddItemToObject(root, "names", names);
    cJSON *raw = cJSON_CreateRaw("raw_data:unparsed");
    cJSON_AddItemToObject(root, "raw_blob", raw);
    cJSON *nul = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "nothing", nul);
    cJSON_bool names_is_array = cJSON_IsArray(names);
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw);

    // step 2: Configure
    cJSON_AddBoolToObject(root, "names_are_array", names_is_array);
    cJSON_AddBoolToObject(root, "raw_is_raw", raw_is_raw);
    cJSON *first = cJSON_GetArrayItem(names, 0);
    const char *first_name = cJSON_GetStringValue(first);
    cJSON *first_copy = cJSON_CreateString(first_name);
    cJSON_AddItemToObject(root, "first_name_copy", first_copy);

    // step 3: Operate & Validate
    int names_count = cJSON_GetArraySize(names);
    cJSON_AddNumberToObject(root, "names_count", (double)names_count);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}