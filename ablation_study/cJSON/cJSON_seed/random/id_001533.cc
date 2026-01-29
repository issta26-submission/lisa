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
//<ID> 1533
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
    cJSON *item_a = (cJSON *)0;
    cJSON *item_b = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_item = (cJSON *)0;
    char *str_value = (char *)0;
    cJSON_bool added_a = 0;
    cJSON_bool added_b = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item_a = cJSON_CreateString("alpha");
    item_b = cJSON_CreateString("beta");

    // step 3: Configure
    added_a = cJSON_AddItemToArray(arr, item_a);
    added_b = cJSON_AddItemToArray(arr, item_b);
    cJSON_AddItemToObject(root, "seq", arr);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "seq");
    retrieved_item = cJSON_GetArrayItem(retrieved_arr, 0);
    str_value = cJSON_GetStringValue(retrieved_item);

    // step 5: Validate
    validation_score = (int)added_a + (int)added_b + (int)(retrieved_arr != (cJSON *)0) + (int)(retrieved_item != (cJSON *)0) + (int)(str_value != (char *)0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)str_value;
    (void)retrieved_item;
    (void)retrieved_arr;
    (void)added_a;
    (void)added_b;
    // API sequence test completed successfully
    return 66;
}