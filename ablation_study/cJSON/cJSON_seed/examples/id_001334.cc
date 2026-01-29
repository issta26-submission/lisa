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
//<ID> 1334
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
    cJSON *raw_payload = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON *flag = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON_AddItemToObject(root, "raw_payload", raw_payload);
    cJSON_AddItemToObject(root, "active", flag);

    // step 3: Operate & Validate
    cJSON_bool has_greet = cJSON_HasObjectItem(root, "greeting");
    char *g_val = cJSON_GetStringValue(greeting);
    cJSON *greeting_copy = cJSON_CreateString(g_val);
    cJSON_AddItemToObject(root, "greeting_copy", greeting_copy);
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    (void)has_greet;
    (void)printed_ok;
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}