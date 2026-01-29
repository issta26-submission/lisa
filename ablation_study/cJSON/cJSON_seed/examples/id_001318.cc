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
//<ID> 1318
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *str0 = cJSON_CreateString("alpha");
    cJSON *str_ref = cJSON_CreateStringReference("beta");
    cJSON *flag = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToArray(arr, str0);
    cJSON_AddItemToArray(arr, cJSON_CreateString("gamma"));
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToObject(root, "ref", str_ref);
    cJSON_AddItemToObject(root, "flag", flag);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    const char *first_val = cJSON_GetStringValue(first);
    (void)first_val;
    cJSON_bool is_bool = cJSON_IsBool(cJSON_GetObjectItem(root, "flag"));
    (void)is_bool;
    cJSON_DeleteItemFromObject(root, "ref");
    cJSON *after_del = cJSON_GetObjectItem(root, "ref");
    (void)after_del;
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    const char *second_val = cJSON_GetStringValue(second);
    (void)second_val;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}