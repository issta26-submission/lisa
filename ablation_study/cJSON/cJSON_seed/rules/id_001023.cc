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
//<ID> 1023
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
    cJSON *name_item = NULL;
    cJSON *flag_item = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *detached_name = NULL;
    cJSON *dup_arr = NULL;
    cJSON *active_ptr = NULL;
    cJSON_bool is_active = 0;
    const char *device = "deviceA";

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString(device);
    cJSON_AddItemToObject(root, "name", name_item);
    flag_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", flag_item);
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    num1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num1);
    num2 = cJSON_CreateNumber(84.0);
    cJSON_AddItemToArray(arr, num2);

    // step 3: Operate / Validate
    detached_name = cJSON_DetachItemFromObject(root, "name");
    dup_arr = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToArray(dup_arr, detached_name);
    active_ptr = cJSON_GetObjectItem(root, "active");
    is_active = cJSON_IsTrue(active_ptr);
    (void)is_active;

    // step 4: Cleanup
    cJSON_Delete(dup_arr);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}