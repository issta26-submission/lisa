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
//<ID> 1238
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
    cJSON *first_item = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool is_raw = 0;
    int array_size = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array, and a true boolean item
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    true_item = cJSON_CreateTrue();

    // step 3: Configure - add the boolean into the array and attach the array to the root object
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToObject(root, "flags", arr);

    // step 4: Operate - inspect the array, check raw-ness of the first element, and print the root into a buffered string
    first_item = cJSON_GetArrayItem(arr, 0);
    is_raw = cJSON_IsRaw(first_item);
    printed = cJSON_PrintBuffered(root, 256, 1);

    // step 5: Validate - derive a small validation score from meaningful data flow
    array_size = cJSON_GetArraySize(arr);
    validation_score = array_size + (int)(first_item != (cJSON *)0) + (int)(!is_raw) + (int)(printed != (char *)0);
    (void)validation_score;

    // step 6: Cleanup - free printed buffer and delete the root (which frees attached items)
    if (printed) { cJSON_free((void *)printed); }
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}