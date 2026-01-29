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
//<ID> 408
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *got_item = (cJSON *)0;
    cJSON_bool got_true = 0;
    char raw_json[] = " { \"a\" : [ true , false ] , \"b\" : 1 } ";

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    true_item = cJSON_CreateTrue();

    // step 3: Configure
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 4: Operate
    cJSON_Minify(raw_json);
    got_item = cJSON_GetArrayItem(arr, 0);
    got_true = cJSON_IsTrue(got_item);

    // step 5: Validate
    (void)got_true;
    (void)raw_json;
    (void)got_item;

    // step 6: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "enabled");
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}