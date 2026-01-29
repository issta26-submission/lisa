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
//<ID> 1217
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
    cJSON *bool_item = (cJSON *)0;
    cJSON *first = (cJSON *)0;
    cJSON *second = (cJSON *)0;
    const char *str_value = (const char *)0;
    int array_size = 0;
    int validation_score = 0;

    // step 2: Setup - create root object and an array member
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure - create a string and a boolean, add them to the array
    str_item = cJSON_CreateString("hello");
    bool_item = cJSON_CreateBool(1);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, bool_item);

    // step 4: Operate - obtain array size and retrieve items by index
    array_size = cJSON_GetArraySize(arr);
    first = cJSON_GetArrayItem(arr, 0);
    second = cJSON_GetArrayItem(arr, 1);
    str_value = cJSON_GetStringValue(first);

    // step 5: Validate - compute a small validation score using meaningful data flow
    validation_score = array_size + (int)(str_value != (const char *)0) + (int)(second == bool_item);

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}