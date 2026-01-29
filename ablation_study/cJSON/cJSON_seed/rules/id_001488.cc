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
//<ID> 1488
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
    cJSON *true_item = NULL;
    cJSON *null_item = NULL;
    cJSON *parsed_nested = NULL;
    cJSON_bool parsed_is_object = 0;
    cJSON_bool true_is_true = 0;
    char json[] = " { \"nested\" : { \"value\" : 42, \"maybe\" : null }, \"flag\" : true } ";

    // step 2: Setup / Configure
    cJSON_Minify(json);
    parsed = cJSON_Parse(json);
    root = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();
    null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "bool_item", true_item);
    cJSON_AddItemToObject(root, "null_item", null_item);
    cJSON_AddItemToObject(root, "imported", parsed);

    // step 3: Operate / Validate
    parsed_nested = cJSON_GetObjectItem(parsed, "nested");
    parsed_is_object = cJSON_IsObject(parsed_nested);
    true_is_true = cJSON_IsTrue(true_item);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    (void)parsed_is_object;
    (void)true_is_true;
    (void)parsed_nested;
    return 66;
}