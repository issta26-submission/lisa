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
//<ID> 1227
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"id\":7,\"values\":[2,4,6]}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    cJSON *root = cJSON_CreateObject();
    int extra_nums[3] = {10, 20, 30};
    cJSON *extra_arr = cJSON_CreateIntArray(extra_nums, 3);

    // step 2: Configure
    cJSON_AddItemToObject(root, "extras", extra_arr);
    cJSON *parsed_clone = cJSON_Duplicate(parsed, 1);
    cJSON_AddItemToObject(root, "original_clone", parsed_clone);

    // step 3: Operate & Validate
    cJSON *id_item = cJSON_GetObjectItem(parsed, "id");
    double id_val = cJSON_GetNumberValue(id_item);
    cJSON *values = cJSON_GetObjectItem(parsed, "values");
    cJSON *v0 = cJSON_GetArrayItem(values, 0);
    cJSON *v1 = cJSON_GetArrayItem(values, 1);
    cJSON *v2 = cJSON_GetArrayItem(values, 2);
    double sum = cJSON_GetNumberValue(v0) + cJSON_GetNumberValue(v1) + cJSON_GetNumberValue(v2) + id_val;
    cJSON *computed = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "computed_sum", computed);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}