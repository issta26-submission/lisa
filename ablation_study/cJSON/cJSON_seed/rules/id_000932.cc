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
//<ID> 932
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
    cJSON *alias_item = NULL;
    cJSON *temp_item = NULL;
    cJSON *retrieved = NULL;
    const char *initial_name = "example_name";
    const char *temp_value = "temporary_value";
    const char *retrieved_name = NULL;
    char *printed_before = NULL;
    char *printed_after = NULL;
    cJSON_bool invalid_before = 0;
    cJSON_bool invalid_after = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name_item = cJSON_AddStringToObject(root, "name", initial_name);
    retrieved_name = cJSON_GetStringValue(name_item);
    alias_item = cJSON_AddStringToObject(root, "alias", retrieved_name);
    temp_item = cJSON_CreateString(temp_value);
    cJSON_AddItemToObject(root, "temp", temp_item);

    // step 3: Operate / Validate
    printed_before = cJSON_PrintUnformatted(root);
    retrieved = cJSON_GetObjectItem(root, "temp");
    invalid_before = cJSON_IsInvalid(retrieved);
    cJSON_DeleteItemFromObject(root, "temp");
    retrieved = cJSON_GetObjectItem(root, "temp");
    invalid_after = cJSON_IsInvalid(retrieved);
    printed_after = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    (void)invalid_before;
    (void)invalid_after;
    cJSON_free(printed_before);
    cJSON_free(printed_after);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}