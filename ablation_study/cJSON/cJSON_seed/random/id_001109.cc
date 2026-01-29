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
//<ID> 1109
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
    cJSON *fetched_first = (cJSON *)0;
    cJSON *fetched_second = (cJSON *)0;
    const char *retrieved_string = (const char *)0;

    // step 2: Setup - create object, array and items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("example_string");
    true_item = cJSON_CreateTrue();

    // step 3: Configure - populate array and attach to root object
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate - retrieve object member and array elements
    fetched_arr = cJSON_GetObjectItem(root, "items");
    fetched_first = cJSON_GetArrayItem(fetched_arr, 0);
    fetched_second = cJSON_GetArrayItem(fetched_arr, 1);
    retrieved_string = cJSON_GetStringValue(fetched_first);

    // step 5: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)fetched_second;
    (void)retrieved_string;
    return 66;
}