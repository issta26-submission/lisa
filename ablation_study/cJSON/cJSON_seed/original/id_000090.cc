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
//<ID> 90
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
    cJSON_AddStringToObject(meta, "name", "test_dataset");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(items, n1);

    // step 2: Configure
    cJSON *flag = cJSON_AddTrueToObject(root, "active");
    cJSON *s1 = cJSON_CreateString("entry");
    cJSON_AddItemToArray(items, s1);
    float fnums[2] = {4.5f, 5.5f};
    cJSON *farr = cJSON_CreateFloatArray(fnums, 2);
    cJSON_AddItemToObject(root, "floats", farr);

    // step 3: Operate and Validate
    int buf_len = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    cJSON_bool root_is_object = cJSON_IsObject(root);
    cJSON_bool active_true = cJSON_IsTrue(flag);
    int summary = (int)printed_ok + (int)root_is_object + (int)active_true + (int)buffer[0];
    (void)summary;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}