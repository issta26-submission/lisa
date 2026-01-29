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
//<ID> 20
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"num\":1,\"text\":\"old\",\"inner\":{\"a\":2}}";
    size_t len = sizeof("{\"num\":1,\"text\":\"old\",\"inner\":{\"a\":2}}") - 1;
    cJSON *root = cJSON_ParseWithLength(json, len);
    cJSON *item_num = cJSON_GetObjectItem(root, "num");
    cJSON *item_text = cJSON_GetObjectItem(root, "text");
    cJSON *inner = cJSON_GetObjectItem(root, "inner");

    // step 2: Configure
    cJSON_SetNumberHelper(item_num, 123.456);
    cJSON_SetValuestring(item_text, "newtext");
    cJSON *inner_ref = cJSON_CreateObjectReference(inner);
    cJSON_AddItemToObject(root, "innerRef", inner_ref);
    cJSON_AddRawToObject(root, "addedRaw", "{\"r\":true}");

    // step 3: Operate and Validate
    double got_number = cJSON_GetNumberValue(item_num);
    char *got_string = cJSON_GetStringValue(item_text);
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool same = cJSON_Compare(root, dup, 1);
    (void)got_number;
    (void)got_string;
    (void)same;

    // step 4: Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}