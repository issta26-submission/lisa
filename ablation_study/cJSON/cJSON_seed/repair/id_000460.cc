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
//<ID> 460
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"arr\":[true,null],\"msg\":\"hello\"}";
    cJSON *root = cJSON_Parse(json);
    cJSON *arr = cJSON_GetObjectItem(root, "arr");

    // step 2: Configure
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_InsertItemInArray(arr, 1, false_item);

    // step 3: Operate
    cJSON *inserted = cJSON_GetArrayItem(arr, 1);
    cJSON_bool was_false = cJSON_IsFalse(inserted);
    cJSON *flag_num = cJSON_CreateNumber((double)was_false);
    cJSON_AddItemToObject(root, "flag_checked", flag_num);
    char *printed = cJSON_Print(root);
    cJSON_free(printed);

    // step 4: Validate and Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}