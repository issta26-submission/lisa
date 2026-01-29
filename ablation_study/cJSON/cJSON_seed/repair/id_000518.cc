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
//<ID> 518
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *initial_json = "{\"name\":\"device\",\"active\":true,\"versions\":[1,2]}";
    const char *parse_end = NULL;
    size_t len = strlen(initial_json);
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, len, &parse_end, 1);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *cfg = cJSON_AddBoolToObject(root, "configured", 1);
    cJSON *extra_versions = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "extra_versions", extra_versions);
    cJSON *v3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(extra_versions, v3);

    // step 3: Operate
    cJSON_bool has_name = cJSON_HasObjectItem(root, "name");
    cJSON_bool has_meta = cJSON_HasObjectItem(root, "meta");
    double flag_value = (double)has_name + 2.0 * (double)has_meta;
    cJSON *flags_num = cJSON_CreateNumber(flag_value);
    cJSON_AddItemToObject(root, "flags", flags_num);
    const char *child_json = "{\"child_key\":\"child_value\"}";
    const char *child_end = NULL;
    cJSON *child = cJSON_ParseWithLengthOpts(child_json, strlen(child_json), &child_end, 1);
    cJSON_AddItemToObject(root, "child", child);

    // step 4: Validate and Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}