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
//<ID> 1332
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
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *raw_item = cJSON_CreateRaw("\"raw_text\"");
    cJSON *greeting = cJSON_CreateString("hello");

    // step 2: Configure
    cJSON_AddItemToObject(meta, "raw", raw_item);
    cJSON_AddItemToObject(meta, "enabled", false_item);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "greeting", greeting);

    // step 3: Operate & Validate
    cJSON_bool has_meta = cJSON_HasObjectItem(root, "meta");
    cJSON *has_meta_num = cJSON_CreateNumber((double)has_meta);
    cJSON_AddItemToObject(root, "has_meta_flag", has_meta_num);
    cJSON *meta_ref = cJSON_GetObjectItem(root, "meta");
    cJSON *raw_ref = cJSON_GetObjectItem(meta_ref, "raw");
    char *raw_value = cJSON_GetStringValue(raw_ref);
    cJSON *raw_copy = cJSON_CreateString(raw_value);
    cJSON_AddItemToObject(root, "raw_copy", raw_copy);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool print_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)print_ok;

    // step 4: Cleanup
    cJSON_Delete(root);

    return 66;
    // API sequence test completed successfully
}