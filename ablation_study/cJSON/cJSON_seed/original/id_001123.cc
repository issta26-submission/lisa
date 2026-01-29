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
//<ID> 1123
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"initial\",\"value\":3.14}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *old_name = cJSON_SetValuestring(name_item, "updated_name");
    double dbl_vals[] = {1.1, 2.2, 3.3};
    cJSON *dbl_arr = cJSON_CreateDoubleArray(dbl_vals, 3);
    cJSON_AddItemToObject(root, "doubles", dbl_arr);

    // step 3: Operate
    cJSON *value_item = cJSON_GetObjectItem(root, "value");
    cJSON_bool value_is_number = cJSON_IsNumber(value_item);
    double value_num = cJSON_GetNumberValue(value_item);
    double computed = value_num + (double)value_is_number + dbl_vals[0] + dbl_vals[1] + dbl_vals[2];
    cJSON_AddNumberToObject(root, "computed", computed);

    // step 4: Validate & Cleanup
    char *unformatted = cJSON_PrintUnformatted(root);
    char *prealloc = (char *)cJSON_malloc(256);
    memset(prealloc, 0, 256);
    cJSON_PrintPreallocated(root, prealloc, 256, 1);
    cJSON_free(unformatted);
    cJSON_free(prealloc);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}