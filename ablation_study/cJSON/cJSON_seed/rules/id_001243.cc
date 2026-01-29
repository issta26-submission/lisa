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
//<ID> 1243
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"meta\":{\"count\":1},\"flags\":{\"on\":true},\"value\":2.5}";
    cJSON *root = NULL;
    cJSON *meta = NULL;
    cJSON *count = NULL;
    cJSON *flags = NULL;
    cJSON *on = NULL;
    cJSON *value = NULL;
    cJSON *arr = NULL;
    cJSON *newnum = NULL;
    char *printed_unfmt = NULL;
    char prebuf[256] = {0};
    cJSON_bool pre_result = 0;
    cJSON_bool is_on = 0;
    double num_value = 0.0;

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    meta = cJSON_GetObjectItem(root, "meta");
    count = cJSON_GetObjectItem(meta, "count");
    flags = cJSON_GetObjectItem(root, "flags");
    on = cJSON_GetObjectItem(flags, "on");
    value = cJSON_GetObjectItem(root, "value");

    // step 3: Operate / Validate
    is_on = cJSON_IsTrue(on);
    num_value = cJSON_GetNumberValue(value);
    newnum = cJSON_CreateNumber(num_value + 10.0);
    arr = cJSON_CreateArray();
    cJSON_AddItemToArray(arr, newnum);
    cJSON_AddItemToObject(root, "extras", arr);
    printed_unfmt = cJSON_PrintUnformatted(root);
    pre_result = cJSON_PrintPreallocated(root, prebuf, (int)sizeof(prebuf), 0);
    (void)count;
    (void)is_on;
    (void)pre_result;
    (void)num_value;
    (void)prebuf;
    (void)arr;

    // step 4: Cleanup
    cJSON_free(printed_unfmt);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}