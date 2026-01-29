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
//<ID> 250
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
    cJSON *num0 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, num0);
    cJSON *b_true = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, b_true);
    cJSON *b_false = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, b_false);

    // step 2: Configure
    cJSON *insert_num = cJSON_CreateNumber(42.0);
    cJSON_InsertItemInArray(arr, 1, insert_num);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "type", "demo");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(arr);
    cJSON *item_at_2 = cJSON_GetArrayItem(arr, 2);
    cJSON_bool is_item2_bool = cJSON_IsBool(item_at_2);
    cJSON_bool is_true_flag = cJSON_IsTrue(b_true);
    double first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    double summary_val = (double)size + (double)is_item2_bool + (double)is_true_flag + first_val;
    cJSON_AddNumberToObject(root, "summary", summary_val);
    char *printed = cJSON_PrintUnformatted(root);
    void *tmp = cJSON_malloc(128);
    memset(tmp, 0, 128);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}