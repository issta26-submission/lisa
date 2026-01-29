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
//<ID> 126
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed = cJSON_Parse("{\"placeholder\":123.5,\"arr\":[9.9]}");
    double numbers[3] = {3.14, 2.71, 1.618};
    cJSON *dbl_array = cJSON_CreateDoubleArray(numbers, 3);
    cJSON *temp_arr = cJSON_AddArrayToObject(root, "double_values");
    cJSON_AddStringToObject(root, "source", "generated");

    // step 2: Configure
    cJSON *ph = cJSON_GetObjectItem(parsed, "placeholder");
    double phv = cJSON_GetNumberValue(ph);
    cJSON_AddNumberToObject(root, "placeholder_val", phv);

    // step 3: Operate and Validate
    cJSON_ReplaceItemViaPointer(root, temp_arr, dbl_array);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}