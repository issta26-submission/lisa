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
//<ID> 1106
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"old\",\"num\":3.5,\"raw\":\"<raw_payload>\"}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 2: Configure
    double nums[2] = {9.9, 8.8};
    cJSON *dbl_arr = cJSON_CreateDoubleArray(nums, 2);
    cJSON_AddItemToObject(root, "doubles", dbl_arr);
    cJSON_AddTrueToObject(root, "active");
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *new_name_ptr = cJSON_SetValuestring(name_item, "new_name");

    // step 3: Operate
    cJSON *raw_item = cJSON_GetObjectItem(root, "raw");
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw_item);
    cJSON *num_item = cJSON_GetObjectItem(root, "num");
    cJSON_bool num_is_number = cJSON_IsNumber(num_item);
    double num_val = cJSON_GetNumberValue(num_item);
    double computed = num_val + (double)raw_is_raw + (double)num_is_number;
    cJSON_AddNumberToObject(root, "computed", computed);
    char *unformatted = cJSON_PrintUnformatted(root);
    char *prealloc = (char *)cJSON_malloc(256);
    memset(prealloc, 0, 256);
    cJSON_PrintPreallocated(root, prealloc, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_free(unformatted);
    cJSON_free(prealloc);
    (void)new_name_ptr;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}