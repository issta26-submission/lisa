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
//<ID> 512
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"name\":\"fuzz\",\"settings\":{\"enabled\":true},\"values\":[1,2,3]}";
    const size_t buffer_length = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, buffer_length, &parse_end, 1);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON *info = cJSON_CreateString("generated_by_fuzzer");
    cJSON_AddItemToObject(meta, "info", info);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_bool has_settings = cJSON_HasObjectItem(root, "settings");
    cJSON_AddBoolToObject(root, "has_settings", has_settings);

    // step 3: Operate
    cJSON *values = cJSON_GetObjectItem(root, "values");
    cJSON *values_copy = cJSON_Duplicate(values, 1);
    cJSON_AddItemToObject(root, "values_copy", values_copy);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_AddNumberToObject(root, "processed_count", (double)cJSON_GetArraySize(values));

    // step 4: Validate and Cleanup
    cJSON_bool has_meta = cJSON_HasObjectItem(root, "meta");
    cJSON_AddBoolToObject(root, "has_meta", has_meta);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}