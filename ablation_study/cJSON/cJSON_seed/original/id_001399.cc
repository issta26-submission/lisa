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
//<ID> 1399
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float floats[] = {1.5f, 2.5f, 3.5f};
    cJSON *float_arr = cJSON_CreateFloatArray(floats, 3);
    double doubles[] = {10.0, 20.0};
    cJSON *double_arr = cJSON_CreateDoubleArray(doubles, 2);
    cJSON_AddItemToObject(root, "float_array", float_arr);
    cJSON_AddItemToObject(root, "double_array", double_arr);
    cJSON_AddStringToObject(root, "label", "initial");

    // step 2: Configure
    double replacement_vals[] = {100.0, 200.0, 300.0};
    cJSON *replacement_double_arr = cJSON_CreateDoubleArray(replacement_vals, 3);
    cJSON_bool replaced = cJSON_ReplaceItemInObject(root, "double_array", replacement_double_arr);
    (void)replaced;

    // step 3: Operate & Validate
    const cJSON *got_float_arr = cJSON_GetObjectItemCaseSensitive(root, "float_array");
    const cJSON *got_double_arr = cJSON_GetObjectItemCaseSensitive(root, "double_array");
    int float_count = cJSON_GetArraySize(got_float_arr);
    int double_count = cJSON_GetArraySize(got_double_arr);
    const cJSON *f0 = cJSON_GetArrayItem(got_float_arr, 0);
    const cJSON *d0 = cJSON_GetArrayItem(got_double_arr, 0);
    double first_float_value = cJSON_GetNumberValue(f0);
    double first_double_value = cJSON_GetNumberValue(d0);
    (void)float_count;
    (void)double_count;
    (void)first_float_value;
    (void)first_double_value;
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}