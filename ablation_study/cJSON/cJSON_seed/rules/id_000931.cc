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
//<ID> 931
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *greeting = NULL;
    cJSON *retrieved_greeting = NULL;
    cJSON *retrieved_farewell = NULL;
    char *serialized = NULL;
    const char *greet_text = "hello";
    const char *farewell_text = "goodbye";
    cJSON_bool invalid_before = 0;
    cJSON_bool invalid_after = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    greeting = cJSON_CreateString(greet_text);
    cJSON_AddItemToObject(root, "greeting", greeting);
    cJSON_AddStringToObject(root, "farewell", farewell_text);

    // step 3: Operate / Validate
    retrieved_greeting = cJSON_GetObjectItem(root, "greeting");
    invalid_before = cJSON_IsInvalid(retrieved_greeting);
    serialized = cJSON_PrintUnformatted(root);
    cJSON_DeleteItemFromObject(root, "greeting");
    retrieved_greeting = cJSON_GetObjectItem(root, "greeting");
    retrieved_farewell = cJSON_GetObjectItem(root, "farewell");
    invalid_after = cJSON_IsInvalid(retrieved_greeting);

    // step 4: Cleanup
    (void)invalid_before;
    (void)invalid_after;
    (void)retrieved_farewell;
    cJSON_free(serialized);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}