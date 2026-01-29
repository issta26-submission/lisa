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
//<ID> 501
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *label_item = cJSON_CreateString("example_label");
    cJSON_AddItemToObject(root, "label", label_item);
    cJSON_AddBoolToObject(root, "enabled", 1);
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON_AddItemToObject(root, "rawdata", raw_item);

    // step 2: Configure
    cJSON *retrieved_label = cJSON_GetObjectItem(root, "label");
    cJSON *retrieved_enabled = cJSON_GetObjectItem(root, "enabled");
    cJSON *retrieved_raw = cJSON_GetObjectItem(root, "rawdata");
    cJSON_bool enabled_is_true = cJSON_IsTrue(retrieved_enabled);
    cJSON_bool raw_is_raw = cJSON_IsRaw(retrieved_raw);
    char *label_str = (char *)cJSON_GetStringValue(retrieved_label);
    size_t buf_len = 64 + (size_t)enabled_is_true * 16 + (size_t)raw_is_raw * 8 + (label_str ? (size_t)8 : (size_t)0);
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);

    // step 3: Operate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *copy_label = cJSON_CreateString(label_str);
    cJSON_AddItemToObject(root, "label_copy", copy_label);
    cJSON_bool copy_label_is_string = cJSON_IsRaw(copy_label); (void)copy_label_is_string;

    // step 4: Validate & Cleanup
    (void)buffer;
    (void)label_str;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}