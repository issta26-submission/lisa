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
//<ID> 1101
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
    cJSON *str_item0 = (cJSON *)0;
    cJSON *str_item1 = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *fetched_index1 = (cJSON *)0;
    cJSON *fetched_flag = (cJSON *)0;
    const char *retrieved_str = (const char *)0;
    int validation_score = 0;

    // step 2: Setup - create root object, an array, two strings and a true boolean
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item0 = cJSON_CreateString("first_value");
    str_item1 = cJSON_CreateString("second_value");
    true_item = cJSON_CreateTrue();

    // step 3: Configure - attach string items into the array and attach array and flag to root
    cJSON_AddItemToArray(arr, str_item0);
    cJSON_AddItemToArray(arr, str_item1);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(root, "enabled", true_item);

    // step 4: Operate - fetch the array and an element by index, and fetch the flag object
    fetched_arr = cJSON_GetObjectItem(root, "values");
    fetched_index1 = cJSON_GetArrayItem(fetched_arr, 1);
    fetched_flag = cJSON_GetObjectItem(root, "enabled");
    retrieved_str = cJSON_GetStringValue(fetched_index1);

    // step 5: Validate - compute a small validation score to ensure meaningful data flow
    validation_score = (int)(root != (cJSON *)0)
                     + (int)(arr != (cJSON *)0)
                     + (int)(str_item0 != (cJSON *)0)
                     + (int)(str_item1 != (cJSON *)0)
                     + (int)(true_item != (cJSON *)0)
                     + (int)(fetched_arr != (cJSON *)0)
                     + (int)(fetched_index1 != (cJSON *)0)
                     + (int)(fetched_flag != (cJSON *)0)
                     + (int)(retrieved_str != (const char *)0);
    (void)validation_score;

    // step 6: Cleanup - delete root which also frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}