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
//<ID> 615
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON *n0 = cJSON_CreateNumber(10.0);
    cJSON *n1 = cJSON_CreateNumber(20.0);
    cJSON *n2 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(numbers, n0);
    cJSON_AddItemToArray(numbers, n1);
    cJSON_AddItemToArray(numbers, n2);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "scale", cJSON_CreateNumber(2.5));
    cJSON_AddItemToObject(meta, "offset", cJSON_CreateNumber(1.0));

    // step 3: Operate and Validate
    int original_count = cJSON_GetArraySize(numbers);
    cJSON *detached = cJSON_DetachItemFromArray(numbers, 1);
    double detached_value = cJSON_GetNumberValue(detached);
    cJSON_Delete(detached);
    double scale = cJSON_GetNumberValue(cJSON_GetObjectItem(meta, "scale"));
    double offset = cJSON_GetNumberValue(cJSON_GetObjectItem(meta, "offset"));
    double transformed = detached_value * scale + offset;
    cJSON *result_num = cJSON_CreateNumber(transformed);
    cJSON_AddItemToObject(root, "transformed_removed", result_num);
    int new_count = cJSON_GetArraySize(numbers);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}