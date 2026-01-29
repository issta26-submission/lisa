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
//<ID> 74
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *person = cJSON_CreateObject();
    cJSON *name_item = NULL;
    cJSON *age_item = NULL;
    char *name_value = NULL;
    double age_value = 0.0;
    char *printed = NULL;

    // step 2: Setup / Configure
    cJSON_AddStringToObject(person, "name", "Alice");
    cJSON_AddNumberToObject(person, "age", 30.0);
    cJSON_AddItemToObject(root, "person", person);
    cJSON_AddNumberToObject(root, "members", 1.0);

    // step 3: Operate / Validate
    name_item = cJSON_GetObjectItem(person, "name");
    age_item = cJSON_GetObjectItem(person, "age");
    name_value = cJSON_GetStringValue(name_item);
    age_value = cJSON_GetNumberValue(age_item);
    cJSON_AddNumberToObject(root, "age_offset", age_value + 5.0);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}