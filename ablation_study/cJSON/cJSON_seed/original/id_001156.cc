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
//<ID> 1156
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(config, "name", name);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(config, "values", values);
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON_AddItemToArray(values, n1);

    // step 2: Configure
    char *name_after_set = cJSON_SetValuestring(name, "updated");
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(config, "enabled", flag);
    cJSON_bool is_obj = cJSON_IsObject(config);
    cJSON_bool is_invalid = cJSON_IsInvalid(flag);
    cJSON *temp = cJSON_AddStringToObject(root, "temp", "transient");
    cJSON *detached = cJSON_DetachItemFromObject(root, "temp");

    // step 3: Operate
    char *detached_after_set = cJSON_SetValuestring(detached, "reattached");
    cJSON_AddItemToArray(values, detached);
    cJSON *dup_config = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "config_copy", dup_config);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(values, n2);
    int arr_size = cJSON_GetArraySize(values);

    // step 4: Validate & Cleanup
    char *flat = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root);

    (void)name_after_set;
    (void)detached_after_set;
    (void)is_obj;
    (void)is_invalid;
    (void)arr_size;
    // API sequence test completed successfully
    return 66;
}