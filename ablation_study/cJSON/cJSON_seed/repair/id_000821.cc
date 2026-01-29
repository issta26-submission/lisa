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
//<ID> 821
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddNumberToObject(meta, "version", 1.0);
    cJSON *data = cJSON_AddObjectToObject(root, "data");
    cJSON *items = cJSON_AddArrayToObject(data, "items");
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(items, num_item);

    // step 2: Configure
    const char *json_text = "{\"name\":\"example\",\"nested\":{\"num\":7}}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *parsed_name = cJSON_GetObjectItem(parsed, "name");
    cJSON *nested = cJSON_GetObjectItem(parsed, "nested");
    cJSON *nested_num = cJSON_GetObjectItem(nested, "num");

    // step 3: Operate and Validate
    cJSON_bool name_is_string = cJSON_IsString(parsed_name);
    cJSON_AddNumberToObject(data, "injected", 99.0);
    cJSON *detached_name = cJSON_DetachItemFromObject(parsed, "name");
    cJSON *detached_num = cJSON_DetachItemViaPointer(items, num_item);

    // step 4: Cleanup
    cJSON_Delete(detached_name);
    cJSON_Delete(detached_num);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}