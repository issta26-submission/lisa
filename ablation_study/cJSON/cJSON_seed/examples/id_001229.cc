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
//<ID> 1229
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"values\":[10,20,30],\"meta\":2}";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *values = cJSON_GetObjectItem(parsed, "values");
    cJSON *meta = cJSON_GetObjectItem(parsed, "meta");
    cJSON *second = cJSON_GetArrayItem(values, 1);
    double second_val = cJSON_GetNumberValue(second);
    double factor = cJSON_GetNumberValue(meta);
    cJSON_AddNumberToObject(root, "computed", second_val * factor);
    int extras_arr[3];
    extras_arr[0] = 7;
    extras_arr[1] = 8;
    extras_arr[2] = 9;
    cJSON *extras = cJSON_CreateIntArray(extras_arr, 3);
    cJSON_AddItemToObject(root, "extra", extras);

    // step 3: Operate & Validate
    char *out = cJSON_Print(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}