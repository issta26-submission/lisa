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
//<ID> 1394
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float f_vals[3] = {1.5f, 2.5f, 3.5f};
    double d_vals[4] = {10.0, 20.0, 30.0, 40.0};
    cJSON *f_array = cJSON_CreateFloatArray(f_vals, 3);
    cJSON *d_array = cJSON_CreateDoubleArray(d_vals, 4);
    cJSON_AddItemToObject(root, "float_array", f_array);
    cJSON_AddItemToObject(root, "double_array", d_array);

    // step 2: Configure
    double replacement_vals[2] = {99.9, 100.1};
    cJSON *replacement_array = cJSON_CreateDoubleArray(replacement_vals, 2);
    cJSON_ReplaceItemInObject(root, "float_array", replacement_array);

    // step 3: Operate
    cJSON *retrieved = cJSON_GetObjectItemCaseSensitive(root, "float_array");
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buffer = (char *)cJSON_malloc(printed_len + 1);
    memset(buffer, 0, printed_len + 1);
    memcpy(buffer, printed, printed_len + 1);
    cJSON_Minify(buffer);
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Validate & Cleanup
    int replaced_count = cJSON_GetArraySize(retrieved);
    cJSON_AddNumberToObject(root, "replaced_count", (double)replaced_count);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}