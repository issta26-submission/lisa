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
//<ID> 1330
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON *raw_item = cJSON_CreateRaw("\"raw_text\"");
    cJSON *greeting = cJSON_CreateString("hello");

    // step 2: Configure
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "flag", flag_false);
    cJSON_AddItemToObject(child, "raw", raw_item);
    cJSON_AddItemToObject(root, "greeting", greeting);

    // step 3: Operate & Validate
    cJSON *g_item = cJSON_GetObjectItem(root, "greeting");
    char *g_value = cJSON_GetStringValue(g_item);
    cJSON *g_copy = cJSON_CreateString(g_value);
    cJSON_AddItemToObject(root, "greeting_copy", g_copy);
    cJSON_bool has_child = cJSON_HasObjectItem(root, "child");
    cJSON *present_num = cJSON_CreateNumber((double)has_child);
    cJSON_AddItemToObject(root, "child_present", present_num);
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    (void)printed_ok;
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}