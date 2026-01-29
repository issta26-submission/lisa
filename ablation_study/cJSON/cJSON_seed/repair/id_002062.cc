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
//<ID> 2062
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
    cJSON *num = cJSON_CreateNumber(3.1415);
    cJSON *label = cJSON_CreateString("sensorA");
    cJSON_AddItemToObject(metrics, "value", num);
    cJSON_AddItemToObject(metrics, "label", label);
    cJSON_AddItemToObject(root, "metrics", metrics);

    // step 2: Configure
    const char raw_json[] = "{\"rawnum\":42}";
    cJSON_AddRawToObject(root, "raw", raw_json);
    cJSON_AddItemToObject(root, "status", cJSON_CreateString("ok"));

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON *got_metrics = cJSON_GetObjectItem(root, "metrics");
    cJSON *got_value = cJSON_GetObjectItem(got_metrics, "value");
    double val = cJSON_GetNumberValue(got_value);
    cJSON *got_label = cJSON_GetObjectItem(got_metrics, "label");
    const char *label_str = cJSON_GetStringValue(got_label);
    buffer[0] = printed[0];
    buffer[1] = label_str[0];
    buffer[2] = (char)((int)val + '0');
    buffer[3] = '\0';
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}