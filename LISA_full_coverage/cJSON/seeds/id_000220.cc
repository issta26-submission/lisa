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
//<ID> 220
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
    cJSON_AddItemToObject(root, "array", arr);

    // step 2: Configure
    cJSON *num = cJSON_CreateNumber(3.14);
    cJSON *str = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToArray(arr, str);
    cJSON *ref_item = cJSON_CreateString("to_be_referenced");
    cJSON_AddItemReferenceToArray(arr, ref_item);
    cJSON *detached = cJSON_DetachItemFromArray(arr, 2);
    cJSON_AddItemToObject(root, "detached", detached);

    // step 3: Operate and Validate
    cJSON *flag = cJSON_AddBoolToObject(root, "ok", 0);
    cJSON_bool flag_is_false = cJSON_IsFalse(flag);
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    int array_size = cJSON_GetArraySize(arr);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double first_num = cJSON_GetNumberValue(first);
    void *tmp = cJSON_malloc(128);
    memset(tmp, 0, 128);
    int summary = array_size + (int)first_num + (int)flag_is_false + (printed ? (int)printed[0] : 0);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}