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
//<ID> 2276
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
    cJSON_AddItemToObject(root, "raw", raw_item);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *name = cJSON_CreateString("device-007");
    cJSON_AddItemToObject(meta, "name", name);

    // step 2: Configure
    cJSON *got_raw = cJSON_GetObjectItem(root, "raw");
    cJSON_bool raw_flag = cJSON_IsRaw(got_raw);
    cJSON *raw_flag_num = cJSON_CreateNumber((double)raw_flag);
    cJSON_AddItemToObject(root, "raw_is_raw", raw_flag_num);
    cJSON *true_node = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "always_true", true_node);

    // step 3: Operate and Validate
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    buffer[0] = (char)('0' + (int)raw_flag);
    (void)printed;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}