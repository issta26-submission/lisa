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
//<ID> 1427
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *parsed = NULL;
    cJSON *dup = NULL;
    cJSON *nested = NULL;
    cJSON *new_person = NULL;
    char *printed = NULL;
    char buffer[256];
    cJSON_bool replaced = 0;
    cJSON_bool printed_ok = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    parsed = cJSON_Parse("{\"name\":\"Alice\",\"age\":30}");
    cJSON_AddItemToObject(root, "person", parsed);
    dup = cJSON_Duplicate(parsed, 1);
    cJSON_AddItemToObject(root, "person_copy", dup);
    nested = cJSON_CreateObject();
    cJSON_AddItemToObject(nested, "friend", cJSON_Parse("\"Bob\""));
    cJSON_AddItemToObject(root, "meta", nested);

    // step 3: Operate / Validate
    new_person = cJSON_Parse("{\"name\":\"Carol\",\"age\":25}");
    replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "person", new_person);
    memset(buffer, 0, sizeof(buffer));
    printed_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    printed = cJSON_Print(root);
    cJSON_Minify(printed);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}