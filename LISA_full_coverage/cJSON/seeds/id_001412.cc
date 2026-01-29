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
//<ID> 1412
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *ver = cJSON_Version();
    cJSON *ver_ref = cJSON_CreateStringReference(ver);
    double initial_vals[] = {3.14159, 2.71828, -1.0};
    cJSON *double_arr = cJSON_CreateDoubleArray(initial_vals, 3);
    cJSON_AddItemToObject(root, "version", ver_ref);
    cJSON_AddItemToObject(root, "constants", double_arr);
    cJSON_AddNumberToObject(root, "answer", 42.0);

    // step 2: Configure
    double replacement_vals[] = {0.0, 1.0};
    cJSON *replacement_arr = cJSON_CreateDoubleArray(replacement_vals, 2);
    cJSON_ReplaceItemInObject(root, "constants", replacement_arr);

    // step 3: Operate & Validate
    cJSON *got_answer = cJSON_GetObjectItem(root, "answer");
    double answer_val = cJSON_GetNumberValue(got_answer);
    cJSON *got_version = cJSON_GetObjectItem(root, "version");
    const char *version_str = cJSON_GetStringValue(got_version);
    cJSON *got_constants = cJSON_GetObjectItem(root, "constants");
    cJSON *first_const = cJSON_GetArrayItem(got_constants, 0);
    double first_const_val = cJSON_GetNumberValue(first_const);
    (void)answer_val;
    (void)version_str;
    (void)first_const_val;
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