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
//<ID> 1323
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_AddStringToObject(root, "device", "sensor-01");
    double readings_vals[3] = {1.5, 2.5, 3.5};
    cJSON *readings_array = cJSON_CreateDoubleArray(readings_vals, 3);

    // step 2: Configure
    cJSON_AddItemToObject(root, "readings", readings_array);

    // step 3: Operate and Validate
    cJSON *found_array = cJSON_GetObjectItem(root, "readings");
    cJSON_bool array_check = cJSON_IsArray(found_array);
    cJSON *found_name = cJSON_GetObjectItem(root, "device");
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = (char)('0' + (array_check ? 1 : 0));
    ((char *)scratch)[2] = (char)('0' + (found_name && found_name->valuestring ? found_name->valuestring[0] : 0));
    cJSON_free(out);
    cJSON_free(scratch);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}