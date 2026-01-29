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
//<ID> 1339
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON *false_item = cJSON_CreateFalse();
    cJSON *msg_item = cJSON_CreateString("hello from cJSON");
    
    // step 2: Configure
    cJSON_AddItemToObject(root, "raw", raw_item);
    cJSON_AddItemToObject(root, "flag", false_item);
    cJSON_AddItemToObject(root, "msg", msg_item);
    
    // step 3: Operate & Validate
    cJSON_bool has_raw = cJSON_HasObjectItem(root, "raw");
    cJSON *has_raw_num = cJSON_CreateNumber((double)has_raw);
    cJSON_AddItemToObject(root, "has_raw", has_raw_num);
    cJSON *msg_lookup = cJSON_GetObjectItem(root, "msg");
    const char *msg_value = cJSON_GetStringValue(msg_lookup);
    cJSON *msg_copy = cJSON_CreateString(msg_value);
    cJSON_AddItemToObject(root, "msg_copy", msg_copy);
    int buf_len = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    (void)printed_ok;
    cJSON *printed_raw = cJSON_CreateRaw(buffer);
    cJSON_AddItemToObject(root, "printed_raw", printed_raw);
    
    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(buffer);
    // API sequence test completed successfully
    return 66;
}