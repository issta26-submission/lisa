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
//<ID> 1580
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
    cJSON *maybe = cJSON_CreateNull();
    cJSON_AddItemToObject(child, "maybe", maybe);
    cJSON *greet = cJSON_CreateString("hello");
    cJSON_AddItemToObject(child, "greet", greet);

    // step 2: Configure
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", child_ref);
    cJSON_bool has_child = cJSON_HasObjectItem(root, "child");
    cJSON_bool is_null = cJSON_IsNull(maybe);

    // step 3: Operate and Validate
    int buf_len = 256;
    char *outBuf = (char *)cJSON_malloc((size_t)buf_len);
    memset(outBuf, 0, (size_t)buf_len);
    cJSON_bool printed = cJSON_PrintPreallocated(root, outBuf, buf_len, 1);
    (void)printed;
    char *scratch = (char *)cJSON_malloc(64);
    memset(scratch, 0, 64);
    scratch[0] = (char)('0' + (int)has_child);
    scratch[1] = (char)('0' + (int)is_null);
    scratch[2] = outBuf[0] ? outBuf[0] : ' ';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(outBuf);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}