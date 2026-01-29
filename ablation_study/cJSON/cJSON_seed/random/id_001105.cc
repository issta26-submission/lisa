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
//<ID> 1105
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
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON *label = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *fetched_elem0 = (cJSON *)0;
    cJSON *fetched_elem1 = (cJSON *)0;
    cJSON *fetched_label = (cJSON *)0;
    const char *retrieved_str = (const char *)0;
    int validation_score = 0;

    // step 2: Setup - create root object, an array and items (string and true)
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    elem0 = cJSON_CreateString("elem_zero");
    elem1 = cJSON_CreateTrue();
    label = cJSON_CreateString("sensor_A");

    // step 3: Configure - assemble the structure
    cJSON_AddItemToArray(arr, elem0);
    cJSON_AddItemToArray(arr, elem1);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddItemToObject(root, "label", label);

    // step 4: Operate - fetch the array and items using getters
    fetched_arr = cJSON_GetObjectItem(root, "data");
    fetched_elem0 = cJSON_GetArrayItem(fetched_arr, 0);
    fetched_elem1 = cJSON_GetArrayItem(fetched_arr, 1);
    fetched_label = cJSON_GetObjectItem(root, "label");
    retrieved_str = cJSON_GetStringValue(fetched_label);

    // step 5: Validate - compute a small validation score based on pointers and values
    validation_score = (int)(root != (cJSON *)0)
                     + (int)(arr != (cJSON *)0)
                     + (int)(elem0 != (cJSON *)0)
                     + (int)(elem1 != (cJSON *)0)
                     + (int)(label != (cJSON *)0)
                     + (int)(fetched_arr != (cJSON *)0)
                     + (int)(fetched_elem0 != (cJSON *)0)
                     + (int)(fetched_elem1 != (cJSON *)0)
                     + (int)(fetched_label != (cJSON *)0)
                     + (int)(retrieved_str != (const char *)0);
    (void)validation_score;

    // step 6: Cleanup - delete root which also frees attached children
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}