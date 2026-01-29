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
//<ID> 1103
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
    cJSON *str_elem = (cJSON *)0;
    cJSON *true_elem = (cJSON *)0;
    cJSON *label_elem = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *fetched_index_item = (cJSON *)0;
    cJSON *fetched_label = (cJSON *)0;
    const char *retrieved_label = (const char *)0;
    int validation_score = 0;

    // step 2: Setup - create root object, an array containing a string and true, and a separate label string
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_elem = cJSON_CreateString("temperature");
    true_elem = cJSON_CreateTrue();
    label_elem = cJSON_CreateString("sensor_007");

    // step 3: Configure - populate the array and attach to root, attach the label
    cJSON_AddItemToArray(arr, str_elem);
    cJSON_AddItemToArray(arr, true_elem);
    cJSON_AddItemToObject(root, "readings", arr);
    cJSON_AddItemToObject(root, "label", label_elem);

    // step 4: Operate - fetch the array and label via object access, fetch the second array item by index
    fetched_arr = cJSON_GetObjectItem(root, "readings");
    fetched_label = cJSON_GetObjectItem(root, "label");
    fetched_index_item = cJSON_GetArrayItem(fetched_arr, 1);

    // step 5: Validate - extract the label string pointer and compute a small validation score
    retrieved_label = cJSON_GetStringValue(fetched_label);
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(str_elem != (cJSON *)0) + (int)(true_elem != (cJSON *)0) + (int)(label_elem != (cJSON *)0) + (int)(fetched_arr != (cJSON *)0) + (int)(fetched_index_item != (cJSON *)0) + (int)(fetched_label != (cJSON *)0) + (int)(retrieved_label != (const char *)0);
    (void)validation_score;

    // step 6: Cleanup - delete root which also frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}