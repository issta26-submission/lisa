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
//<ID> 2148
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
    cJSON_AddItemToObject(root, "data", arr);

    // step 2: Configure
    cJSON *msg = cJSON_AddStringToObject(root, "message", "hello_world");
    cJSON *obj_item = cJSON_CreateObject();
    cJSON_AddStringToObject(obj_item, "name", "item_name");
    cJSON_bool added = cJSON_AddItemToObject(root, "item_object", obj_item);
    cJSON_bool is_arr = cJSON_IsArray(arr);
    cJSON *flagstr = cJSON_AddStringToObject(root, "array_flag", is_arr ? "true" : "false");

    // step 3: Operate and Validate
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON *dup_msg = cJSON_GetObjectItem(dup, "message");
    const char *dup_msg_str = cJSON_GetStringValue(dup_msg);
    char *buffer = (char *)cJSON_malloc((size_t)32);
    memset(buffer, 0, (size_t)32);
    buffer[0] = dup_msg_str ? dup_msg_str[0] : '\0';
    buffer[1] = (char)('0' + (added ? 1 : 0));
    buffer[2] = '\0';
    cJSON_AddStringToObject(dup, "meta", buffer);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}