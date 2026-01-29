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
//<ID> 1478
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *numbers = NULL;
    cJSON *arr = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *child = NULL;
    cJSON *str = NULL;
    cJSON *first_item = NULL;
    cJSON *dup_item = NULL;
    cJSON *detached_item = NULL;
    cJSON_bool added_arr = 0;
    cJSON_bool added_child = 0;
    cJSON_bool added_num1 = 0;
    cJSON_bool added_num2 = 0;
    cJSON_bool added_dup = 0;
    cJSON_bool added_detached = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(1.23);
    num2 = cJSON_CreateNumber(4.56);
    added_num1 = cJSON_AddItemToArray(arr, num1);
    added_num2 = cJSON_AddItemToArray(arr, num2);
    added_arr = cJSON_AddItemToObject(root, "numbers", arr);
    child = cJSON_CreateObject();
    str = cJSON_CreateString("hello");
    added_child = cJSON_AddItemToObject(child, "greeting", str);
    added_child = cJSON_AddItemToObject(root, "child", child);

    // step 3: Operate / Validate
    numbers = cJSON_GetObjectItem(root, "numbers");
    first_item = cJSON_GetArrayItem(numbers, 0);
    dup_item = cJSON_Duplicate(first_item, 0);
    added_dup = cJSON_AddItemToObject(cJSON_GetObjectItem(root, "child"), "dup", dup_item);
    detached_item = cJSON_DetachItemViaPointer(numbers, first_item);
    added_detached = cJSON_AddItemToObject(root, "detached", detached_item);

    // step 4: Cleanup
    cJSON_Delete(root);
    (void)added_arr;
    (void)added_child;
    (void)added_num1;
    (void)added_num2;
    (void)added_dup;
    (void)added_detached;

    // API sequence test completed successfully
    return 66;
}