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
//<ID> 1392
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float float_vals[3] = {1.5f, 2.5f, 3.5f};
    double double_vals[2] = {10.0, 20.0};
    cJSON *float_array = cJSON_CreateFloatArray(float_vals, 3);
    cJSON *double_array = cJSON_CreateDoubleArray(double_vals, 2);
    cJSON_AddItemToObject(root, "floats", float_array);
    cJSON_AddItemToObject(root, "doubles", double_array);
    cJSON_AddStringToObject(root, "status", "initial");

    // step 2: Configure
    cJSON *replacement_status = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInObject(root, "status", replacement_status);

    // step 3: Operate
    cJSON *got_floats = cJSON_GetObjectItemCaseSensitive(root, "floats");
    cJSON *got_doubles = cJSON_GetObjectItemCaseSensitive(root, "doubles");
    cJSON *first_double_item = cJSON_GetArrayItem(got_doubles, 0);
    double first_double_value = cJSON_GetNumberValue(first_double_item);
    cJSON *adjusted = cJSON_CreateNumber(first_double_value + 1.0);
    cJSON_AddItemToObject(root, "adjusted_first_double", adjusted);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);

    // step 4: Validate & Cleanup
    cJSON *verify_status = cJSON_GetObjectItemCaseSensitive(root, "status");
    const char *status_str = cJSON_GetStringValue(verify_status);
    (void)status_str;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}