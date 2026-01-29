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
//<ID> 798
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
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved0 = (cJSON *)0;
    cJSON *retrieved1 = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added0 = 0;
    cJSON_bool added1 = 0;
    cJSON_bool added_obj = 0;
    cJSON_bool is_true = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("example");
    true_item = cJSON_CreateTrue();

    // step 3: Configure
    added0 = cJSON_AddItemToArray(arr, str_item);
    added1 = cJSON_AddItemToArray(arr, true_item);
    added_obj = cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "items");
    retrieved0 = cJSON_GetArrayItem(retrieved_arr, 0);
    retrieved1 = cJSON_GetArrayItem(retrieved_arr, 1);
    is_true = cJSON_IsTrue(retrieved1);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    validation_score = (retrieved_arr != (cJSON *)0) + (retrieved0 != (cJSON *)0) + (retrieved1 != (cJSON *)0) + (int)is_true + (int)added0 + (int)added1 + (int)added_obj + (printed != (char *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}