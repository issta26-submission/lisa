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
//<ID> 1397
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float floats_init[3] = {1.1f, 2.2f, 3.3f};
    cJSON *floats = cJSON_CreateFloatArray(floats_init, 3);
    cJSON_AddItemToObject(root, "floats", floats);
    double doubles_initial[2] = {10.1, 20.2};
    cJSON *doubles_a = cJSON_CreateDoubleArray(doubles_initial, 2);
    cJSON_AddItemToObject(root, "doubles", doubles_a);
    cJSON_AddStringToObject(root, "name", "original_name");

    // step 2: Configure
    double doubles_replacement_vals[3] = {30.3, 40.4, 50.5};
    cJSON *doubles_b = cJSON_CreateDoubleArray(doubles_replacement_vals, 3);
    cJSON_ReplaceItemInObject(root, "doubles", doubles_b);
    cJSON *replacement_name = cJSON_CreateString("replaced_name");
    cJSON_ReplaceItemInObject(root, "name", replacement_name);

    // step 3: Operate
    cJSON *got_doubles = cJSON_GetObjectItemCaseSensitive(root, "doubles");
    int got_doubles_count = cJSON_GetArraySize(got_doubles);
    cJSON *got_floats = cJSON_GetObjectItemCaseSensitive(root, "floats");
    int got_floats_count = cJSON_GetArraySize(got_floats);
    (void)got_doubles_count;
    (void)got_floats_count;

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