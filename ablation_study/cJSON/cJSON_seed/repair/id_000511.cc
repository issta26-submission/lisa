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
//<ID> 511
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json[] = "{\"active\":false,\"name\":\"tester\",\"values\":[1,2,3]}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, (size_t)(sizeof(json) - 1), &parse_end, 1);
    cJSON *orig_values = cJSON_GetObjectItem(root, "values");
    int orig_count = cJSON_GetArraySize(orig_values);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *additional = cJSON_CreateArray();
    cJSON *num = cJSON_CreateNumber((double)orig_count);
    cJSON_AddItemToArray(additional, num);
    cJSON_AddItemToObject(root, "values_extra", additional);
    cJSON_bool active_present = cJSON_HasObjectItem(root, "active");
    cJSON *flag = cJSON_AddBoolToObject(root, "was_active", active_present);

    // step 3: Operate
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    const char *name_str = cJSON_GetStringValue(name_item);
    cJSON *echo = cJSON_CreateString(name_str);
    cJSON_AddItemToObject(root, "echo_name", echo);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate and Cleanup
    cJSON_bool has_meta = cJSON_HasObjectItem(root, "meta");
    cJSON *detached_values = cJSON_DetachItemFromObject(root, "values");
    cJSON_Delete(detached_values);
    (void)has_meta;
    (void)flag;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}