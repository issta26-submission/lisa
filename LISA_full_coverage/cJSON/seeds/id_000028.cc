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
//<ID> 28
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"num\": 1, \"s\": \"old\", \"obj\": {\"a\": 1}}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);

    // step 2: Configure
    cJSON *num_item = cJSON_GetObjectItem(root, "num");
    double updated_num = cJSON_SetNumberHelper(num_item, 3.1415);
    cJSON *str_item = cJSON_GetObjectItem(root, "s");
    char *new_str_ptr = cJSON_SetValuestring(str_item, "new");
    cJSON *str_ref = cJSON_CreateObjectReference(str_item);
    cJSON_AddItemToObject(root, "sRef", str_ref);
    cJSON_AddRawToObject(root, "extra", "{\"x\":123, \"y\":[1,2,3]}");

    // step 3: Operate and Validate
    double read_num = cJSON_GetNumberValue(num_item);
    const char *read_str = cJSON_GetStringValue(str_item);
    cJSON *detached_obj = cJSON_DetachItemFromObject(root, "obj");

    // step 4: Cleanup
    cJSON_Delete(detached_obj);
    (void)updated_num;
    (void)new_str_ptr;
    (void)read_num;
    (void)read_str;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}