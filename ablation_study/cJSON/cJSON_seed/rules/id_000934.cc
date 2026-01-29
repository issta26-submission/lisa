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
//<ID> 934
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *added_greeting = NULL;
    cJSON *created_temp = NULL;
    cJSON *retrieved_greeting = NULL;
    char *printed = NULL;
    cJSON_bool was_invalid = 0;
    const char *greeting = "hello world";
    const char *temp_text = "transient";

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    added_greeting = cJSON_AddStringToObject(root, "greeting", greeting);
    created_temp = cJSON_CreateString(temp_text);
    cJSON_AddItemToObject(root, "temp", created_temp);

    // step 3: Operate / Validate
    retrieved_greeting = cJSON_GetObjectItem(root, "greeting");
    was_invalid = cJSON_IsInvalid(retrieved_greeting);
    printed = cJSON_PrintUnformatted(root);
    cJSON_DeleteItemFromObject(root, "greeting");

    // step 4: Cleanup
    (void)was_invalid;
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}