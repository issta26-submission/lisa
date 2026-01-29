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
//<ID> 847
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("Alice");
    cJSON *age_item = cJSON_CreateNumber(30.0);

    // step 2: Configure
    cJSON_AddItemToObject(root, "name_ref", name_item);
    cJSON_AddItemToObject(root, "age", age_item);
    cJSON *city_item = cJSON_AddStringToObject(root, "city", "Wonderland");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    char *city_str = cJSON_GetStringValue(city_item);
    cJSON *city_copy = cJSON_CreateString(city_str);
    cJSON_AddItemToObject(root, "city_copy", city_copy);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}