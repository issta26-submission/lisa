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
//<ID> 1027
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *flag = NULL;
    cJSON *num = NULL;
    cJSON *str = NULL;
    cJSON *dup = NULL;
    cJSON *detached_flag = NULL;
    cJSON *dup_items = NULL;
    cJSON *moved_flag_in_array = NULL;
    char *printed = NULL;
    const char *name = "device1";
    double value = 3.1415;
    cJSON_bool is_true = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    num = cJSON_CreateNumber(value);
    cJSON_AddItemToArray(arr, num);
    str = cJSON_CreateString(name);
    cJSON_AddItemToArray(arr, str);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    dup = cJSON_Duplicate(root, 1);
    detached_flag = cJSON_DetachItemFromObject(dup, "flag");
    dup_items = cJSON_GetObjectItem(dup, "items");
    cJSON_AddItemToArray(dup_items, detached_flag);
    moved_flag_in_array = cJSON_GetArrayItem(dup_items, 2);
    is_true = cJSON_IsTrue(moved_flag_in_array);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}