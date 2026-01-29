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
//<ID> 954
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *array_name = "my_array";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *null_item1 = (cJSON *)0;
    cJSON *null_item2 = (cJSON *)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    cJSON_bool add_ok3 = (cJSON_bool)0;
    cJSON *fetched_arr = (cJSON *)0;
    char *json_str = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create root and array
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure - create null items and add them to the array
    null_item1 = cJSON_CreateNull();
    null_item2 = cJSON_CreateNull();
    add_ok1 = cJSON_AddItemToArray(arr, null_item1);
    add_ok2 = cJSON_AddItemToArray(arr, null_item2);

    // step 4: Attach array to root
    add_ok3 = cJSON_AddItemToObject(root, array_name, arr);

    // step 5: Operate / Validate - retrieve the array, serialize, and compute a simple score
    fetched_arr = cJSON_GetObjectItem(root, array_name);
    json_str = cJSON_PrintUnformatted(root);
    validation_score = (int)(root != (cJSON *)0) + (int)add_ok1 + (int)add_ok2 + (int)add_ok3 + (int)(fetched_arr != (cJSON *)0) + (int)(json_str != (char *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free((void *)json_str);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}