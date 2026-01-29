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
//<ID> 1419
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *version_text = cJSON_Version();
    cJSON *version_ref = cJSON_CreateStringReference(version_text);
    double nums[] = {1.23, 4.56, 7.89};
    cJSON *double_arr = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "version", version_ref);
    cJSON_AddItemToObject(root, "values", double_arr);
    cJSON_AddNumberToObject(root, "pi", 3.14159);

    // step 2: Configure
    cJSON *got_pi_item = cJSON_GetObjectItem(root, "pi");
    double got_pi = cJSON_GetNumberValue(got_pi_item);
    cJSON *got_values = cJSON_GetObjectItem(root, "values");
    cJSON *first_val = cJSON_GetArrayItem(got_values, 0);
    double first_val_num = cJSON_GetNumberValue(first_val);
    cJSON *got_version = cJSON_GetObjectItem(root, "version");
    const char *got_version_str = cJSON_GetStringValue(got_version);

    // step 3: Operate & Validate
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    (void)got_pi;
    (void)first_val_num;
    (void)got_version_str;
    cJSON_free(printed);
    cJSON_free(buf);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}