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
//<ID> 1338
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON *raw_blob = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON *greeting = cJSON_CreateString("hello world");

    // step 2: Configure
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "flag", flag_false);
    cJSON_AddItemToObject(root, "raw_blob", raw_blob);
    cJSON_AddItemToObject(root, "greeting", greeting);

    // step 3: Operate & Validate
    cJSON_bool has_raw = cJSON_HasObjectItem(root, "raw_blob");
    cJSON *has_raw_num = cJSON_CreateNumber((double)has_raw);
    cJSON_AddItemToObject(root, "has_raw", has_raw_num);
    cJSON *g_item = cJSON_GetObjectItem(root, "greeting");
    char *g_value = (char *)cJSON_GetStringValue(g_item);
    cJSON *g_copy = cJSON_CreateString(g_value);
    cJSON_AddItemToObject(root, "greeting_copy", g_copy);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)printed_ok;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}