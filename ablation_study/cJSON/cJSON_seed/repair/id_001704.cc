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
//<ID> 1704
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

    // step 2: Configure
    cJSON_AddStringToObject(meta, "status", "ok");
    cJSON_AddNumberToObject(meta, "version", 1.2);
    float nums[3] = {3.14f, 2.71f, 1.0f};
    cJSON *farr = cJSON_CreateFloatArray(nums, 3);
    cJSON_AddItemToObject(root, "floats", farr);
    cJSON *flag_true = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "flag_true", flag_true);
    cJSON_AddFalseToObject(root, "flag_false");

    // step 3: Operate and Validate
    cJSON *retr_status = cJSON_GetObjectItem(meta, "status");
    const char *status_val = cJSON_GetStringValue(retr_status);
    cJSON *ver_item = cJSON_GetObjectItem(meta, "version");
    double version = cJSON_GetNumberValue(ver_item);
    cJSON *retr_floats = cJSON_GetObjectItem(root, "floats");
    cJSON *second = cJSON_GetArrayItem(retr_floats, 1);
    double second_val = cJSON_GetNumberValue(second);
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = status_val[0];
    buffer[1] = printed[0];
    buffer[2] = (char)('0' + (int)second_val);
    buffer[3] = (char)('0' + (int)version);
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}