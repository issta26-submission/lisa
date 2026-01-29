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
//<ID> 336
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
    cJSON *true_item = (cJSON *)0;
    cJSON *got_item = (cJSON *)0;
    cJSON *dup = (cJSON *)0;
    char *got_str = (char *)0;
    char *printed = (char *)0;
    cJSON_bool added_str = (cJSON_bool)0;
    cJSON_bool added_true = (cJSON_bool)0;
    cJSON_bool is_str = (cJSON_bool)0;
    cJSON_bool is_obj = (cJSON_bool)0;
    cJSON_bool dup_is_obj = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure
    str_item = cJSON_CreateString("alpha");
    added_str = cJSON_AddItemToArray(arr, str_item);
    true_item = cJSON_CreateTrue();
    added_true = cJSON_AddItemToArray(arr, true_item);

    // step 4: Operate
    got_item = cJSON_GetArrayItem(arr, 0);
    is_str = cJSON_IsString(got_item);
    is_obj = cJSON_IsObject(root);
    got_str = cJSON_GetStringValue(got_item);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    dup = cJSON_Duplicate(root, (cJSON_bool)1);
    dup_is_obj = cJSON_IsObject(dup);
    cJSON_AddItemToObject(dup, "copied_first", cJSON_CreateString(got_str));
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}