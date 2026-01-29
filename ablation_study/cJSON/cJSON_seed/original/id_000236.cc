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
//<ID> 236
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
    cJSON *b = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, b);
    cJSON *s = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, s);
    cJSON *num_added = cJSON_AddNumberToObject(root, "pi", 3.14159);

    // step 2: Configure
    cJSON_bool is_bool = cJSON_IsBool(b);
    cJSON_DeleteItemFromArray(arr, 1);

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    int arr_size = cJSON_GetArraySize(arr);
    int bool_flag = (int)is_bool;
    int summary = arr_size + bool_flag + (printed ? (int)printed[0] : 0) + (num_added ? num_added->type : 0);
    void *tmp = cJSON_malloc(128);
    memset(tmp, 0, 128);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}