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
//<ID> 1333
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
    cJSON *greeting = cJSON_CreateString("hello world");
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON *raw_blob = cJSON_CreateRaw("{\"raw_key\":123}");

    // step 2: Configure
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "greeting", greeting);
    cJSON_AddItemToObject(root, "flag", flag_false);
    cJSON_AddItemToObject(root, "raw_blob", raw_blob);

    // step 3: Operate & Validate
    cJSON_bool has_meta = cJSON_HasObjectItem(root, "meta");
    cJSON *has_meta_num = cJSON_CreateNumber((double)has_meta);
    cJSON_AddItemToObject(root, "has_meta_num", has_meta_num);
    char *g_val = cJSON_GetStringValue(greeting);
    cJSON *g_copy = cJSON_CreateString(g_val);
    cJSON_AddItemToObject(root, "greeting_copy", g_copy);
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    cJSON *print_status = cJSON_CreateNumber((double)printed_ok);
    cJSON_AddItemToObject(root, "print_ok", print_status);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}