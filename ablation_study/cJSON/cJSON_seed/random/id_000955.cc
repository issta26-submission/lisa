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
//<ID> 955
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *array_name = "numbers";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *null_item = (cJSON *)0;
    cJSON_bool add_ok = (cJSON_bool)0;
    cJSON *fetched = (cJSON *)0;
    char *json_str = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create root object, an array, and a null item
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    null_item = cJSON_CreateNull();

    // step 3: Configure - add the null item to the array and attach the array to the root object
    add_ok = cJSON_AddItemToArray(arr, null_item);
    cJSON_AddItemToObject(root, array_name, arr);

    // step 4: Operate - retrieve the array from the root and serialize the root to a string
    fetched = cJSON_GetObjectItem(root, array_name);
    json_str = cJSON_PrintUnformatted(root);

    // step 5: Validate - compute a simple validation score from the operations and results
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)add_ok + (int)(fetched == arr) + (int)(json_str != (char *)0);
    (void)validation_score;

    // step 6: Cleanup - free serialized string and delete the JSON structure
    cJSON_free(json_str);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}