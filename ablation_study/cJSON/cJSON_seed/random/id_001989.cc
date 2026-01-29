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
//<ID> 1989
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[32];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *str_item = NULL;
    cJSON *num_item = NULL;
    cJSON *num_dup = NULL;
    cJSON *bool_item = NULL;
    cJSON *replacement = NULL;
    cJSON_bool added_str = 0;
    cJSON_bool added_num = 0;
    cJSON_bool added_num_dup = 0;
    cJSON_bool has_greet = 0;
    cJSON_bool replace_ok = 0;
    int validation = 0;

    // step 2: Setup (Initialize root and items)
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("hello");
    num_item = cJSON_CreateNumber(2025.0);
    added_str = cJSON_AddItemToObject(root, "greet", str_item);
    added_num = cJSON_AddItemToObject(root, "year", num_item);
    bool_item = cJSON_AddBoolToObject(root, "active", 1);

    // step 3: Operate (duplicate a number item and add it; check presence; prepare replacement)
    num_dup = cJSON_Duplicate(num_item, 0);
    added_num_dup = cJSON_AddItemToObject(root, "year_copy", num_dup);
    has_greet = cJSON_HasObjectItem(root, "greet");
    replacement = cJSON_CreateString("world");
    replace_ok = cJSON_ReplaceItemViaPointer(root, str_item, replacement);

    // step 4: Validate (aggregate simple checks into buffer)
    validation ^= (int)(root != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(num_dup != NULL);
    validation ^= (int)(bool_item != NULL);
    validation ^= (int)(added_str != 0);
    validation ^= (int)(added_num != 0);
    validation ^= (int)(added_num_dup != 0);
    validation ^= (int)(has_greet != 0);
    validation ^= (int)(replace_ok != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}