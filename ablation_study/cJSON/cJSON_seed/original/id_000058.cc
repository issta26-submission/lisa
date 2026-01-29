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
//<ID> 58
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_str[] = "{\"name\":\"alice\",\"age\":30,\"meta\":{\"active\":true}}";
    size_t json_len = sizeof(json_str) - 1;
    cJSON *root = cJSON_ParseWithLength(json_str, json_len);

    // step 2: Configure
    cJSON *new_name = cJSON_CreateString("bob");
    cJSON_bool replaced_name = cJSON_ReplaceItemInObject(root, "name", new_name);
    cJSON *new_age = cJSON_CreateNumber(42.0);
    cJSON_bool replaced_age = cJSON_ReplaceItemInObject(root, "age", new_age);

    // step 3: Operate and Validate
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *age_item = cJSON_GetObjectItem(root, "age");
    cJSON *meta_item = cJSON_GetObjectItem(root, "meta");
    cJSON *active_item = cJSON_GetObjectItem(meta_item, "active");
    cJSON_DeleteItemFromObjectCaseSensitive(root, "meta");
    const char *name_str = cJSON_GetStringValue(name_item);
    double age_val = cJSON_GetNumberValue(age_item);
    int first_char = name_str ? (int)name_str[0] : 0;
    int summary = (int)age_val + (int)replaced_name + (int)replaced_age + first_char;
    (void)summary;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}