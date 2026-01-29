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
//<ID> 2281
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *active = cJSON_CreateTrue();
    cJSON_AddItemToObject(meta, "active", active);
    cJSON_AddNullToObject(root, "optional");

    // step 2: Configure
    cJSON *sensor = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "sensor", sensor);
    cJSON_AddNumberToObject(sensor, "threshold", 75.5);
    cJSON_AddStringToObject(sensor, "id", "sensor-42");
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag_false);

    // step 3: Operate
    cJSON *maybe = cJSON_GetObjectItem(root, "optional");
    cJSON *flag = cJSON_GetObjectItem(root, "flag");
    cJSON_bool is_optional_null = cJSON_IsNull(maybe);
    cJSON_bool flag_is_bool = cJSON_IsBool(flag);
    cJSON_bool flag_is_false = cJSON_IsFalse(flag);
    cJSON_AddNumberToObject(root, "optional_is_null", (double)is_optional_null);
    cJSON_AddNumberToObject(root, "flag_is_bool", (double)flag_is_bool);
    cJSON_AddNumberToObject(root, "flag_is_false", (double)flag_is_false);

    // step 4: Validate and Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}