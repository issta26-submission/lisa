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
//<ID> 23
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"num\":1,\"str\":\"old\"}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "childKey", "childVal");

    // step 2: Configure
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "childRef", child_ref);

    // step 3: Operate and Validate
    cJSON *num_item = cJSON_GetObjectItem(root, "num");
    double set_res = cJSON_SetNumberHelper(num_item, 2.71828);
    double read_num = cJSON_GetNumberValue(num_item);
    double combined = set_res + read_num;
    cJSON *str_item = cJSON_GetObjectItem(root, "str");
    char *set_str = cJSON_SetValuestring(str_item, "updated");
    char *read_str = cJSON_GetStringValue(str_item);
    cJSON_AddRawToObject(root, "injectedRaw", "{\"x\":123}");
    cJSON_AddNumberToObject(root, "combined", combined);
    cJSON_AddStringToObject(root, "readStr", read_str);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(child);

    // API sequence test completed successfully
    return 66;
}