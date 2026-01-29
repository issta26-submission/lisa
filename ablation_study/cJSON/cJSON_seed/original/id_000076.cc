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
//<ID> 76
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float values[3] = {1.5f, 2.5f, 3.5f};
    cJSON *arr = cJSON_CreateFloatArray(values, 3);
    cJSON_AddItemToObject(root, "floats", arr);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "status", "ok");
    cJSON_AddNumberToObject(meta, "count", 3.0);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    char *printed = cJSON_Print(root);
    char *buffered = cJSON_PrintBuffered(root, 64, 1);
    cJSON *meta_item = cJSON_GetObjectItem(root, "meta");
    cJSON_bool meta_is_object = cJSON_IsObject(meta_item);

    // step 3: Operate and Validate
    int size = cJSON_GetArraySize(arr);
    cJSON *e0 = cJSON_GetArrayItem(arr, 0);
    cJSON *e1 = cJSON_GetArrayItem(arr, 1);
    cJSON *e2 = cJSON_GetArrayItem(arr, 2);
    double v0 = cJSON_GetNumberValue(e0);
    double v1 = cJSON_GetNumberValue(e1);
    double v2 = cJSON_GetNumberValue(e2);
    double sum = v0 + v1 + v2;
    int summary = (int)sum + size + (int)meta_is_object + (int)(printed ? printed[0] : 0) + (int)(buffered ? buffered[0] : 0);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffered);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}