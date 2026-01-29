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
//<ID> 790
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *array = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *retrieved_array = (cJSON *)0;
    cJSON *retrieved_str_item = (cJSON *)0;
    cJSON *retrieved_true_item = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_str = 0;
    cJSON_bool added_true = 0;
    cJSON_bool is_true = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    array = cJSON_CreateArray();
    str_item = cJSON_CreateString("example");
    true_item = cJSON_CreateTrue();

    // step 3: Configure
    cJSON_AddItemToObject(root, "arr", array);
    added_str = cJSON_AddItemToArray(array, str_item);
    added_true = cJSON_AddItemToArray(array, true_item);

    // step 4: Operate
    retrieved_array = cJSON_GetObjectItem(root, "arr");
    retrieved_str_item = cJSON_GetArrayItem(retrieved_array, 0);
    retrieved_true_item = cJSON_GetArrayItem(retrieved_array, 1);
    is_true = cJSON_IsTrue(retrieved_true_item);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    validation_score = (retrieved_array != (cJSON *)0) + (retrieved_str_item != (cJSON *)0) + (retrieved_true_item != (cJSON *)0) + (printed != (char *)0) + (int)is_true + (int)added_str + (int)added_true;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}