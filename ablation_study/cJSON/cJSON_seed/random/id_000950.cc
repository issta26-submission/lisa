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
//<ID> 950
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
    cJSON *n1 = (cJSON *)0;
    cJSON *n2 = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON_bool ok1 = (cJSON_bool)0;
    cJSON_bool ok2 = (cJSON_bool)0;
    char *json = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create root object and an array to hold nulls
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, array_key, arr);

    // step 3: Core operations - create null items and add them to the array
    n1 = cJSON_CreateNull();
    n2 = cJSON_CreateNull();
    ok1 = cJSON_AddItemToArray(arr, n1);
    ok2 = cJSON_AddItemToArray(arr, n2);

    // step 4: Operate - retrieve the array using object lookup and serialize
    fetched_arr = cJSON_GetObjectItem(root, array_key);
    json = cJSON_PrintUnformatted(root);

    // step 5: Validate - compute a simple score based on successful operations
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)ok1 + (int)ok2 + (int)(fetched_arr != (cJSON *)0) + (int)(json != (char *)0);
    (void)validation_score;

    // step 6: Cleanup - free serialized string and delete the JSON tree
    cJSON_free(json);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}