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
//<ID> 1108
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
    cJSON *true_item = (cJSON *)0;
    cJSON *name_str = (cJSON *)0;
    cJSON *fetched_readings = (cJSON *)0;
    cJSON *fetched_name = (cJSON *)0;
    cJSON *fetched_element = (cJSON *)0;
    cJSON_bool element_is_true = (cJSON_bool)0;
    const char *retrieved_name = (const char *)0;
    int validation_score = 0;

    // step 2: Setup - create root object, an array, a true item and a string
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    true_item = cJSON_CreateTrue();
    name_str = cJSON_CreateString("sensor_unit");

    // step 3: Configure - attach the true item to the array and attach array and string to root
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToObject(root, "readings", arr);
    cJSON_AddItemToObject(root, "name", name_str);

    // step 4: Operate - fetch the array and name from the root, and fetch the first array element
    fetched_readings = cJSON_GetObjectItem(root, "readings");
    fetched_name = cJSON_GetObjectItem(root, "name");
    fetched_element = cJSON_GetArrayItem(fetched_readings, 0);

    // step 5: Validate - inspect fetched values and compute a small validation score
    element_is_true = cJSON_IsTrue(fetched_element);
    retrieved_name = cJSON_GetStringValue(fetched_name);
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(true_item != (cJSON *)0) + (int)(name_str != (cJSON *)0) + (int)(fetched_readings != (cJSON *)0) + (int)(fetched_name != (cJSON *)0) + (int)(fetched_element != (cJSON *)0) + (int)(element_is_true) + (int)(retrieved_name != (const char *)0);
    (void)validation_score;

    // step 6: Cleanup - delete root which also frees attached children
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}