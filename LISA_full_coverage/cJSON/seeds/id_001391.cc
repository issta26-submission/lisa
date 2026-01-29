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
//<ID> 1391
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float fvals1[3] = {1.1f, 2.2f, 3.3f};
    double dvals1[3] = {4.4, 5.5, 6.6};
    cJSON *float_array = cJSON_CreateFloatArray(fvals1, 3);
    cJSON *double_array = cJSON_CreateDoubleArray(dvals1, 3);
    cJSON_AddItemToObject(root, "floats", float_array);
    cJSON_AddItemToObject(root, "doubles", double_array);

    // step 2: Configure
    double dvals_repl[3] = {7.7, 8.8, 9.9};
    cJSON *replacement_double_array = cJSON_CreateDoubleArray(dvals_repl, 3);
    cJSON_ReplaceItemInObject(root, "floats", replacement_double_array);
    cJSON *found_doubles = cJSON_GetObjectItemCaseSensitive(root, "doubles");
    cJSON *found_floats_after_replace = cJSON_GetObjectItemCaseSensitive(root, "floats");

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buffer_copy = (char *)cJSON_malloc(printed_len + 1);
    memset(buffer_copy, 0, printed_len + 1);
    memcpy(buffer_copy, printed, printed_len + 1);
    cJSON_Minify(buffer_copy);
    cJSON_free(printed);
    cJSON_free(buffer_copy);

    // step 4: Validate & Cleanup
    (void)found_doubles;
    (void)found_floats_after_replace;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}