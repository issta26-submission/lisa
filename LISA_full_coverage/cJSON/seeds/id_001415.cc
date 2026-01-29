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
//<ID> 1415
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double initial_values[] = {1.1, 2.2};
    cJSON *double_arr = cJSON_CreateDoubleArray(initial_values, 2);
    cJSON_AddItemToObject(root, "values", double_arr);
    cJSON *label_ref = cJSON_CreateStringReference("initial_label");
    cJSON_AddItemToObject(root, "label", label_ref);
    cJSON *num_item = cJSON_AddNumberToObject(root, "pi", 3.14159);

    // step 2: Configure
    const char *version = cJSON_Version();
    double replacement_values[] = {10.0, 20.0, 30.0};
    cJSON *replacement_arr = cJSON_CreateDoubleArray(replacement_values, 3);
    cJSON_ReplaceItemInObject(root, "values", replacement_arr);

    // step 3: Operate & Validate
    cJSON *got_values = cJSON_GetObjectItem(root, "values");
    int got_count = cJSON_GetArraySize(got_values);
    cJSON *first_elem = cJSON_GetArrayItem(got_values, 0);
    double first_val = cJSON_GetNumberValue(first_elem);
    cJSON *got_label = cJSON_GetObjectItem(root, "label");
    const char *label_str = cJSON_GetStringValue(got_label);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    (void)version;
    (void)got_count;
    (void)first_val;
    (void)label_str;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}