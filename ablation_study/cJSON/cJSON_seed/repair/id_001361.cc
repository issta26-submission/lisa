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
//<ID> 1361
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *raw = cJSON_CreateRaw("{\"note\":\"raw_value\"}");
    cJSON_AddItemToArray(arr, raw);
    cJSON *num_item = cJSON_CreateNumber(42.5);
    cJSON_AddItemToArray(arr, num_item);

    // step 2: Configure
    cJSON_AddItemToObject(root, "data", arr);
    cJSON *str_item = cJSON_CreateString("hello_world");
    cJSON_AddItemToObject(root, "greeting", str_item);
    cJSON *count_item = cJSON_AddNumberToObject(root, "count", 7.0);

    // step 3: Operate and Validate
    double count_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));
    const char *greet = cJSON_GetStringValue(cJSON_GetObjectItem(root, "greeting"));
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    char *buf = (char *)scratch;
    buf[0] = out[0];
    buf[1] = greet[0];
    buf[2] = (char)('0' + ((int)count_val % 10));
    buf[3] = (char)('0' + ((int)num_item->valuedouble % 10));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}