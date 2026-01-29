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
//<ID> 1706
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *labelItem = cJSON_CreateString("sensor");
    cJSON_AddItemToObject(root, "label", labelItem);
    cJSON *enabledItem = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "enabled", enabledItem);
    cJSON_AddFalseToObject(root, "error");

    // step 2: Configure
    cJSON_AddNumberToObject(root, "threshold", 12.5);
    float readings_vals[3];
    readings_vals[0] = 1.0f;
    readings_vals[1] = 2.5f;
    readings_vals[2] = 3.75f;
    cJSON *readings = cJSON_CreateFloatArray(readings_vals, 3);
    cJSON_AddItemToObject(root, "readings", readings);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "version", 1.0);

    // step 3: Operate and Validate
    char *label_val = cJSON_GetStringValue(labelItem);
    cJSON *thr_item = cJSON_GetObjectItem(root, "threshold");
    double thr_val = cJSON_GetNumberValue(thr_item);
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[80];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = label_val ? label_val[0] : 'Y';
    buffer[2] = (char)('0' + (int)thr_val);
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}