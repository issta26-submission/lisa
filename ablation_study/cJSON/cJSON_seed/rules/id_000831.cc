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
//<ID> 831
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num = NULL;
    cJSON *str = NULL;
    cJSON *dup_root = NULL;
    cJSON *orig_num = NULL;
    cJSON *dup_num = NULL;
    cJSON *orig_str = NULL;
    cJSON *dup_str = NULL;
    char *printed = NULL;
    cJSON_bool nums_equal = 0;
    cJSON_bool strs_equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num = cJSON_CreateNumber(123.456);
    cJSON_AddItemToObject(root, "value", num);
    str = cJSON_CreateString("HelloWorld");
    cJSON_AddItemToObject(root, "greeting", str);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    dup_root = cJSON_Duplicate(root, 1);
    orig_num = cJSON_GetObjectItemCaseSensitive(root, "value");
    dup_num = cJSON_GetObjectItemCaseSensitive(dup_root, "value");
    nums_equal = cJSON_Compare(orig_num, dup_num, 1);
    orig_str = cJSON_GetObjectItemCaseSensitive(root, "greeting");
    dup_str = cJSON_GetObjectItemCaseSensitive(dup_root, "greeting");
    strs_equal = cJSON_Compare(orig_str, dup_str, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)nums_equal;
    (void)strs_equal;
    return 66;
}