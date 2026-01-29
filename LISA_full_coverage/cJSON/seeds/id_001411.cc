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
//<ID> 1411
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double vals[] = { 2.718, 3.1415, 1.4142 };
    cJSON *double_arr = cJSON_CreateDoubleArray(vals, 3);
    const char *version = cJSON_Version();
    cJSON *version_ref = cJSON_CreateStringReference(version);
    cJSON_AddItemToObject(root, "values", double_arr);
    cJSON_AddItemToObject(root, "version", version_ref);
    cJSON *num_item = cJSON_AddNumberToObject(root, "threshold", 42.0);

    // step 2: Configure
    cJSON *got_values = cJSON_GetObjectItem(root, "values");
    int values_count = cJSON_GetArraySize(got_values);
    cJSON *first_value_item = cJSON_GetArrayItem(got_values, 0);
    double first_value = cJSON_GetNumberValue(first_value_item);
    cJSON *got_num = cJSON_GetObjectItem(root, "threshold");
    double threshold = cJSON_GetNumberValue(got_num);
    const char *got_version = cJSON_GetStringValue(cJSON_GetObjectItem(root, "version"));
    (void)values_count;
    (void)first_value;
    (void)threshold;
    (void)got_version;
    (void)num_item;

    // step 3: Operate & Validate
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