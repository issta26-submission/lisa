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
//<ID> 1446
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *child1 = NULL;
    cJSON *child2 = NULL;
    cJSON *null_item = NULL;
    cJSON *replacement = NULL;
    cJSON *raw_item = NULL;
    char *printed = NULL;
    cJSON_bool add1 = 0;
    cJSON_bool add2 = 0;
    cJSON_bool add3 = 0;
    cJSON_bool replaced_ok = 0;
    cJSON_bool is_null = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    child1 = cJSON_CreateString("value_one");
    child2 = cJSON_CreateNumber(42.0);
    null_item = cJSON_CreateNull();
    add1 = cJSON_AddItemToObjectCS(root, "first", child1);
    add2 = cJSON_AddItemToObjectCS(root, "second", child2);
    add3 = cJSON_AddItemToObjectCS(root, "nil", null_item);

    // step 3: Operate / Validate
    replacement = cJSON_CreateString("replaced_value");
    replaced_ok = cJSON_ReplaceItemViaPointer(root, child1, replacement);
    is_null = cJSON_IsNull(null_item);
    raw_item = cJSON_AddRawToObject(root, "rawdata", "{\"raw\":true}");
    printed = cJSON_Print(root);
    (void)add1;
    (void)add2;
    (void)add3;
    (void)replaced_ok;
    (void)is_null;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}