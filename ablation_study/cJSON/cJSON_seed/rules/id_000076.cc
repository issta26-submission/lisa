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
//<ID> 76
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    const char *person_name = "alice";
    double base_age = 29.0;
    cJSON *name_item = NULL;
    cJSON *age_item = NULL;
    const char *name_val = NULL;
    double age_val = 0.0;
    char *printed = NULL;

    // step 2: Setup / Configure
    cJSON_AddStringToObject(root, "name", person_name);
    cJSON_AddNumberToObject(root, "age", base_age);

    // step 3: Operate / Validate
    name_item = cJSON_GetObjectItem(root, "name");
    age_item = cJSON_GetObjectItem(root, "age");
    name_val = cJSON_GetStringValue(name_item);
    age_val = cJSON_GetNumberValue(age_item);
    cJSON_AddNumberToObject(root, "next_age", age_val + 1.0);
    cJSON_AddNumberToObject(root, "double_age", age_val * 2.0);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}