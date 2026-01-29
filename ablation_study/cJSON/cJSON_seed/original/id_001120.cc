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
//<ID> 1120
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"initial\",\"count\":3,\"values\":[1,2,3]}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *old_name = cJSON_SetValuestring(name_item, "updated_name");
    double extra_values[] = {4.1, 5.2, 6.3};
    cJSON *double_arr = cJSON_CreateDoubleArray(extra_values, 3);
    cJSON_AddItemToObject(root, "extras", double_arr);

    // step 3: Operate
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    cJSON_bool count_is_number = cJSON_IsNumber(count_item);
    double count_val = cJSON_GetNumberValue(count_item);
    double computed = count_val + (double)count_is_number;
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