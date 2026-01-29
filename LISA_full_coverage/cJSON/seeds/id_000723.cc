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
//<ID> 723
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"active\":true,\"count\":5,\"name\":\"tester\"}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    cJSON *active_item = cJSON_GetObjectItem(root, "active");
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    const char *name_str = cJSON_GetStringValue(name_item);

    // step 2: Configure
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "active", false_item);

    // step 3: Operate
    cJSON_AddNumberToObject(root, "count_copy", count_val);
    cJSON_AddStringToObject(root, "name_copy", name_str);
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    cJSON_free((void *)printed);

    // step 4: Validate & Cleanup
    cJSON *active_after = cJSON_GetObjectItem(root, "active");
    cJSON_bool is_false = cJSON_IsFalse(active_after);
    cJSON_AddBoolToObject(root, "was_replaced", replaced);
    cJSON_AddBoolToObject(root, "is_now_false", is_false);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}