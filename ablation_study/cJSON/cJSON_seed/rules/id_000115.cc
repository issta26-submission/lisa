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
//<ID> 115
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("original");
    cJSON *age = cJSON_CreateNumber(30.0);
    cJSON *city = cJSON_CreateString("Townsville");
    cJSON *new_name = cJSON_CreateString("updated");
    cJSON *old_name = NULL;
    char *printed = NULL;
    double age_value = 0.0;
    cJSON_bool replaced = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddItemToObject(root, "age", age);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "city", city);

    // step 3: Operate / Validate
    old_name = cJSON_GetObjectItem(root, "name");
    age_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "age"));
    cJSON_AddNumberToObject(root, "age_copy", age_value);
    replaced = cJSON_ReplaceItemViaPointer(root, old_name, new_name);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}