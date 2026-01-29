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
//<ID> 239
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *count_node = cJSON_AddNumberToObject(root, "count", 3.0);
    cJSON *n1 = cJSON_CreateNumber(12.34);
    cJSON *n2 = cJSON_CreateNumber(56.78);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON *b = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, b);

    // step 2: Configure
    cJSON_DeleteItemFromArray(arr, 1);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON *first_val_node = cJSON_AddNumberToObject(root, "first_value", first_val);
    int bool_flag = (int)cJSON_IsBool(b);
    cJSON *bool_flag_node = cJSON_AddNumberToObject(root, "bool_flag", (double)bool_flag);

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    void *tmp = cJSON_malloc(64);
    memset(tmp, 0, 64);
    int summary = (int)first_val + bool_flag + cJSON_GetArraySize(arr);
    (void)summary;
    (void)count_node;
    (void)first_val_node;
    (void)bool_flag_node;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}