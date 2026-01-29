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
//<ID> 1581
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
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(child, "num", num);
    cJSON *ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", ref);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "nothing", null_item);
    cJSON *str = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greet", str);

    // step 2: Configure
    cJSON_bool has_ref = cJSON_HasObjectItem(root, "child_ref");
    cJSON_bool has_nothing = cJSON_HasObjectItem(root, "nothing");
    cJSON *got_nothing = cJSON_GetObjectItem(root, "nothing");
    cJSON_bool is_null = cJSON_IsNull(got_nothing);

    // step 3: Operate and Validate
    int buf_len = 256;
    char *outbuf = (char *)cJSON_malloc((size_t)buf_len);
    memset(outbuf, 0, (size_t)buf_len);
    cJSON_bool printed = cJSON_PrintPreallocated(root, outbuf, buf_len, 1);
    char *scratch = (char *)cJSON_malloc(16);
    memset(scratch, 0, 16);
    scratch[0] = (char)('0' + (int)has_ref);
    scratch[1] = (char)('0' + (int)is_null);
    scratch[2] = (char)('0' + (int)printed);
    scratch[3] = outbuf[0] ? outbuf[0] : ' ';

    // step 4: Cleanup
    cJSON_free(outbuf);
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_Delete(child);
    return 66;
    // API sequence test completed successfully
}