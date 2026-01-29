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
//<ID> 1231
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
    int array_size = 0;
    cJSON_bool raw_flag = 0;
    int validation_score = 0;

    // step 2: Initialize - create an object, an array and a true boolean
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    true_item = cJSON_CreateTrue();

    // step 3: Configure - attach the boolean to the array and the array to the root object
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate - inspect the array, check raw-ness of the boolean and generate a buffered print
    array_size = cJSON_GetArraySize(arr);
    first_item = cJSON_GetArrayItem(arr, 0);
    raw_flag = cJSON_IsRaw(first_item);
    printed = cJSON_PrintBuffered(root, 256, (cJSON_bool)1);

    // step 5: Validate - compute a small validation score from meaningful data flow
    validation_score = array_size + (int)(first_item != (cJSON *)0) + (int)(printed != (char *)0) + (int)(!raw_flag) + (int)(root != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - free printed buffer and delete the root (which frees children)
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}