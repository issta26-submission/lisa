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
//<ID> 530
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *val_item = cJSON_AddNumberToObject(root, "value", 21.0);
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", false_item);

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(meta, "active", true_item);

    // step 3: Operate
    cJSON *got_value = cJSON_GetObjectItem(root, "value");
    double value = cJSON_GetNumberValue(got_value);
    double doubled = value * 2.0;
    cJSON *double_item = cJSON_CreateNumber(doubled);
    cJSON_AddItemToObject(meta, "double_value", double_item);
    cJSON *meta_dup = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToObject(root, "meta_copy", meta_dup);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate and Cleanup
    cJSON *check = cJSON_GetObjectItem(cJSON_GetObjectItem(root, "meta_copy"), "double_value");
    double check_val = cJSON_GetNumberValue(check);
    (void)check_val;
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}