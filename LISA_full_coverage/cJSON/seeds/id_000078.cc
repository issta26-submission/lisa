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
//<ID> 78
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    float values[3] = {1.5f, -2.0f, 3.25f};
    cJSON *array = cJSON_CreateFloatArray(values, 3);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", array);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "info", "float array");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    char *pretty = cJSON_Print(root);
    char *buffered = cJSON_PrintBuffered(root, 128, 1);
    cJSON_bool meta_obj = cJSON_IsObject(meta);

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(array);
    double first = cJSON_GetNumberValue(cJSON_GetArrayItem(array, 0));
    int summary = size + (int)meta_obj + (int)first + (int)pretty[0] + (int)buffered[0];
    (void)summary;

    // step 4: Cleanup
    cJSON_free(pretty);
    cJSON_free(buffered);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}