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
//<ID> 1418
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *label = "example_label";
    cJSON *label_ref = cJSON_CreateStringReference(label);
    double initial_vals[] = {1.0, 2.0, 3.0};
    cJSON *double_arr = cJSON_CreateDoubleArray(initial_vals, 3);
    cJSON_AddItemToObject(root, "label", label_ref);
    cJSON_AddItemToObject(root, "values", double_arr);
    cJSON_AddNumberToObject(root, "threshold", 0.75);

    // step 2: Configure
    double replacement_vals[] = {10.0, 20.0};
    cJSON *replacement_arr = cJSON_CreateDoubleArray(replacement_vals, 2);
    cJSON_ReplaceItemInObject(root, "values", replacement_arr);

    // step 3: Operate & Validate
    const cJSON *got_label = cJSON_GetObjectItemCaseSensitive(root, "label");
    const cJSON *got_values = cJSON_GetObjectItem(root, "values");
    const cJSON *got_threshold = cJSON_GetObjectItem(root, "threshold");
    const char *label_str = cJSON_GetStringValue(got_label);
    double threshold_val = cJSON_GetNumberValue(got_threshold);
    int values_count = cJSON_GetArraySize(got_values);
    const cJSON *first_val_item = cJSON_GetArrayItem(got_values, 0);
    double first_value = cJSON_GetNumberValue(first_val_item);
    (void)label_str;
    (void)threshold_val;
    (void)values_count;
    (void)first_value;
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);
    const char *version = cJSON_Version();
    (void)version;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}