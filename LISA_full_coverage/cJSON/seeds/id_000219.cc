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
//<ID> 219
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *words[] = { "alpha", "beta", "gamma" };
    cJSON *str_arr = cJSON_CreateStringArray(words, 3);
    cJSON_AddItemToObject(root, "words", str_arr);
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "count", num);

    // step 2: Configure
    double set_result = cJSON_SetNumberHelper(num, 100.5);
    cJSON *dup_arr = cJSON_Duplicate(str_arr, 1);
    cJSON_AddItemToObject(root, "words_copy", dup_arr);
    cJSON *first_item = cJSON_GetArrayItem(str_arr, 0);
    cJSON *detached = cJSON_DetachItemViaPointer(str_arr, first_item);
    cJSON_AddItemToObject(root, "first_word", detached);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    int array_size = cJSON_GetArraySize(str_arr);
    double number_value = cJSON_GetNumberValue(num);
    const char *first_str = cJSON_GetStringValue(detached);
    int summary = array_size + (int)number_value + (first_str ? (int)first_str[0] : 0) + root->type;
    void *tmp_buf = cJSON_malloc(64);
    memset(tmp_buf, 0, 64);
    (void)set_result;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}