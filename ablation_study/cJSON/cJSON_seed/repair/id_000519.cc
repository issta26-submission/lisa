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
//<ID> 519
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"unit_test\",\"active\":true,\"count\":7}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);
    const cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_value = cJSON_GetNumberValue(count_item);

    // step 2: Configure
    cJSON *metadata = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "metadata", metadata);
    cJSON_bool has_name = cJSON_HasObjectItem(root, "name");
    cJSON *has_name_item = cJSON_AddBoolToObject(metadata, "has_name", has_name);
    (void)has_name_item;
    double doubled = count_value * 2.0;
    cJSON *double_item = cJSON_CreateNumber(doubled);
    cJSON_AddItemToObject(metadata, "double_count", double_item);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate and Cleanup
    cJSON_bool has_metadata = cJSON_HasObjectItem(root, "metadata");
    (void)has_metadata;
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}