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
//<ID> 1422
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
    cJSON *replacement = NULL;
    char print_buffer[256];
    char *allocated = NULL;
    cJSON_bool added_ok1 = 0;
    cJSON_bool added_ok2 = 0;
    cJSON_bool replaced_ok = 0;
    cJSON_bool printed_ok = 0;
    char json1[] = "{\"key1\":123, \"replace\":\"old\"}";
    char json2[] = "{\"nested\":{\"a\":1}}";

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    parsed1 = cJSON_Parse(json1);
    added_ok1 = cJSON_AddItemToObject(root, "orig", parsed1);
    parsed2 = cJSON_Parse(json2);
    dup = cJSON_Duplicate(parsed2, 1);
    added_ok2 = cJSON_AddItemToObject(root, "dup_nested", dup);
    replacement = cJSON_CreateObject();
    cJSON_AddItemToObject(replacement, "newnum", cJSON_CreateNumber(999.0));
    replaced_ok = cJSON_ReplaceItemInObjectCaseSensitive(root, "orig", replacement);

    // step 3: Operate / Validate
    memset(print_buffer, 0, sizeof(print_buffer));
    printed_ok = cJSON_PrintPreallocated(root, print_buffer, (int)sizeof(print_buffer), 1);
    allocated = cJSON_Print(root);
    cJSON_free(allocated);
    (void)added_ok1;
    (void)added_ok2;
    (void)replaced_ok;
    (void)printed_ok;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}