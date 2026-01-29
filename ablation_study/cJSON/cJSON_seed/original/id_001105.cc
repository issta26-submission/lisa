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
//<ID> 1105
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"initial\",\"value\":2.718,\"raw\":\"<raw>\"}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 2: Configure
    double doubles_arr[] = {1.1, 2.2, 3.3};
    cJSON *darray = cJSON_CreateDoubleArray(doubles_arr, 3);
    cJSON_AddItemToObject(root, "doubles", darray);
    cJSON_AddTrueToObject(root, "flag");
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON_SetValuestring(name_item, "updated_name");

    // step 3: Operate
    cJSON *raw_item = cJSON_GetObjectItem(root, "raw");
    cJSON_bool raw_check = cJSON_IsRaw(raw_item);
    cJSON *val_item = cJSON_GetObjectItem(root, "value");
    cJSON_bool val_is_number = cJSON_IsNumber(val_item);
    double val_num = cJSON_GetNumberValue(val_item);
    double computed_val = val_num + (double)raw_check + 2.0 * (double)val_is_number;
    cJSON_AddNumberToObject(root, "computed", computed_val);

    // step 4: Validate & Cleanup
    char *unformatted = cJSON_PrintUnformatted(root);
    char *prealloc = (char *)cJSON_malloc(256);
    memset(prealloc, 0, 256);
    cJSON_PrintPreallocated(root, prealloc, 256, 0);
    cJSON_free(unformatted);
    cJSON_free(prealloc);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}