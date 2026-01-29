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
//<ID> 1708
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *title = cJSON_CreateString("sensor-data");
    cJSON_AddItemToObject(root, "title", title);

    // step 2: Configure
    float readings_arr[3];
    readings_arr[0] = 1.2f;
    readings_arr[1] = 3.4f;
    readings_arr[2] = 5.6f;
    cJSON *readings = cJSON_CreateFloatArray(readings_arr, 3);
    cJSON_AddItemToObject(root, "readings", readings);
    cJSON_AddNumberToObject(root, "count", 3.0);
    cJSON_AddFalseToObject(root, "active");
    cJSON *ok = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "ok", ok);

    // step 3: Operate and Validate
    cJSON *retr_title = cJSON_GetObjectItem(root, "title");
    const char *title_str = cJSON_GetStringValue(retr_title);
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = title_str ? title_str[0] : 'Y';
    buffer[2] = (char)('0' + (int)cJSON_GetArraySize(readings));
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}