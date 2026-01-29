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
//<ID> 1410
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double initial_nums[] = {1.1, 2.2, 3.3};
    cJSON *double_arr = cJSON_CreateDoubleArray(initial_nums, 3);
    cJSON_AddItemToObject(root, "numbers", double_arr);
    cJSON *version_ref = cJSON_CreateStringReference(cJSON_Version());
    cJSON_AddItemToObject(root, "cjson_version", version_ref);
    cJSON_AddNumberToObject(root, "pi", 3.141592653589793);

    // step 2: Configure
    double replacement_vals[] = {10.0, 20.0, 30.0};
    cJSON *replacement_double_arr = cJSON_CreateDoubleArray(replacement_vals, 3);
    cJSON_ReplaceItemInObject(root, "numbers", replacement_double_arr);

    // step 3: Operate & Validate
    cJSON *got_numbers = cJSON_GetObjectItem(root, "numbers");
    int count = cJSON_GetArraySize(got_numbers);
    cJSON *first_elem = cJSON_GetArrayItem(got_numbers, 0);
    double first_val = cJSON_GetNumberValue(first_elem);
    cJSON *got_version_item = cJSON_GetObjectItem(root, "cjson_version");
    const char *version_str = cJSON_GetStringValue(got_version_item);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);
    (void)count;
    (void)first_val;
    (void)version_str;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}