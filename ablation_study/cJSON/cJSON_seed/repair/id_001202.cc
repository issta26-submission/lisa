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
//<ID> 1202
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("example");
    cJSON *enabled = cJSON_CreateBool(1);
    cJSON *value = cJSON_CreateNumber(42.5);
    cJSON *maybe = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddItemToObject(root, "enabled", enabled);
    cJSON_AddItemToObject(root, "value", value);
    cJSON_AddItemToObject(root, "maybe", maybe);

    // step 2: Configure
    cJSON *info = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "info", info);
    cJSON *owner = cJSON_CreateString("tester");
    cJSON *idnum = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(info, "owner", owner);
    cJSON_AddItemToObject(info, "id", idnum);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    cJSON *maybe_ptr = cJSON_GetObjectItem(root, "maybe");
    cJSON_bool is_null_flag = cJSON_IsNull(maybe_ptr);
    const char *name_val = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    double value_num = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "value"));
    double final_value = value_num + (double)(is_null_flag) + (double)((unsigned char)out[0]);
    cJSON_AddNumberToObject(root, "final", final_value);
    (void)name_val;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}