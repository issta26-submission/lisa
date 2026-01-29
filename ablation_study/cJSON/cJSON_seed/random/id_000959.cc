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
//<ID> 959
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
    cJSON *first_element = (cJSON *)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    char *json_str = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create root object and an array
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure - create a null item and add it to the array, then attach the array to the root
    null_item = cJSON_CreateNull();
    add_ok1 = cJSON_AddItemToArray(arr, null_item);
    add_ok2 = cJSON_AddItemToObject(root, array_key, arr);

    // step 4: Operate - retrieve the array via object lookup and inspect its first element
    fetched_arr = cJSON_GetObjectItem(root, array_key);
    first_element = cJSON_GetArrayItem(fetched_arr, 0);

    // step 5: Validate - serialize and compute a simple validation score that exercises results
    json_str = cJSON_PrintUnformatted(root);
    validation_score = (int)(root != (cJSON *)0) + (int)add_ok1 + (int)add_ok2 + (int)(fetched_arr == arr) + (int)cJSON_IsNull(first_element) + (int)(json_str != (char *)0);
    (void)validation_score;

    // step 6: Cleanup - free serialized string and delete the root (which owns the array and its child)
    cJSON_free(json_str);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}