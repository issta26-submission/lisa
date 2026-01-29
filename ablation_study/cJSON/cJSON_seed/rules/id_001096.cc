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
//<ID> 1096
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *person = NULL;
    cJSON *name = NULL;
    cJSON *aliases = NULL;
    cJSON *got_name = NULL;
    char *got_cstr = NULL;
    char *printed = NULL;
    int aliases_count = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    person = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "person", person);
    name = cJSON_CreateString("Alice");
    cJSON_AddItemToObjectCS(person, "name", name);
    aliases = cJSON_CreateArray();
    cJSON_AddItemToObjectCS(person, "aliases", aliases);
    cJSON_AddItemReferenceToArray(aliases, name);

    // step 3: Operate / Validate
    got_name = cJSON_GetObjectItem(person, "name");
    got_cstr = cJSON_GetStringValue(got_name);
    aliases_count = cJSON_GetArraySize(aliases);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}