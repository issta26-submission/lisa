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
//<ID> 678
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *nested = cJSON_CreateObject();
    cJSON *extra_item = cJSON_CreateString("value_extra");
    cJSON *dup_nested = NULL;
    cJSON_bool cmp_equal = 0;
    char *scratch = (char *)cJSON_malloc(128);
    memset(scratch, 0, 128);

    // step 2: Setup / Configure
    cJSON_AddStringToObject(root, "app", "cJSON_test");
    cJSON_AddItemToObject(root, "config", nested);
    cJSON_AddStringToObject(nested, "mode", "fast");
    cJSON_AddItemToObject(nested, "extra", extra_item);

    // step 3: Operate / Validate
    dup_nested = cJSON_Duplicate(nested, 1);
    cmp_equal = cJSON_Compare(nested, dup_nested, 1);

    // step 4: Cleanup
    cJSON_Delete(dup_nested);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}