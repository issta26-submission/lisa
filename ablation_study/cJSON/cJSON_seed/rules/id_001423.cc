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
//<ID> 1423
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *parsed1 = NULL;
    cJSON *parsed2 = NULL;
    cJSON *dup = NULL;
    cJSON *copy = NULL;
    char *printed = NULL;
    const char json1[] = "{\"key\":123, \"sub\":{\"a\":true}}";
    const char json2[] = "{\"key\":999, \"extra\":\"value\"}";
    char scratch[64];
    memset(scratch, 0, sizeof(scratch));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    parsed1 = cJSON_Parse(json1);
    cJSON_AddItemToObject(root, "original", parsed1);
    parsed2 = cJSON_Parse(json2);
    dup = cJSON_Duplicate(parsed2, 1);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "original", dup);
    copy = cJSON_Duplicate(dup, 1);
    cJSON_AddItemToObject(root, "copy_of_replacement", copy);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(parsed2);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}