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
//<ID> 734
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
    cJSON *name_item = (cJSON *)0;
    cJSON *age_item = (cJSON *)0;
    cJSON *temp_item = (cJSON *)0;
    cJSON *array_item1 = (cJSON *)0;
    cJSON *array_item2 = (cJSON *)0;
    cJSON *detached_temp = (cJSON *)0;
    cJSON *retrieved_name = (cJSON *)0;
    cJSON_bool added_name = 0;
    cJSON_bool added_age = 0;
    cJSON_bool added_items_array = 0;
    cJSON_bool added_array_item1 = 0;
    cJSON_bool added_array_item2 = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    name_item = cJSON_CreateString("tester");
    age_item = cJSON_CreateNumber(28.0);
    temp_item = cJSON_CreateString("transient");
    array_item1 = cJSON_CreateTrue();
    array_item2 = cJSON_CreateNull();

    // step 3: Configure
    added_items_array = cJSON_AddItemToObject(root, "items", arr);
    added_name = cJSON_AddItemToObject(root, "name", name_item);
    added_age = cJSON_AddItemToObject(root, "age", age_item);
    cJSON_AddItemToObject(root, "temp", temp_item);
    added_array_item1 = cJSON_AddItemToArray(arr, array_item1);
    added_array_item2 = cJSON_AddItemToArray(arr, array_item2);

    // step 4: Operate
    detached_temp = cJSON_DetachItemFromObject(root, "temp");
    retrieved_name = cJSON_GetObjectItem(root, "name");

    // step 5: Validate
    validation_score = (int)added_items_array + (int)added_name + (int)added_age + (int)added_array_item1 + (int)added_array_item2 + (retrieved_name != (cJSON *)0) + (detached_temp != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(detached_temp);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}