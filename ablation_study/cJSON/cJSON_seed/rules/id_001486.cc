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
//<ID> 1486
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *true_item = NULL;
    cJSON *null_item = NULL;
    cJSON *nested = NULL;
    cJSON *dup_root = NULL;
    cJSON *dup_nested = NULL;
    cJSON_bool is_true = 0;
    cJSON_bool is_null = 0;
    char json_buf[128] = " {  \"a\" : true ,  \"b\" : null , \"nested\" : { \"x\" : true } } ";

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "a", true_item);
    null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "b", null_item);
    nested = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "nested", nested);
    cJSON_AddItemToObject(nested, "x", cJSON_CreateTrue());

    // step 3: Operate / Validate
    cJSON_Minify(json_buf);
    is_true = cJSON_IsTrue(true_item);
    is_null = cJSON_IsNull(null_item);
    dup_root = cJSON_Duplicate(root, 1);
    dup_nested = cJSON_GetObjectItem(dup_root, "nested");

    // step 4: Cleanup
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    (void)is_true;
    (void)is_null;
    (void)dup_nested;
    // API sequence test completed successfully
    return 66;
}