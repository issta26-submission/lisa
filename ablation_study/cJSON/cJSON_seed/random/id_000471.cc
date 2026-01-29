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
//<ID> 471
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON *null_added = (cJSON *)0;
    cJSON *label_item = (cJSON *)0;
    cJSON_bool is_raw = 0;
    const char *str_val = (const char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("hello");
    raw_item = cJSON_CreateRaw("raw content");

    // step 3: Configure
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, raw_item);
    cJSON_AddItemToObject(root, "items", arr);
    null_added = cJSON_AddNullToObject(root, "optional");

    // step 4: Operate
    retrieved = cJSON_GetArrayItem(arr, 1);
    is_raw = cJSON_IsRaw(retrieved);
    str_val = cJSON_GetStringValue(str_item);
    label_item = cJSON_CreateString(str_val);
    cJSON_AddItemToObject(root, "label", label_item);
    cJSON_AddItemToObject(root, "is_raw", cJSON_CreateBool(is_raw));

    // step 5: Validate
    (void)null_added;
    (void)retrieved;
    (void)str_val;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}