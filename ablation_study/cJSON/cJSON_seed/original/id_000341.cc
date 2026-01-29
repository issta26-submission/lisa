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
//<ID> 341
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"person\":{\"name\":\"Alice\",\"age\":30},\"meta\":{\"id\":42}}";
    cJSON *parsed = cJSON_Parse(json);
    cJSON *person = cJSON_GetObjectItemCaseSensitive(parsed, "person");
    cJSON *name = cJSON_GetObjectItemCaseSensitive(person, "name");

    // step 2: Configure
    cJSON *new_root = cJSON_CreateObject();
    cJSON *refs = cJSON_CreateArray();
    cJSON *name_dup = cJSON_Duplicate(name, 1);
    cJSON_AddItemReferenceToArray(refs, name_dup);
    cJSON_AddItemToObject(new_root, "refs", refs);

    // step 3: Operate and Validate
    char *printed_new = cJSON_Print(new_root);
    char *printed_parsed = cJSON_Print(parsed);

    // step 4: Cleanup
    cJSON_free(printed_new);
    cJSON_free(printed_parsed);
    cJSON_Delete(new_root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}