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
//<ID> 1354
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");

    // step 2: Configure
    cJSON_AddNumberToObject(settings, "threshold", 3.14);
    cJSON_AddNumberToObject(settings, "max", 100.0);
    cJSON *new_threshold = cJSON_CreateNumber(2.718);
    cJSON_bool replaced = cJSON_ReplaceItemInObject(settings, "threshold", new_threshold);

    // step 3: Operate and Validate
    double threshold_value = cJSON_GetNumberValue(cJSON_GetObjectItem(settings, "threshold"));
    double max_value = cJSON_GetNumberValue(cJSON_GetObjectItem(settings, "max"));
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(80);
    memset(scratch, 0, 80);
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = (char)('0' + ((int)threshold_value % 10));
    ((char *)scratch)[2] = (char)('0' + ((int)max_value % 10));
    ((char *)scratch)[3] = (char)('0' + (replaced ? 1 : 0));
    ((char *)scratch)[4] = (char)('0' + (((int)(threshold_value + max_value)) % 10));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}