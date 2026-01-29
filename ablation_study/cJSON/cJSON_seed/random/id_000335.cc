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
//<ID> 335
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
    cJSON *got0 = (cJSON *)0;
    cJSON *got1 = (cJSON *)0;
    cJSON *dup = (cJSON *)0;
    char *got_str = (char *)0;
    char *printed = (char *)0;
    cJSON_bool is_str = (cJSON_bool)0;
    cJSON_bool is_obj = (cJSON_bool)0;
    cJSON_bool dup_is_str = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure
    str_item = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, str_item);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddNumberToObject(root, "count", 2.0);

    // step 4: Operate
    got0 = cJSON_GetArrayItem(arr, 0);
    got1 = cJSON_GetArrayItem(arr, 1);
    got_str = cJSON_GetStringValue(got0);
    is_str = cJSON_IsString(got0);
    is_obj = cJSON_IsObject(root);
    printed = cJSON_PrintUnformatted(root);
    dup = cJSON_Duplicate(root, (cJSON_bool)1);

    // step 5: Validate
    dup_is_str = cJSON_IsString(cJSON_GetArrayItem(dup, 0));
    (void)got1;
    (void)got_str;
    (void)is_str;
    (void)is_obj;
    (void)dup_is_str;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}