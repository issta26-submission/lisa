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
//<ID> 1109
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"old\",\"value\":2.5,\"raw\":\"some_raw\",\"list\":[1,2]}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON_SetValuestring(name_item, "new_name");
    cJSON_AddTrueToObject(root, "enabled");
    double extra_vals[] = {4.4, 5.5, 6.6};
    cJSON *double_arr = cJSON_CreateDoubleArray(extra_vals, 3);
    cJSON_AddItemToObject(root, "dbls", double_arr);

    // step 3: Operate
    cJSON *raw_item = cJSON_GetObjectItem(root, "raw");
    cJSON_bool raw_check = cJSON_IsRaw(raw_item);
    cJSON *val_item = cJSON_GetObjectItem(root, "value");
    cJSON_bool val_is_number = cJSON_IsNumber(val_item);
    double val_num = cJSON_GetNumberValue(val_item);
    double computed = val_num + (double)raw_check + (double)val_is_number;
    cJSON_AddNumberToObject(root, "computed", computed);

    // step 4: Validate & Cleanup
    char *unformatted = cJSON_PrintUnformatted(root);
    char *prealloc = (char *)cJSON_malloc(512);
    memset(prealloc, 0, 512);
    cJSON_PrintPreallocated(root, prealloc, 512, 1);
    cJSON_free(unformatted);
    cJSON_free(prealloc);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}