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
//<ID> 1331
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON *raw_item = cJSON_CreateRaw("{\"in_raw\":true}");
    cJSON *str_item = cJSON_CreateString("example_value");
    cJSON *num_item = cJSON_CreateNumber(42.0);

    // step 2: Configure
    cJSON_AddItemToObject(root, "active", flag_false);
    cJSON_AddItemToObject(root, "raw_data", raw_item);
    cJSON_AddItemToObject(root, "greeting", str_item);
    cJSON_AddItemToObject(root, "answer", num_item);

    // step 3: Operate & Validate
    cJSON_bool has_raw = cJSON_HasObjectItem(root, "raw_data");
    cJSON *has_raw_num = cJSON_CreateNumber((double)has_raw);
    cJSON_AddItemToObject(root, "has_raw", has_raw_num);
    cJSON *g_item = cJSON_GetObjectItem(root, "greeting");
    char *g_value = cJSON_GetStringValue(g_item);
    cJSON *g_copy = cJSON_CreateString(g_value);
    cJSON_AddItemToObject(root, "greeting_copy", g_copy);
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}