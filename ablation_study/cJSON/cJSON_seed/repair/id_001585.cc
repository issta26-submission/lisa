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
//<ID> 1585
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
    cJSON_AddItemToObject(root, "child", child);
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(child, "value", num);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(child, "maybe_null", maybe_null);
    cJSON *greeting = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", child_ref);

    // step 2: Configure
    cJSON_bool has_child = cJSON_HasObjectItem(root, "child");
    cJSON_bool has_value = cJSON_HasObjectItem(child, "value");
    cJSON *maybe_null_ref = cJSON_GetObjectItem(child, "maybe_null");
    cJSON_bool is_null = cJSON_IsNull(maybe_null_ref);

    // step 3: Operate and Validate
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buf, buf_len, 1);
    char *scratch = (char *)cJSON_malloc(16);
    memset(scratch, 0, 16);
    scratch[0] = (char)('0' + (int)has_child);
    scratch[1] = (char)('0' + (int)has_value);
    scratch[2] = (char)('0' + (int)is_null);
    scratch[3] = printed ? (buf[0] ? buf[0] : ' ') : 'E';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(buf);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}