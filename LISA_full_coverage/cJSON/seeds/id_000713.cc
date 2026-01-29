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
//<ID> 713
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const int numbers[] = { 7, 14, 21, 28 };
    cJSON *int_arr = cJSON_CreateIntArray(numbers, 4);
    cJSON *name = cJSON_CreateString("cjson_example");
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "numbers", int_arr);
    cJSON_AddItemToObject(root, "name", name);

    // step 2: Configure
    const char json_literal[] = "{\"nested\":[10,20,30],\"status\":\"ok\"}";
    size_t json_len = (size_t)(sizeof(json_literal) - 1);
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json_literal, json_len, &parse_end, 1);
    cJSON_AddItemToObject(root, "config", parsed);
    cJSON *nested = cJSON_GetObjectItem(parsed, "nested");

    // step 3: Operate
    int main_count = cJSON_GetArraySize(int_arr);
    int nested_count = cJSON_GetArraySize(nested);
    cJSON_AddNumberToObject(root, "main_count", (double)main_count);
    cJSON_AddNumberToObject(root, "nested_count", (double)nested_count);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate & Cleanup
    const char *orig_name = cJSON_GetStringValue(name);
    cJSON *name_copy = cJSON_CreateString(orig_name);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}