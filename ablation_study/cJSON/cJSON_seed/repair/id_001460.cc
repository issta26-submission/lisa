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
//<ID> 1460
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"obj\":{\"key\":\"val\",\"numbers\":[10,20,30]},\"arr\":[1,2,3]}";
    cJSON *root = cJSON_Parse(json);
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    int arr_count = cJSON_GetArraySize(arr);

    // step 2: Configure
    cJSON *orig_obj = cJSON_GetObjectItem(root, "obj");
    cJSON *dup_obj = cJSON_Duplicate(orig_obj, 1);
    cJSON *wrapper = cJSON_CreateObject();
    cJSON_AddItemToObject(wrapper, "original_obj", dup_obj);

    // step 3: Operate and Validate
    cJSON *got_dup = cJSON_GetObjectItem(wrapper, "original_obj");
    cJSON *numbers = cJSON_GetObjectItem(got_dup, "numbers");
    int numbers_count = cJSON_GetArraySize(numbers);
    cJSON *key_item = cJSON_GetObjectItem(got_dup, "key");
    char *key_str = cJSON_GetStringValue(key_item);
    int buf_len = arr_count + numbers_count + 16;
    char *scratch = (char *)cJSON_malloc((size_t)buf_len);
    memset(scratch, 0, (size_t)buf_len);
    scratch[0] = (char)('0' + (arr_count % 10));
    scratch[1] = (char)('0' + (numbers_count % 10));
    scratch[2] = key_str ? key_str[0] : 'n';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(wrapper);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}