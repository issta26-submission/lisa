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
//<ID> 1228
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"arr\":[10,20,30],\"meta\":{\"scale\":2}}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *arr = cJSON_GetObjectItem(parsed, "arr");
    cJSON *a0 = cJSON_GetArrayItem(arr, 0);
    cJSON *a1 = cJSON_GetArrayItem(arr, 1);
    cJSON *a2 = cJSON_GetArrayItem(arr, 2);
    int numbers[3];
    numbers[0] = (int)cJSON_GetNumberValue(a0);
    numbers[1] = (int)cJSON_GetNumberValue(a1);
    numbers[2] = (int)cJSON_GetNumberValue(a2);
    cJSON *int_array = cJSON_CreateIntArray(numbers, 3);
    cJSON_AddItemToObject(root, "imported_ints", int_array);

    // step 3: Operate & Validate
    cJSON *meta = cJSON_GetObjectItem(parsed, "meta");
    cJSON *scale_item = cJSON_GetObjectItem(meta, "scale");
    double scale = cJSON_GetNumberValue(scale_item);
    int sum = numbers[0] + numbers[1] + numbers[2];
    cJSON *scaled_sum = cJSON_CreateNumber(sum * scale);
    cJSON_AddItemToObject(root, "scaled_sum", scaled_sum);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}