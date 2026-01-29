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
//<ID> 776
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *key_false = "active";
    const char *key_null = "data";
    char *printed = NULL;
    char *copy = NULL;
    size_t len = 0;
    cJSON *root = NULL;
    cJSON *false_item = NULL;
    cJSON *null_item = NULL;
    cJSON *got_false = NULL;
    cJSON *got_null = NULL;
    int is_false = 0;
    int is_null = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    false_item = cJSON_AddFalseToObject(root, key_false);
    null_item = cJSON_AddNullToObject(root, key_null);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    len = strlen(printed);
    copy = (char *)cJSON_malloc(len + 1);
    memset(copy, 0, len + 1);
    memcpy(copy, printed, len + 1);
    got_false = cJSON_GetObjectItem(root, key_false);
    got_null = cJSON_GetObjectItem(root, key_null);
    is_false = cJSON_IsFalse(got_false);
    is_null = cJSON_IsNull(got_null);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(copy);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}