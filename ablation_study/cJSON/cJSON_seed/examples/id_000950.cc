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
//<ID> 950
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metrics = cJSON_CreateObject();
    cJSON *temp = cJSON_CreateNumber(23.5);
    cJSON *hum = cJSON_CreateNumber(48.0);
    cJSON *raw = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON *name = cJSON_CreateString("device-123");

    // step 2: Configure
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddItemToObject(root, "metrics", metrics);
    cJSON_AddItemToObject(metrics, "temp", temp);
    cJSON_AddItemToObject(metrics, "humidity", hum);
    cJSON_AddItemToObject(metrics, "raw_payload", raw);
    cJSON_bool raw_flag = cJSON_IsRaw(raw);
    cJSON_AddItemToObject(metrics, "raw_is_raw", cJSON_CreateBool(raw_flag));
    const char *ver = cJSON_Version();
    cJSON_AddItemToObject(root, "cjson_version", cJSON_CreateString(ver));
    cJSON *metrics_copy = cJSON_Duplicate(metrics, 1);
    cJSON_AddItemToObject(root, "metrics_copy", metrics_copy);

    // step 3: Operate & Validate
    cJSON *got_metrics = cJSON_GetObjectItem(root, "metrics");
    cJSON *got_temp = cJSON_GetObjectItem(got_metrics, "temp");
    double extracted_temp = cJSON_GetNumberValue(got_temp);
    cJSON_AddItemToObject(root, "extracted_temp", cJSON_CreateNumber(extracted_temp));
    cJSON *got_raw_flag = cJSON_GetObjectItem(got_metrics, "raw_is_raw");
    cJSON_AddItemToObject(root, "raw_flag_reflection", cJSON_Duplicate(got_raw_flag, 0));

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}