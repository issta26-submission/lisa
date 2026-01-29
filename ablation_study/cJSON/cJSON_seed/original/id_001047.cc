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
//<ID> 1047
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"person\":{\"name\":\"John\",\"flags\":[true,false],\"age\":30}}";
    cJSON *root = cJSON_Parse(initial_json);

    // step 2: Configure
    cJSON *person = cJSON_GetObjectItem(root, "person");
    cJSON *name = cJSON_GetObjectItem(person, "name");
    cJSON_SetValuestring(name, "Johnny");
    cJSON *out = cJSON_CreateObject();
    cJSON *active_flag = cJSON_CreateBool(1);
    cJSON_AddItemToObjectCS(out, "active", active_flag);
    cJSON *flags = cJSON_GetObjectItem(person, "flags");
    cJSON_AddNumberToObject(out, "flags_is_array", (double)cJSON_IsArray(flags));

    // step 3: Operate
    cJSON *dup_person = cJSON_Duplicate(person, 1);
    cJSON *dup_name = cJSON_GetObjectItem(dup_person, "name");
    cJSON_SetValuestring(dup_name, "DupJohnny");
    cJSON_AddItemToObjectCS(out, "person_copy", dup_person);
    char *pretty = cJSON_Print(out);
    char *compact = cJSON_PrintUnformatted(out);
    char *prealloc = (char *)cJSON_malloc(512);
    memset(prealloc, 0, 512);
    cJSON_PrintPreallocated(out, prealloc, 512, 1);

    // step 4: Validate & Cleanup
    cJSON_free(pretty);
    cJSON_free(compact);
    cJSON_free(prealloc);
    cJSON_Delete(root);
    cJSON_Delete(out);

    // API sequence test completed successfully
    return 66;
}