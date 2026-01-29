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
//<ID> 1987
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[64];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *str_item = NULL;
    cJSON *obj_item = NULL;
    cJSON *dup_item = NULL;
    cJSON *flag_item = NULL;
    cJSON_bool added_str = 0;
    cJSON_bool replaced = 0;
    cJSON_bool has_key = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("initial_value");
    obj_item = cJSON_CreateObject();
    cJSON_AddItemToObject(obj_item, "inner", cJSON_CreateNumber(7.0));
    added_str = cJSON_AddItemToObject(root, "key", str_item);
    flag_item = cJSON_AddBoolToObject(root, "flag", 1);

    // step 3: Core operations
    dup_item = cJSON_Duplicate(obj_item, 1);
    replaced = cJSON_ReplaceItemViaPointer(root, str_item, dup_item);
    has_key = cJSON_HasObjectItem(root, "key");

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(obj_item != NULL);
    validation ^= (int)(dup_item != NULL);
    validation ^= (int)(added_str != 0);
    validation ^= (int)(flag_item != NULL);
    validation ^= (int)(replaced != 0);
    validation ^= (int)(has_key != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(obj_item);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}