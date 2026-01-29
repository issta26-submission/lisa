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
//<ID> 1218
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
    cJSON *first_in_array = (cJSON *)0;
    char *string_value = (char *)0;
    int array_size = 0;
    int validation_score = 0;

    // step 2: Setup - create root object and an array, attach array to root
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);

    // step 3: Configure - create a string and a boolean, add them to the array
    str_item = cJSON_CreateString("example");
    cJSON_AddItemToArray(arr, str_item);
    bool_item = cJSON_CreateBool(1);
    cJSON_AddItemToArray(arr, bool_item);

    // step 4: Operate - obtain array size and retrieve the first element, read its string value
    array_size = cJSON_GetArraySize(arr);
    first_in_array = cJSON_GetArrayItem(arr, 0);
    string_value = cJSON_GetStringValue(first_in_array);

    // step 5: Validate - compute a small validation score from meaningful data flow
    validation_score = array_size + (int)(string_value != (char *)0) + (int)(bool_item != (cJSON *)0);

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}