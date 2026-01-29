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
//<ID> 958
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *array_key = "my_array";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *null_item = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    char *json_str = (char *)0;
    cJSON_bool add_ok_array = (cJSON_bool)0;
    cJSON_bool add_ok_object = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object, an array and a null item
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    null_item = cJSON_CreateNull();

    // step 3: Configure - add the null item to the array and attach the array to the root object
    add_ok_array = cJSON_AddItemToArray(arr, null_item);
    add_ok_object = cJSON_AddItemToObject(root, array_key, arr);

    // step 4: Operate - retrieve the array back from the root using GetObjectItem
    fetched_arr = cJSON_GetObjectItem(root, array_key);

    // step 5: Validate - serialize the root to a string and compute a light validation score
    json_str = cJSON_PrintUnformatted(root);
    validation_score = (int)(root != (cJSON *)0) + (int)add_ok_array + (int)add_ok_object + (int)(fetched_arr == arr) + (int)(json_str != (char *)0);

    // step 6: Cleanup - free the serialized string and delete the root (which frees children)
    cJSON_free(json_str);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}