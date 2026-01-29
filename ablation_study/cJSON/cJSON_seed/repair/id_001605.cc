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
//<ID> 1605
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(child, "flag", true_item);
    cJSON *bool_item = cJSON_CreateBool(0);
    cJSON_AddItemToObject(root, "enabled", bool_item);

    // step 2: Configure
    cJSON *num = cJSON_CreateNumber(123.456);
    cJSON_AddItemToObject(child, "value", num);
    cJSON *str = cJSON_CreateString("sample");
    cJSON_AddItemToObject(root, "name", str);

    // step 3: Operate and Validate
    const char *ver = cJSON_Version();
    char *out = cJSON_PrintUnformatted(root);
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = out[0];
    buffer[1] = ver[0];
    cJSON *got_num = cJSON_GetObjectItem(child, "value");
    double val = cJSON_GetNumberValue(got_num);
    cJSON *num2 = cJSON_CreateNumber(val * 2.0);
    cJSON_AddItemToObject(root, "double_value", num2);
    char *out2 = cJSON_PrintUnformatted(root);
    buffer[2] = out2[0];
    cJSON_free(out);
    cJSON_free(out2);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}