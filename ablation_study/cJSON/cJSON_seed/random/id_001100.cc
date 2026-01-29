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
//<ID> 1100
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
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *fetched_item0 = (cJSON *)0;
    cJSON *fetched_item1 = (cJSON *)0;
    const char *retrieved_str = (const char *)0;
    int validation_score = 0;

    // step 2: Setup - create root object, an array, a string item and a true boolean
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("first_value");
    true_item = cJSON_CreateTrue();

    // step 3: Configure - populate array and attach to root
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate - fetch the array from the root and retrieve elements
    fetched_arr = cJSON_GetObjectItem(root, "items");
    fetched_item0 = cJSON_GetArrayItem(fetched_arr, 0);
    fetched_item1 = cJSON_GetArrayItem(fetched_arr, 1);
    retrieved_str = cJSON_GetStringValue(fetched_item0);

    // step 5: Validate - compute a small validation score based on successful operations
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(str_item != (cJSON *)0) + (int)(true_item != (cJSON *)0) + (int)(fetched_arr != (cJSON *)0) + (int)(fetched_item0 != (cJSON *)0) + (int)(fetched_item1 != (cJSON *)0) + (int)(retrieved_str != (const char *)0);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}