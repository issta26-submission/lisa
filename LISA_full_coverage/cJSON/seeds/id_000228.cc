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
//<ID> 228
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
    cJSON *num_ref1 = cJSON_CreateNumber(123.456);
    cJSON_AddItemReferenceToArray(arr, num_ref1);
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, str_item);
    cJSON *num_ref2 = cJSON_CreateNumber(7.0);
    cJSON_AddItemReferenceToArray(arr, num_ref2);
    cJSON *bool_item = cJSON_AddBoolToObject(root, "flag", 0);

    // step 2: Configure
    cJSON *detached = cJSON_DetachItemFromArray(arr, 1);
    cJSON_AddItemToObject(root, "detached", detached);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    int arr_size = cJSON_GetArraySize(arr);
    double first_num = cJSON_GetNumberValue(num_ref1);
    cJSON *got_bool = cJSON_GetObjectItem(root, "flag");
    cJSON_bool is_false = cJSON_IsFalse(got_bool);
    int summary = arr_size + (int)first_num + (int)is_false + root->type;
    void *tmp = cJSON_malloc(64);
    memset(tmp, 0, 64);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}