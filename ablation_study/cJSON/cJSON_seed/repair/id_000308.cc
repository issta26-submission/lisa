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
//<ID> 308
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize and Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *ver = cJSON_AddNumberToObject(root, "version", 1.0);
    cJSON *nested = cJSON_CreateObject();
    cJSON_bool added_nested = cJSON_AddItemToObject(root, "nested", nested);
    cJSON *val = cJSON_AddNumberToObject(nested, "value", 3.14159);

    // step 2: Operate (serialize representations)
    char *unformatted_before = cJSON_PrintUnformatted(root);
    char *pretty_before = cJSON_Print(root);

    // step 3: Validate and adjust (read values and add computed result)
    cJSON *nested_item = cJSON_GetObjectItem(root, "nested");
    cJSON *value_item = cJSON_GetObjectItem(nested_item, "value");
    double value_num = cJSON_GetNumberValue(value_item);
    cJSON *version_item = cJSON_GetObjectItem(root, "version");
    double version_num = cJSON_GetNumberValue(version_item);
    double computed = value_num + version_num;
    cJSON *computed_item = cJSON_AddNumberToObject(root, "computed", computed);
    char *unformatted_after = cJSON_PrintUnformatted(root);
    char *pretty_after = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(unformatted_before);
    cJSON_free(pretty_before);
    cJSON_free(unformatted_after);
    cJSON_free(pretty_after);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}