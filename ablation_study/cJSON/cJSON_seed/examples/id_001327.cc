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
//<ID> 1327
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *person = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("Alice");
    cJSON *age = cJSON_CreateNumber(30.0);
    cJSON *active = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToObjectCS(person, "name", name);
    cJSON_AddItemToObject(person, "age", age);
    cJSON_AddItemToObject(person, "active", active);
    cJSON_AddItemToObjectCS(root, "person", person);

    // step 3: Operate & Validate
    cJSON *got_name = cJSON_GetObjectItemCaseSensitive(person, "name");
    char *name_str = cJSON_GetStringValue(got_name);
    cJSON *name_copy = cJSON_CreateString(name_str);
    cJSON_AddItemToObject(root, "name_copy", name_copy);
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    size_t printed_len = printed ? strlen(printed) : 0;
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}