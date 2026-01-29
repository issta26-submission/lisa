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
//<ID> 903
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json_text = "{\"flags\": [true, false, true], \"value\": 42}";
    cJSON *root = NULL;
    cJSON *flags = NULL;
    cJSON *first_item = NULL;
    cJSON_bool first_is_true = 0;
    cJSON *result = NULL;
    char *printed = NULL;
    char *copy = NULL;
    size_t copy_size = 16;

    // step 2: Setup / Configure
    root = cJSON_Parse(json_text);
    flags = cJSON_GetObjectItem(root, "flags");
    first_item = cJSON_GetArrayItem(flags, 0);

    // step 3: Operate / Validate
    first_is_true = cJSON_IsTrue(first_item);
    result = cJSON_CreateObject();
    cJSON_AddItemToObject(result, "first_is_true", cJSON_CreateBool(first_is_true));
    printed = cJSON_Print(result);
    copy = (char *)cJSON_malloc(copy_size);
    memset(copy, 0, copy_size);
    copy[0] = printed[0];
    copy[1] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(copy);
    cJSON_Delete(root);
    cJSON_Delete(result);

    // API sequence test completed successfully
    return 66;
}