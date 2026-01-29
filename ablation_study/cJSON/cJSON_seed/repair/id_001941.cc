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
//<ID> 1941
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *name_str = cJSON_CreateString("example_name");
    cJSON_AddItemToObject(meta, "name", name_str);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(meta, "flag", flag_false);

    // step 2: Configure
    cJSON_bool arr_is_array = cJSON_IsArray(arr);
    cJSON *got_flag = cJSON_GetObjectItem(meta, "flag");
    cJSON_bool flag_is_false = cJSON_IsFalse(got_flag);

    // step 3: Operate and Validate
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    cJSON *got_name_item = cJSON_GetObjectItem(meta, "name");
    char *name_value = cJSON_GetStringValue(got_name_item);
    buffer[0] = name_value ? name_value[0] : 'X';
    buffer[1] = (char)('0' + (arr_is_array % 10));
    buffer[2] = (char)('0' + (flag_is_false % 10));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}