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
//<ID> 1235
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
    int arr_size = 0;
    cJSON_bool is_raw_flag = 0;
    int validation_score = 0;

    // step 2: Initialize - create a root object and an array
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure - create a true boolean and add it into the array
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, true_item);

    // step 4: Operate - inspect item type and produce a buffered print of the root
    is_raw_flag = cJSON_IsRaw(true_item);
    printed = cJSON_PrintBuffered(root, 256, 1);

    // step 5: Validate - compute a small validation score from meaningful data flow
    arr_size = cJSON_GetArraySize(arr);
    validation_score = arr_size + (int)(printed != (char *)0) + (int)(!is_raw_flag) + (int)(root != (cJSON *)0) + (int)(true_item != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - free printed buffer and delete the root (which frees children)
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}