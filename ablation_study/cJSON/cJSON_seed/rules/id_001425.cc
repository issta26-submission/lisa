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
//<ID> 1425
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
    cJSON *parsed2 = NULL;
    char json1[] = "{\"alpha\":123, \"beta\":\"text\"}";
    char json2[] = "{\"alpha\":999, \"beta\":\"replaced\", \"gamma\":true}";

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    parsed = cJSON_Parse(json1);
    cJSON_AddItemToObject(root, "original", parsed);
    dup = cJSON_Duplicate(parsed, 1);
    cJSON_AddItemToObject(root, "original_copy", dup);
    parsed2 = cJSON_Parse(json2);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "original", parsed2);

    // step 3: Operate / Validate
    cJSON *verify_dup = cJSON_Duplicate(root, 0);
    (void)verify_dup;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}