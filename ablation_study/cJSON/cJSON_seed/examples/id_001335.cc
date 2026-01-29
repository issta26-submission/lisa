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
//<ID> 1335
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
    cJSON *flag = cJSON_CreateFalse();
    cJSON *raw_blob = cJSON_CreateRaw("{\"inner\":true}");
    cJSON *greeting = cJSON_CreateString("hello cJSON");

    // step 2: Configure
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "flag", flag);
    cJSON_AddItemToObject(root, "raw_blob", raw_blob);
    cJSON_AddItemToObject(root, "greeting", greeting);

    // step 3: Operate & Validate
    cJSON_bool has_flag = cJSON_HasObjectItem(meta, "flag");
    cJSON *has_flag_num = cJSON_CreateNumber((double)has_flag);
    cJSON_AddItemToObject(root, "has_flag", has_flag_num);
    const char *g_val = cJSON_GetStringValue(greeting);
    cJSON *g_copy = cJSON_CreateString(g_val);
    cJSON_AddItemToObject(root, "greeting_copy", g_copy);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    cJSON *raw_from_print = cJSON_CreateRaw(buffer);
    cJSON_AddItemToObject(root, "raw_from_print", raw_from_print);

    // step 4: Cleanup
    cJSON_Delete(root);

    return 66;
    // API sequence test completed successfully
}