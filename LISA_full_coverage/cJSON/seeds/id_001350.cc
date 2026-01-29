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
//<ID> 1350
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const float numbers[4] = {1.25f, 2.5f, -3.75f, 4.0f};
    cJSON *root = cJSON_CreateObject();
    cJSON *nested = cJSON_AddObjectToObject(root, "nested");
    cJSON *farr = cJSON_CreateFloatArray(numbers, 4);
    cJSON_AddItemToObject(nested, "floats", farr);
    cJSON *nill = cJSON_CreateNull();
    cJSON_AddItemToObject(nested, "maybe", nill);
    cJSON_AddTrueToObject(root, "ready");

    // step 2: Configure
    cJSON *nested_copy = cJSON_Duplicate(nested, 1);
    cJSON_AddItemToObject(root, "nested_copy", nested_copy);

    // step 3: Operate
    cJSON *got_arr = cJSON_GetObjectItem(nested, "floats");
    cJSON *second = cJSON_GetArrayItem(got_arr, 1);
    double second_val = cJSON_GetNumberValue(second);
    cJSON *doubled = cJSON_CreateNumber(second_val * 2.0);
    cJSON_AddItemToObject(root, "second_doubled", doubled);

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