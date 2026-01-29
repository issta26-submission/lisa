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
    // step 1: Declarations / Initialize
    const char json[] = "{\"name\":\"example\",\"arr\":[\"one\",\"two\"]}";
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *dup = NULL;
    cJSON *added_ver = NULL;
    cJSON *added = NULL;
    const char *ver = NULL;
    cJSON_bool eq = 0;
    int size = 0;

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    arr = cJSON_GetObjectItem(root, "arr");
    size = cJSON_GetArraySize(arr);

    // step 3: Operate / Validate
    ver = cJSON_Version();
    added_ver = cJSON_AddStringToObject(root, "version", ver);
    added = cJSON_AddStringToObject(root, "added", "new_value");
    dup = cJSON_Duplicate(root, 1);
    eq = cJSON_Compare(root, dup, (cJSON_bool)(size == 2));
    cJSON_AddBoolToObject(root, "equal_to_dup_with_case", eq);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(dup);
    // API sequence test completed successfully
    return 66;
}