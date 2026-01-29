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
//<ID> 1587
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
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(child, "value", num);
    cJSON_AddItemToObject(root, "child", child);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "nullable", null_item);
    cJSON *greeting = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", greeting);

    // step 2: Configure
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", child_ref);

    // step 3: Operate and Validate
    cJSON_bool has_child = cJSON_HasObjectItem(root, "child");
    cJSON *nullable_ptr = cJSON_GetObjectItem(root, "nullable");
    cJSON_bool is_null = cJSON_IsNull(nullable_ptr);
    cJSON *value_ptr = cJSON_GetObjectItem(child, "value");
    double val = cJSON_GetNumberValue(value_ptr);
    int buf_len = 512;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buf, buf_len, 1);
    size_t out_len = buf ? strlen(buf) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(64);
    memset(scratch, 0, 64);
    scratch[0] = (char)('0' + (int)has_child);
    scratch[1] = (char)('0' + (int)is_null);
    scratch[2] = out_len ? buf[0] : ' ';
    scratch[3] = (char)('0' + ((int)val % 10));
    scratch[4] = (char)('0' + (int)printed);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(buf);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}