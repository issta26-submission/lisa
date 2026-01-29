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
//<ID> 178
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
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *got1 = (cJSON *)0;
    char *json_text = (char *)0;
    int count = 0;
    cJSON_bool had_true = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(10.0);
    num2 = cJSON_CreateNumber(20.0);
    true_item = cJSON_CreateTrue();

    // step 3: Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_InsertItemInArray(arr, 1, true_item);
    cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate
    count = cJSON_GetArraySize(arr);
    got1 = cJSON_GetArrayItem(arr, 1);
    had_true = cJSON_IsTrue(got1);

    // step 5: Validate
    cJSON_AddNumberToObject(root, "count", (double)count);
    cJSON_AddBoolToObject(root, "contains_true_at_1", had_true);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}