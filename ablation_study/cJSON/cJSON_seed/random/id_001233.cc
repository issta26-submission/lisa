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
//<ID> 1233
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
    cJSON_bool is_raw = 0;
    int array_size = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate
    is_raw = cJSON_IsRaw(true_item);
    printed = cJSON_PrintBuffered(root, 128, 1);

    // step 5: Validate
    array_size = cJSON_GetArraySize(arr);
    validation_score = array_size + (int)(printed != (char *)0) + (int)(!is_raw) + (int)(root != (cJSON *)0) + (int)(true_item != (cJSON *)0);

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}