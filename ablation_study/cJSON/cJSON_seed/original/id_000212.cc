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
//<ID> 212
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *const words[] = { "alpha", "beta", "gamma" };
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateStringArray(words, 3);
    cJSON_AddItemToObject(root, "words", arr);
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", num);
    double set_result = cJSON_SetNumberHelper(num, 84.0);

    // step 2: Configure
    cJSON *dup_arr = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "words_copy", dup_arr);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(child, "inner", cJSON_CreateString("inside"));
    cJSON_AddItemToObject(root, "child", child);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemViaPointer(root, child);
    cJSON_AddItemToObject(root, "detached", detached);
    cJSON *ans_item = cJSON_GetObjectItemCaseSensitive(root, "answer");
    double ans_value = cJSON_GetNumberValue(ans_item);
    int size_original = cJSON_GetArraySize(arr);
    int size_dup = cJSON_GetArraySize(dup_arr);
    cJSON *inner = cJSON_GetObjectItemCaseSensitive(detached, "inner");
    const char *inner_str = cJSON_GetStringValue(inner);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_PrintPreallocated(root, buffer, 256, 0);
    cJSON_Minify(buffer);
    int summary = (int)ans_value + size_original + size_dup + (inner_str ? (int)inner_str[0] : 0) + (int)set_result + root->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}