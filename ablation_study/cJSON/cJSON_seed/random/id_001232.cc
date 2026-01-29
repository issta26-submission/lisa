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
//<ID> 1232
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
    char *printed = (char *)0;
    int array_size = 0;
    cJSON_bool is_raw = 0;
    int validation_score = 0;

    // step 2: Initialize - create object, array and a true boolean
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    true_item = cJSON_CreateTrue();

    // step 3: Configure - add the boolean into the array and attach the array to the object
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToObject(root, "data", arr);

    // step 4: Operate - inspect array, check raw-flag on the boolean, and produce a buffered print
    array_size = cJSON_GetArraySize(arr);
    is_raw = cJSON_IsRaw(true_item);
    printed = cJSON_PrintBuffered(root, 256, 1);

    // step 5: Validate - compute a small validation score from meaningful data flow
    validation_score = array_size + (int)(!is_raw) + (int)(printed != (char *)0) + (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(true_item != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - free the printed buffer and delete the root (which frees attached items)
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}