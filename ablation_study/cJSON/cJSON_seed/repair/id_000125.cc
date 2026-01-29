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
//<ID> 125
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"device\":{\"name\":\"sensor\"}}";
    cJSON *root = cJSON_Parse(json_text);

    // step 2: Configure
    double measurements_vals[3] = {1.1, 2.2, 3.3};
    cJSON *dbl_arr = cJSON_CreateDoubleArray(measurements_vals, 3);
    cJSON *placeholder_arr = cJSON_AddArrayToObject(root, "measurements");
    cJSON *name_item = cJSON_AddStringToObject(cJSON_GetObjectItem(root, "device"), "model", "A1");

    // step 3: Operate and Validate
    cJSON_ReplaceItemViaPointer(root, placeholder_arr, dbl_arr);
    cJSON *meas = cJSON_GetObjectItem(root, "measurements");
    int count = cJSON_GetArraySize(meas);
    cJSON *first = cJSON_GetArrayItem(meas, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON_AddNumberToObject(root, "measurements_count", (double)count);
    cJSON_AddNumberToObject(root, "first_measure", first_val);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}