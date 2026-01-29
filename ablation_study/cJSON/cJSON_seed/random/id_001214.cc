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
//<ID> 1214
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *bool_true = (cJSON *)0;
    cJSON *bool_false = (cJSON *)0;
    cJSON *first_in_array = (cJSON *)0;
    cJSON *second_in_array = (cJSON *)0;
    char *greeting_str = (char *)0;
    int array_size = 0;
    int validation_score = 0;

    // step 2: Setup - create root object and an array, attach array to root
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure - create a string member and two boolean items, add them appropriately
    str_item = cJSON_CreateString("hello_world");
    cJSON_AddItemToObject(root, "greeting", str_item);
    bool_true = cJSON_CreateBool(1);
    bool_false = cJSON_CreateBool(0);
    cJSON_AddItemToArray(arr, bool_true);
    cJSON_AddItemToArray(arr, bool_false);

    // step 4: Operate - retrieve array size and fetch items by index, read string value
    array_size = cJSON_GetArraySize(arr);
    first_in_array = cJSON_GetArrayItem(arr, 0);
    second_in_array = cJSON_GetArrayItem(arr, 1);
    greeting_str = cJSON_GetStringValue((const cJSON *const)str_item);

    // step 5: Validate - compute a small validation score derived from meaningful data flow
    validation_score = array_size + (int)(first_in_array != (cJSON *)0) + (int)(second_in_array != (cJSON *)0) + (int)(greeting_str != (char *)0);

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}