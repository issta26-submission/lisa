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
//<ID> 667
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *auto_array = cJSON_AddArrayToObject(root, "array_auto");
    cJSON *manual_array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "array_manual", manual_array);
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON_AddItemToArray(manual_array, str_item);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON_AddStringToObject(child, "greeting", "world");
    cJSON_AddNumberToObject(child, "num", 42.0);
    cJSON_AddStringToObject(root, "root_note", "testing cJSON sequence");
    cJSON *dup_child = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObject(root, "child_copy", dup_child);

    // step 3: Operate and Validate
    cJSON *greet = cJSON_GetObjectItem(child, "greeting");
    cJSON_bool greet_is_string = cJSON_IsString(greet);
    cJSON_AddBoolToObject(root, "greeting_is_string", greet_is_string);
    int buf_len = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    cJSON_AddStringToObject(root, "snapshot", buffer);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}