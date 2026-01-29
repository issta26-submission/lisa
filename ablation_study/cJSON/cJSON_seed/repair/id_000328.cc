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
//<ID> 328
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *orig_num = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToObject(child, "value", orig_num);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddBoolToObject(root, "active", 1);

    // step 2: Configure
    cJSON *got_child = cJSON_GetObjectItem(root, "child");
    cJSON *got_value = cJSON_GetObjectItem(got_child, "value");
    double value = cJSON_GetNumberValue(got_value);
    cJSON *double_num = cJSON_CreateNumber(value * 2.0);
    cJSON_AddItemToObject(root, "double", double_num);

    // step 3: Operate and Validate
    cJSON *got_double = cJSON_GetObjectItem(root, "double");
    double doubled = cJSON_GetNumberValue(got_double);
    cJSON *active_item = cJSON_GetObjectItem(root, "active");
    (void)doubled;
    (void)active_item;

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}