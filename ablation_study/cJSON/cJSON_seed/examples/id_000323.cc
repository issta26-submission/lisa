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
//<ID> 323
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(num_item, 42.0);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "answer", num_item);
    cJSON_AddItemToObject(root, "empty", null_item);

    // step 2: Configure
    cJSON *arr = cJSON_CreateArray();
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "list", arr);

    // step 3: Operate & Validate
    cJSON *answer = cJSON_GetObjectItem(root, "answer");
    double answer_val = cJSON_GetNumberValue(answer);
    cJSON *sum_item = cJSON_CreateNumber(answer_val + 8.0);
    cJSON_AddItemToObject(root, "sum", sum_item);
    char *printed = cJSON_Print(root);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "empty");

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}