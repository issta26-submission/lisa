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
//<ID> 1473
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num = NULL;
    cJSON *str = NULL;
    cJSON *arr = NULL;
    cJSON *elem0 = NULL;
    cJSON *elem1 = NULL;
    cJSON *obj_item = NULL;
    cJSON *arr_item0 = NULL;
    cJSON *duplicated = NULL;
    cJSON *detached = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(root, "num", num);
    str = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greet", str);
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    elem0 = cJSON_CreateString("zero");
    cJSON_AddItemToArray(arr, elem0);
    elem1 = cJSON_CreateNumber(1.5);
    cJSON_AddItemToArray(arr, elem1);

    // step 3: Operate / Validate
    obj_item = cJSON_GetObjectItem(root, "list");
    arr_item0 = cJSON_GetArrayItem(obj_item, 0);
    duplicated = cJSON_Duplicate(arr_item0, 1);
    detached = cJSON_DetachItemViaPointer(arr, arr_item0);
    cJSON_Delete(duplicated);

    // step 4: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}