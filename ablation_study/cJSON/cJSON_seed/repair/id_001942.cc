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
//<ID> 1942
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
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON *greeting = cJSON_CreateString("hello");
    cJSON_AddItemToObject(meta, "greeting", greeting);

    // step 2: Configure
    char *buffer = (char *)cJSON_malloc((size_t)128);
    memset(buffer, 0, (size_t)128);
    cJSON_PrintPreallocated(root, buffer, 128, 1);

    // step 3: Operate and Validate
    cJSON_bool items_is_array = cJSON_IsArray(items);
    cJSON_bool flag_is_false = cJSON_IsFalse(flag);
    cJSON *got_greeting = cJSON_GetObjectItem(meta, "greeting");
    const char *got_str = cJSON_GetStringValue(got_greeting);
    buffer[0] = (char)('0' + (items_is_array % 10));
    buffer[1] = (char)('0' + (flag_is_false % 10));
    buffer[2] = got_str ? got_str[0] : 'X';
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}