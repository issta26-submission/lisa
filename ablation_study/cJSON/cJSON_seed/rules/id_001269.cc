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
//<ID> 1269
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char raw_json[] = "{\"rawKey\":123}";
    cJSON *root = NULL;
    cJSON *child = NULL;
    cJSON *raw_item = NULL;
    cJSON *dup = NULL;
    char *printed = NULL;
    cJSON *child_item = NULL;
    cJSON *flag_item = NULL;
    cJSON_bool compare_result = 0;
    cJSON_bool is_true = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child = cJSON_AddObjectToObject(root, "child");
    raw_item = cJSON_CreateRaw(raw_json);
    cJSON_AddItemToObjectCS(child, "rawEntry", raw_item);
    cJSON_AddTrueToObject(child, "flag");

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    dup = cJSON_Duplicate(root, 1);
    compare_result = cJSON_Compare(root, dup, 1);
    cJSON_Minify(printed);
    child_item = cJSON_GetObjectItem(root, "child");
    flag_item = cJSON_GetObjectItem(child_item, "flag");
    is_true = cJSON_IsTrue(flag_item);
    (void)compare_result;
    (void)is_true;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}