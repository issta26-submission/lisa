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
//<ID> 1354
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"alpha\",\"values\":[1.5,2.5,3.5],\"flag\":true}";
    cJSON *parsed = cJSON_Parse(json_text);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "source", parsed);
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddTrueToObject(meta, "verified");
    float extras_arr[3];
    extras_arr[0] = 4.5f;
    extras_arr[1] = 5.5f;
    extras_arr[2] = 6.5f;
    cJSON *farr = cJSON_CreateFloatArray(extras_arr, 3);
    cJSON_AddItemToObject(root, "extra_floats", farr);
    cJSON *nul = cJSON_CreateNull();
    cJSON_AddItemToObject(meta, "optional", nul);

    // step 3: Operate
    cJSON *vals = cJSON_GetObjectItem(parsed, "values");
    cJSON *second = cJSON_GetArrayItem(vals, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON_AddNumberToObject(meta, "second_val", second_val);

    // step 4: Validate & Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}