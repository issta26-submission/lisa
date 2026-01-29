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
//<ID> 497
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"items\":[1,2,3],\"meta\":{\"name\":\"test\"}}";
    cJSON *root = cJSON_Parse(json);
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON_bool items_is_array = cJSON_IsArray(items);
    double first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(items, 0));

    // step 2: Configure
    size_t buf_len = (size_t)64 + (size_t)(32 * (size_t)items_is_array);
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    cJSON *false_item = cJSON_AddFalseToObject(root, "ok");
    cJSON *null_item = cJSON_AddNullToObject(root, "missing");
    cJSON *computed = cJSON_CreateNumber(first_val + (double)items_is_array);
    cJSON_AddItemToObject(root, "computed", computed);

    // step 3: Operate
    char *snapshot = cJSON_PrintUnformatted(root);
    char *computed_snapshot = cJSON_PrintUnformatted(computed);
    double comp_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "computed"));

    // step 4: Validate & Cleanup
    (void)comp_val;
    cJSON_free(snapshot);
    cJSON_free(computed_snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}