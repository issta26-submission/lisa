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
//<ID> 1298
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
    cJSON *flag = cJSON_CreateFalse();
    cJSON *label = cJSON_CreateString("example");
    cJSON_AddItemToObject(child, "enabled", flag);
    cJSON_AddItemToObject(child, "label", label);
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON *dupe = cJSON_Duplicate(child, 1);

    // step 3: Operate and Validate
    cJSON_bool are_equal = cJSON_Compare(child, dupe, 1);
    cJSON *flag_item = cJSON_GetObjectItem(child, "enabled");
    cJSON_bool is_bool = cJSON_IsBool(flag_item);
    cJSON_bool is_false = cJSON_IsFalse(flag_item);
    cJSON *label_item = cJSON_GetObjectItem(child, "label");
    char *label_str = cJSON_GetStringValue(label_item);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = label_str[0];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_Delete(dupe);
    // API sequence test completed successfully
    return 66;
}