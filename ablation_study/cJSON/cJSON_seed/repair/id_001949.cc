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
//<ID> 1949
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
    cJSON *label = cJSON_CreateString("init_label");
    cJSON_AddItemToObject(meta, "label", label);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 2: Configure
    cJSON *info = cJSON_AddObjectToObject(meta, "info");
    cJSON *desc = cJSON_CreateString("detailed description");
    cJSON_AddItemToObject(info, "description", desc);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(meta, "ok", flag);

    // step 3: Operate and Validate
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    cJSON_bool arr_is_array = cJSON_IsArray(arr);
    cJSON_bool flag_is_false = cJSON_IsFalse(flag);
    buffer[0] = (char)('0' + (arr_is_array % 10));
    buffer[1] = (char)('0' + (flag_is_false % 10));
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}