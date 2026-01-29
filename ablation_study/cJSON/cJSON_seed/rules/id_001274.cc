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
//<ID> 1274
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *num_item = cJSON_CreateNumber(3.14159);
    cJSON *added_num = NULL;
    cJSON *null_item = cJSON_CreateNull();
    cJSON *child_lookup = NULL;
    cJSON *greet_item = NULL;
    cJSON *e_item = NULL;
    const char *greet_str = NULL;
    double e_val = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "pi_manual", num_item);
    added_num = cJSON_AddNumberToObject(root, "e", 2.71828);
    cJSON_AddStringToObject(child, "greeting", "hello");
    cJSON_AddItemToObject(root, "missing", null_item);

    // step 3: Operate / Validate
    child_lookup = cJSON_GetObjectItem(root, "child");
    greet_item = cJSON_GetObjectItem(child_lookup, "greeting");
    greet_str = cJSON_GetStringValue(greet_item);
    e_item = cJSON_GetObjectItem(root, "e");
    e_val = cJSON_GetNumberValue(e_item);
    (void)greet_str;
    (void)e_val;
    (void)added_num;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}