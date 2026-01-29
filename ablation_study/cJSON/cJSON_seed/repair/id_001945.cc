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
//<ID> 1945
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
    cJSON_AddItemToObject(root, "flag", cJSON_CreateFalse());

    // step 2: Configure
    cJSON *elem0 = cJSON_CreateString("elem0");
    cJSON_AddItemToArray(arr, elem0);

    // step 3: Operate and Validate
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    cJSON *flag_item = cJSON_GetObjectItem(root, "flag");
    cJSON_bool flag_is_false = cJSON_IsFalse(flag_item);
    cJSON_bool array_check = cJSON_IsArray(arr);
    cJSON *got_label = cJSON_GetObjectItem(meta, "label");
    char *label_value = cJSON_GetStringValue(got_label);
    buffer[0] = label_value[0];
    buffer[1] = (char)('0' + (array_check % 10));
    buffer[2] = (char)('0' + (flag_is_false % 10));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}