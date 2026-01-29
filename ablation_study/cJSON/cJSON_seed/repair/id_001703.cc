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
//<ID> 1703
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *title = cJSON_CreateString("sensor");
    cJSON_AddItemToObject(root, "title", title);
    cJSON *enabled = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "enabled", enabled);
    cJSON_AddFalseToObject(root, "disabled_flag");
    float readings_arr[] = {1.1f, 2.2f, 3.3f};
    cJSON *readings = cJSON_CreateFloatArray(readings_arr, 3);
    cJSON_AddItemToObject(root, "readings", readings);
    cJSON_AddNumberToObject(root, "threshold", 42.5);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "version", 1.0);
    cJSON_AddItemToObject(meta, "owner", cJSON_CreateString("unit-test"));

    // step 3: Operate and Validate
    cJSON *retr_title = cJSON_GetObjectItem(root, "title");
    const char *title_val = cJSON_GetStringValue(retr_title);
    cJSON *retr_thresh = cJSON_GetObjectItem(root, "threshold");
    double thresh_val = cJSON_GetNumberValue(retr_thresh);
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = title_val ? title_val[0] : 'Y';
    buffer[2] = (char)('0' + (int)((int)thresh_val % 10));
    buffer[3] = (char)('0' + (int)cJSON_GetArraySize(readings));
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}