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
//<ID> 1532
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
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_name_item = (cJSON *)0;
    const char *retrieved_name = (const char *)0;
    cJSON_bool added_to_array = 0;
    cJSON_bool added_to_object = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    name_item = cJSON_CreateString("Bob");

    // step 3: Configure
    added_to_array = cJSON_AddItemToArray(arr, name_item);
    added_to_object = cJSON_AddItemToObject(root, "people", arr);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "people");
    retrieved_name_item = cJSON_GetArrayItem(retrieved_arr, 0);
    retrieved_name = cJSON_GetStringValue(retrieved_name_item);

    // step 5: Validate
    validation_score = (int)(root != (cJSON *)0) + (int)added_to_array + (int)added_to_object + (int)(retrieved_arr != (cJSON *)0) + (int)(retrieved_name_item != (cJSON *)0) + (int)(retrieved_name != (const char *)0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_name;
    (void)retrieved_name_item;
    (void)retrieved_arr;
    // API sequence test completed successfully
    return 66;
}