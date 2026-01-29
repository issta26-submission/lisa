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
//<ID> 835
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *name_item = NULL;
    cJSON *num_item = NULL;
    cJSON *dup_root = NULL;
    cJSON *orig_name = NULL;
    cJSON *dup_name = NULL;
    cJSON *orig_num = NULL;
    cJSON *dup_num = NULL;
    char *printed = NULL;
    cJSON_bool name_equal = 0;
    cJSON_bool num_equal = 0;
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("example_name");
    cJSON_AddItemToObject(root, "name", name_item);
    num_item = cJSON_CreateNumber(123.456);
    cJSON_AddItemToObject(root, "value", num_item);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    dup_root = cJSON_Duplicate(root, 1);
    orig_name = cJSON_GetObjectItemCaseSensitive(root, "name");
    dup_name = cJSON_GetObjectItemCaseSensitive(dup_root, "name");
    orig_num = cJSON_GetObjectItemCaseSensitive(root, "value");
    dup_num = cJSON_GetObjectItemCaseSensitive(dup_root, "value");
    name_equal = cJSON_Compare(orig_name, dup_name, 1);
    num_equal = cJSON_Compare(orig_num, dup_num, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    memset(buffer, 0, sizeof(buffer));

    // API sequence test completed successfully
    (void)name_equal;
    (void)num_equal;
    (void)printed;
    return 66;
}