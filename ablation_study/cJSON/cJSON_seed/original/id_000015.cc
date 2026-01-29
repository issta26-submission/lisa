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
//<ID> 15
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"name\":\"Alice\",\"age\":30,\"active\":true}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    cJSON *root_dup = cJSON_Duplicate(root, 1);

    // step 2: Configure
    cJSON *container = cJSON_CreateObject();
    cJSON *t = cJSON_AddTrueToObject(container, "isPresent");
    cJSON *f = cJSON_AddFalseToObject(container, "isDeleted");
    cJSON_AddItemToObject(container, "original", root_dup);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(container);
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(root_dup, "name");
    const char *name = cJSON_GetStringValue(name_item);
    cJSON *age_item = cJSON_GetObjectItem(root_dup, "age");
    double age = cJSON_GetNumberValue(age_item);
    cJSON_bool same = cJSON_Compare(root, root_dup, 1);
    cJSON *detached_original = cJSON_DetachItemFromObject(container, "original");

    // step 4: Cleanup
    cJSON_Delete(detached_original);
    cJSON_Delete(root);
    cJSON_free(out);
    cJSON_Delete(container);

    // API sequence test completed successfully
    return 66;
}