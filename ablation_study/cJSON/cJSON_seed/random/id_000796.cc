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
//<ID> 796
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
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool is_true = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("hello");
    true_item = cJSON_CreateTrue();

    // step 3: Configure
    cJSON_AddItemToObject(root, "arr", arr);
    added1 = cJSON_AddItemToArray(arr, str_item);
    added2 = cJSON_AddItemToArray(arr, true_item);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "arr");
    item0 = cJSON_GetArrayItem(retrieved_arr, 0);
    item1 = cJSON_GetArrayItem(retrieved_arr, 1);
    is_true = cJSON_IsTrue(item1);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    validation_score = (retrieved_arr != (cJSON *)0) + (item0 != (cJSON *)0) + (item1 != (cJSON *)0) + (int)is_true + (printed != (char *)0) + (int)added1 + (int)added2;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}